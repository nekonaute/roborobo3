=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
=-=-=-=-=-=-=-=-=-=-=-= ROBOROBO.3=-=-=-=-=-=-=-=
=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
=- 2008-2017 -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
-=-=-=-= nicolas.bredeche(at)upmc.fr -=-=-=-=-=-=
=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

How to know about current version and target build? Launch "roborobo -v" in the console.

Check _INSTALL.TXT for installation guide.

Check _FAQ.TXT for trouble shooting.

Check this file for anything else.


==== QUICK START : COMPILING AND RUNNING ROBOROBO ====

>>> Check _INSTALL.TXT <<<

If roborobo is installed and running, and you want to quickly get into the code, check Section DEVELOPER'S QUICK START below.

==== DESCRIPTION ====

Roborobo! is a fast and simple 2D mobile robot simulator written in C++ loosely based on a khepera/epuck model. It is targeted for fast single and multi-robots simulation (primary motivation is evolutionary robotics et swarm/collective robotics). Its dependencies to external libraries are kept to the minimum (SDL 2 and BOOST) and the code rely on a simple philosophy: "Keep it simple". It can be compiled with a simple Makefile. And new projects can be introduced quickly and easily. Any new project can be created in a dedicated location in the prj/ directory, without any modifications to the roborobo core.

Some random bits of information:
- Robot position and mouvement are real-valued (ie. precise)
- Collision is performed on a pixel-based information for fast speed (ie. fast but not precise)
- both GUI mode and batch mode are available. Note that batch mode is the fastest mode (gBatchMode=true, gVerbose=false). It also makes it possible to run roborobo "headless", which is useful if you use a cluster of computers.
- only two external dependencies: SDL2 and SDL2_image library (multi-platform fast 2D library), and the boost librairy, header-only.
- most parameters are to be found in the config/ subdirectory (e.g. default.properties)
- full environment and robot specifications can set up directly with an image editor (check data directories). E.g. Robot proximity sensor positions are set up in the robot image. 
- you can specify a properties file as command line option (see "./roborobo -h" for help)
- you can easily clone project with the clone_project.py script in the prj/ directory. 
- you can activate/deactivate existing projects for compilation with the makefile_manager script.
- you can launch many replicates of roborobo with the replicate.py script. (useful for running large-scale experiments, e.g. for clusters)
- by defaut, several demos are available: random walker, boids collective behaviours, embodied evolution and environment-driven embodied evolutionary robotics (improved mEDEA algorithm [Bredeche, Montanier, PPSN 2009]).
- the very first version of roborobo! can be found here: http://neuronik.free.fr/nemelith/?p=67 (it is quite different as it was first a game sort-of experience, eventhough i already had roborobo! in mind). An included html help file describes the overall idea behind parameterization through image files (some modifications may have occured since then). It uses SDL1.2.

==== HISTORY ====

[Ver.1: 12/2008]
[Ver.2: 09/2013]
[Ver.3: 01/2016]

The current version is Roborobo.3 (since 2016).
Compared to version 1, it requires Boost as dependencies.
Compared to version 2, it uses SDL2 instead of SDL1.2
Of course, every version changes also included heavy useful upgrades.

==== CREDITS ====

roborobo!: 
 - main developper: nicolas.bredeche(at)upmc.fr
included contributions:
 - the Properties class from Evert Haasdijk (VU Amsterdam)
 - other contribs are mentionned in the code (check: core/Misc.h and contrib/*)
 - past contributor(s): Jean-Marc Montanier, Leo Cazenille, Pierre Delarboulas


==== CITING ROBOROBO ====

If you use roborobo in your work, please cite the following paper:

N. Bredeche, J.-M. Montanier, B. Weel, and E. Haasdijk. Roborobo! a fast robot simulator for swarm and collective robotics. CoRR, abs/1304.2888, 2013. 

@article{roborobo,
  author    = {Nicolas Bredeche and Jean-Marc Montanier and Berend Weel and Evert Haasdijk},
  title     = {Roborobo! a Fast Robot Simulator for Swarm and Collective Robotics},
  journal   = {CoRR},
  volume    = {abs/1304.2888},
  year      = {2013},
  ee        = {http://arxiv.org/abs/1304.2888},
  bibsource = {DBLP, http://dblp.uni-trier.de}
}

The paper is available on Arxiv: http://arxiv.org/abs/1304.2888 


=================================
==== DEVELOPER'S QUICK START ====
=================================

** VERY QUICK START **

Check the Template* projects in prj/ (start with TemplateRandomwalk, then TemplateBoids). Everything you need is there. The code is meant to illustrate how it worksand contains a lot of comments.
Check also the config/ directory, which contains "properties" files, used to launch a particular instance of roborobo. It specifies both parameters and project classes to be used.
A typical command to launch roborobo is: './roborobo -l config/boids.properties'

If you want to start coding:
1. clone a project (go to prj/, use the clone_project.py script)
2. create your own config file (go to config/, copy an existing file)
3. add your project to the Makefile (use makefile_manager script)
4. compile ('make')
5. run ('./roborobo -l config/myconfigurationfile.properties')

Detailed example, based on cloning the TemplateRandomWalk project:

STEP 1: cloning a project
    <from roborobo root directory>
    cd prj/
    python clone_project.py TemplateRandomwalk MyOwnProject

STEP 2: creating a configuration file ("properties" file)
    <from roborobo root directory>
    cd config/
    cp template_randomwalk.properties myownproject.properties
    <then edit myownproject.properties to change ConfigurationLoaderObjectName parameter value to: "MyOwnProjectConfigurationLoader">

STEP 3-4-5: activate your project, then compile and run.
    Linux:
        ./makefile-manager -a MyOwnProject
        make clean
        make  (or 'make -jXX' if you have XX cores on your computer. Will compile faster.)
        ./roborobo -l config/myownproject.properties
    Mac OS X with Xcode:
        add missing files to the project:
            - all files in prj/MyOwnProject
            - src/ext/MyOwnProjectConfigurationLoader.cpp
            - include/ext/config/MyOwnProjectConfigurationLoader.h
it may also be necessary to register your files to be compiled. To do so: click on the project name (upper-left), then select ''Roborobo3'' as target (not as project), then ''Build phases'', then ''Compile Sources''.
        add new configuration file (edit scheme, add and select "-l config/myownproject.properties" as argument passed on launch -- remove other arguments).
        clean and build
        run

That's it. You can start to code.


** ROBOROBO: A QUICK OVERVIEW **

The philosophy is that in the standard case, the designer should be able to code the agent behavior (the ''Controller'') and to access both the world and agents states in a seperate fashion (''World'' and ''Agent'' observers). The world and agents observers are run before the actual agent behavioral update, meaning that the observers do observe a ''stable'' snapshot of the environment. Then, agent behavioral update are performed in turn-based fashion (ie. asynchroneous), to deal with possible collision. While roborobo is turn-based, the update order of robots is randomized between each updates of the world (to avoid deleterious effects due to ordering).

- WorldObserver: object dealing with world level, used for accessing state of the world, including all agents (methods: reset and step).
- AgentObserver: object dealing with agent level, used for accessing state of agent. All agent observers are called before behavior update (methods: reset and step)
	=> Observers are really useful for monitoring, logging, computing fitness updates, managing off-line evolutionary optimization, etc.
- Controller: object dealing with behavior at the robot level (useful methods: init and reset)
- *WorldModel: object where the (robot) world model is stored. May or may not be extended depending on your needs.

From the Controllers, you can access the world and robot(s) world-models. All the rest is simulator-specific implementation.
- of course, you may want to create a new properties file in the config sub-directory

** SIMULATION INIT AND UPDATE CYCLE **

Roborobo is turn-based with randomized-ordering updates for both objects and robots. Randomization is used to break any ordering effects, and updates for control and movements are asynchroneous. The update ordering is the following (check World::updateWorld):

1. perform update (and move?) on all objects, ie. set up the environment before updating the robots
2. call a world observer (WorldObserve::.stepPre()), ie. observe (and possibly modify) the state of the world before the robots compute&move.
3. for each robot, call an observer, ie. observe (and possibly modify) the state of the agent before moving.
4. perform state update for all robots, ie. robots make decisions
5. perform location update for all robots, ie. robots move
6. call a world observer (WorldObserve::.stepPost()), ie. observe (and possibly modify) the state of the world *after* the robots moved

More general information about the simulation update cycle:
- Observers are pretty useful for logging data.
- AgentObserver is called N times per iterations (N = nb of agents)
- WorldObserver is called once per iterations
- Update method: turn-based, synchroneous and shuffle-ordered update method.
- update method call sequence: WorldObserver => N_a * agentObservers => (N_e * energyPoints update) => N_a * agent steps => N_a agent moves
- worldObserver is called *before* agentObservers, agentObservers are all called *before* agent.stepBehavior
- environment physics and actual moving of agents are performed in a row *after* all agent.stepBehavior has been called
- Agent position movements is performed using synchroneous position updates. Hence, solving collisions is robust to agent index ordering
- the update ordering is shuffled for each new world iterations, ie. agent indexes are shuffled (to avoid update ordering nasty effects accross time)
- a typical problem if shuffling is not done is that agents with low indexes always act and move first, hence a big survival advantage.
- however, agentObservers and agent.stepBehavior are called in the same order (ie. no re-shuffling)
- WorldModel: contains all information on on agent, ie. its representation of the outside world, including its own status (e.g. energy level).
- in practical, neither controlarchitecture or observers should contain general agent-related information. All should be stored in the worldmodel.

Prior to this update cycle, a World::initWorld() is called to setup the whole simulation. The initialisation order is:
1. call WorldObserver::initPre() -- does nothing by default. User-implementation is possible.
2. initialise landmarks
3. initialise objects
4. initialise robots
5. call WorldObserver::initPost() -- does nothing by default. User-implementation is possible.

More general information about the simulation initialization:
- WorldObserver's initPre() and initPost() methods are useful to log the state of the world before simulation starts (especially initPost()).
    - initPre() is called before landmarks/objects/robots are set up according to the Properties file
    - initPost() is called after landmarks/objects/robots are set up according to the Properties file, ie. just before simulation starts
    - These methods can also be used to change/initialize the simulation, e.g. setting up specifics (e.g. add/remove/move some objects, robots, landmarks)
- initialisation uses information from the Properties file

The best way to learn is to practice. Clone an existing project and toy with it. See next paragraph to start playing.


** METHODOLOGY GUIDELINES **

- Any new project should start, *and be limited to*, a specific sub-directory in prj/, with both an include/ and src/ sub-directories. E.g. ./prj/MyProject/[include&src].
- Observers should be understood as "deals with everything that happens inbetween two lifetimes" while Controller should be understood as "everything that happens during lifetime". Another way to consider it is that Observers perform actions that would be done by a human supervisor or any other external devices (e.g.: move the robots/objects, monitor current state) while the Controller implements the robot's decision making apparatus.

A last remark: roborobo! is not exactly the paragon of Clean Coding philosophy as it was originaly, and still is, a single-coder-in-a-very-big-hurry "short" term project (though I admit it escaped my grasp). While I tried to keep the code clean, there are several do-as-i-say-not-as-i-do features (lack of accessing methods, etc.). Hence, roborobo! may not be the best starting point for developping your homebrew robot simulator. However, it can still be pretty useful if you just intend to extend it for a particular application (e.g. a specific swarm mobile robotics environment and simulation), and it has proven to be quite useful for several published works in the last few years, from several authors and places (Paris, Orsay, Nancy, Amsterdam, Trondheim, Barcelona, Edinburgh, etc.).


** DIRECTORY STRUCTURE **

- prj    : project locations, each project is indenpendant, and may be seen as a special instance of roborobo - parameterized through ConfigurationLoader and .properties file.
- log    : log files.
- data   : contains image files.
- config : contains properties files.
- core   : core roborobo! code. Contains the elementary elements for running roborobo.
- ext    : derived class and extensions for roborobo! code, which may be of global interest. It contains addition to roborobo core, including all code that can be factorised from projects (ie. re-usable code, such as e.g. neural nets, logger), as well as project specific code.
- contrib: imported code with useful features.

** PROPERTIES FILE **

Properties files are plain-text file where you can specify the parameters for your run with keys and values (e.g. gScreenWidth=800). Some parameters are required by roborobo, some other you can add to fit your needs. It is recommended to clone an existing Properties file if you need to create your own keys and values.

Useful information on Properties file:
- only one Properties file can be passed as argument to roborobo
- duplicated symbols are forbidden (i.e. multiple declarations of the same key will trigger a critical error (with message))
- the main Properties file can import any number of other Properties files (syntax: "import (config/mysecondlevelpropertiesfile.properties)")
- recursive imports are not allowed (only imports from the main Properties file are done)

the import(.) command can be very useful for two reasons:
1. to seperate different kind of parameters (e.g. environment-related properties vs agent-related properties)
2. to re-use a similar subset of parameters in two different experiments
    Examples:
        obstacleavoidanceexperiement.properties and foragingexperiment.properties may use the same evolutionaryparameters.properties
        a defaultsetup.properties could contain the general settings to be imported in experiment-specific Properties files.


** CONTENT OF ./DATA/ : IMAGES OF ROBOTS AND ENVIRONMENT **

Environments and robots are all described as images.

>>>>IMPORTANT: USE ONLY 32-bits BMP IMAGES!<<<<

gRobotMaskImageFilename				: the robot as how it is displayed and used for collision.
gRobotSpecsImageFilename			: the robot specification regarding proximity sensor (location and orientation).

gEnvironmentImageFilename  		: the environment description image.
									R component is unused
									G component is used to determine agent #G starting position (if more than one pixel with same G value, choose the most upper-left), 
									B component is used to determine object type.
									obstacle (and agent) Id ("type") are returned by proximity sensors. 0..255: #obstacle, 1000..1256..+++: #agent
									R=255 means "no obstacle", G=255 means nothing, ie.: pure white means "no obstacle"
									(!) use png to avoid pixel exact color loss
									note that the sensor beam lenght parameter is set in the config file.
gFootprintImageFilename      : this image specifies the footprint RGB value that will be sensed by the ground sensor.


gForegroundImageFilename 		: this image is decorative. Generally useful to beautify the environment.
gBackgroundImageFilename  		: this image is decorative. Generally useful to beautify the footprints.

Some image files are available by default in the data subdirectory. Here's a quick summary (but you may add anything you like):
- maxirobot*: a 32x32 robot image, with 8 sensors (similar to an e-puck or khepera robot)
- minirobot*: a 5x5 robot image, with 8, 12 or 16 sensors (sensor belt)
- ...and several environments

When creating an image, be very very careful about the values of pixels.

[-n].
