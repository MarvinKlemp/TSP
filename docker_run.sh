#!/bin/bash
cmake -E make_directory ./build
cd /opt/build
cmake ../ -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON -DCMAKE_BUILD_TYPE=RELEASE
cmake --build . --config RELEASE

bin/tsp 1000 100 1000

cd /opt
./visualize.py build/genetic_tsp.csv
