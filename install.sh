#!/bin/sh
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX="/usr"
cmake --build . --config Release
sudo make install
kbuildsycoca5
