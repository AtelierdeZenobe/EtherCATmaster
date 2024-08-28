#!/bin/bash
echo "Require password to insert module ec_master (ethercatctl start)"
sudo ethercatctl start
sudo chmod 777 /dev/EtherCAT0
./ectest
