#!/bin/bash

# DIR Variable   - '/home/cloudnet/dinushan'
DIR='/home/cloudnet/dinushan/'
HOST_PATH='/scripts/Hosts/WS/main.py'
Observer_PATH='/scripts/Observer/WS/main.py'
FOLDER_NAME='Algo-Test'

SERVER_IP=10.22.196.151

# navigate to the directory
cd $DIR

# Remove the existing project folder
rm -rfd $FOLDER_NAME

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

FULL_PATH=$DIR$FOLDER_NAME$FILE_PATH




echo "Using the file: $FULL_PATH"

# Run the python script
python3 $FULL_PATH $SERVER_IP



