# 2D Puzzle Springs Assembler

This is a C++ project to solve 2D puzzles for convex and concave polygons. It acts as a physical simulation, with each piece considered a rigid body. Springs are connected between them to create an assembly. 

## Table of Contents

- [Installing](#installing)
- [Executing the Program](#executing-the-program)
- [Quick Start](#quick-start)
- [Help](#help)

## Installing

Please note, the following instructions are specified for Windows, which was used for development.

1. **SFML**: Install SFML using the vcpkg installer. You can download vcpkg and follow the instructions provided in their repository to install SFML.

2. **Eigen**: Download Eigen 3.4.0 from the official website. After downloading, place the files inside the project folder in a subfolder named `Eigen`. The CMakeLists.txt file references this folder.

3. **Box2D**: Clone the Box2D repository (version 2.4.1 was used for development). After cloning, place the repository inside the project folder in a subfolder named `box2d`. The CMakeLists.txt file references this folder.

Once the dependencies are set up, you need to generate the build files:

1. **Create a build directory**: From the project's root directory, create a new directory named `build`.

2. **Generate build files with CMake**: Navigate to the newly created `build` directory and use CMake to generate the build files. 

If you are using CMake GUI:

- Set the "Where is the source code" field to your project root directory.
- Set the "Where to build the binaries" field to the `build` directory you just created.
- Click on `Configure` and `Generate`.

If you are using command line, navigate to the `build` directory and enter:

```shell
cmake ..
make
```

For more detailed instructions, please refer to the official documentation of each library.

## Executing the Program

To run the 2D Puzzle Springs Assembler, you will need to provide certain parameters:

1. **Puzzle Directory** (`--puzzleDir`): This is the directory where all the puzzle files are. It should contain the `pieces.csv` and `springs_anchors.csv` files, and a subfolder named `images` containing all the images of the pieces. The name of the image files should correspond to the ids of the pieces as given in the `pieces.csv` file.

2. **Manual Simulation** (`--ManualSimulation`): This is an optional parameter. If you want to control the flow of the physical simulation, specify this option.

When running in manual mode, you can use the following keyboard commands:

- `p`: Toggle polygon rendering (for debug)
- `o`: Toggle image rendering
- `m`: Connect the next spring
- `e`: Give random impulse to each piece
- `c`: Enable/disable collision between the fragments

## Quick Start

For an automatic run of the program with the provided example, open your terminal or command line and navigate to the project directory, then enter the following command:

```shell
./build/2D_puzzle_springs_assembler --puzzleDir data/example/group_39
```

This command will run the project using the puzzle files located in the data/example/group_39 directory.

## Help

If you encounter an error related to DLL missing, a workaround is to switch the running mode to `RelWithDebInfo` in CMake, instead of the default `Debug` mode.

