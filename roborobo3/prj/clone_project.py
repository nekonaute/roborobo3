# roborobo project cloner
# MUST BE RUN FROM THE PRJ/ DIRECTORY
# -*- coding: utf-8 -*-

import os
from os import listdir
from os.path import isfile, join
import shutil
import errno
import sys
from random import *

# =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

# copy file or directory (full content)
# from: http://www.pythoncentral.io
def copy(source, target):
    try:
        shutil.copytree(source, target)
    except OSError as e:
        # If the error was caused because the source wasn't a directory
        if e.errno == errno.ENOTDIR:
            shutil.copy(source, target)
        else:
            print('%s' % e)

# copy file or directory (full content)
# from: http://www.pythoncentral.io
def copy(source, target):
    try:
	IGNORE_PATTERNS = ('.svn','CVS','.git','.test')
        shutil.copytree(source, target,ignore=shutil.ignore_patterns(*IGNORE_PATTERNS))
    except OSError as e:
        # If the error was caused because the source wasn't a directory
        if e.errno == errno.ENOTDIR:
            shutil.copy(source, target)
        else:
            print('%s' % e)

# =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

def deleteFile(target):
    os.remove(target)

# =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

# from: stackoverflow
def getFilenamesAt(sourcepath):
    filenames = [ f for f in listdir(sourcepath) if isfile(join(sourcepath,f)) ]
    return filenames

# =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

def getContentAt(sourcepath):
    content = os.listdir(sourcepath)
    return content

# =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

def replaceInFile(filename,source,target):
    f_source = open(filename,"r")
    lines=f_source.readlines()
    f_source.close()
    
    for i in range(len(lines)):
        lines[i] = lines[i].replace(source,target)

    f_target = open(filename,"w")
    for l in lines:
        f_target.write(l)
    f_target.close()

# =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

def insertInFile(filename,tag,text_to_insert):
    f_source = open(filename,"r")
    lines=f_source.readlines()
    f_source.close()
    
    f_target = open(filename,"w")
    for l in lines:
        if tag in l:
            f_target.write(text_to_insert)
        f_target.write(l)
    f_target.close()

# =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

def cloneProject(source,target):

    print ("")
    print ("ROBOROBO PROJECT CLONER")
    print ("rev.2014-06-28_22h33")
    print ("")

    if os.path.exists(source) == False:
        print ("[ERROR] source does not exist. Stop.")
        return
    if os.path.exists(target):
        print ("[ERROR] target already exists. Stop.")
        return

    print ("WARNING: use at your own risks.")
    print ("This script create a new directory in the prj/ directory, and also add/update files in <roborobo>/src and <roborobo>/include. This script may not work with all projects. It assumes the source project can be duplicated without creating conflicts (ie. prefix \"+source+\" is used everywhere, no fancy names (class names, macro, global variables)).")
    print ("Note: if everything goes wrong, clean <roborobo>/src/ext/ConfigurationLoader.cpp (optional: delete extra files created).")
    userInput = raw_input("Do you want to continue (\"y\" or \"n\"): ")
    if userInput.lower()!="y" and userInput.lower()!="yes":
        print("Abort.")    
        return

    # create new project structure (target) from previous project (source)

    print ("\n### Creating new project (\""+target+"\") from previous project (\""+source+"\") ###\n")
    
    copy(source, target)
    
    print ("<roborobo>/prj/"+target+" created.")
 
    # set-up new project (update filenames, and file contents)

    subdirs = ("/src/","/include/")
    for subdir in subdirs:
        filenames = getFilenamesAt(target+subdir)
        for oldname in filenames:
            fileName, fileExtension = os.path.splitext(oldname)
            if fileExtension in [".cpp",".h",".hpp"]:
                newname = target + oldname.split(source)[1]
                os.rename(target+subdir+oldname,target+subdir+newname)
                replaceInFile(target+subdir+newname,source.upper(),target.upper())
                replaceInFile(target+subdir+newname,source,target)
                print("<roborobo>/prj/"+ target+subdir+newname + " created.")
            else:
		os.remove(target+subdir+oldname)
                print("<roborobo>/prj/"+ target+subdir+oldname + " ignored and deleted (unknown type).")        ###  CHANGED!!!

    # create (from copy) configuration files in <roborobo>/src/

    oldname = "../src/ext/"+source+"ConfigurationLoader.cpp"
    newname = "../src/ext/"+target+"ConfigurationLoader.cpp"

    copy(oldname,newname)
    replaceInFile(newname,source.upper(),target.upper())
    replaceInFile(newname,source,target)

    print(newname.replace("../","<roborobo>/") + " created.")
    
    # create (from copy) configuration files in <roborobo>/include/

    oldname = "../include/ext/Config/"+source+"ConfigurationLoader.h"
    newname = "../include/ext/Config/"+target+"ConfigurationLoader.h"
    copy(oldname,newname)
    replaceInFile(newname,source.upper(),target.upper())
    replaceInFile(newname,source,target)
    
    print(newname.replace("../","<roborobo>/") + " created.")
    
    # update src/ext/ConfigurationLoader.cpp

    configurationLoaderFilename = "../src/ext/ConfigurationLoader.cpp"
    tag = "//###DO-NOT-DELETE-THIS-LINE###TAG:INCLUDE###//"
    inserted_text = "#include \"Config/"+target+"ConfigurationLoader.h\"\n"
    insertInFile(configurationLoaderFilename,tag,inserted_text)
    tag = "//###DO-NOT-DELETE-THIS-LINE###TAG:SWITCH###//"
    inserted_text = "#if defined PRJ_" + target.upper() + " || !defined MODULAR\n\telse if (configurationLoaderObjectName == \""+target+"ConfigurationLoader\" )\n\t{\n\t\treturn new "+target+"ConfigurationLoader();\n\t}\n#endif\n"
    insertInFile(configurationLoaderFilename,tag,inserted_text)
    
    print(configurationLoaderFilename.replace("../","<roborobo>/") + " updated.")

    # Terminating.
    
    print ("")
    print ("DONE.")
    print ("")
    print "To delete this project:"
    print "  1. delete directory <roborobo>/"+target+""
    print "  2. edit file "+configurationLoaderFilename+""
    print "\tdelete line: \"#include \"Config/"+target+"ConfigurationLoader.h\""
    print "\tdelete block: \"#if define PRJ_"+target.upper()+"\" (...) #endif\""
    print "\tdelete files: \""+target+"ConfigurationLoader.cpp/.h\" in <roborobo>/src/ext and <roborobo>/include/ext/Config"
    print ("")
    print "To run this project:"
    print "  1. Create a configuration file in the config/ directory, it should contain the following line:"
    print "\t\"ConfigurationLoaderObjectName = "+target+"ConfigurationLoader\""
    print "  2. Launch roborobo:"
    print "\tCommand-line (Linux):"
    print "\t\tcd <roborobo_root_directory>"
    print "\t\t./makefile-manager -a " + target
    print "\t\t make clean ; make"
    print "\t\t\"./roborobo -l Config/<yourconfigurationfile>.properties\""
    print "\tXcode on Mac"
    print "\t\tAdd new files to project"
    print "\t\tRun with new configuration file as argument"
    print "\n"
    print ""


#print 'Number of arguments:', len(sys.argv), 'arguments.'
#print 'Argument List:', str(sys.argv)
if len(sys.argv) == 3:
    cloneProject(sys.argv[1],sys.argv[2])
else:
    print ("roborobo project cloner\n\tusage: \"python clone_project.py source_project_name target_project_name\"")
