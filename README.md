# README

## Team 7 Members

- **bdurbin313**: Benjamin Durbin
- **graduatedgrate24324**: Colin Smith
- **wearymango**: Miraya Hampton
- **willrwhitlow**: Will Whitlow
- **DeltaProcess**: Dylan Gray

## Description

## First Time Installation

Run the following commands in order to update the VM so dependencies can install:

1. `sudo nano`
2. ctrl + R
3. `/etc/apt/sources.list/`
4. edit the sources.list file to look like this:
   
       # See https://wiki.debian.org/SourcesList for more information.  
       deb http://archive.debian.org/debian bullseye main non-free  
       deb-src http://archive.debian.org/debian bullseye main  
        
       deb http://archive.debian.org/debian bullseye-updates main non-free  
       deb-src http://archive.debian.org/debian bullseye-updates main  
        
       # deb http://security.debian.org/debian-security/ bullseye-security main non-free  
       # deb-src http://security.debian.org/debian-security/ bullseye-security main
   
5. ctrl + S
6. `/etc/apt/sources.list/`
7. Y
8. ctrl + X
9. `sudo apt update`

Then, run the following commands to install all dependencies and 

10. `sudo apt install cmake tcl8.6-dev tk8.6-dev libboost-all-dev libpqxx-dev`
11. `git clone https://github.com/DeltaProcess/software_engineering_team_7_best_team`

## Running The Software

1. `cd software_engineering_team_7_best_team`
2. `git pull`
3. `cmake -S . -B ./build -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++`
4. `cmake --build ./build`
5. `./build/main`
