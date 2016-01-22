## How to create a new project called <targetname> from an existing project called <sourcename>? ##

=-=-=

Pre-amble:
	THE EASIEST WAY TO CREATE A PROJECT IS TO USE THE PYTHON SCRIPT "clone_project.py" WHICH
	IS FOUND IN THIS DIRECTORY. THE FOLLOWING IS A STEP BY STEP DESCRIPTION OF WHAT IS DONE
	AUTOMATICALLY USING THIS SCRIPT.

Also:
- The following is valid only if <sourcename> is the prefix for directory name, filenames and class names.
	- e.g. TemplateWander, TemplateMedea and TemplateBoids are valid wrt. these contraints. Any other projects may, or may not, be valid. (that said, chance are they are valid)
- If you are using Xcode on MacOS X, do not forget to update the project content.

=-=-=

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

.2014-06-28n.