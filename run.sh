#!/bin/bash
echo "Require password to insert module ec_master (ethercatctl start)"
cd "$(dirname "$(realpath "$0")")"
sudo ethercatctl start
sudo chmod 777 /dev/EtherCAT0
./ectest
