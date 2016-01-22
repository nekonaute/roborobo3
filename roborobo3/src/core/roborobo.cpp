///////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                               //
// ROBOROBO^3                                                                                    //
//                                                                                               //
// Author: Nicolas Bredeche                                                                      //
// Contact: nicolas.bredeche(at)upmc.fr / niko(at)factorycity.com                                //
// http://factorycity.com // http://pages.isir.upmc.fr/~bredeche                                 //
//                                                                                               //
// 2008-2016 (started 20081216, version 3: 20160122)                                             //
// current version: (see gVersion value)                                                         //
//                                                                                               //
// People involved, at some point or another, in roborobo:                                       //
//   JM Montanier, L. Cazenille [NNlib], E. Haasdijk [Properties], B. Weel                       //
//                                                                                               //
// Official reference:                                                                           //
//                                                                                               //
//   Nicolas Bredeche, Jean-Marc Montanier, Berend Weel, Evert Haasdijk.                         //
//   Roborobo! a Fast Robot Simulator for Swarm and Collective Robotics.                         //
//   Arxiv, CoRR:abs/1304.2888, 2013.                                                            //
//                                                                                               //
/*
 @article{bredeche2013roborobo,
    author    = {Nicolas Bredeche and Jean-Marc Montanier and Berend Weel and Evert Haasdijk},
    title     = {Roborobo! a Fast Robot Simulator for Swarm and Collective Robotics},
    journal   = {CoRR},
    volume    = {abs/1304.2888},
    year      = {2013},
    ee        = {http://arxiv.org/abs/1304.2888}
    }
*/
//                                                                                               //
//                                                                                               //
// Acknowledgements:                                                                             //
//                                                                                               //
// * Useful ressources when starting with SDL                                                    //
//  http://lazyfoo.net/SDL_tutorials/                                                            //
//                                                                                               //
// * Projects acknowledgements                                                                   //
//  http://factorycity.com                                                                       //
//  Symbrion EU project, grant agreement 216342 (EU FET Proactive Intiative)                     //
//  Grid'5000 experimental testbed ( https://www.grid5000.fr )                                   //
//  BQR UPMC (2012-2013)                                                                         //
//  DREAM EU H2020 PROJECT, grant agreement 640891 (2015-2018) - http://robotsthatdream.org      //
//                                                                                               //
///////////////////////////////////////////////////////////////////////////////////////////////////


#include "RoboroboMain/roborobo.h"

// Project headers

//#include "common.h"
#include "Utilities/Graphics.h"
#include "World/World.h"
#include "Agents/Agent.h"
#include "Agents/Robot.h"
#include "Agents/InspectorAgent.h"
#include "Utilities/Timer.h"
#include "Utilities/Misc.h"
#include "Utilities/Graphics.h"

#include "Config/GlobalConfigurationLoader.h"

/* **************************
 * * global data definition *			// dont forget to modify header file if you add other global data
 * **************************/

// =-=-=-=-=

long int gVersion  = 20160122;
std::string gLogCommentText  = "(under development)"; //

std::string gStartTime = getCurrentTimeAsReadableString();
time_t gStartTimeRawFormat = time(0);
std::string gStopTime;
time_t gStopTimeRawFormat = time(0);

std::string gCompileTime = __TIME__;
std::string gCompileDate = __DATE__;


std::string gPropertiesFilename = "config/template_wander_bigrobots.properties";
std::string gCurrentBuildInfo   = "Lutetia build (roborobo^3 w/ SDL2)";  // display through "-v[ersion]" command line option -- check/set value in config.h
// start development: 2009
// Bristol build -- june 2010
// Krakow build -- sept 2010
// Berlin build -- april 2011
// Amsterdam build -- june 2012
// Paris build -- november 2013 -- roborobo^2 (major revision)
// Locmaria build -- april 2014 -- roborobo^2 (rev 1)
// Heraklion build -- june 2014 -- roborobo^2 (rev 2)
// Ljubljana build -- sept 2014 -- roborobo on github
// Lutetia build -- sept 2015 -- roborobo^3 (major revision: switch to SDL2)
// Tokyo build -- jan 2016 -- roborobo^3 (rev 1, public release on github)

ExtendedProperties gProperties;

bool gVideoRecording = false;


bool gTrajectoryMonitor = false;
int gTrajectoryMonitorMode = 0;

//std::vector<std::string> gRemainingCommandLineParameters;    //todelete: 2014-09-17, deprecated


//filenames

std::ofstream gLogFile;
LogManager *gLogManager = NULL;

bool gVerbose_commandlineargument =             false; // set to true if given as command line argument (priority over properties file)
bool gLogDirectoryname_commandlineargument =    false; // set to true only if given as command line argument (priority over properties file)
std::string gLogDirectoryname =                 "logs";
std::string gLogFilename =						"datalog.txt";
std::string gLogFullFilename =                  ""; // cf. the initLog method

std::string gRobotMaskImageFilename =			"data/agent-mask.png";
std::string gRobotDisplayImageFilename =		"data/agent-mask.png";
std::string gRobotSpecsImageFilename =			"data/agent-specs.png";
std::string gForegroundImageFilename =			"data/foreground.png";   // MANDATORY: use png (avoid jpg approximation)
std::string gEnvironmentImageFilename =			"data/environment.png";
std::string gBackgroundImageFilename =			"data/background.png";			
std::string gGroundSensorImageFilename =		"data/ground.png";

//general purpose

int gRandomSeed = -1; // (default value should be "-1" => time-based random seed)

bool gVerbose = true;
bool gBatchMode = false;

int gScreenBPP = 32; // bits per pixel color (32 bits)
int gFramesPerSecond = 60; // frame rate

int gLocationFinderMaxNbOfTrials = 100; 

bool gRobotDisplayFocus = false;

ConfigurationLoader *gConfigurationLoader = NULL;

std::vector<std::vector<int> > gRobotMaskData;

//general data

bool gRadioNetwork = false; // default is false -- see header for information about computational cost.
int gMaxRadioDistance = -1; // not implemented (yet)

int gDisplaySensors = 2; // 0:no, 1:if-contact, 2:always+red, 3: always
bool gDisplayTail = true;
bool gRobotLEDdisplay = true;

bool gExtendedSensoryInputs = false;

bool gPauseMode = false;
bool gInspectorMode = false; // manual control mode -- false: agent-centered ; true: envt centered (ie. if inspector agent exists)
bool gInspectorAgent = false; // is there an inspector agent? 

int gMaxIt = 0; // note: value *must* be defined in the properties file. ("-1" (ie. infinite) is a nice default setting).

int gNbOfLandmarks = 0;
std::vector<LandmarkObject> gLandmarks;

int gNbOfPhysicalObjects = 0;
int gPhysicalObjectDefaultType = 0;
int gPhysicalObjectDefaultRegrowTimeMax = -1;
bool gPhysicalObjectDefaultRelocate = false;
bool gPhysicalObjectDefaultOverwrite = false;
int gPhysicalObjectDefaultRadius = 8;
int gPhysicalObjectDefaultFootprintRadius = 14;
int gPhysicalObjectDefaultDisplayColorRed = 0;
int gPhysicalObjectDefaultDisplayColorGreen = 0;
int gPhysicalObjectDefaultDisplayColorBlue = 0;
int gPhysicalObjectDefaultSolid_w = 16;
int gPhysicalObjectDefaultSolid_h = 16;
int gPhysicalObjectDefaultSoft_w = 22;
int gPhysicalObjectDefaultSoft_h = 22;

std::vector<PhysicalObject*> gPhysicalObjects;
bool gPhysicalObjectsRedraw = false;

bool gEnergyLevel = false;
double gEnergyMax = 100;
double gEnergyInit = 100;
bool gEnergyRefill = false;

double gEnergyItemDefaultInit = 100;
int gEnergyItemDefaultMode = 0;

int gNumberOfRobots = 0;
int gRobotIndexFocus = 0;

int gNumberOfRobotGroups = 1;

int gPhysicalObjectIndexStartOffset = 0x01;
int gRobotIndexStartOffset = 0xFFFFF;

//Screen attributes
int gScreenWidth = 0;
int gScreenHeight = 0;

//The robot dimensions (default)
int gRobotWidth=0;
int gRobotHeight=0;

int gInspectorCursorHorizontalSpeed = 1;
int gInspectorCursorVerticalSpeed = 1;

int gInspectorAgentXStart = 1;
int gInspectorAgentYStart = 1;

// agent dynamics and structure

int	gInitialNumberOfRobots = 2;

float gMaxTranslationalSpeed = 2; // pixels/sec.
float gMaxTranslationalDeltaValue = gMaxTranslationalSpeed; // maximum delta update value btw current and desired speed.
float gMaxRotationalSpeed = 30; // degrees/sec.
int gSensorRange = 64;
int gTailLength = 16;

int gInspectorCursorMaxSpeedOnXaxis = 5; // "god" inspector mode max speed on both axis ("G" key to activate)
int gInspectorCursorMaxSpeedOnYaxis = 10;

int gLocomotionMode = 0; // cf. header file

bool gMonitorRobot = false;

int gTrajectoryMonitorIt = 0;

// =-=-=-=-=

bool gKeyUp=false, gKeyDown=false, gKeyRight=false, gKeyLeft=false;
bool gFullScreenMode=false;
bool gSlowMotionMode=false;

bool gNiceRendering = true;

int backup_gDisplayMode = 0;

int  gDisplayMode=0;
int  gFastDisplayModeSpeed = 60;//500;
bool gRefreshUserDisplay = true;

bool gUserCommandMode=false;

//dimensions of the level
int gAreaWidth = 0;
int gAreaHeight = 0;

//camera
SDL_Rect gCamera;

//image surfaces

SDL_Surface  *gScreen = NULL;
SDL_Texture  *gScreenTexture = NULL;
SDL_Renderer *gScreenRenderer = NULL;
SDL_Window   *gScreenWindow = NULL;

SDL_Surface *gForegroundImage = NULL;
SDL_Surface *gEnvironmentImage = NULL;
SDL_Surface *gBackgroundImage = NULL;
SDL_Surface *gRobotMaskImage = NULL;
SDL_Surface *gRobotDisplayImage = NULL;
SDL_Surface *gRobotSpecsImage = NULL;
SDL_Surface *gTrajectoryMonitorImage = NULL;
SDL_Surface *gGroundSensorImage = NULL;

// SDL event handler
SDL_Event gEvent; //The event structure


// special simulation data

InspectorAgent *inspectorAgent = NULL;
World *gWorld = NULL;
Timer fps;
int timetag=-1;
Timer timeWatch;


/* ********************
 * * global functions *
 * ********************/

void clean_up()
{
    //Free the surfaces

    SDL_FreeSurface( gRobotMaskImage );
    SDL_FreeSurface( gRobotDisplayImage );
    SDL_FreeSurface( gForegroundImage );
	SDL_FreeSurface( gEnvironmentImage );
    
	if ( gBackgroundImage != NULL )
		SDL_FreeSurface( gBackgroundImage );
	SDL_FreeSurface( gGroundSensorImage );
    
    if ( gTrajectoryMonitor )
    {
        if ( gVerbose ) std::cout << "[EXIT] Stopping recording trajectory/ies." << std::endl;
        saveTrajectoryImage();
        if ( gVerbose ) std::cout << "[EXIT] Image file created (see ./logs)." << std::endl;
    }

	if ( inspectorAgent != NULL ) delete(inspectorAgent);

    //Quit SDL
    //SDL_Quit();
}


void displayHelp()
{	
	//if ( gVerbose && !gBatchMode)
	if ( !gBatchMode)
	{
		std::cout << "\n=-=-=-=-=-=-=-=-=-=-=-= roborobo! =-=-=-=-=-=-=-=-=-=-=-=\n";
		std::cout << "Fast 2D autonomous agent simulator for large population of robots/agents\nnicolas.bredeche(at)upmc.fr\n";
		std::cout << "Official version tag    : " << gVersion << std::endl;
		std::cout << "Current build name      : " << gCurrentBuildInfo << std::endl << std::endl;
		std::cout << "Compilation version tag : " << gCompileDate << " - " << gCompileTime << std::endl;

		std::cout << " >>>> Keys:" << std::endl;
		std::cout << "       h : help! (ie. this text)" << std::endl;
		std::cout << "       r : refresh on-screen rendering (no impact on simulation)" << std::endl;

		std::cout << "       n : radio network communication on/off" << std::endl;
		std::cout << "       d : set display mode - (1) default-60-fps; (2) fast; (3) fastest-no-display. (shift+d: inverse)" << std::endl;
		std::cout << "       v : verbose on/off (console)" << std::endl;
		std::cout << "       p : pause/freeze simulation (display mode)" << std::endl;
		std::cout << "       s : slow mode on/off (switch it off if display is off)" << std::endl;

		std::cout << "       x : (\"X-ray mode\") debug display mode on/off" << std::endl;
		std::cout << "       f : highlight current focused agent on/off (display mode)" << std::endl;
		std::cout << "       z : display ground type caption on/off (display mode)" << std::endl;
		std::cout << "       g : (\"god mode\") switch agent/inspector mode - follow selected agent or move freely (display mode)" << std::endl;
		std::cout << "       i : inspect currently selected agent on/off (display sensor and motor values on console)" << std::endl;
		std::cout << "       j : show / dont show sensor raycasting and visual markers (display mode)" << std::endl;
		std::cout << "       l : show / dont show LED color on top of the robots (display mode)" << std::endl;
		
        std::cout << "       o : take screenshot - save image to disk (in ./logs) (display mode)" << std::endl;
		std::cout << "       O : video recording start/stop - save images to disk (in ./logs) (display mode)" << std::endl;
		
        std::cout << "       t : start/stop tracking trajectories (all agents) - save image to disk (in ./logs) (display mode)" << std::endl;
        std::cout << "       T : start/stop tracking trajectories (target agent) - save image to disk (in ./logs) (display mode)" << std::endl;
        
		std::cout << "   <tab> : switch focus to next agent (shift+o: previous agent)" << std::endl;
		std::cout << " <enter> : (in agent following mode) trigger manual agent control mode" << std::endl;
		std::cout << " <enter> : (in inspector mode) sample and display sensor values at current location" << std::endl;
		std::cout << "   <esc> : quit" << std::endl;
		std::cout << "=-=-=-=-=-=-=-=-=-=-=-= roborobo! =-=-=-=-=-=-=-=-=-=-=-=\n\n";
	}
}


/* ******** *
 *   MAIN   *
 * ******** */

bool checkQuitEvent()
{
	bool quit = false;
	//While there's events to handle
	while( SDL_PollEvent( &gEvent ) )
	{
		//If the user has Xed out the window
		if( gEvent.type == SDL_QUIT )
		{
			//Quit the program
			quit = true;
			break;
		}
	}
	return quit;
}




// return true if quit.
bool handleKeyEvent(const Uint8 *keyboardStates)
{
	if ( keyboardStates[ SDL_SCANCODE_ESCAPE ] )
	{
		if ( gVerbose )
			std::cout << std::endl << "Terminated by user!" << std::endl;
			
		SDL_Delay(PAUSE_COMMAND);
		return true;
	}

	if ( keyboardStates[ SDL_SCANCODE_D ] )
	{
		if ( gSlowMotionMode )
			gSlowMotionMode = false;
		if ( keyboardStates[ SDL_SCANCODE_RSHIFT ] || keyboardStates[ SDL_SCANCODE_LSHIFT ] )
			gDisplayMode = ( gDisplayMode + 2 ) % 3;
		else
			gDisplayMode = ( gDisplayMode + 1 ) % 3;

		if ( gVerbose )
			std::cout << "Display mode is now " << gDisplayMode << std::endl;

        if ( gDisplayMode <= 1 )
            gRefreshUserDisplay = true;
        
		SDL_Delay(PAUSE_COMMAND);
	}

	if (keyboardStates[ SDL_SCANCODE_R ])
	{
        if ( gDisplayMode <= 1 )
        {
            gRefreshUserDisplay = true;
            if ( gVerbose )
                std::cout << "Refresh screen." << std::endl;
        }
			
		SDL_Delay(PAUSE_COMMAND);
	}

	if ( gDisplayMode == 0 || gDisplayMode == 1 )	
	{				
		if ( keyboardStates[ SDL_SCANCODE_P ] )
		{
			SDL_Delay(PAUSE_COMMAND); // 200ms delay
			gPauseMode = !gPauseMode;
			if ( gVerbose )
			{
				if ( gPauseMode )
					std::cout << "pause mode is ON. (at " << gWorld->getIterations() << " steps)" << std::endl;
				else
					std::cout << "pause mode is OFF." << std::endl;
			}
		}

		if ( keyboardStates[ SDL_SCANCODE_H ] )
		{
			displayHelp();
			SDL_Delay(PAUSE_COMMAND); // 200ms delay
		}
        
		if ( keyboardStates[ SDL_SCANCODE_O ] ) // screenshot and movie recording
		{
			if ( keyboardStates[ SDL_SCANCODE_RSHIFT ] || keyboardStates[ SDL_SCANCODE_LSHIFT ] )
			{
				// start/stop movie recording

				gVideoRecording = !gVideoRecording;
				
				if ( gVerbose )
				{
					if ( gVideoRecording )
						std::cout << "Starting video recording (shift+o to stop)." << std::endl;
					else
						std::cout << "Stopping video recording." << std::endl;
				}
			}
			else
			{
				// save screenshot

				saveScreenshot();
			
				if ( gVerbose )
					std::cout << "Screenshot saved." << std::endl;
			}
			
			SDL_Delay(PAUSE_COMMAND); // 200ms delay
		}
        
        
		if ( keyboardStates[ SDL_SCANCODE_T ] ) // build/dump image with robot trajectories
		{
            if ( !gTrajectoryMonitor ) // check if start or stop
            {
                // start.
                
                initTrajectoriesMonitor();

                if ( keyboardStates[ SDL_SCANCODE_RSHIFT ] || keyboardStates[ SDL_SCANCODE_LSHIFT ] )
                {
                    gTrajectoryMonitorMode = 1; // focus agent only
                    if ( gVerbose )
                		std::cout << "Starting recording trajectory for focused agent ONLY." << std::endl;
                }
                else
                {
                    gTrajectoryMonitorMode = 0; // all agents
				    if ( gVerbose )
                		std::cout << "Starting recording trajectories for ALL agents." << std::endl;
                }
			}
            else
            {
                if ( gVerbose ) std::cout << "Stopping recording trajectory/ies." << std::endl;
                saveTrajectoryImage();
                if ( gVerbose ) std::cout << "Image file created (see ./logs)." << std::endl;
            }
            
			SDL_Delay(PAUSE_COMMAND); // 200ms delay
		}
				
		if ( keyboardStates[ SDL_SCANCODE_V ] )
		{
			gVerbose = !gVerbose;

			if ( gVerbose )
				std::cout << "Verbose mode is now ON." << std::endl;
			else
				std::cout << "Verbose mode is now OFF." << std::endl;

			SDL_Delay(PAUSE_COMMAND); // 200ms delay
		}
		
		if ( keyboardStates[ SDL_SCANCODE_G ] )
		{
			gInspectorMode = !gInspectorMode;
			if ( gInspectorMode == true && gUserCommandMode == true )
				gUserCommandMode = false;					

			if ( gVerbose )
			{
				if ( gInspectorMode )
					std::cout << "Inspector agent is now active." << std::endl;
				else
					std::cout << "Inspector agent is now inactive." << std::endl;
			}

			SDL_Delay(PAUSE_COMMAND); // 200ms delay
		}
		
		if ( keyboardStates[ SDL_SCANCODE_RETURN ] )
		{
			if ( gInspectorMode == false )  // (1) on/off take command of the robot OR (2) display id at inspector target
			{
				gUserCommandMode = !gUserCommandMode;
				if ( gVerbose )
				{
					if ( gUserCommandMode )
						std::cout << "Agent #" << gRobotIndexFocus << " is now remote-controlled." << std::endl;
					else
						std::cout << "Agent #" << gRobotIndexFocus << " is now autonomous." << std::endl;
				}
			}
			else
			{	
				if ( gVerbose )
				{
					// * inspector mode. Return key trigger sensor display. (note: non-collision enabled robot cannot be seen)
			
					// inspector virtual sensors values
					std::cout << "## Inspector Agent ##" << std::endl; //"Inspector virtual sensors:"

					int xTmp,yTmp;
					inspectorAgent->getCoord(xTmp,yTmp);

					// location
					std::cout << "\tcoordinates: (" << xTmp << "," << yTmp << ")" << std::endl; 
					
					// virtual range sensor
					Uint32 inspectorSensorValue = getPixel32(gEnvironmentImage, xTmp, yTmp);
					Uint8 r, g, b;
					SDL_GetRGB(inspectorSensorValue,gEnvironmentImage->format,&r,&g,&b); 
					inspectorSensorValue = (r<<16)+(g<<8)+b; 
					std::cout << "\tvirtual range sensor: ";
					if ( inspectorSensorValue == 0xFFFFFF ) // either nothing or unregistered agent(s).
					{	
						std::cout << "0xFFFFFF (nothing)" << std::endl;
						
						// Agents may not be visible in the internal scene buffer due to optimization
						// Hence, we scan the list of agents to compare actual inspector location and agent location
						// Results from this scan should be interpreted as a list of either
						//  - nearby agents (possibly registered, or not)
						//  - agent precisely at this location, but not registered
						// note: registering Agent in the internal scene buffer is not mandatory if it is sure
						//       that it is not within the perceptual range of any other agents (speed up simulation).
						
						int radiusMax = gRobotWidth > gRobotHeight ? ( gRobotWidth + 1 ) / 2 : ( gRobotHeight + 1 ) / 2; // assume an upper bound for dimension.
						for ( int i = 0 ; i != gNumberOfRobots ; i++ ) // test for agents proximity based on localization 
						{
						  int x = (int)(gWorld->getRobot(i)->getWorldModel()->getXReal());
						  int y = (int)(gWorld->getRobot(i)->getWorldModel()->getYReal());
						  if ( abs(x - xTmp) < radiusMax && abs(y - yTmp) < radiusMax )
							std::cout << "\t\tAgent #" << i << " detected (closeby and/or unregistered)." << std::endl;
						}
					}
					else
					{
						std::cout << inspectorSensorValue;
						if ( inspectorSensorValue >= (Uint32)gRobotIndexStartOffset )
						  std::cout << " (agent #" << inspectorSensorValue - gRobotIndexStartOffset << ")";
						std::cout << std::endl; 
					}

					// virtual floor sensor
                    inspectorSensorValue = getPixel32( gGroundSensorImage, xTmp, yTmp);
                    SDL_GetRGB(inspectorSensorValue,gGroundSensorImage->format,&r,&g,&b);

                    int value = ((int)r)*256*256 + ((int)g)*256 + (int)b;
                    
                    std::cout << "\tvirtual floor sensor: ( " << value << " : " << (int)r << "," << (int)g << "," << (int)b << ")" << std::endl;
				}
			}				
			SDL_Delay(PAUSE_COMMAND); // 200ms delay
		}

		if (keyboardStates[ SDL_SCANCODE_F ]) // display agent focus
		{
			gRobotDisplayFocus = !gRobotDisplayFocus;
            if ( gVerbose )
			{
				if ( gRobotDisplayFocus )
					std::cout << "Visual Focus on current robot is on." << std::endl;
				else
					std::cout << "Visual Focus on current robot is off." << std::endl;
			}
			SDL_Delay(PAUSE_COMMAND); // 200ms delay
		}

        
		if ( keyboardStates[ SDL_SCANCODE_L ] )
		{
			gRobotLEDdisplay = !gRobotLEDdisplay;
			
			if ( gVerbose )
			{
				if ( gRobotLEDdisplay )
					std::cout << "Robots' color LED (RGB) is displayed." << std::endl;
				else
					std::cout << "Robots' color LED (RGB) is *not* displayed." << std::endl;
			}			
			SDL_Delay(PAUSE_COMMAND); // 200ms delay
		}
		
		if ( keyboardStates[ SDL_SCANCODE_X ] )
		{
			gNiceRendering = !gNiceRendering;
			
			if ( gVerbose )
			{
				if ( gNiceRendering )
					std::cout << "Render mode is \"user-compliant\" (ie. nice but possibly wrong rendering)." << std::endl;
				else
					std::cout << "Render mode is \"x-ray\" mode (ie. world as seen by robot(s))." << std::endl;
			}
			
			SDL_Delay(PAUSE_COMMAND); // 200ms delay
		}
		
		if ( keyboardStates[ SDL_SCANCODE_J ] )
		{
            if ( keyboardStates[ SDL_SCANCODE_RSHIFT ] || keyboardStates[ SDL_SCANCODE_LSHIFT ] )
                gDisplaySensors = (gDisplaySensors+4-1)%4;
            else
                gDisplaySensors = (gDisplaySensors+1)%4;

			
			if ( gVerbose )
			{
				switch ( gDisplaySensors )
                {
                    case 0:
                        std::cout << "Sensor rays are not shown." << std::endl;
                        break;
                    case 1:
                        std::cout << "Sensor rays are shown if impact (possibly approximate rendering)." << std::endl;
                        break;
                    case 2:
                        std::cout << "Sensor rays are always shown. Activate sensors are highlighted." << std::endl;
                        break;
                    case 3:
                        std::cout << "Sensor rays are always shown." << std::endl;
                        break;
                }
			}
			
			SDL_Delay(PAUSE_COMMAND); // 200ms delay
		}
        
        if ( keyboardStates[ SDL_SCANCODE_K ] )
        {
            gDisplayTail = !gDisplayTail;
            if ( gVerbose )
            {
                if ( gDisplayTail )
                    std::cout << "Virtual tails are shown." << std::endl;
                else
                    std::cout << "Virtual tails are not shown." << std::endl;
            }

            SDL_Delay(PAUSE_COMMAND); // 200ms delay
        }
		
		if ( keyboardStates[ SDL_SCANCODE_TAB ] )
		{
			if ( keyboardStates[ SDL_SCANCODE_RSHIFT ] || keyboardStates[ SDL_SCANCODE_LSHIFT ] )
				gRobotIndexFocus = ( (gRobotIndexFocus-1) + gNumberOfRobots )  % gNumberOfRobots;
			else
				gRobotIndexFocus = (gRobotIndexFocus+1) % gNumberOfRobots;
			
			if ( gVerbose )
				std::cout << "Agent #" << gRobotIndexFocus << " is selected." << std::endl;
					
			SDL_Delay(PAUSE_COMMAND); // 200ms delay
		}

		if ( keyboardStates[ SDL_SCANCODE_I ] )
		{
			if ( gVerbose )
				std::cout << "Inspect agent #" << gRobotIndexFocus << "." << std::endl;
			gMonitorRobot = !gMonitorRobot;
            gRobotDisplayFocus = true;
			SDL_Delay(PAUSE_COMMAND);
		}

		if ( keyboardStates[ SDL_SCANCODE_S ] )
		{
			gSlowMotionMode = !gSlowMotionMode;
			if ( gSlowMotionMode )
				gDisplayMode = 0;

			if ( gVerbose )
			{
				if ( gSlowMotionMode )
					std::cout << "Slow motion is ON." << std::endl;
				else
					std::cout << "Slow motion is off." << std::endl;
			}

			SDL_Delay(PAUSE_COMMAND);
		}
        
		if ( keyboardStates[ SDL_SCANCODE_N ] )
		{
            gRadioNetwork = !gRadioNetwork;
            
            if ( gVerbose )
            {
                if ( gRadioNetwork )
                    std::cout << "Radio network is active." << std::endl;
                else
                    std::cout << "Radio network is inactive." << std::endl;
            }
			SDL_Delay(PAUSE_COMMAND); // 200ms delay
		}
        
		if ( gSlowMotionMode )
			SDL_Delay( 100 );
	}/**/
		
	return false;
}


void updateDisplay() // display is called starting when gWorld->getIterations > 0.
{
        if ( gDisplayMode == 0 || ( gDisplayMode == 1 && gWorld->getIterations() % gFastDisplayModeSpeed == 0 ) )
		{			
			//Set the camera to either focused agent or inspector virtual location 
			if ( gInspectorMode )
				inspectorAgent->set_camera();
			else
				gWorld->getRobot(gRobotIndexFocus)->set_camera();

			//Show the background image and foreground image (active borders) [note: this is what costs a lot wrt. computation time]
			if ( gBackgroundImage != NULL && gNiceRendering )
				apply_surface( 0, 0, gBackgroundImage, gScreen, &gCamera );
			else
				if ( gNiceRendering )
					SDL_FillRect( gScreen, &gScreen->clip_rect, SDL_MapRGBA( gScreen->format, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE ) ); // clear screen if no background image
				else
					apply_surface( 0, 0, gGroundSensorImage, gScreen, &gCamera );

			if ( gNiceRendering ) 
				apply_surface( 0, 0, gForegroundImage, gScreen, &gCamera );
			else
				apply_surface( 0, 0, gEnvironmentImage, gScreen, &gCamera );

            if ( gWorld->getIterations() == 1 )
            {
                saveScreenshot("firstIteration_environment");
            }
            else
            {
                if ( gWorld->getIterations() == gMaxIt-1 )
                {
                    saveScreenshot("lastIteration_environment");
                }
            }
			
            //Show agents on the screen
			for ( int i = 0 ; i != gNumberOfRobots ; i++ )
			{
				if ( gWorld->isRobotRegistered(i) )
					gWorld->getRobot(i)->unregisterRobot(); // remove agent from memory so as to correctly cast sensors (otw: may see itself)
				
				gWorld->getRobot(i)->show(); // show sensor rays.
				
				// re-registering agents (post-display)
				if ( gWorld->isRobotRegistered(i) )
					gWorld->getRobot(i)->registerRobot();
			}
            
            
            // * Snapshots: take screenshots of first and ~ultimate iteration
            
            if ( gWorld->getIterations() == 1 )
            {
                saveScreenshot("firstIteration");
                saveRobotTrackerIndex("firstIteration");
            }
            else
            {
                if ( gWorld->getIterations() == gMaxIt-1 )
                {
                    saveScreenshot("lastIteration");
                    saveRobotTrackerIndex("lastIteration");
                }
            }

			
			// show inspector agent location (single point)
			if ( gInspectorMode )
				inspectorAgent->show();

            if ( !gBatchMode )
            {
                SDL_UpdateTexture(gScreenTexture, NULL, gScreen->pixels, gScreen->pitch);
                SDL_RenderClear(gScreenRenderer);
                SDL_RenderCopy(gScreenRenderer, gScreenTexture, NULL, NULL);
                SDL_RenderPresent(gScreenRenderer);
            }
    		
			//Cap the frame rate
			if( fps.get_ticks() < 1000 / gFramesPerSecond )
			{
				SDL_Delay( ( 1000 / gFramesPerSecond ) - fps.get_ticks() );
			}
			/**/
			
			// video capture (sync with screen update)
			if ( gVideoRecording == true )
				saveScreenshot("movie");
	}
    
    if ( gWorld->getIterations() == 1 )
        gDisplayMode = backup_gDisplayMode;
    else
        if ( gWorld->getIterations() == gMaxIt-2 )
            gDisplayMode = 0;
}


void initLogging()
{
	// test log directory.
    
    /*
     
    // notes, 2014-09-02: unfortunatly, boost::filesystem is not a header-only boost library...
    // http://www.boost.org/doc/libs/1_53_0/more/getting_started/windows.html#header-only-libraries
     
    boost::filesystem::path dir (gLogDirectoryname);
    try
    {
		if (boost::filesystem::exists(dir))
	    {
			if (boost::filesystem::is_regular_file(dir))
			{
	        	std::cout << "[ERROR] directory for logging \"" << dir << "\" already exists, but is a regular file!\n";
				exit (-1);
			}
			else
				if (!boost::filesystem::is_directory(dir))
				{
					// directory does not exist. Create it.
                    std::cout << "[INFO] directory for logging \"" << dir << "\" did not exist. Creating new directory.\n";
					boost::filesystem::create_directories(dir);
				}
	    }
	    else
		{
			// directory does not exist. Create it.
            std::cout << "[INFO] directory for logging \"" << dir << "\" did not exist. Creating new directory.\n";
			boost::filesystem::create_directories(dir);
		}
	}
	catch (const boost::filesystem::filesystem_error& ex)
	{
		std::cout << ex.what() << std::endl;
		exit (-1);
	}
     
    */
    
    
    // init log file
    
    gLogFullFilename = gLogDirectoryname + "/" + gLogFilename;
    
	gLogFile.open(gLogFullFilename.c_str());//, std::ofstream::out | std::ofstream::app);
	
	if(!gLogFile) { 
		std::cout << "[CRITICAL] Cannot open log file " << gLogFullFilename << "." << std::endl << std::endl;
		exit(-1);
	} 

	gLogFile << "# =-=-=-=-=-=-=-=-=-=-=" << std::endl;
	gLogFile << "# LOG DATA " << std::endl;
	gLogFile << "# =-=-=-=-=-=-=-=-=-=-=" << std::endl;
	gLogFile << "#" << std::endl;
	gLogFile << "# =-= Roborobo^2 " << std::endl;
	gLogFile << "# =-= Official version tag    : " << gVersion << std::endl;
	gLogFile << "# =-= Current build name      : " << gCurrentBuildInfo << std::endl;
	gLogFile << "# =-= Compilation version tag : " << gCompileDate << " - " << gCompileTime << std::endl;
	gLogFile << "#" << std::endl;
	gLogFile << "# Loaded time stamp           : " << gStartTime << std::endl;
	gLogFile << "#" << std::endl;

	//gLogFile << "# log comment      : " << gLogCommentText << std::endl; 

    gLogManager = LogManager::make_DefaultLogManager(); // it is recommended (though not forced) to use gLogManager instead of gLogFile.
}


void stopLogging()
{
	gLogFile.close();
}


void initMonitor( bool __isInspectorAgent ) // default argument value is false
{
	//The frame rate regulator -- dont use them before SDL is initialized.
	timeWatch.start();
	timetag = timeWatch.get_ticks();
	
	if ( __isInspectorAgent )
	{
		gInspectorAgent = true;
		inspectorAgent = new InspectorAgent();	
	}
	else
	{
		gInspectorAgent = false;
	}
}

void updateMonitor(const Uint8* __keyboardStates)
{
	if ( timetag == -1 )
	{
		std::cout << "[CRITICAL]: Monitor not initialized (call to initMonitor() missing)." << std::endl;
		exit(-1);
	}

	if ( gPauseMode == false )
	{
		int frames = 1000;
		if ( gWorld->getIterations()%frames == 0 )
		{
			int timetag2 = timeWatch.get_ticks();
			int instantFps = (int)(frames*1000.0/((double)(timetag2-timetag)));
			if ( gVerbose ) std::cout << "[" << instantFps << " fps]" << std::endl;
			timetag = timetag2;
		}
	}
	
	if ( gInspectorAgent == true )
	{
		// update inspector agent
		if ( gDisplayMode == 0 || ( gDisplayMode == 1 && gWorld->getIterations() % gFastDisplayModeSpeed == 0 ) )
			if ( inspectorAgent != NULL && gInspectorMode )
			{
				inspectorAgent->stepBehavior(__keyboardStates);
				inspectorAgent->move();
			}
	}	
}


bool loadProperties( std::string __propertiesFilename )
{

	bool returnValue = true;

	std::ifstream in(__propertiesFilename.c_str());
	
    if ( !in.is_open() ) // WAS: if ( in == NULL )
		return false;
	gProperties.load(in);
	in.close();

	// =-=-=-=-=-=
    
    /*
     
    //todelete: 2014-09-17, deprecated
     
    // Load properties given directly in the command line parameters (if any)
    for(size_t i = 0; i < gRemainingCommandLineParameters.size(); i++)
    {
        std::string line = gRemainingCommandLineParameters[i];
        size_t equalPos = line.find("=");
        if(equalPos != std::string::npos)
        {
            line.erase(0, line.find_first_not_of("\t "));
            equalPos = line.find('=');
            std::string propertyName = line.substr(0, equalPos);
            if (propertyName.find('\t') != line.npos || propertyName.find(' ') != line.npos)
                propertyName.erase(propertyName.find_first_of("\t "));
            if ( gProperties.hasProperty(propertyName) )
            {
                gProperties.load(line);
                std::cerr << "[WARNING] configuration property \"" << propertyName << "\" is already set in configuration file. Overloading it anyway, with value \"" << gProperties.getProperty(propertyName) << "\".\n";
            }
            else
            {
                gProperties.load(line);
            }
        }
    }
    */
    
    // =-=-=-=-=-=
    
    // Load properties given in the config file
	
    std::string s;
	if ( gProperties.hasProperty("ConfigurationLoaderObjectName") )
	{
		std::string configurationLoaderObjectName = gProperties.getProperty("ConfigurationLoaderObjectName");
		gConfigurationLoader = ConfigurationLoader::make_ConfigurationLoader(configurationLoaderObjectName);
		if ( ! gConfigurationLoader )
		{
			std::cerr << "[CORRUPT] The configuration " << configurationLoaderObjectName << " isn't known by the ConfigurationLoader\n";
			returnValue = false;
		}
	}
	else
	{
		std::cerr << "[MISSING] ConfigurationLoaderObjectName string value is missing.\n";
		returnValue = false;
	}
    
	if ( gProperties.hasProperty("gNumberOfRobotGroups") )
    {
		convertFromString<int>(gNumberOfRobotGroups, gProperties.getProperty("gNumberOfRobotGroups"), std::dec);
        if ( gNumberOfRobotGroups < 1 )
        {
            std::cerr << "[ERROR] gNumberOfRobotGroups cannot be less than 1.\n";
            returnValue = false;
        }
    }
	else
		std::cerr << "[MISSING] gNumberOfRobotGroups value is missing. Assume default value (" << gNumberOfRobotGroups << ").\n";
	
	if ( gProperties.hasProperty("gInspectorAgentXStart") )
		convertFromString<int>(gInspectorAgentXStart, gProperties.getProperty("gInspectorAgentXStart"), std::dec);
	else
	{
		std::cerr << "[MISSING] gInspectorAgentXStart value is missing.\n";
		returnValue = false;
	}
	
	if ( gProperties.hasProperty("gInspectorAgentYStart") )
		convertFromString<int>(gInspectorAgentYStart, gProperties.getProperty("gInspectorAgentYStart"), std::dec);
	else
	{
		std::cerr << "[MISSING] gInspectorAgentYStart value is missing.\n";
		returnValue = false;
	}
	
	if ( gProperties.hasProperty("gDisplayMode") )
    {
		convertFromString<int>(backup_gDisplayMode, gProperties.getProperty("gDisplayMode"), std::dec);
        gDisplayMode = 0; // first iteration is displayed anyway (then gDisplayMode is switched to its intended user-value).
    }
	else
	{
		std::cerr << "[MISSING] gDisplayMode value is missing.\n";
		returnValue = false;
	}
	
	if ( gProperties.hasProperty("gFastDisplayModeSpeed") )
		convertFromString<int>(gFastDisplayModeSpeed, gProperties.getProperty("gFastDisplayModeSpeed"), std::dec);
	else
	{
		std::cerr << "[MISSING] gFastDisplayModeSpeed value is missing.\n";
		returnValue = false;
	}
	
	if ( gProperties.hasProperty("gFramesPerSecond") )
		convertFromString<int>(gFramesPerSecond, gProperties.getProperty("gFramesPerSecond"), std::dec);
	else
	{
		std::cerr << "[MISSING] gFramesPerSecond value is missing.\n";
		returnValue = false;
	}
	
	if ( gProperties.hasProperty("gLocomotionMode") )
		convertFromString<int>(gLocomotionMode, gProperties.getProperty("gLocomotionMode"), std::dec);
	else
	{
		std::cerr << "[MISSING] gLocomotionMode value is missing.\n";
		returnValue = false;
	}
		
	if ( gProperties.hasProperty("gInspectorCursorMaxSpeedOnXaxis") )
		convertFromString<int>(gInspectorCursorMaxSpeedOnXaxis, gProperties.getProperty("gInspectorCursorMaxSpeedOnXaxis"), std::dec);
	else
	{
		std::cerr << "[MISSING] gInspectorCursorMaxSpeedOnXaxis value is missing.\n";
		returnValue = false;
	}
	
	if ( gProperties.hasProperty("gInspectorCursorMaxSpeedOnYaxis") )
		convertFromString<int>(gInspectorCursorMaxSpeedOnYaxis, gProperties.getProperty("gInspectorCursorMaxSpeedOnYaxis"), std::dec);
	else
	{
		std::cerr << "[MISSING] gInspectorCursorMaxSpeedOnYaxis value is missing.\n";
		returnValue = false;
	}
	
	if ( gProperties.hasProperty("gMaxTranslationalSpeed") )
		convertFromString<float>(gMaxTranslationalSpeed, gProperties.getProperty("gMaxTranslationalSpeed"), std::dec);
	else
	{
		std::cerr << "[MISSING] gMaxTranslationalSpeed value is missing.\n";
		returnValue = false;
	}
	
	if ( gProperties.hasProperty("gMaxTranslationalDeltaValue") ) // ie. max update (delta) btw current speed and desired speed
		convertFromString<float>(gMaxTranslationalDeltaValue, gProperties.getProperty("gMaxTranslationalDeltaValue"), std::dec);
	else
	{
		std::cerr << "[MISSING] gMaxTranslationalDeltaValue value is missing.\n";
		returnValue = false;
	}
	
	if ( gProperties.hasProperty("gMaxRotationalSpeed") )
		convertFromString<float>(gMaxRotationalSpeed, gProperties.getProperty("gMaxRotationalSpeed"), std::dec);
	else
	{
		std::cerr << "[MISSING] gMaxRotationalSpeed value is missing.\n";
		returnValue = false;
	}

	if ( gProperties.hasProperty("gInspectorCursorVerticalSpeed") )
		convertFromString<int>(gInspectorCursorVerticalSpeed, gProperties.getProperty("gInspectorCursorVerticalSpeed"), std::dec);
	else
	{
		std::cerr << "[MISSING] gInspectorCursorVerticalSpeed value is missing.\n";
		returnValue = false;
	}
	
	if ( gProperties.hasProperty("gInspectorCursorHorizontalSpeed") )
		convertFromString<int>(gInspectorCursorHorizontalSpeed, gProperties.getProperty("gInspectorCursorHorizontalSpeed"), std::dec);
	else
	{
		std::cerr << "[MISSING] gInspectorCursorHorizontalSpeed value is missing.\n";
		returnValue = false;
	}

	if ( gProperties.hasProperty("gMaxIt") )
		convertFromString<int>(gMaxIt, gProperties.getProperty("gMaxIt"), std::dec);
	else
	{
		std::cerr << "[MISSING] gMaxIt value is missing.\n";
		returnValue = false;
	}
	
	if ( gProperties.hasProperty("gNbOfLandmarks") )
		convertFromString<int>(gNbOfLandmarks, gProperties.getProperty("gNbOfLandmarks"), std::dec);
	else
	{
		std::cerr << "[MISSING] gNbOfLandmarks value is missing.\n";
		returnValue = false;
	}

	if ( gProperties.hasProperty("gNbOfPhysicalObjects") )
		convertFromString<int>(gNbOfPhysicalObjects, gProperties.getProperty("gNbOfPhysicalObjects"), std::dec);
	else
	{
		std::cerr << "[MISSING] gNbOfPhysicalObjects value is missing.\n";
		returnValue = false;
	}
    
    if ( gProperties.hasProperty("gPhysicalObjectDefaultRadius") )
		convertFromString<int>(gPhysicalObjectDefaultRadius, gProperties.getProperty("gPhysicalObjectDefaultRadius"), std::dec);
	else
	{
		std::cerr << "[MISSING] gPhysicalObjectDefaultRadius value is missing. Assume value is 8.\n";
		gPhysicalObjectDefaultRadius = 8;
	}

    if ( gProperties.hasProperty("gPhysicalObjectDefaultFootprintRadius") )
		convertFromString<int>(gPhysicalObjectDefaultFootprintRadius, gProperties.getProperty("gPhysicalObjectDefaultFootprintRadius"), std::dec);
	else
	{
		std::cerr << "[MISSING] gPhysicalObjectDefaultFootprintRadius value is missing. Assume value is 14.\n";
		gPhysicalObjectDefaultFootprintRadius = 14;
	}

    if ( gProperties.hasProperty("gPhysicalObjectDefaultDisplayColorRed") )
		convertFromString<int>(gPhysicalObjectDefaultDisplayColorRed, gProperties.getProperty("gPhysicalObjectDefaultDisplayColorRed"), std::dec);
	else
	{
		std::cerr << "[MISSING] gPhysicalObjectDefaultDisplayColorRed value is missing. Assume value is " << gPhysicalObjectDefaultDisplayColorRed << ".\n";
	}

    if ( gProperties.hasProperty("gPhysicalObjectDefaultDisplayColorGreen") )
		convertFromString<int>(gPhysicalObjectDefaultDisplayColorGreen, gProperties.getProperty("gPhysicalObjectDefaultDisplayColorGreen"), std::dec);
	else
	{
		std::cerr << "[MISSING] gPhysicalObjectDefaultDisplayColorGreen value is missing. Assume value is " << gPhysicalObjectDefaultDisplayColorGreen << ".\n";
	}
    
    if ( gProperties.hasProperty("gPhysicalObjectDefaultDisplayColorBlue") )
		convertFromString<int>(gPhysicalObjectDefaultDisplayColorBlue, gProperties.getProperty("gPhysicalObjectDefaultDisplayColorBlue"), std::dec);
	else
	{
		std::cerr << "[MISSING] gPhysicalObjectDefaultDisplayColorBlue value is missing. Assume value is " << gPhysicalObjectDefaultDisplayColorBlue << ".\n";
	}
    
    if ( gProperties.hasProperty("gPhysicalObjectDefaultSolid_w") )
		convertFromString<int>(gPhysicalObjectDefaultSolid_w, gProperties.getProperty("gPhysicalObjectDefaultSolid_w"), std::dec);
	else
	{
		std::cerr << "[MISSING] gPhysicalObjectDefaultSolid_w value is missing. Assume value is 16.\n";
		gPhysicalObjectDefaultSolid_w = 16;
	}
    
    if ( gProperties.hasProperty("gPhysicalObjectDefaultSolid_h") )
		convertFromString<int>(gPhysicalObjectDefaultSolid_h, gProperties.getProperty("gPhysicalObjectDefaultSolid_h"), std::dec);
	else
	{
		std::cerr << "[MISSING] gPhysicalObjectDefaultSolid_h value is missing. Assume value is 16.\n";
		gPhysicalObjectDefaultSolid_h = 16;
	}
    
    if ( gProperties.hasProperty("gPhysicalObjectDefaultSoft_w") )
		convertFromString<int>(gPhysicalObjectDefaultSoft_w, gProperties.getProperty("gPhysicalObjectDefaultSoft_w"), std::dec);
	else
	{
		std::cerr << "[MISSING] gPhysicalObjectDefaultSoft_w value is missing. Assume value is 22.\n";
		gPhysicalObjectDefaultSoft_w = 22;
	}

    if ( gProperties.hasProperty("gPhysicalObjectDefaultSoft_h") )
		convertFromString<int>(gPhysicalObjectDefaultSoft_h, gProperties.getProperty("gPhysicalObjectDefaultSoft_h"), std::dec);
	else
	{
		std::cerr << "[MISSING] gPhysicalObjectDefaultSoft_h value is missing. Assume value is 22.\n";
		gPhysicalObjectDefaultSoft_h = 22;
	}
    
    if ( gProperties.hasProperty("gPhysicalObjectDefaultType") )
		convertFromString<int>(gPhysicalObjectDefaultType, gProperties.getProperty("gPhysicalObjectDefaultType"), std::dec);
	else
	{
		std::cerr << "[MISSING] gPhysicalObjectDefaultType value is missing. Assume value is 0.\n";
		gPhysicalObjectDefaultType = 0;
	}

    if ( gProperties.hasProperty("gPhysicalObjectDefaultRegrowTimeMax") )
		convertFromString<int>(gPhysicalObjectDefaultRegrowTimeMax, gProperties.getProperty("gPhysicalObjectDefaultRegrowTimeMax"), std::dec);
	else
	{
		std::cerr << "[MISSING] gPhysicalObjectDefaultRegrowTimeMax value is missing. Assume value is -1 (instant regrow, if applicable).\n";
		gPhysicalObjectDefaultRegrowTimeMax = -1;
	}
    
    gProperties.checkAndGetPropertyValue("gExtendedSensoryInputs",&gExtendedSensoryInputs,false);
    
    gProperties.checkAndGetPropertyValue("gEnergyLevel",&gEnergyLevel,false);
    
    gProperties.checkAndGetPropertyValue("gEnergyRefill",&gEnergyRefill,false);

    gProperties.checkAndGetPropertyValue("gEnergyMax",&gEnergyMax,false);

    gProperties.checkAndGetPropertyValue("gEnergyInit",&gEnergyInit,false);

    gProperties.checkAndGetPropertyValue("gEnergyItemDefaultInit",&gEnergyItemDefaultInit,false);

    gProperties.checkAndGetPropertyValue("gEnergyItemDefaultMode",&gEnergyItemDefaultMode,false);

	if ( gProperties.hasProperty("gInitialNumberOfRobots") )
		convertFromString<int>(gInitialNumberOfRobots, gProperties.getProperty("gInitialNumberOfRobots"), std::dec);
	else
	{
		std::cerr << "[MISSING] gInitialNumberOfRobots value is missing.\n";
		returnValue = false;
	}
	
	if ( gProperties.hasProperty("gLocationFinderMaxNbOfTrials") )
		convertFromString<int>(gLocationFinderMaxNbOfTrials, gProperties.getProperty("gLocationFinderMaxNbOfTrials"), std::dec);
	else
	{
		std::cerr << "[MISSING] gLocationFinderMaxNbOfTrials value is missing.\n";
		returnValue = false;
	}

    if ( gProperties.hasProperty("gPhysicalObjectIndexStartOffset") )
		convertFromString<int>(gPhysicalObjectIndexStartOffset, gProperties.getProperty("gPhysicalObjectIndexStartOffset"), std::dec);
	else
	{
		std::cerr << "[MISSING] gPhysicalObjectIndexStartOffset value is missing.\n";
		returnValue = false;
	}

    if ( gProperties.hasProperty("gRobotIndexStartOffset") )
		convertFromString<int>(gRobotIndexStartOffset, gProperties.getProperty("gRobotIndexStartOffset"), std::dec);
	else
	{
		std::cerr << "[MISSING] gRobotIndexStartOffset value is missing.\n";
		returnValue = false;
	}
    
	if ( gProperties.hasProperty("gRandomSeed") )
	{
		convertFromString<int>(gRandomSeed, gProperties.getProperty("gRandomSeed"), std::dec);
		
		if ( gRandomSeed == -1 ) // value = -1 means random seed. set seed, then update content of properties.
		{
			// set seed value
			gRandomSeed = (unsigned int)time(NULL); // time-based random seed, if needed.

			// update properties

			gProperties.setProperty("gRandomSeed",convertToString(gRandomSeed)); // update value.
		}
	}
	else
	{
		std::cerr << "[MISSING] gRandomSeed value is missing.\n";
		returnValue = false;
	}
		
	if ( gProperties.hasProperty("gScreenHeight") )
		convertFromString<int>(gScreenHeight, gProperties.getProperty("gScreenHeight"), std::dec);
	else
	{
		std::cerr << "[MISSING] gScreenHeight value is missing.\n";
		returnValue = false;
	}
	
	if ( gProperties.hasProperty("gScreenWidth") )
		convertFromString<int>(gScreenWidth, gProperties.getProperty("gScreenWidth"), std::dec);
	else
	{
		std::cerr << "[MISSING] gScreenWidth value is missing.\n";
		returnValue = false;
	}
	
	if ( gProperties.hasProperty("gSensorRange") )
		convertFromString<int>(gSensorRange, gProperties.getProperty("gSensorRange"), std::dec);
	else
	{
		std::cerr << "[MISSING] gSensorRange value is missing.\n";
		returnValue = false;
	}
    
    if ( gProperties.hasProperty("gTailLength") )
        convertFromString<int>(gTailLength, gProperties.getProperty("gTailLength"), std::dec);
    else
        std::cerr << "[MISSING] gTailLength value is missing. Assume default value (" << gTailLength << ").\n";

    if ( gProperties.hasProperty("gMaxRadioDistance") )
	{
        std::cerr << "[WARNING] gMaxRadioDistance is set, but is useless.\n";
        convertFromString<int>(gMaxRadioDistance, gProperties.getProperty("gMaxRadioDistance"), std::dec);
	}
	else
	{
		std::cerr << "[MISSING] gMaxRadioDistance value is missing (set to -1 if not used).\n";
		returnValue = false;
	}
    
	if ( gProperties.hasProperty("gDisplaySensors") )
	{
        convertFromString<int>(gDisplaySensors, gProperties.getProperty("gDisplaySensors"), std::dec);

        if ( gDisplaySensors < 0 || gDisplaySensors > 3 )
        {
            std::cerr << "[ERROR] gDisplaySensors value must be 0, 1, 2 or 3.\n";
            returnValue = false;
        }
	}
	else
	{
		std::cerr << "[MISSING] gDisplaySensors value is missing.\n";
		returnValue = false;
	}
    
	s = gProperties.getProperty("gRobotDisplayFocus");
	if ( s == "true" || s == "True" || s == "TRUE" ) 
		gRobotDisplayFocus = true;
	else
		if ( s == "false" || s == "False" || s == "FALSE" ) 
			gRobotDisplayFocus = false;
		else
		{
			std::cerr << "[WARNING] gRobotDisplayFocus is missing or corrupt (default is \"false\").\n";
			//returnValue = false; // not critical
		}	

	s = gProperties.getProperty("gBatchMode");
	if ( s == "true" || s == "True" || s == "TRUE" ) 
		gBatchMode = true;
	else
		if ( s == "false" || s == "False" || s == "FALSE" ) 
			gBatchMode = false;
		else
		{
			std::cerr << "[CORRUPT] gBatchMode should be boolean (\"true\" or \"false\").\n";
			returnValue = false;
		}
		
	s = gProperties.getProperty("gTrajectoryMonitor");
	if ( s == "true" || s == "True" || s == "TRUE" )
		gTrajectoryMonitor = true;
	else
		if ( s == "false" || s == "False" || s == "FALSE" )
			gTrajectoryMonitor = false;
		else
		{
			std::cerr << "[WARNING] gTrajectoryMonitor is missing or corrupt (default is \"false\").\n";
			//returnValue = false;
		}
    
	s = gProperties.getProperty("gVideoRecording");
	if ( s == "true" || s == "True" || s == "TRUE" ) 
		gVideoRecording = true;
	else
		if ( s == "false" || s == "False" || s == "FALSE" ) 
			gVideoRecording = false;
		else
		{
			std::cerr << "[WARNING] gVideoRecording is missing or corrupt (default is \"false\").\n";
			//returnValue = false;
		}
    
	s = gProperties.getProperty("gRobotLEDdisplay");
	if ( s == "true" || s == "True" || s == "TRUE" )
		gRobotLEDdisplay = true;
	else
		if ( s == "false" || s == "False" || s == "FALSE" )
			gRobotLEDdisplay = false;
		else
		{
			std::cerr << "[WARNING] gRobotLEDdisplay is missing (default is \"true\").\n";
			//returnValue = false;
		}
    
	s = gProperties.getProperty("gDisplayTail");
	if ( s == "true" || s == "True" || s == "TRUE" )
		gDisplayTail = true;
	else
		if ( s == "false" || s == "False" || s == "FALSE" )
			gDisplayTail = false;
		else
		{
			std::cerr << "[WARNING] gDisplayTail is missing (default is \"true\").\n";
			//returnValue = false;
		}
    
	s = gProperties.getProperty("gPhysicalObjectsRedraw");
	if ( s == "true" || s == "True" || s == "TRUE" )
		gPhysicalObjectsRedraw = true;
	else
		if ( s == "false" || s == "False" || s == "FALSE" )
			gPhysicalObjectsRedraw = false;
		else
		{
			std::cerr << "[CORRUPT] gPhysicalObjectsRedraw should be boolean (\"true\" or \"false\").\n";
			returnValue = false;
		}

	s = gProperties.getProperty("gMonitorRobot");
	if ( s == "true" || s == "True" || s == "TRUE" ) 
		gMonitorRobot = true;
	else
		if ( s == "false" || s == "False" || s == "FALSE" ) 
			gMonitorRobot = false;
		else
		{
			std::cerr << "[CORRUPT] gMonitorRobot should be boolean (\"true\" or \"false\").\n";
			returnValue = false;
		}
		
	s = gProperties.getProperty("gInspectorAgent");
	if ( s == "true" || s == "True" || s == "TRUE" ) 
		gInspectorAgent = true;
	else
		if ( s == "false" || s == "False" || s == "FALSE" ) 
			gInspectorAgent = false;
		else
		{
			std::cerr << "[CORRUPT] gInspectorAgent should be boolean (\"true\" or \"false\").\n";
			returnValue = false;
		}
	
	s = gProperties.getProperty("gInspectorMode");
	if ( s == "true" || s == "True" || s == "TRUE" ) 
		gInspectorMode = true;
	else
		if ( s == "false" || s == "False" || s == "FALSE" ) 
			gInspectorMode = false;
		else
		{
			std::cerr << "[CORRUPT] gInspectorMode should be boolean (\"true\" or \"false\").\n";
			returnValue = false;
		}

	s = gProperties.getProperty("gNiceRendering");
	if ( s == "true" || s == "True" || s == "TRUE" ) 
		gNiceRendering = true;
	else
		if ( s == "false" || s == "False" || s == "FALSE" ) 
			gNiceRendering = false;
		else
		{
			std::cerr << "[CORRUPT] gNiceRendering should be boolean (\"true\" or \"false\").\n";
			returnValue = false;
		}

	s = gProperties.getProperty("gPauseMode");
	if ( s == "true" || s == "True" || s == "TRUE" ) 
		gPauseMode = true;
	else
		if ( s == "false" || s == "False" || s == "FALSE" ) 
			gPauseMode = false;
		else
		{
			std::cerr << "[CORRUPT] gPauseMode should be boolean (\"true\" or \"false\").\n";
			returnValue = false;
		}

	s = gProperties.getProperty("gUserCommandMode");
	if ( s == "true" || s == "True" || s == "TRUE" ) 
		gUserCommandMode = true;
	else
		if ( s == "false" || s == "False" || s == "FALSE" ) 
			gUserCommandMode = false;
		else
		{
			std::cerr << "[CORRUPT] gUserCommandMode should be boolean (\"true\" or \"false\").\n";
			returnValue = false;
		}
	
    if ( gVerbose_commandlineargument == false ) // command line argument overrules properties file
    {
        s = gProperties.getProperty("gVerbose");
        if ( s == "true" || s == "True" || s == "TRUE" )
            gVerbose = true;
        else
            if ( s == "false" || s == "False" || s == "FALSE" )
                gVerbose = false;
            else
            {
                std::cerr << "[CORRUPT] gVerbose should be boolean (\"true\" or \"false\").\n";
                returnValue = false;
            }
    }
	
	s = gProperties.getProperty("gRadioNetwork");
	if ( s == "true" || s == "True" || s == "TRUE" ) 
		gRadioNetwork = true;
	else
		if ( s == "false" || s == "False" || s == "FALSE" ) 
			gRadioNetwork = false;
		else
		{
			std::cerr << "[CORRUPT] gRadioNetwork should be boolean (\"true\" or \"false\").\n";
			returnValue = false;
		}
    
	s = gProperties.getProperty("gPhysicalObjectDefaultRelocate");
	if ( s == "true" || s == "True" || s == "TRUE" )
		gPhysicalObjectDefaultRelocate = true;
	else
		if ( s == "false" || s == "False" || s == "FALSE" )
			gPhysicalObjectDefaultRelocate = false;
		else
		{
			std::cerr << "[CORRUPT] gPhysicalObjectDefaultRelocate should be boolean (\"true\" or \"false\").\n";
			returnValue = false;
		}
    
	s = gProperties.getProperty("gPhysicalObjectDefaultOverwrite");
	if ( s == "true" || s == "True" || s == "TRUE" )
		gPhysicalObjectDefaultOverwrite = true;
	else
		if ( s == "false" || s == "False" || s == "FALSE" )
			gPhysicalObjectDefaultOverwrite = false;
		else
		{
			std::cerr << "[CORRUPT] gPhysicalObjectDefaultOverwrite should be boolean (\"true\" or \"false\").\n";
			returnValue = false;
		}
	
	if ( gProperties.hasProperty("gTrajectoryMonitor") ) // optional
    {
        s = gProperties.getProperty("gTrajectoryMonitor");
        if ( s == "true" || s == "True" || s == "TRUE" ) 
            gTrajectoryMonitor = true;
        else
            if ( s == "false" || s == "False" || s == "FALSE" ) 
                gTrajectoryMonitor = false;
            else
            {
                std::cerr << "[CORRUPT] gTrajectoryMonitor should be boolean (\"true\" or \"false\").\n";
                returnValue = false;
            }
        
        if ( gProperties.hasProperty("gTrajectoryMonitorMode") ) // optional
            convertFromString<int>(gTrajectoryMonitorMode, gProperties.getProperty("gTrajectoryMonitorMode"), std::dec);
    }


	if ( gProperties.hasProperty("gRobotMaskImageFilename") )
		gRobotMaskImageFilename = gProperties.getProperty("gRobotMaskImageFilename");
	else
	{
		std::cerr << "[MISSING] gRobotMaskImageFilename string value is missing.\n";
		returnValue = false;
	}
	
	if ( gProperties.hasProperty("gRobotDisplayImageFilename") )
		gRobotDisplayImageFilename = gProperties.getProperty("gRobotDisplayImageFilename");
	else
	{
		std::cout << "[WARNING] gRobotDisplayImageFilename string value is missing (value will be copied from gRobotMaskImageFilename).\n";
        gRobotDisplayImageFilename = gProperties.getProperty("gRobotMaskImageFilename");
	}
	
    if ( gProperties.hasProperty("gRobotSpecsImageFilename") )
		gRobotSpecsImageFilename = gProperties.getProperty("gRobotSpecsImageFilename");
	else
	{
		std::cerr << "[MISSING] gRobotSpecsImageFilename string value is missing.\n";
		returnValue = false;
	}
	
	if ( gProperties.hasProperty("gBackgroundImageFilename") )
		gBackgroundImageFilename = gProperties.getProperty("gBackgroundImageFilename");
	else
	{
		std::cerr << "[MISSING] gBackgroundImageFilename string value is missing.\n";
		returnValue = false;
	}
	
	if ( gProperties.hasProperty("gEnvironmentImageFilename") )
		gEnvironmentImageFilename = gProperties.getProperty("gEnvironmentImageFilename");
	else
	{
		std::cerr << "[MISSING] gEnvironmentImageFilename string value is missing.\n";
		returnValue = false;
	}
	
	if ( gProperties.hasProperty("gForegroundImageFilename") )
		gForegroundImageFilename = gProperties.getProperty("gForegroundImageFilename");
	else
	{
		std::cerr << "[MISSING] gForegroundImageFilename string value is missing.\n";
		returnValue = false;
	}
	
	if ( gProperties.hasProperty("gLogCommentText") )
		gLogCommentText = gProperties.getProperty("gLogCommentText");
	else
	{
		std::cerr << "[MISSING] gLogCommentText string value is missing.\n";
		returnValue = false;
	}
	
	if ( gProperties.hasProperty("gLogFilename") )
		gLogFilename = gProperties.getProperty("gLogFilename");
	else
	{
		gLogFilename = "datalog_" + gStartTime + ".txt";
		gProperties.setProperty("gLogFilename",gLogFilename);

		std::cout << "[WARNING] No default gLogFilename string value. Log data will be written in \"" << gLogFilename << "\"\n";
		//returnValue = false;
	}

    if ( gLogDirectoryname_commandlineargument == true ) // command line argument overrules properties file
    {
        if ( gProperties.hasProperty("gLogDirectoryname") )
            gLogDirectoryname = gProperties.getProperty("gLogDirectoryname");
        else
        {
            gLogDirectoryname = "logs/";
            gProperties.setProperty("gLogDirectoryname",gLogDirectoryname);
            
            std::cout << "[WARNING] No default gLogDirectoryname string value. Log data will be written in \"" << gLogDirectoryname << "\"\n";
            //returnValue = false;
        }
    }
    
	if ( gProperties.hasProperty("gGroundSensorImageFilename") )
		gGroundSensorImageFilename = gProperties.getProperty("gGroundSensorImageFilename");
	else
	{
		std::cerr << "[MISSING] gGroundSensorImageFilename string value is missing.\n";
		returnValue = false;
	}
	


	// * Dump a raw copy of the properties file from gProperties, ie. as it was parsed and understood.

	// prepare filename
	std::string outputFileNameTmp = gLogDirectoryname;
    outputFileNameTmp += "/";
	outputFileNameTmp += "properties_";
	outputFileNameTmp += gStartTime;
	outputFileNameTmp += ".txt";
	
	// open file
	const std::string outputFile = outputFileNameTmp;
	std::ofstream out(outputFile.c_str());	
	
	// dump header information
	out << "# =-=-=-=-=-=-=-=-=-=-=" << std::endl;
	out << "# PROPERTIES FILE DUMP " << std::endl;
	out << "# =-=-=-=-=-=-=-=-=-=-=" << std::endl;
	out << "#" << std::endl;
	out << "# =-= Roborobo^2 " << std::endl;
	out << "# =-= Official version tag    : " << gVersion << std::endl;
	out << "# =-= Current build name      : " << gCurrentBuildInfo << std::endl;
	out << "# =-= Compilation version tag : " << gCompileDate << " - " << gCompileTime << std::endl;
	out << "#" << std::endl;
	out << "# Loaded time stamp           : " << gStartTime << std::endl;
	out << "#" << std::endl;
	out << "# Original Properties file    : " << __propertiesFilename << std::endl;
	out << "#" << std::endl;
	out << std::endl << std::endl;
		
	// dump properties content
	gProperties.store(out); 
	
	// close file
	out.close();
	
	return returnValue;
}



// ---- ---- ---- ----“

void initRoborobo()
{
    
	// load properties
	if ( loadProperties(gPropertiesFilename) == false )
	{
		std::cout << "[CRITICAL] properties file contains error(s) or does not exist (\"" << gPropertiesFilename << "\"). EXITING." << std::endl << std::endl;
		exit (-1);
	}

	if ( gBatchMode == false && gVerbose == true )
		displayHelp(); // display welcome text and help commands
	
	// * Initialize SDL	
	
	gCamera.x = 0;
	gCamera.y = 0;
	gCamera.w = gScreenWidth;
	gCamera.h = gScreenHeight;

    if( initSDL(SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE) == false )
	{
		std::cout << "[CRITICAL] cannot initialize SDL: " << SDL_GetError() << std::endl << std::endl;
		exit(-2);
	}
    
	// * Initialize log file(s)
	
	initLogging();
	
    
	// * Initialize Random seed -- loaded, or initialized, in loadProperties(...)
	
	srand(gRandomSeed); // fixed seed - useful to reproduce results (ie. deterministic sequence of random values)
	gLogFile << "# random seed             : " << gRandomSeed << std::endl; 

	gWorld = new World();

	// * run
	gWorld->initWorld();

	if ( gBatchMode == false )
		initMonitor(true); // add speed monitoring and inspector agent
}

bool runRoborobo(int __maxIt) // default parameter is -1 (infinite)
{
	//While the user hasn't quit
	bool quit = false;
	int currentIt = 0;
	while( quit == false && ( currentIt < __maxIt || __maxIt == -1 ) )
	{
		if ( gBatchMode )
		{
			gWorld->updateWorld();
			if ( gWorld->getIterations()%10000 == 0 )
				if ( gVerbose ) std::cout << ".";
            
            // monitor trajectories (if needed)
            if ( gTrajectoryMonitor == true )
                updateTrajectoriesMonitor();
		}
		else
		{
			const Uint8 *keyboardStates = SDL_GetKeyboardState( NULL );
			quit = checkQuitEvent() | handleKeyEvent(keyboardStates);
            
			//Start the frame timer
			fps.start();
			
			if ( gPauseMode == false )
			{
				if ( gUserCommandMode == true && gInspectorMode == false )
					gWorld->updateWorld(keyboardStates);
				else
					gWorld->updateWorld();
			}
            
			//Update the screen
			updateDisplay();
            
            // monitor trajectories (if needed)
            if ( gTrajectoryMonitor == true )
                updateTrajectoriesMonitor();
            
			updateMonitor(keyboardStates);
		}
		
		currentIt++;
		if (gWorld->getNbOfRobots() <= 0)
		{
			quit = true;
		}
    }
    
	return quit;
}


void closeRoborobo()
{
	// * clean up and quit
    
    gStopTime = getCurrentTimeAsReadableString();
    gStopTimeRawFormat = time(0);

    time_t elapsed = int(gStopTimeRawFormat - gStartTimeRawFormat);
    int seconds = (int)elapsed;
    int minutes = seconds/60;
    seconds = seconds - ( minutes * 60 );
    int hours = minutes/60;
    minutes = minutes - (hours * 60 );
    int days = hours/24;
    hours = hours - (days*24);
    
    gLogFile << "# Started: " << gStartTime << std::endl;
    gLogFile << "# Stopped: " << gStopTime << std::endl;
    gLogFile << "# Elapsed: ";
    if ( days > 0 )
        gLogFile << days << " day(s), ";
    if ( hours > 0 )
        gLogFile << hours << " hour(s), ";
    if ( minutes > 0 )
        gLogFile << minutes << " minutes(s), ";
    gLogFile <<  seconds << " second";
    if ( seconds > 1 )
        gLogFile << "s";
    gLogFile << "." << std::endl;

    
	stopLogging();
	clean_up();
	SDL_Quit();

    std::cout << std::endl;
    std::cout << "[timestamp::start] " << gStartTime << std::endl;
    std::cout << "[timestamp::stop ] " << gStopTime << std::endl;
    std::cout << "[Chronometer] ";
    if ( days > 0 )
        std::cout << days << " day(s), ";
    if ( hours > 0 )
        std::cout << hours << " hour(s), ";
    if ( minutes > 0 )
        std::cout << minutes << " minutes(s), ";
    std::cout << seconds << " second";
    if ( seconds > 1 )
        std::cout << "s";
    std::cout << "." << std::endl << std::endl;
}

void resetRoboroboWorld()
{
	gWorld->resetWorld();
}

void initTrajectoriesMonitor()
{
    if ( !gTrajectoryMonitor )
    {
        gTrajectoryMonitorImage = load_image( gEnvironmentImageFilename ); // clean/prepare image
        gTrajectoryMonitorIt = gWorld->getIterations();
        gTrajectoryMonitor = true;
    }
    else
        std::cout << "[WARNING] cannot monitor trajectories. Monitor already in use.\n";

}

void updateTrajectoriesMonitor()
{
    int indexStart = 0;
    int indexStop = gNumberOfRobots;
    
    if ( gTrajectoryMonitorMode == 1 ) // exception: monitor only agent with focus
    {   
        indexStart = gRobotIndexFocus;
        indexStop = indexStart+1;
    }
    
    for ( int i = indexStart ; i != indexStop ; i++ ) // test for agents proximity based on localization 
    {
        int x = (int)(gWorld->getRobot(i)->getWorldModel()->getXReal());
        int y = (int)(gWorld->getRobot(i)->getWorldModel()->getYReal());
        
        Uint32 pixel = getPixel32(gTrajectoryMonitorImage,x,y);
        Uint8 r, g, b;
		SDL_GetRGB(pixel,gTrajectoryMonitorImage->format,&r,&g,&b);
        

        
        /*
        // METHOD 1 : RED lines
        if ( r < 0xFF && r > 0x01 )
            putPixel32( gTrajectoryMonitorImage, x, y,  SDL_MapRGBA( gTrajectoryMonitorImage->format, (r-10), 0x00, 0x00, SDL_ALPHA_OPAQUE ) );
        else
            if ( r == 0xFF )
                putPixel32( gTrajectoryMonitorImage, x, y,  SDL_MapRGBA( gTrajectoryMonitorImage->format, 0xFE, 0x00, 0x00, SDL_ALPHA_OPAQUE ) );
            else
                putPixel32( gTrajectoryMonitorImage, x, y,  SDL_MapRGBA( gTrajectoryMonitorImage->format, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE ) ); // saturation
        */
        
        // METHOD 2 : LIGHT TO DARK GREY
        /*
        if ( r > 0x00 )
            if ( r == 0xFF )
                putPixel32( gTrajectoryMonitorImage, x, y,  SDL_MapRGBA( gTrajectoryMonitorImage->format, 0xF0, 0xF0, 0xF0, SDL_ALPHA_OPAQUE ) );
            else
                putPixel32( gTrajectoryMonitorImage, x, y,  SDL_MapRGBA( gTrajectoryMonitorImage->format, (r-1), (r-1), (r-1), SDL_ALPHA_OPAQUE ) );
        */

        // METHOD 3 : LIGHT TO DARK RED (with starting threshold)
        if ( g > 0x00 )
        {
            if ( g == 0xFF )
            {
                putPixel32( gTrajectoryMonitorImage, x, y,  SDL_MapRGBA( gTrajectoryMonitorImage->format, 0xFF, 0xA0, 0xA0, SDL_ALPHA_OPAQUE ) );
            }
            else
            {
                putPixel32( gTrajectoryMonitorImage, x, y,  SDL_MapRGBA( gTrajectoryMonitorImage->format, 0xFF, (g-16), (g-16), SDL_ALPHA_OPAQUE ) );
            }
        }
    }
}


// ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----


int launchRoborobo() // the main loop.
{
    initRoborobo();
	runRoborobo(gMaxIt);
	closeRoborobo();

	return 0;
}

