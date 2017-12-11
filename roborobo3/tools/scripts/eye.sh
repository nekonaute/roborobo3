if [ "$#" -ne 3 ]; then
  echo "  Syntax: $0 file_to_watch_every_Xs nb_of_lines_at_end_of_file time_in_sec"
  echo "  (replaces watch command when missing)"
  exit    
fi

while :; do clear; tail -n $2 $1 ; sleep $3; done
