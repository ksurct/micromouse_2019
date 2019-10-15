#!/bin/bash


usage()
{
    echo "usage: capture [-f file ] [-p port ] [-b baud ] [-h]"
}

filename=
port=7
baud=115200

while [ "$1" != "" ]; do
    case $1 in
        -f | --file )           shift
                                filename=$1
                                ;;
        -p | --port )           shift
                                port=$1
                                ;;
        -b | --baud )           shift
                                baud=$1
                                ;;
        -h | --help )           usage
                                exit
                                ;;
        * )                     usage
                                exit 1
    esac
    shift
done

##### Main
sudo chmod 666 /dev/ttyS$port
stty -F /dev/ttyS$port sane $baud

if [ "$filename" != "" ]; then
    cat /dev/ttyS$port > $filename
else
    cat /dev/ttyS$port
fi

