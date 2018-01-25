#!/usr/bin/python
# -*- coding:utf-8 -*-

# nicolas.bredeche(at)upmc.fr
# Last revision: 2017-01-30 14h48

import os
import sys
import datetime
import time
import glob

from mylib import *

debug = False   # if True: shell command are not issued, but displayed.

title = "Test"
xLabel="unnamed x-axis"
yLabel="unnamed y-axis"
# ylimMin=0, ylimMax=210
ylimMin = -1 #0 #-1
ylimMax = -1
xlimMin = -1
xlimMax = -1 #210 #-1
resolution = 10 # consider data every {resolution} line(s). value>0
autoscaling = True


if len(sys.argv) == 4 or len(sys.argv) == 5:
    filenames = glob.glob(sys.argv[1])
    xIndex = int(sys.argv[2])
    yIndex = int(sys.argv[3])
    if len(sys.argv) == 5:
        prefix = sys.argv[4]
    else:
        prefix = ""
else:
    print "Trace and write PDF with results from input file(s) -- single line or boxplots"
    print "\tSyntax:"
    print "\t\t", sys.argv[0], " <path/files.ext> <x-axis index> <y-axis index> {<prefix>}"
    print "\tRemarks:"
    print "\t\tcan be used with a single file or with multiple files"
    print "\t\tonly lines that starts with prefix are considered (optional)"
    print "\t\tprefix is removed before considering axis index number"
    print "\t\tlines beginning with \"\#\" are ignored"
    print "\t\t<path/files.ext> may use wildcards to select several files at once"
    print "\t\tExamples: "
    print "\t\t\t",sys.argv[0], " myLog_\\*/\\*.data 0 3"
    print "\t\t\t",sys.argv[0], " myLog_onefile.data 1 3 prefixofline:"
    quit()

if debug == True:
    print filenames

# load file(s)

rawlines = []
lines = []

for i in range (len(filenames)):
    print "Loading file:", filenames[i]
    rawlines.append( getContentFromFile( filenames[i] ) )
    rawlines[i] = getLinesWithPrefix ( rawlines[i], prefix, True)
    lines.append( parseLinesToArrayOfValues( rawlines[i] ) )

if debug == True:
    print rawlines
    print lines

# display data

if len(filenames) == 1:
    # display raw data from one single file
    xData = []
    yData = []
    i = 0
    for l in lines[0]:
        if i % resolution == 0:
            xData.append( l[xIndex] )
            yData.append( l[yIndex] )
            print ( str(l[xIndex]) + "," + str(l[yIndex]) )
        i = i + 1
    traceData( xData, yData, title=title, ylimMin=ylimMin, ylimMax=ylimMax, xlimMin=xlimMin, xlimMax=xlimMax, autoscaling=autoscaling, locLegend='lower right', xLabel=xLabel, yLabel=yLabel)
else:
    # compile data from multiple files and display boxplots
    
    if len(lines)<5:
        print "[ERROR] at least 5 data file are required to trace boxplots. Stop."
        quit()
    elif len(lines)<11:
        print "[WARNING] at least 11 data file are recommended to trace boxplots. Continue."
    
    for i in range (1,len(lines)):
        if len(lines[0]) != len(lines[i]):
            print "[ERROR] all data file must have the same amount of data. Stop."
            quit()

xData = []
    yData = []
    
    for i in range(len(lines[0])):
        xData.append( lines[0][i][xIndex] )
        l = []
        for j in range(len(lines)):
            l.append( lines[j][i][yIndex] )
        yData.append(l)
        print ( str(xData[i]) + "," + str(yData[i]))
    
    traceData( xData, yData, "multi", title=title, ylimMin=ylimMin, ylimMax=ylimMax, xlimMin=xlimMin, xlimMax=xlimMax, autoscaling=autoscaling, locLegend='lower right' )



