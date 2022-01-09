# ⚙️ 9181S Robot Code: 2018-2019 Season

This folder contains a [PROS 3](http://pros.cs.purdue.edu) project on the V5 platform with 9181S robot code for the 2018-2019 season of VEX Robotics Competition.

## Main Files

Initialization: `src\competition.cpp - ::initialize()`

Autonomous: `src\competition.cpp - ::autonomous()`

Operator control: `src\competition.cpp - ::operatorControl()`

---

## Hot/Cold Linking

This projects utilizes PROS hot/cold image linking to support wireless uploading for PROS 3.1.6.

Cold image: `bin\cold.package.bin`

Hot image: `bin\hot.package.bin`

Most source files are included in the **cold** library, thus not needing reoccurring uploads to the v5 Brain. The following files are the **hot** files and will need to be uploaded each time:

- src/competition.cpp
- src/hot.cpp
