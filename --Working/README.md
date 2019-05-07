# In Progress Robot Code

**Started on:** March 23, 2019

This repository contains working 9181S robot code for VEX Robotics Competition

This folder contains a PROS 3 project.

## Main Files

Initialization: `src\initialize.cpp`

Autonomous: `src\autonomous.cpp`

Operator control: `src\opcontrol.cpp`


Cold image: `bin\cold.package.bin`

Hot image: `bin\hot.package.bin`

## Hot/Cold Linking

This projects utilizes PROS hot/cold image linking to support wireless uploading for PROS 3.1.6.

Most source files are included in the **cold** library, thus not needing reoccurring uploads to the v5 Brain. The following files are the **hot** files and will need to be uploaded each time:

 - `src/initialize.cpp`
 - `src/autonomous.cpp`
 - `src/opcontrol.cpp`
