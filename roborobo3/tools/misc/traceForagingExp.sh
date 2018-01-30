echo Trace graphs for foraging experiment 2018-01-31

python2 multiplot2.py $1 0 7 -p log: -title balance &
python2 multiplot2.py $1 0 8 -p log: -title rsc0 &
python2 multiplot2.py $1 0 10 -p log: -title rsc1 &
python2 multiplot2.py $1 0 5 -p log: -title avgFit &
python2 multiplot2.py $1 0 6 -p log: -title sumFit &
python2 multiplot2.py $1 0 12 -p log: -title globalWelfare &
python2 multiplot2.py $1 0 15 -p log: -title avgGenomeReservoirSize &

