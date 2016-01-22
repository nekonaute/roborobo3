/*
 *  SDL_gfxRoborobo.cpp
 *  roborobo
 *
 *  Created by Nicolas on 16/01/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "Utilities/Graphics.h"
#include "Utilities/Misc.h"


int gScreenshotIndex = 0; // numbering screenshot (used by saveScreenshot(...))
int gRobotTrackerIndex = 0;
int gTrajectoryFileIndex = 0; // numbering trajectory images (used by saveTrajectoryImage(...))


void saveImage ( SDL_Surface *image, std::string __prefix, std::string __comment ) // comment is optional
{
    std::string sLog = gLogDirectoryname + "/" + __prefix + "_" + gStartTime;

	if ( __comment != "" )
		sLog += "_" + __comment;
	
	sLog += ".bmp";
	
	SDL_SaveBMP(image,sLog.c_str());
}

void saveTrajectoryImage ( std::string __comment )
{

    if ( gTrajectoryMonitor )
    {
        std::string trajectoryFileIndexStr = convertToString(gTrajectoryFileIndex);
        while( trajectoryFileIndexStr.length() < 6 )
        {
            trajectoryFileIndexStr =  "0" + trajectoryFileIndexStr;
        }
        
        std::string timestamp = convertToString(gTrajectoryMonitorIt);
        //while( timestamp.length() < 10 )
        //{
        //    timestamp =  "0" + timestamp;
        //}
        timestamp = "_it" + timestamp;

        std::string s = trajectoryFileIndexStr;
        
        if ( gTrajectoryMonitorMode == 0 )
            s += timestamp + "_all";
        else
            s += timestamp + "_single";
        
        if ( __comment != "" )
            s += "_" + __comment;
        
        saveImage(gTrajectoryMonitorImage,"trajectory",s);
        
        SDL_FreeSurface( gTrajectoryMonitorImage );
        gTrajectoryMonitor = false;
        
        gTrajectoryFileIndex++;
        
    }
    else
    {
        std::cout << "[WARNING] Cannot save trajectories. Monitor is not in use.\n";
    }
}

void saveScreenshot(std::string __comment) // parameter is optional
{
    std::string screenShotIndexStr = convertToString(gScreenshotIndex);
    
    while( screenShotIndexStr.length() < 6 )
    {
        screenShotIndexStr =  "0" + screenShotIndexStr;
    }
    
    saveImage(gScreen,"screenshot",screenShotIndexStr+"_"+__comment);

    gScreenshotIndex++;
}

void saveRobotTrackerIndex( std::string __comment) // parameter is optional
{
    std::string robotTrackerIndexStr = convertToString(gRobotTrackerIndex);
    
    while( robotTrackerIndexStr.length() < 6 )
    {
        robotTrackerIndexStr =  "0" + robotTrackerIndexStr;
    }
    
    saveImage(gEnvironmentImage,"robotsId",robotTrackerIndexStr+"_"+__comment);
    
	gRobotTrackerIndex++;
}

bool initSDL(Uint32 flags) // parameter is optional (default: SDL_HWSURFACE | SDL_DOUBLEBUF)
{
    
    if ( gBatchMode )
    {
            gDisplayMode = 0;
    }
	
    //Initialize all SDL subsystems
    if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
    {
        return false;
    }

    if ( !gBatchMode )
    {
        gScreenWindow = SDL_CreateWindow("Roborobo^3",
                                     //SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,
                                     SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                     gScreenWidth, gScreenHeight,
                                     flags);
    }

    gScreen = SDL_CreateRGBSurface (
                                    0, // flags (unused)
                                    gScreenWidth,
                                    gScreenHeight,
                                    32,
                                    0x00FF0000,
                                    0x0000FF00,
                                    0x000000FF,
                                    0xFF000000);
    
    if( gScreen == NULL ) // error?
        return false;
    
    if ( !gBatchMode )
    {
        gScreenRenderer = SDL_CreateRenderer(gScreenWindow, -1, SDL_RENDERER_ACCELERATED);

        gScreenTexture = SDL_CreateTexture(gScreenRenderer,
                                       SDL_PIXELFORMAT_ARGB8888,
                                       SDL_TEXTUREACCESS_STREAMING,
                                       gScreenWidth, gScreenHeight);
    }
    
    return true;
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination , SDL_Rect* clip ) // last argument is optional (cf. function header)
{
    //Holds offsets
    SDL_Rect offset;
    
    //Get offsets
    offset.x = x;
    offset.y = y;
    
    //Blit
    SDL_BlitSurface( source, clip, destination, &offset );
}

void register_surface( int __x, int __y, SDL_Surface* source, SDL_Surface* destination , int __id ) // last argument is optional (cf. function header)
{
	for ( int x = 0 ; x != source->w ; x++ )
		for ( int y = 0 ; y != source->h ; y++ )
		{
			Uint32 pixel = getPixel32(source,x,y);
			if ( pixel != SDL_MapRGBA( destination->format, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE ) )
				//putPixel32( destination, __x+x, __y+y, __id );
				putPixel32( destination, __x+x, __y+y,  SDL_MapRGBA( destination->format, ((__id & 0xFF0000)>>16), ((__id & 0xFF00)>>8), (__id & 0xFF), SDL_ALPHA_OPAQUE ) );
		}
}

void clean_surface( int __x, int __y, SDL_Surface* source, SDL_Surface* destination ) // last argument is optional (cf. function header)
{

	for ( int x = 0 ; x != source->w ; x++ )
		for ( int y = 0 ; y != source->h ; y++ )
		{
			Uint32 pixel = getPixel32(source,x,y);
			if ( pixel != SDL_MapRGBA( destination->format, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE ) )
				putPixel32( destination, __x+x, __y+y, SDL_MapRGBA( destination->format, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE ) );
		}
}

// used to register a robot into the environment (used since 2012-6-6)
void register_robotMask( int __x, int __y, SDL_Surface* destination , int __id ) // last argument is mandatory to track objects (in particular: agents)
{
    for ( int i = 0 ; (size_t)i != gRobotMaskData.size() ; i++ )
    {
        int x = __x + gRobotMaskData[i][0];
        int y = __y + gRobotMaskData[i][1];
        putPixel32( destination, x, y,  SDL_MapRGBA( destination->format, ( (__id & 0xFF0000)>>16), ((__id & 0xFF00)>>8), (__id & 0xFF), SDL_ALPHA_OPAQUE ) );
    }
}

// used to unregister a robot from the environment (used since 2012-6-6)
void clean_robotMask( int __x, int __y, SDL_Surface* destination ) // last argument is optional (cf. function header)
{
    //std::cout << "gRobotMaskData.size() => " << gRobotMaskData.size() << std::endl;
    
    for ( unsigned int i = 0 ; i != gRobotMaskData.size() ; i++ )
    {
        int x = __x + gRobotMaskData[i][0];
        int y = __y + gRobotMaskData[i][1];
        
        //std::cout << "coordinates: " << x << " , " << y << std::endl;
        
        putPixel32( destination, x, y, SDL_MapRGBA( destination->format, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE ) );
    }
}


/* **************** */
/* * SIMULATOR WINDOW * */
/* **************** */



void toggle_fullscreen()
{
	std::cout << "[critical] Fullscreen mode is not implemented." << std::endl;

	/*
	clean_up();

	gFullScreenMode= !gFullScreenMode;
	
	if( initSDL(SDL_HWSURFACE | SDL_DOUBLEBUF | (gFullScreenMode?SDL_FULLSCREEN:0) ) == false )
    {
		std::cerr << "fullscreen error 1 (toggling fullscreen) \n";
		exit(-2);
	}
    
    if( gWorld->loadFiles() == false ) // note: if this block is removed, computer may hang. + gWorld is not in the scope
    {
		std::cerr << "fullscreeen error 2 (re-loading files) \n";
		exit(-2);
	}
 
	//if (SDL_WM_ToggleFullScreen(gScreen) == 0)
	*/
}


SDL_Surface *load_image( std::string filename ) 
{
    SDL_Surface* loadedImage = NULL;
    SDL_Surface* optimizedImage = NULL;
    
    //loadedImage = IMG_Load( filename.c_str() );
    
    loadedImage = SDL_LoadBMP( filename.c_str() );
    
    if( loadedImage != NULL )
    {
        //If the image was successfully loaded, then convert color scheme to match 32-bits-ARGB (if needed)

        if ( loadedImage->format->format == SDL_PIXELFORMAT_ARGB8888 )
            return loadedImage;
        else
        {
            std::cout << "[INFO] file \"" << filename << "\" not in 32-bits-ARGB format. Converting." << std::endl;
            
            optimizedImage = SDL_ConvertSurfaceFormat( loadedImage, SDL_PIXELFORMAT_ARGB8888, 0 ); // SDL_PIXELFORMAT_ARGB8888
            SDL_FreeSurface( loadedImage );
            
            return optimizedImage;
        }
    
    }
    else
    {
        std::cerr << "[CRITICAL] cannot load image from file \"" << filename << "\". EXITING.";
        exit(-1);
    }

    return NULL;
}

// draw a line from (x1,y1) to (x2,y2)
// algorithm: Bresenham
// adapted from: http://www.roguebasin.com/index.php?title=Bresenham%27s_Line_Algorithm

void drawLine(SDL_Surface * image,
               int x1,
               int y1,
               int const x2,
               int const y2,
               uint8_t r, uint8_t g, uint8_t b )
{
    uint32_t color = SDL_MapRGBA( image->format, r, g, b, SDL_ALPHA_OPAQUE );
    
    int delta_x(x2 - x1);
    // if x1 == x2, then it does not matter what we set here
    signed char const ix((delta_x > 0) - (delta_x < 0));
    delta_x = std::abs(delta_x) << 1;
    
    int delta_y(y2 - y1);
    // if y1 == y2, then it does not matter what we set here
    signed char const iy((delta_y > 0) - (delta_y < 0));
    delta_y = std::abs(delta_y) << 1;
    
    putPixel32_secured( image, x1, y1 , color );
    
    if (delta_x >= delta_y)
    {
        // error may go below zero
        int error(delta_y - (delta_x >> 1));
        
        while (x1 != x2)
        {
            if ((error >= 0) && (error || (ix > 0)))
            {
                error -= delta_x;
                y1 += iy;
            }
            // else do nothing
            
            error += delta_y;
            x1 += ix;
            
            putPixel32_secured( image, x1, y1 , color );
        }
    }
    else
    {
        // error may go below zero
        int error(delta_x - (delta_y >> 1));
        
        while (y1 != y2)
        {
            if ((error >= 0) && (error || (iy > 0)))
            {
                error -= delta_y;
                x1 += ix;
            }
            // else do nothing
            
            error += delta_x;
            y1 += iy;
            
            putPixel32_secured( image, x1, y1 , color );
        }
    }
}

// cast a line from (x1,y1) to (x2,y2), without tracing it, and stopping whenever a non-blank pixel is met. Returns: updated x2 and y2 values (passed as pointers), and collision (true|false).
// algorithm: Bresenham
// adapted from: http://www.roguebasin.com/index.php?title=Bresenham%27s_Line_Algorithm

bool castLine(SDL_Surface * image,
              int x1,
              int y1,
              int *x2pt,
              int *y2pt,
              int maxLength )
{
    int x2 = *x2pt;
    int y2 = *y2pt;
    
    uint32_t color = SDL_MapRGBA( image->format, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE );
    
    int delta_x(x2 - x1);
    // if x1 == x2, then it does not matter what we set here
    signed char const ix((delta_x > 0) - (delta_x < 0));
    delta_x = std::abs(delta_x) << 1;
    
    int delta_y(y2 - y1);
    // if y1 == y2, then it does not matter what we set here
    signed char const iy((delta_y > 0) - (delta_y < 0));
    delta_y = std::abs(delta_y) << 1;
    
    int dist = 0; // distance to obstacle (or to max_length)

    if ( getPixel32( image, x1, y1 ) != color )
    {
        *x2pt = x1;
        *y2pt = y1;
        return true;
    }

    if (delta_x >= delta_y)
    {
        // error may go below zero
        int error(delta_y - (delta_x >> 1));
        
        while (x1 != x2)
        {
            if ((error >= 0) && (error || (ix > 0)))
            {
                error -= delta_x;
                y1 += iy;
            }
            // else do nothing
            
            error += delta_y;
            x1 += ix;
            
            dist++;

            if ( getPixel32( image, x1, y1 ) != color )
            {
                *x2pt = x1;
                *y2pt = y1;
                return true;
            }
            
        }
    }
    else
    {
        // error may go below zero
        int error(delta_x - (delta_y >> 1));
        
        while (y1 != y2)
        {
            if ((error >= 0) && (error || (iy > 0)))
            {
                error -= delta_y;
                x1 += ix;
            }
            // else do nothing
            
            error += delta_x;
            y1 += iy;
            
            dist++;

            if ( getPixel32( image, x1, y1 ) != color )
            {
                *x2pt = x1;
                *y2pt = y1;
                return true;
            }
        }
    }
    
    return false; // no collision
}
