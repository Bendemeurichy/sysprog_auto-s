# Project Systeemprogrammeren 2022

This is the repository with the assignment for the project of Systeemprogrammeren, for Project Groep 17.
The full assignment for the Sysprog 2022 project can be found in the [assigment document](https://github.ugent.be/Systeemprogrammeren/docs/blob/master/project_sysprog22_opgave.pdf)

Students:

* Ben De Meurichy

* Thibaud Collyn


Assistant for this group: Sander Borny (Sander.Borny@UGent.be)

The goal of the project is to implement a puzzle game.

You are expected to work on your solution in this repo, and use the github issues to communicate with each other and with the assistents.

After each project deadline:

* We will retrieve the code from this repository, and use it to grade the project (We will look at the `master` branch!).
* We expect to find a file `REPORT_1.md` (part1) or `REPORT_2.md` (part2) in this repository, which contains a short report. Check the assigment document on what this should include.

## Contents

* [Assignment files](#files)
* [Install the development environment](#devenv)
* [Building the project in Visual Studio Code](#build-vscode)
* [Debugging in Visual Studio Code](#debug-vscode)
* [List of all tasks in Visual Studio Code](#tasks-reference)
* [Building the project on the Raspberry Pi](#build-raspi)

## Assignment files<a name="files"></a>

* The [assigment document](https://github.ugent.be/Systeemprogrammeren/docs/blob/master/project_sysprog22_opgave.pdf), describes in details what you have to do.
* The assignment code is in this repository. This code is a basic starting point. It already does part of what is required. You can compiled and run it, and you should see the first demo level.
  * `spg_part1_piexe`, `spg_part1_edit_piexe` and `spg_part2_piexe` are a binary executable for raspberry Pi, demonstrating a working solution. This is useful to know exactly what is expected.
  * `REPORT_1.md` and `REPORT_2.md` are starting points for the report we expect to find at each deadline.
  * `GRADING_1.md` and `GRADING_2.md` are lists of required functionalities. These serve as an illustration of what the project will be graded on. You can edit them to keep track of your progress. Change `[ ]` to `[*]` to mark an item.
  * The binary levels `game/level/level_files/*.blvl`, the simplified binary levels `game/level/level_files/*_simple.blvl`, and the template levels `game/level/level_files/*.tlvl`.

## Install the development environment<a name="devenv"></a>

[Instructions for how to install development environment](https://github.ugent.be/Systeemprogrammeren/docs#how-to-install-the-development-environment).

## Building the project in Visual Studio Code<a name="build-vscode"></a>

> *Note: Visual Studio code is **not** used on the Raspberry Pi. For build instructions on the **Raspberry Pi, look at ["Building the project on the Raspberry Pi"](#build-raspi).***

1. Clone the Git repository to the location you want by opening a terminal, navigating to the directory where the project needs to be and executing the following commands.

   ```bash
   # Navigate to the directory where you want to place the project folder
   cd Desktop/
   # Clone the repository. This downloads the repository to the current directory.
   git clone https://github.ugent.be/Systeemprogrammeren2022/Systeemprogrammeren2022-project-groep-X.git
   # Go into the repository directory
   cd Systeemprogrammeren2022-project-groep-X
   ```
2. Run the "cmake" task by choosing "Terminal" > "Run Task.." and clicking on "cmake".

   ![Run Task.. in the menu.](https://github.ugent.be/Systeemprogrammeren/docs/blob/master/img/tasks-menu.png)

   ![All tasks.](https://github.ugent.be/Systeemprogrammeren/docs/blob/master/img/tasks-dropdown.png)

3. Run the "build spg_part1" task by choosing "Terminal" > "Run Build Task.." and clicking on "build spg_part1".

   See [List of all tasks in Visual Studio Code](#tasks-reference) to find a description of what each task does.

## Debugging in Visual Studio Code<a name="debug-vscode"></a>

You can debug by selecting `(gdb) Launch (sysprog-2022-groepX)` in the debug window and press run to debug. Note that you first need to run the "cmake" task once before you start debugging the first time.

For more info on the vscode debugger see [Debugging in vscode](https://code.visualstudio.com/docs/editor/debugging).

## Visual Studio code tasks reference<a name="tasks-reference"></a>

The project workspace has a number of tasks preconfigured that you should use to compile and run the game.

* `cmake` generates the build files, you should only run this task the first time you open the workspace.
* `cmake rm cache` removes the build files. Use this if you experience strange build errors. After this task, you will need to rerun the cmake task.
* `build spg_part1` Builds part one.
* `run spg_part1 (+ build)` Builds and runs part one; this starts `spg_game.c`, linked to all C code.
* `build spg_part2` Builds part two.
* `run spg_part2 (+ build)` Builds and runs part two; this starts `spg_game.cpp`, linked to the C and C++ code.
* `clean all` Remove all previous compiled files. Use this to force a full recompilation or "clean build".

> *Note: use the <kbd>crl</kbd> - <kbd>shift</kbd> - <kbd>b</kbd> shortcut to quickly access al build and run tasks.*

## Building the project on the Raspberry Pi<a name="build-raspi"></a>

```bash
# Navigate to the directory where you want to place the project folder
cd ~
# Clone the repository. This downloads the repository to the current directory.
git clone https://github.ugent.be/Systeemprogrammeren2022/Systeemprogrammeren2022-project-groep-X.git
# Go into the repository directory
cd project-groepX

# Run the demo provided along with the assignment code
./spg_part1_piexe
# or ./spg_part1_edit_piexe
# or ./spg_part2_piexe

# Compile your own code
cmake .
make -j 4

# Run your code
./spg_part1
./spg_part2
```

## Copyright & License info

### Code

Copyright © teaching staff of the course "Systeemprogrammeren" at the Faculty of Engineering and Architecture - Ghent University.

You can use and modify this code as part of your education, but you are not allowed to share this code, your modifications, and your solutions.
Please contact the teaching staff if you want to use (part of) this code for teaching other courses.

Note that this code is written to be a good programming project assignment.
When implemented a real game, a lot of choices would be very different.

### Graphics

Most graphics are from kenney.nl, and are licensed under [Creative Commons Zero](https://creativecommons.org/choose/zero/) (CC0). Additional graphics by teaching staff of the course "Systeemprogrammeren" are CC0 as well.

### Libraries

* SDL2 library: [zlib license](https://www.zlib.net/zlib_license.html)
* glm library: [MIT License](https://mit-license.org/)
* glmc library:  [MIT License](https://mit-license.org/)