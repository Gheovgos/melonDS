#!/bin/bash

# Create a new build, I created it for the sake of simplicity
# You need bios and firmware to run it
# To send the build on your device, open hbmenu, press Y and from command line send:
# nxlink -a SWITCH-IP -s /path/to/melonDS.nro 

export BUILD_NRO_PATH="" # Optional, your NRO path from build
export DEST_NRO_PATH=""       # Optional, your NRO dest path

cd build
make clean && cd .. && rm -rf build && mkdir build && cd build
cmake .. -DENABLE_OGLRENDERER=OFF -DBUILD_QT_SDL=OFF -DCMAKE_TOOLCHAIN_FILE=../cmake/Toolchain-cross-Switch.cmake -DCMAKE_BUILD_TYPE=Release
make -j$(nproc --all)

# Create switch folder structure
mkdir -p switch/melonDS/lang
cp melonDS.nro switch/melonDS/
cp -r ../lang/* switch/melonDS/lang/

cp $BUILD_NRO_PATH/melonDS.nro $DEST_NRO_PATH
