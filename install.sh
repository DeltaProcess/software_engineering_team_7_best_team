#!/bin/bash
sudo apt update
sudo apt install -y cmake tcl-dev tk-dev libboost-all-dev libpqxx-dev libopencv-dev
cmake -S . -B ./build -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++
cmake --build ./build
