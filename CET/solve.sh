#!/bin/bash
# author: 
make

STARTTIME=$(date +%s)

./test


ENDTIME=$(date +%s)
echo "It takes $(($ENDTIME-$STARTTIME)) seconds to complete this task..."
echo "Done."
