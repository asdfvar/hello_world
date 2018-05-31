#!/bin/bash
echo `date` > /home/euler/asdf_cron

frames=`grep 'frame' /home/euler/projects/misc/cron_job/cron_script.sh`

let x=3+4
echo $x
let y=$x+13
echo $y

echo $frames

echo "who there
start frame = $x; // stuff
end frame = $y; // stuff
what is that" > afile

#stuff here
#stuff here
#stuff here
#got to frame 1
#stuff here
#got to frame 2
#stuff here
#stuff here
#got to frame 3
#stuff here
#got to frame 4
