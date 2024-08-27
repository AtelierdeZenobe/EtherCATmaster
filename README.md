# EtherCAT master
The master is, for now, only meant to be ran with a precise slave counterpart, found here: https://github.com/AtelierdeZenobe/EtherCATslave

## Install
TODO: doc directly here
For now: check Notion: https://www.notion.so/Ethercat-974932e2afe847ffa96708ea65085133

## Environment setup
TO BE FIXED:
For now a precise alias of "69" for the slave must be set:
`ethercat alias 69 -p 0 #If only one slave is connected, its ID should be 0.`


## Compile  
`gcc ectest.c -o ectest -I path/to/ecrt.h_folder path/to/liberthercat.a`  

If EC Master was build following Notion instructions, use
`./compile.sh`. Just update EC_DIR (line 3) witht the path to the folder ($BUILD_DIR).  

## Run
`./run.sh`

