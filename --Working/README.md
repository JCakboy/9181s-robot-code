
# In Progress Robot Code

**Started on:** September 7, 2018

This folder contains a PROS 3 project.
Changes to this folder on the `master` branch should include output binaries

## Main Files

Initialization: `src\competition.cpp - ::initialize()`

Autonomous: `src\competition.cpp - ::autonomous()`

Operator control: `src\competition.cpp - ::operatorControl()`


Cold image: `bin\cold.package.bin`

Hot image: `bin\hot.package.bin`

## Hot/Cold Linking

This projects utilizes PROS hot/cold image linking to support wireless uploading for PROS 3.1.6.

Most source files are included in the **cold** library, thus not needing reoccurring uploads to the v5 Brain. The following files are the **hot** files and will need to be uploaded each time:

 - src/competition.cpp
 - src/hot.cpp
