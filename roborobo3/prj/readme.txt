# ### ### ### ### ###
#
# README.TXT - contains helpful tips wrt the project directory
#
# ### ### ### ### ###

=-=-=-=-=-=-=-=-=-=-=
=-=-=-=-=-=-=-=-=-=-=

CONTENT:
0. How to start in 2 minutes
1. List of "template" projects
2. Creating your own project

=-=-=-=-=-=-=-=-=-=-=
=-=-=-=-=-=-=-=-=-=-=

## 0. HOW TO START IN 2 MINUTES

Check the "Tutorial" project, in particular the step() method from TutorialController.cpp. 

=-=-=-=-=-=-=-=-=-=-=
=-=-=-=-=-=-=-=-=-=-=

## 1. LIST OF "TEMPLATE" PROJECTS

This directory contains several template projects:
- TemplateVanillaEE : a basic task-driven embodied evolution setup solving a foraging problem (ie. explicit fitness function for foraging), use with config/template_vanillaEE.properties
- TemplateMedeaEE   : a basic mEDEA setup (with random selection) with energy constraints (ie. environmental pressure points towards foraging), use with config/template_medea.properties or config/movingobjects.properties
- TemplateRandowalk : a simple example of random walkers with position reset every N iterations, use with config/template_randowalk.properties
- TemplateWander    : a simple example of wandering robots, use with config/template_wander_{small|big}robots.properties
- TemplateBoids     : a simple example of boids, use with config/template_boids.properties
- ForagingRegions   : illustrate how to define a new object type, and implement a particular object's behavior

All this projects can be tested from the command line: Use "./makefile-manager -l" to check if the project of interest is activated. If not, check "./makefile-manager -h" to activate it. Then re-compile (make clean; make). Then use "./roborobo -l config/name_of_properties_file.properties", with the relevant properties filename as argument (check list of projects above).

Remark: the TemplateEE project is an abstract project from which TemplateVanillaEE and TemplateMedeaEE are derived. Prefer to derive from the two latter to start a project on embodied evolution.

Do *not* modify template projects, they can be quite useful. To create your own project, see below.

=-=-=-=-=-=-=-=-=-=-=
=-=-=-=-=-=-=-=-=-=-=

## 2. CREATING YOUR OWN PROJECT

The easiest (and recommended) way is to create a new project from an existing project using the "clone_project.py" python script. Once this is done, do not forget to create a new properties file and to update the Makefile (and/or IDE project). Detailed how-to:
1. in the prj directory, use "python clone_project.py TemplateProjectName MyProjectName"
2. in the config directory, copy an existing properties file. E.g.: "cp TemplateProjetName.properties MyProjectName.properties"
3. edit your equivalent of MyProjectName.properties to correctly set the ConfigurationLoaderObjectName key. E.g.: "ConfigurationLoaderObjectName = MyProjectNameConfigurationLoader"
4. in the root directory, set the new project as active in the Makefile. Use: "./makefile-manager -a MyProjectName" (check with: "./makefile-manager -l")
5. recompile: "make clean, make" (use "make -j XX" if you have XX cores)
6. run: "./roborobo -l config/MyProjectName.properties"

Remarks:
1. The clone_project.py script uses Python 2.x.
2. This script works for cloning any projects as long as they follow the same structure than that of the templates, i.e. only if <sourcename> is the prefix for directory name, filenames and class names.
3. If you are using Xcode on MacOS X (or whatever IDE), do not forget to update the project content.

ADDITIONAL INFO: below is the detailed list of operations if you do not wish to use the clone_project.py script (which is highly not recommended)

Pre-amble:
	THE EASIEST WAY TO CREATE A PROJECT IS TO USE THE PYTHON SCRIPT "clone_project.py" WHICH
	IS FOUND IN THIS DIRECTORY. THE FOLLOWING IS A STEP BY STEP DESCRIPTION OF WHAT IS DONE
	AUTOMATICALLY USING THIS SCRIPT.

1. copy directory (e.g. BasicProject => Sandbox)
2. change all filename prefix (e.g. BasicProject => Sandbox, for all .h and .cpp)
3. Within each file, change all matching prefixes (e.g. BasicProject => Sandbox)
		rename in file <SOURCENAME> to <TARGETNAME> (forced upper-case, macro identifier in .h files)
		rename in file <sourcename> to <targetname>

4. in include/ext/Config
		copy <sourcename>ConfigurationLoader.h to <targetname>ConfigurationLoader.h
		rename in file <SOURCENAME> to <TARGETNAME> (forced upper-case)
		rename in file <sourcename> to <targetname>
5. in src/ext/
		copy <sourcename>ConfigurationLoader.cpp to <targetname>ConfigurationLoader.cpp
		rename in file <SOURCENAME> to <TARGETNAME> (forced upper-case)
		rename in file <sourcename> to <targetname>

6. in src/ext/ConfigurationLoader.cpp:
	6.a above the line ''//###DO-NOT-DELETE-THIS-LINE###TAG:INCLUDE###//'', write:

			#include "Config/<targetname>ConfigurationLoader.h"

	6.b above the line ''//###DO-NOT-DELETE-THIS-LINE###TAG:SWITCH###//'', write:

			#if defined PRJ_<TARGETNAME> || !defined MODULAR
			else if (configurationLoaderObjectName == "<targetname>ConfigurationLoader" )
			{
				return new <targetname>ConfigurationLoader();
			}
		    #endif

7. Create/copy a compatible properties file in the config/ directory. It should contain:
		ConfigurationLoaderObjectName = <targetname>ConfigurationLoader

8. run. (''./roborobo -l config/mynewpropertiesfile.config'')

=-=-=
