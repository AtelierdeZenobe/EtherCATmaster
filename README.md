# EtherCAT master
The master is, for now, only meant to be ran with a precise slave counterpart, found here: https://github.com/AtelierdeZenobe/EtherCATslave

## Install
TODO: doc directly here
For now: check Notion: https://www.notion.so/Ethercat-974932e2afe847ffa96708ea65085133

## Environment setup
TO BE FIXED:
For now a precise alias of "69" for the slave must be set:
`ethercat alias 69 -p 0 #If only one slave is connected, its ID should be 0.`

### Shared memory setup
```
home=$(git rev-parse --show-toplevel)

cd $home/scripts
gcc shm_init.c -o shm_init
sudo mv shm_init /usr/local/bin/
sudo chmod +x /usr/local/bin/shm_init

cp shm_init.service /etc/systemd/system/

sudo systemctl daemon-reload
sudo systemctl enable shm_init
sudo systemctl start shm_init

sudo chmod 666 /dev/shm/my_shared_memory
```

## Compile  
`gcc ectest.c -o ectest -I path/to/ecrt.h_folder path/to/liberthercat.a`  

If EC Master was build following Notion instructions, use
`make`. Just update EC_DIR (line 3) witht the path to the folder ($BUILD_DIR).  

## Run
`./run.sh`

### Shared memory publisher example
compile and run scripts/shm_writer.c

## CROSS compile

### install
`sudo apt-get install gcc-aarch64-linux-gnu`

### setup
Use the direnv  

### compile
`make`

### Run
Copy the ectest file and run run.sh on target
