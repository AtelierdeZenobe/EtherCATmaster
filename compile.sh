#!/bin/bash
gcc -static ectest.c -o ectest\
 -I/home/vpu-env/ethercat/include\
 /home/vpu-env/ethercat/install/lib/libethercat.a