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
#include "Agents/Robot.h"
#include "World/PhysicalObjectFactory.h"
#include "World/PhysicalObject.h"

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
extern bool gFullLoggerRecording;
extern bool gOutputImageFormat;

extern bool gTrajectoryMonitor;
extern int gTrajectoryMonitorMode;

extern bool gCustomSnapshot_niceRendering;
extern bool gCustomSnapshot_showLandmarks;
extern bool gCustomSnapshot_showObjects;
extern bool gCustomSnapshot_showRobots;
extern bool gCustomSnapshot_showSensorRays;


// files

extern std::string gLogCommentText; // user comment that will be written in the log file (e.g. description of experimental setup)
extern bool gVerbose_commandlineargument;
extern bool gLogDirectoryname_commandlineargument;
extern bool gBatchMode_commandlineargument;
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
extern std::string gFootprintImageFilename;
extern std::string gConfigFilename;

// general purpose

extern int gRandomSeed;				// random seed. Default value (="-1") means time based.

extern bool gVerbose;				// trigger console verbose on/off
extern bool gBatchMode;				// trigger no display / no verbose mode.
extern int  gDisplayMode;			// [0]: 60fps standard display speed, [1]: ignore some refresh (see gFastDisplayModeSpeed value), [2]: fastest, but no display)

extern int backup_gDisplayMode;

									// much faster without display, in mode [0] (default); display speed == world refresh speed
extern int  gFastDisplayModeSpeed;  // #images ignored in fast display mode (the higher, the faster)
extern bool gMonitorRobot;			// display currently selected robot input/output value on console 
extern int gTrajectoryMonitorIt;    // store the iteration when the last trajectory monitoring started. Use to create the trajectory image filename.
extern long int gVersion;

// global data

class World;
extern World *gWorld;				// pointer to the World

extern std::vector<Robot*> gRobots;
extern std::vector<bool> gRobotsRegistry;

extern int	gInitialNumberOfRobots;			// number of robots that should be created at start-up
extern int	gNbOfRobots;	    // actual number of robots existing in the simulation right now
extern int	gRobotIndexFocus;		// focused robot id.

extern int gNumberOfRobotGroups;     // number of different robot types/families (default: 1)

extern int gPhysicalObjectIndexStartOffset; // used for encoding the physical object's index into the environment image. (less than gRobotIndexStartOffset)
extern int gRobotIndexStartOffset; // used for encoding the robot's index into the environment image.

extern bool gMovableObjects; // enable physics for moving (some) objects

extern bool gRobotDisplayFocus;  // make focused robot more visible

#define SENSOR_RAY_RED 224   //192,192,255
#define SENSOR_RAY_GREEN 224
#define SENSOR_RAY_BLUE 255
#define SENSOR_RAY_CONTACT_RED 255   //255,0,0
#define SENSOR_RAY_CONTACT_GREEN 192
#define SENSOR_RAY_CONTACT_BLUE 192

extern int gDisplaySensors; // display sensor rays on screen (0:no,1:if-contact,2:always+red,3:always)
extern bool gDisplayTail; // display tail on screen (back of robot)
extern bool gRobotLEDdisplay; // display LED status (RGB) on top of the robot

//extern bool gExtendedSensoryInputs; // DEPRECATED AND REMOVED AS OF 2017-12-28. Check TemplateEEController::getInputs() for help.
extern bool gSensoryInputs_distanceToContact;
extern bool gSensoryInputs_physicalObjectType;
extern bool gSensoryInputs_isOtherAgent;
extern bool gSensoryInputs_otherAgentGroup;
extern bool gSensoryInputs_otherAgentOrientation;
extern bool gSensoryInputs_isWall;
extern bool gSensoryInputs_groundSensors;
extern int gSensoryInputs_landmarkTrackerMode;
extern bool gSensoryInputs_distanceToLandmark;
extern bool gSensoryInputs_orientationToLandmark;
extern bool gSensoryInputs_energyLevel;

extern bool gReentrantMapping_motorOutputs;
extern bool gReentrantMapping_virtualOutputs;
extern int gVirtualOutputs;

extern bool gPauseMode;				// pause on/off (default is on)
extern bool gStepByStep;
extern bool gInspectorMode;			// defines if manual control is robot-centered or "god"-centered (ie. envt viewpoint)
extern bool gInspectorAgent;		// defines if there *actually* is an inspector agent (note: if using batch mode for, e.g. evolution, no inspector agent is needed)

extern int gMaxIt;

extern int gNbOfLandmarks;
extern std::vector<LandmarkObject*> gLandmarks; // list of inanimate objects (landmarks).

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
extern int gScreenDisplayWidth;
extern int gScreenDisplayHeight;
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
extern bool gLocationFinderExitOnFail;

// area where objects can be randomly located (ie. not a constraint over explicit location)
extern int gPhysicalObjectsInitAreaX;
extern int gPhysicalObjectsInitAreaY;
extern int gPhysicalObjectsInitAreaWidth;
extern int gPhysicalObjectsInitAreaHeight;

// area where agents can be randomly located (ie. not a constraint over explicit location)
extern int gAgentsInitAreaX;
extern int gAgentsInitAreaY;
extern int gAgentsInitAreaWidth;
extern int gAgentsInitAreaHeight;

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
extern SDL_Surface *gSnapshot;			// surface for rendering snapshot (prior to save to disk)
extern SDL_Surface *gScreen;			// surface for rendering (possibly used for display)
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

extern SDL_Surface *gFootprintImage;  // gFootprintImage
extern SDL_Surface *gFootprintImageBackup;  // gFootprintImageBackup - used to conserve initial image value.

extern bool gFootprintImage_restoreOriginal; // If true, then any object footprint that disappears is replaced by the original ground values. Otherwise: write white pixels.
                                                // Also impact localisation of new objects. if true: new objects can be localized on non-empty ground values. If false: new objects can only be localized on empty (=white) pixels

// general purpose
extern SDL_Event gEvent; //The event structure

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
void inspectAtPixel(int xIns, int yIns);
bool checkEvent();
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

void initTrajectoriesMonitor();
void updateTrajectoriesMonitor();

int launchRoborobo(); // the main entry point


#endif // ROBOROBO_H

