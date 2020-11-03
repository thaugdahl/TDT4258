#! /usr/bin/env bash
echo "start heresy"
rm -r /home/tdt4258/Desktop/support_files/OSELAS.BSP-EnergyMicro-Gecko/local_src
cp -r /mnt/hgfs/TDT4258/TDT4258/ex3_full_game/local_src /home/tdt4258/Desktop/support_files/OSELAS.BSP-EnergyMicro-Gecko/local_src

cd /home/tdt4258/Desktop/support_files/OSELAS.BSP-EnergyMicro-Gecko/

if [ $# -eq 0 ]; then

    echo "No input args"

elif [ "$1" == "os" ]; then

    ptxdist clean
    ptxdist images
    echo "Do you want to flash? (y/n)"
    read user_input

    if [ "$user_input" == "y" ]; then
        echo "Flashing..."
        ptxdist test flash-all
        echo "Flashing complete"
    elif [ "$user_input" == "n" ]; then
        echo "Skipped flash"
    fi

elif  [ "$1" == "game" ]; then

    ptxdist clean game
    ptxdist compile game 
    echo "Compile successful ? (y/n)"
    read user_input
    if [ "$user_input" == "y" ]; then
        ptxdist targetinstall game
        ptxdist image root.romfs
        ptxdist test flash-rootfs
    elif [ "$user_input" == "n" ]; then
        echo "Skipping flash n' stuff"
    fi

elif [ "$1" == "driver" ]; then

    ptxdist clean driver-gamepad
    ptxdist compile driver-gamepad
    echo "Compile successful ? (y/n)"
    read user_input
    if [ "$user_input" == "y" ]; then
        ptxdist targetinstall driver-gamepad
        ptxdist targetinstall kernel
        ptxdist image root.romfs
        ptxdist test flash-rootfs
    elif [ "$user_input" == "n" ]; then
        echo "Skipping flash n' stuff"
    fi

elif [ "$1" == "both" ]; then

    ptxdist clean game
    ptxdist compile game

    ptxdist clean driver-gamepad
    ptxdist compile driver-gamepad

    echo "Do you want to flash? (y/n)"
    read user_input
    if [ "$user_input" == "y" ]; then
        ptxdist targetinstall game

        ptxdist targetinstall driver-gamepad
        ptxdist targetinstall kernel

        ptxdist image root.romfs
        ptxdist test flash-rootfs

    elif [ "$user_input" == "n" ]; then
        echo "Skipping flash n'"
    fi

elif [ "$1" == "setup" ]; then

    echo "WARNING: Not yet implemented"

fi