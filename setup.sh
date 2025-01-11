#!/bin/bash

# DIR Variable   - '/home/cloudnet/dinushan'
DIR='/home/cloudnet/dinushan'
HOST_PATH='/scripts/Hosts/WS'
Observer_PATH='/scripts/Hosts/Observer'

# navigate to the directory
cd $DIR

# Clone the project from the git repository
git clone https://github.com/DinushanVimukthi/Algo-Test.git

# Navigate to the project directory
# shellcheck disable=SC2164
cd Algo-Test

# Get Arguments from the user if not provided use hosts
# Arguments - host, observer
# If user pass \hosts as the argument, it will use the hosts file
# If user pass \observer as the argument, it will use the observer file

if [ -z "$1" ]
then
    echo "No arguments supplied, using the default host file"
    FILE_PATH=$HOST_PATH
else
    if [ $1 == "hosts" ]
    then
        FILE_PATH=$HOST_PATH
    elif [ $1 == "observer" ]
    then
        FILE_PATH=$Observer_PATH
    else
        echo "Invalid argument, using the default host file"
        FILE_PATH=$HOST_PATH
    fi
fi

# Run the python script
python3 main.py $FILE_PATH



