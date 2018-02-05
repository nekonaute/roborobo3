#!/usr/bin/python

import os
import sys
import datetime
import time

debug = False   # if True: shell command are not issued, but displayed.

def getTimestamp():
    return datetime.datetime.now().strftime("%Y%m%d_%H%M%S")

def createdir(directory):
	if not os.path.exists(directory):
        	if debug == False:
	                os.makedirs(directory)
	        print "[INFO] directory \"",directory,"\" created."
	else:
	        print "[ERROR] directory already exists."
	        quit()

if len(sys.argv) == 1:
	print "Syntax: ", sys.argv[0], " <path/file.properties> <nb_of_replicates> {<directory_suffix>}"	
	print "\texample: python replicate.py config/template_vanillaEE.properties 32"
	quit()
else:
	propertiesfile = sys.argv[1]
	nbrep = int(sys.argv[2])
	if len(sys.argv) == 4:
		suffix = sys.argv[3]
	else:
		suffix = "exp"

prefix = "./logs/"+getTimestamp()

for i in range(nbrep):
	print "[INFO] Starting replicate #",i
	
	dirname = prefix + "_" + str(i).zfill(len(str(nbrep)))+ "_" + suffix 
	
	createdir(dirname)
	
	command = "env SDL_VIDEODRIVER=dummy nohup ./roborobo -b -l " + propertiesfile + " -o " + dirname + " 1> " + dirname + "/output.stdout 2> " + dirname + "/output.stderr &"
	
	if debug == False:
		os.system(command)
	else:
		print command
	
	time.sleep(0.2)

print "[INFO] Terminated."
