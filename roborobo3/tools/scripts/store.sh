echo "Create a directory and relocate all experimental data from current directory"

if [ "$#" -ne 1 ]; then
  echo "  Syntax: $0 <directory_name>"
  echo "  directory will be created (failed if already exists)"
  exit    
fi

if [ -d "$1" ]; then
  echo "Directory $1 already exists. Cancel command."
  exit
fi

mkdir $1 
mv datalog_*.txt $1
mv lite_*.txt $1
mv graph*pdf $1
mv properties_*.txt $1
mv screenshot_* $1
mv snapshot_* $1
mv trajectory_* $1
mv robotsId* $1
mv *.eps $1
mv log.txt $1
mv extract*.dat* $1
mv output.std* $1

echo "End."
