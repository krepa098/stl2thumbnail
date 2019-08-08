#!/bin/sh
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX="/usr" -DBUILD_GENERATOR_GNOME=ON -DBUILD_GENERATOR_KDE=ON
cmake --build . --config Release
sudo make install
kbuildsycoca5
