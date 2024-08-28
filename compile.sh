#!/bin/bash
#Modify with ethercat install dir
EC_DIR=/home/$USER/Documents/ethercat
gcc ectest.c -o ectest\
 -I$EC_DIR/include\
 $EC_DIR/install/lib/libethercat.a
