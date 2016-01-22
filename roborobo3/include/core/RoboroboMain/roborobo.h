/*
 *  roborobo.h
 *  roborobo
 *
 *  Created by Nicolas on 16/12/08.
 *
 */

#ifndef ROBOROBO_H
#define ROBOROBO_H

#include "RoboroboMain/common.h"
#include "Utilities/ExtendedProperties.h"
#include "Utilities/LogManager.h"
#include "World/LandmarkObject.h"

#include "World/PhysicalObjectFactory.h"
#include "World/PhysicalObject.h"

#include <boost/multi_array.hpp>
//#include <boost/filesystem.hpp> // unfortunately, not a header-only part of boost (cf. http://www.boost.org/doc/libs/1_53_0/more/getting_started/windows.html#header-only-libraries)

#include <string.h>
#include <vector>

// project-wide Global data

#define PAUSE_COMMAND 200 //400 // Pause between each user keyboard command (GUI mode only)

extern ExtendedProperties gProperties;

extern std::string gPropertiesFilename; // default is default.properties
extern std::string gCurrentBuildInfo;  // display through "-v[ersion]" command line option -- check/set value in config.h
extern std::string gStartTime; // a string containing a readable form of the time when the current program was started (useful for naming log files).
extern std::string gStopTime; // a string containing a readable form of the time when the current program was stopped.
extern time_t gStartTimeRawFormat;
extern time_t gStopTimeRawFormat;

extern std::string gCompileTime;
extern std::string gCompileDate;

extern bool gVideoRecording;

extern bool gTrajectoryMonitor;
extern int gTrajectoryMonitorMode;

// extern std::vector<std::string> gRemainingCommandLineParameters;     //todelete: 2014-09-17, deprecated


// files

extern std::string gLogCommentText; // user comment that will be written in the log file (e.g. description of experimental setup)
extern bool gVerbose_commandlineargument;
extern bool gLogDirectoryname_commandlineargument;
extern std::string gLogDirectoryname;
extern std::string gLogFilename;
extern std::string gLogFullFilename;

extern std::ofstream gLogFile;
extern LogManager *gLogManager;

extern std::string gRobotMaskImageFilename;
extern std::string gRobotDisplayImageFilename;
extern std::string gRobotSpecsImageFilename;
extern std::string gForegroundImageFilename;
extern std::string gEnvironmentImageFilename;
extern std::string gBackgroundImageFilename;
extern std::string gGroundSensorImageFilename;
extern std::string gConfigFilename;

// general purpose

extern int gRandomSeed;				// random seed. Default value (="-1") means time based.

extern bool gVerbose;				// trigger console verbose on/off
extern bool gBatchMode;				// trigger no display / no verbose mode.
extern int  gDisplayMode;			// [0]: 60fps standard display speed, [1]: ignore some refresh (see gFastDisplayModeSpeed value), [2]: fastest, but no display)
extern bool gRefreshUserDisplay;    // used to force refresh of screen (only for the user's eyes in displayMode =0 or =1

extern int backup_gDisplayMode;

									// much faster without display, in mode [0] (default); display speed == world refresh speed
extern int  gFastDisplayModeSpeed;  // #images ignored in fast display mode (the higher, the faster)
extern bool gMonitorRobot;			// display currently selected robot input/output value on console 
extern int gTrajectoryMonitorIt;    // store the iteration when the last trajectory monitoring started. Use to create the trajectory image filename.
extern long int gVersion;

// global data

class World;
extern World *gWorld;				// pointer to the World

extern int	gInitialNumberOfRobots;			// number of robots that should be created at start-up
extern int	gNumberOfRobots;	    // actual number of robots existing in the simulation right now
extern int	gRobotIndexFocus;		// focused robot id.

extern int gNumberOfRobotGroups;     // number of different robot types/families (default: 1)

extern int gPhysicalObjectIndexStartOffset; // used for encoding the physical object's index into the environment image. (less than gRobotIndexStartOffset)
extern int gRobotIndexStartOffset; // used for encoding the robot's index into the environment image.

extern bool gRobotDisplayFocus;  // make focused robot more visible

extern bool gRadioNetwork;			// update radio network.

extern int gDisplaySensors; // display sensor rays on screen (0:no,1:if-contact,2:always+red,3:always)
extern bool gDisplayTail; // display tail on screen (back of robot)
extern bool gRobotLEDdisplay; // display LED status (RGB) on top of the robot

extern bool gExtendedSensoryInputs; // adds type of object or class of agents in controller sensory inputs.

extern bool gPauseMode;				// pause on/off (default is on)
extern bool gInspectorMode;			// defines if manual control is robot-centered or "god"-centered (ie. envt viewpoint)
extern bool gInspectorAgent;		// defines if there *actually* is an inspector agent (note: if using batch mode for, e.g. evolution, no inspector agent is needed)

extern int gMaxIt;

extern int gNbOfLandmarks;
extern std::vector<LandmarkObject> gLandmarks; // list of inanimate objects (landmarks).

extern int gNbOfPhysicalObjects;
extern int gPhysicalObjectDefaultType;
extern int gPhysicalObjectDefaultRegrowTimeMax;
extern bool gPhysicalObjectDefaultRelocate;
extern bool gPhysicalObjectDefaultOverwrite;
extern int gPhysicalObjectDefaultRadius;
extern int gPhysicalObjectDefaultFootprintRadius;
extern int gPhysicalObjectDefaultDisplayColorRed;
extern int gPhysicalObjectDefaultDisplayColorGreen;
extern int gPhysicalObjectDefaultDisplayColorBlue;
extern int gPhysicalObjectDefaultSolid_w;
extern int gPhysicalObjectDefaultSolid_h;
extern int gPhysicalObjectDefaultSoft_w;
extern int gPhysicalObjectDefaultSoft_h;

extern std::vector<PhysicalObject*> gPhysicalObjects; // list of inanimate objects (landmarks).
extern bool gPhysicalObjectsRedraw;

extern bool gEnergyLevel; // energy level exists? yes/no
extern double gEnergyMax; //! maximum energy given -- used for initialization, revive and max threshold
extern double gEnergyInit; //! energy given at start-up
extern bool gEnergyRefill; // refill dead robot with gEnergyInit when energy=0

extern double gEnergyItemDefaultInit;
extern int gEnergyItemDefaultMode; // 0,1,2,... cf. EnergyItem class.


//Screen attributes
extern int gScreenWidth;
extern int gScreenHeight;
extern int gScreenBPP; // bits per pixel color (32 bits)
extern int gFramesPerSecond; // frame rate

//The robot dimensions
extern int gRobotWidth;
extern int gRobotHeight;

extern int gInspectorCursorHorizontalSpeed; // inspector agent X speed
extern int gInspectorCursorVerticalSpeed;// inspector agent Y speed

extern int gInspectorCursorMaxSpeedOnXaxis;
extern int gInspectorCursorMaxSpeedOnYaxis;

extern int gInspectorAgentXStart;
extern int gInspectorAgentYStart;

// max nb of trials when looking for a location for a physical object or a robot
extern int gLocationFinderMaxNbOfTrials;

extern bool gKeyUp, gKeyDown, gKeyRight, gKeyLeft;
extern bool gFullScreenMode;
extern bool gNiceRendering;

extern bool gUserCommandMode; // robot tele-operation (true) or auto-control (false) ?

//The dimensions of the level

extern int gAreaWidth;
extern int gAreaHeight;

// SDL Objects

//camera
extern SDL_Rect gCamera;


//image surfaces
extern SDL_Surface *gScreen;			// surface for rendering
extern SDL_Texture *gScreenTexture;     // texture to transfer surface to renderer
extern SDL_Renderer *gScreenRenderer;   // renderer
extern SDL_Window   *gScreenWindow;     // window

extern SDL_Surface *gForegroundImage;	// "foreground" image, mostly decorative -- intended to be a "nice" representation of egEnvironmentImage
extern SDL_Surface *gEnvironmentImage;	// the real environment: all obstacle assume color #000001, should be close to gForegroundImage
										// note: for the sake of simplicity, one may load the same image into both gForegroundImage and gEnvironmentImage
extern SDL_Surface *gBackgroundImage;	// decorative background image
extern SDL_Surface *gRobotMaskImage;	// mask used for collision
extern SDL_Surface *gRobotDisplayImage;	// image used for display (only) - if missing in config file, then copied from gRobotMaskImage
extern SDL_Surface *gRobotSpecsImage;	// image contains specs wrt. proximity sensor (location and orientation)

extern SDL_Surface *gTrajectoryMonitorImage; // this image is used when monitoring robot trajectories

extern std::vector< std::vector<int> > gRobotMaskData; // list of inanimate objects - (x,y) from a (0,0) reference.

extern SDL_Surface *gGroundSensorImage;  // gGroundSensorImage

// general purpose
extern SDL_Event gEvent; //The event structure

extern int gMaxRadioDistance;

// robot dynamics and structure
extern float gMaxTranslationalSpeed; // pixels/sec.
extern float gMaxTranslationalDeltaValue; // default: = gMaxTranslationSpeed (however, may favor bang-bang)
extern float gMaxRotationalSpeed; // degrees/sec.
extern int gSensorRange;
extern int gTailLength;

extern int gLocomotionMode; // locomotion mode dynamics
							// =0: friction is ok. Robot crashes but can still rotate - ie. rotate while crash (no obstacle friction)
							// =1: friction matters. Robot can rotate only if not crashing again - ie. rotate if no translation twd obstacle (full obstacle friction)

// function hearders

void clean_up();
void displayHelp();
bool checkQuitEvent();
bool handleKeyEvent(Uint8 *keyboardStates);
void updateDisplay();
void initLogging();
void stopLogging();
void initMonitor( bool __isInspectorAgent = false );
void updateMonitor(const Uint8* __keyboardStates);
bool loadProperties( std::string __propertiesFilename );

void initRoborobo();
bool runRoborobo( int __maxIt = -1);
void closeRoborobo();
void resetRoboroboWorld();

void initTrajectoriesMonitor();
void updateTrajectoriesMonitor();

int launchRoborobo(); // the main entry point


#endif // ROBOROBO_H

