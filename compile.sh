#!/bin/bash
#Modify with ethercat install dir
: ${CC:="gcc"}
: ${EC_DIR:="/home/$USER/Documents/ethercat"}
$CC ectest.c -o ectest\
 -I$EC_DIR/include\
 $EC_DIR/install/lib/libethercat.a
