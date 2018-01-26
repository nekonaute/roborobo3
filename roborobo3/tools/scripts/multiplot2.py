#!/usr/bin/python
# -*- coding:utf-8 -*-
# TROUBLE SHOOTING:
#  if you get an error like "ValueError: unknown locale: UTF-8"
#  then first type in the terminal: "export LC_ALL=en_US.UTF-8"

# nicolas.bredeche(at)upmc.fr
# Last revision: 2017-01-30 14h48

import os
import sys
import datetime
import time
import glob
import argparse

from mylib import *

debug = False   # if True: shell command are not issued, but displayed.

parser = argparse.ArgumentParser(description=
"Trace and write PDF with results from input file(s) -- single line or boxplots. Remarks: (1) can be used with a single file or with multiple files (2) only lines that starts with prefix are considered (optional) (3) prefix is removed before considering axis index number (4) lines beginning with \"#\" are ignored (5) <path/files.ext> may use wildcards to select several files at once.")
#\n\t\tExamples:\n\t\t\t",sys.argv[0], " myLog_\\*/\\*.data 0 3\n\t\t\t",sys.argv[0], " myLog_onefile.data 1 3 prefixofline:\n")

parser.add_argument('filenames', nargs='+', help='filename or filename template (using \"*\" character)')
parser.add_argument('xIndex', type=int, help='index of x-axis values')
parser.add_argument('yIndex', type=int, help='index of y-axis values')
parser.add_argument('--prefix','-p', type=str, nargs='?', default="", help='line prefix (ex.: \"log:\") [optional]')

parser.add_argument('-title', type=str, nargs='?', default="unnamed graph", help='Graph title [optional]')
parser.add_argument('-xLabel', type=str, nargs='?', default="unnamed x-axis", help='X-axis name [optional]')
parser.add_argument('-yLabel', type=str, nargs='?', default="unnamed y-axis", help='Y-axis name [optional]')

parser.add_argument('-ylimMin', type=float, nargs='?', default=-1, help='min Y displayed value [optional]')
parser.add_argument('-ylimMax', type=float, nargs='?', default=-1, help='max Y displayed value [optional]')
parser.add_argument('-xlimMin', type=float, nargs='?', default=-1, help='min X displayed value [optional]')
parser.add_argument('-xlimMax', type=float, nargs='?', default=-1, help='max X displayed value [optional]')

parser.add_argument('-resolution', type=int, nargs='?', default=1, help='data resolution (1: trace each data values, 2: trace every other data values, etc.) [optional]') # consider data every {resolution} line(s). value>0

parser.add_argument('-autoscaling', type=bool, nargs='?', default=True, help='graph autoscaling [optional]') # consider data every {resolution} line(s). value>0

parser.add_argument('-locLegend', type=str, nargs='?', default='lower right', help='location of legend [optional]')

args = parser.parse_args()

if debug == True:
    print(args)

#print ("Value 1:",args.value1)
#print ("Value 2:",args.value2)
#print ("Value 3:",args.value3)

if debug == True:
    print args.filenames

# load file(s)

rawlines = []
lines = []

for i in range (len(args.filenames)):
    print "Loading file:", args.filenames[i]
    rawlines.append( getContentFromFile( args.filenames[i] ) )
    rawlines[i] = getLinesWithPrefix ( rawlines[i], args.prefix, True)
    lines.append( parseLinesToArrayOfValues( rawlines[i] ) )

if debug == True:
    print rawlines
    print lines

# display data

if len(args.filenames) == 1:
    # display raw data from one single file
    xData = []
    yData = []
    for l in lines[0]:
        xData.append( l[args.xIndex] )
        yData.append( l[args.yIndex] )
        print ( str(l[args.xIndex]) + "," + str(l[args.yIndex]) )
    traceData( xData, yData, title=args.title, ylimMin=args.ylimMin, ylimMax=args.ylimMax, xlimMin=args.xlimMin, xlimMax=args.xlimMax, autoscaling=args.autoscaling, locLegend=args.locLegend, xLabel=args.xLabel, yLabel=args.yLabel)
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
        xData.append( int(lines[0][i][args.xIndex]) )
        l = []
        for j in range(len(lines)):
            l.append( lines[j][i][args.yIndex] )
        yData.append(l)
        print ( str(xData[i]) + "," + str(yData[i]))

    traceData( xData, yData, "multi", title=args.title, ylimMin=args.ylimMin, ylimMax=args.ylimMax, xlimMin=args.xlimMin, xlimMax=args.xlimMax, autoscaling=args.autoscaling, locLegend=args.locLegend, xLabel=args.xLabel, yLabel=args.yLabel)
