/*
 *  World.cpp
 *  roborobo
 *
 *  Created by Nicolas on 16/01/09.
 *
 */


#include "World/World.h"

#include "zsu/Properties.h"

#include "Config/GlobalConfigurationLoader.h"


/********/

/**/
//#include "tbb/blocked_range.h"
//#include "tbb/parallel_for.h"
//#include "tbb/task_scheduler_init.h"
#include <iostream>
#include <vector>

/*
struct executor
{
    std::vector<int>& _vect;
    executor(std::vector<int>& t) : _vect(t) {}
    //    executor(executor& e,tbb::split):_tasks(e._tasks) {}
    
    void operator()(const tbb::blocked_range<size_t>& r) const
    {
        for (size_t i=r.begin();i!=r.end();++i)
            _vect[i] = 0;
        std::cout << "testing TBB\n";  // will be messed up because of // cout.
    }
};

int initElement() { return (0); }

static void testTBB2()
{
    tbb::task_scheduler_init init;  // Automatic number of threads
    // tbb::task_scheduler_init init(2);  // Explicit number of threads
    
    std::vector<int> vect(100000000); //100000000);
    //std::generate(vect.begin(), vect.end(), initElement);
    
    executor exec(vect);
    tbb::parallel_for(tbb::blocked_range<size_t>(0,vect.size()-1),executor(vect));
    //tbb::parallel_for(size_t(0), size_t(vect.size()),executor(vect));
    std::cerr << std::endl;
}
*/


/********/

World::World()
{
    //testTBB2();
    
	_iterations = 0;
	_agentsVariation = false;
	_worldObserver = gConfigurationLoader->make_WorldObserver(this);
}



World::~World()
{
	for ( int i = 0 ; i != gNumberOfRobots ; i++ )
		if ( robots[i] != NULL ) delete robots[i];
	
	delete _worldObserver;
}

void World::initWorld()
{
	// * load environment and agents files
    if( loadFiles() == false )
	{
		std::cout << "[CRITICAL] cannot load image files." << std::endl;
		exit(-1);
	}
    
    // * initialize landmarks
    
    for ( int i = 0 ; i != gNbOfLandmarks ; i++)
    {
        LandmarkObject landmarkObject;
        gLandmarks.push_back(landmarkObject);
    }
    
    // * initialize physical objects
    
    for ( int i = 0 ; i != gNbOfPhysicalObjects ; i++)
    {
        PhysicalObjectFactory::makeObject();
    }
    
    // * Analyse agent mask and make it into a list of coordinates
    
    int nbPointsInMask = 0;
    
    // count number of significant pixels in mask.
    
    for ( int i = 0 ; i != gRobotWidth ; i++ ) 
        for ( int j = 0 ; j != gRobotHeight ; j++ )
            if ( getPixel32( gRobotMaskImage , i , j) != SDL_MapRGBA( gForegroundImage->format, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE ) )
            {
                nbPointsInMask++;
            }

    gRobotMaskData.resize(nbPointsInMask);
	for ( int i = 0 ; i != nbPointsInMask ; i++)
        (gRobotMaskData.at(i)).reserve(2);
    
    // count number of significant pixels in mask.
    
    int currentIndex = 0;
    for ( int i = 0 ; i != gRobotWidth ; i++ ) 
        for ( int j = 0 ; j != gRobotHeight ; j++ )
            if ( getPixel32( gRobotMaskImage , i , j) != SDL_MapRGBA( gForegroundImage->format, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE ) )
            {
                gRobotMaskData[currentIndex][0]=i;
                gRobotMaskData[currentIndex][1]=j;
                currentIndex++;
            }
    
	// * initialize agents
    
	for ( int i = 0 ; i != gInitialNumberOfRobots ; i++ )
	{
		Robot *robot = new Robot(this);
        this->addRobot(robot);
	}
    
	_worldObserver->reset();

    for ( int i = 0 ; i != gNumberOfRobots ; i++ )
		robotRegistry[i]=false;
}

void World::resetWorld()
{
	gRobotIndexFocus = 0;
	_iterations = 0;
	
	for ( int i = 0 ; i != gNumberOfRobots ; i++ )
	{
		robots[i]->unregisterRobot();
		robots[i]->reset();
		robots[i]->registerRobot();
		robotRegistry[i]=true;
	}
}


void World::updateWorld(const Uint8 *__keyboardStates)
{
    // update physical object, if any
    for( std::vector<PhysicalObject*>::iterator it = gPhysicalObjects.begin(); it != gPhysicalObjects.end(); ++it )
	{
		(*it)->step();
	}
    
    // update landmark object, if any
	for (std::vector<LandmarkObject>::iterator it = gLandmarks.begin(); it < gLandmarks.end() ; it++ )
	{
		it->step();
	}
    
	// update world level observer
	_worldObserver->step();


    // * update agents
    // Remark: agents are updated *in random order* so as to avoid "effect" order (e.g. low index agents moves first).
	// => create an array that contains shuffled indexes. Used afterwards for randomly update agents.
	//    This is very important to avoid possible nasty effect from ordering such as "agents with low indexes moves first"
	//    outcome: among many iterations, the effect of ordering is reduced.
	//    This means that roborobo is turn-based, with stochastic update ordering within one turn
	
    int shuffledIndex[gNumberOfRobots];
    
    for ( int i = 0 ; i < gNumberOfRobots ; i++ )
        shuffledIndex[i] = i;
	
    for ( int i = 0 ; i < gNumberOfRobots-1 ; i++ ) // exchange randomly indexes with one other
    {
        int r = i + (rand() % (gNumberOfRobots-i)); // Random remaining position.
        int tmp = shuffledIndex[i];
        shuffledIndex[i] = shuffledIndex[r];
        shuffledIndex[r] = tmp;
    }

	// update agent level observers
	for ( int i = 0 ; i != gNumberOfRobots ; i++ )
		robots[i]->callObserver();


	// * update world and agents

	// controller step
	for ( int i = 0 ; i < gNumberOfRobots ; i++ )
	{
		if ( __keyboardStates == NULL )
			robots[shuffledIndex[i]]->stepBehavior();
		else
		{
			if ( shuffledIndex[i] == gRobotIndexFocus )
				robots[shuffledIndex[i]]->stepBehavior(__keyboardStates);
			else
				robots[shuffledIndex[i]]->stepBehavior();
		}
	}

	// * move the agent -- apply (limited) physics
	for ( int i = 0 ; i < gNumberOfRobots ; i++ )
	{
		// unregister itself (otw: own sensor may see oneself)
		if ( robotRegistry[shuffledIndex[i]] )
		{
			robots[shuffledIndex[i]]->unregisterRobot();
		}

		// move agent
		robots[shuffledIndex[i]]->move();
			
        // register robot (remark: always register is fine with small robots and/or high density)
        robots[shuffledIndex[i]]->registerRobot();
        robotRegistry[shuffledIndex[i]]=true;
    }
    
    gLogManager->flush();
    
	_iterations++;
    
    if ( gRefreshUserDisplay == true )
    {
        gRefreshUserDisplay = false;
    }
}


bool World::loadFiles()
{
	bool returnValue = true;

    // Load the dot image
    
    gRobotMaskImage = load_image( gRobotMaskImageFilename );
    gRobotDisplayImage = load_image( gRobotDisplayImageFilename );
	
	// Load the agent specifications image
    
	gRobotSpecsImage = load_image( gRobotSpecsImageFilename ); // no jpg (color loss)	

    // Load the foreground image (active borders)
    
    gForegroundImage = load_image( gForegroundImageFilename );   // RECOMMENDED: png rather than jpeg (pb with transparency otw)
	if ( gForegroundImageFilename.compare(gForegroundImageFilename.length()-3, 3, "jpg", 0, 3) == 0 )
	{
		std::cerr << "foreground: PNG format is *mandatory* (JPG may feature transparency problems due to compression with loss)\n";
		returnValue = false;
	}
	
    gEnvironmentImage = load_image( gEnvironmentImageFilename );
	if ( gEnvironmentImageFilename.compare(gEnvironmentImageFilename.length()-3, 3, "jpg", 0, 3) == 0 )
	{
		std::cerr << "environment: PNG format is *mandatory* (JPG may feature transparency problems due to compression with loss)\n";
		returnValue = false;
    }
	
    //gTrajectoryMonitorImage = load_image( gEnvironmentImageFilename ); // prepare for logging trajectories (useful if requested in the config file)   ---- // Created in roborobo::initTrajectoriesMonitor

	// load background image
	
    gBackgroundImage = load_image( gBackgroundImageFilename );

	// Load the ground type image
	
    gGroundSensorImage = load_image( gGroundSensorImageFilename );
	
    // Managing problems with loading files (agent mask and specs)
    
    if( gRobotMaskImage == NULL )
    {
		std::cerr << "Could not load agent mask image\n";
		returnValue = false;
    }
	
    if ( gRobotDisplayImage == NULL )
    {
		std::cerr << "Could not load agent display image\n";
		returnValue = false;
    }
            
    if( gRobotSpecsImage == NULL )
    {
		std::cerr << "Could not load agent specification image\n";
		returnValue = false;
    }
	
    //If there was a problem in loading the foreground image
    if( gForegroundImage == NULL )
    {
		std::cerr << "Could not load foreground image\n";
		returnValue = false;
    }
	
	if ( gEnvironmentImage == NULL )
    {
		std::cerr << "Could not load environment image\n";
		returnValue = false;
    }	
	
	
	//no background image (not a critical error)
	if ( gBackgroundImage == NULL )
	{
		std::cout << "warning: could not load background image (will proceed anyway)\n";
	}
	
	// mandatory: image dimensions must be more than 1024x768 (otw: screen underfitting)
	if ( gForegroundImage->w < gScreenWidth || gForegroundImage->h < gScreenHeight )
	{
		std::cerr << "foreground image dimensions must be " << gScreenWidth << "x" << gScreenHeight << " or higher (given: " << gForegroundImage->w << "x" << gForegroundImage->h << ") \n";
		returnValue = false;
	}
	
	//If there was a problem in loading the ground type image
    if(  gGroundSensorImage == NULL )
    {
		std::cerr << "Could not load ground image\n";
        returnValue = false;    
    }
	else
    {
        if( ( gGroundSensorImage->w != gForegroundImage->w ) || ( gGroundSensorImage->h != gForegroundImage->h ) )
        {
            std::cerr << "Ground image dimensions do not match that of the foreground image\n";
            returnValue = false;
        }
    }
	
	// set reference dimensions
	gRobotWidth = gRobotMaskImage->w ;
	gRobotHeight = gRobotMaskImage->h ;
	
	if ( gMaxTranslationalSpeed > gRobotWidth || gMaxTranslationalSpeed > gRobotHeight )
	{
		std::cerr << "[ERROR] gMaxTranslationalSpeed value *should not* be superior to agent dimensions (image width and/or height) -- may impact collision accuracy (e.g. teleporting through walls)\n";
		returnValue = false;
	}
	
	gAreaWidth = gForegroundImage->w;
	gAreaHeight = gForegroundImage->h;

	// set transparency color
	SDL_SetColorKey( gRobotMaskImage, SDL_TRUE, SDL_MapRGBA( gRobotMaskImage->format, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE ) );
   	SDL_SetColorKey( gRobotDisplayImage, SDL_TRUE, SDL_MapRGBA( gRobotMaskImage->format, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE ) );

	SDL_SetColorKey( gForegroundImage, SDL_TRUE, SDL_MapRGBA( gForegroundImage->format, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE ) );
	SDL_SetColorKey( gEnvironmentImage, SDL_TRUE, SDL_MapRGBA( gEnvironmentImage->format, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE ) );

	// preparing Environment Image (ie. anything not color white is an obstacle)
	for ( int x = 0 ; x != gEnvironmentImage->w ; x++ )
		for ( int y = 0 ; y != gEnvironmentImage->h ; y++ )
		{
			Uint32 pixel = getPixel32(gEnvironmentImage,x,y);
			if ( pixel != SDL_MapRGBA( gEnvironmentImage->format, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE ) )
				//putPixel32( gEnvironmentImage, x, y,  SDL_MapRGBA( gEnvironmentImage->format, 0, 0, pixel&0x0000FF, SDL_ALPHA_OPAQUE ) ); // only the blue component is used
                putPixel32( gEnvironmentImage, x, y,  SDL_MapRGBA( gEnvironmentImage->format, 0, 0, 0, SDL_ALPHA_OPAQUE ) ); // BLACK
		}

    //If everything loaded fine
	if ( returnValue == false ) 
		return false;
	else
		return true;
}


Robot* World::getRobot( int index )
{
	return robots[index];
}

bool World::isRobotRegistered( int index )
{
	return  robotRegistry[index];
}

// Cf. World.h for explanation about why this function should NEVER be called -- function is implemented as is only to avoid any further temptation.
void World::deleteRobot (int index )
{
    std::cerr << "[CRITICAL] World::deleteRobot should NEVER be called (instead: inactivate+unregister robot ; ordering by index must be kept).\n";
    exit(-1);
    /*
	agents[__agentIndex]->unregisterRobot();
	agents.erase(agents.begin() + __agentIndex);
	robotRegistry.erase(robotRegistry.begin() + __agentIndex);
	gNumberOfRobots --;
	_agentsVariation = true;
    */
}

void World::addRobot(Robot *robot)
{
	robots.push_back(robot);
	robotRegistry.push_back(true);
	_agentsVariation = true;
}

void World::deleteLandmark (int __index )
{
	gLandmarks.erase(gLandmarks.begin() + __index );
}

//void World::addLandmark( LandmarkObject* objectToAdd )
//{
//	gLandmarks.push_back(objectToAdd);
//}

int World::getIterations()
{
	return _iterations;
}

WorldObserver* World::getWorldObserver()
{
	return _worldObserver;
}

int World::getNbOfRobots()
{
	return (int)robots.size();
}

