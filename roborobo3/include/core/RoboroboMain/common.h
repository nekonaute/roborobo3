/*
 *  common.h
 *  roborobo
 *
 *  Created by Nicolas on 16/12/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef COMMON_H
#define COMMON_H

// pre-processor options
// pre-processos system option

// SDL location changes depending on the system
#ifndef __unix__ // similar to #ifdef __APPLE__
#define MACOSX // MACOSX or WINXP/LINUX (WINXP tested with MinGW, Linux tested with Ubuntu)
#endif

// SDL Include headers

#ifdef MACOSX						// MAC OS X
#include "SDL2/SDL.h"
#include "SDL2_image/SDL_image.h"
#else								// LINUX or WINXP
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#endif

// System headers

#include <string>
#include <iostream>
#include <fstream>
#include <vector> 
#include <math.h> 
#include <assert.h>
#include <sstream>
#include <map>
#include <string>
#include <iomanip>
#include <cmath>

// boost header

#include <boost/multi_array.hpp>
//#include <boost/filesystem.hpp> // unfortunately, not a header-only part of boost (cf. http://www.boost.org/doc/libs/1_53_0/more/getting_started/windows.html#header-only-libraries)



// --- --- --- --- ---

#endif

