# 2D Puzzle Springs Assembler

This is a C++ project to solve 2D puzzles for convex and concave polygons. It acts as a physical simulation, with each piece considered a rigid body. Springs are connected between them to create an assembly. To ease the integration to multiple third parties, the simulator runs as an HTTP server, responding in the reconstruction information and optionally visual demonstartion of it. 

TODO: describe here it is for simple polygons, having vertices... 

multiple running modes: interactive, silent, visual,... Offon, Off...

## Table of Contents

- [Installing](#installing)
- [Executing the Program](#executing-the-program)
- [Quick Start](#quick-start)
- [Help](#help)

## Installing

Please note, the following instructions are specified for Windows, which was used for development.

1. **SFML**: Install SFML using the vcpkg installer. You can download vcpkg and follow the instructions provided in their repository to install SFML.

2. **Box2D**: Clone the Box2D repository (version 2.4.1 was used for development). After cloning, place the repository inside the project folder in a subfolder named `box2d`. The CMakeLists.txt file references this folder.

Once the dependencies are set up, you need to generate the build files:

1. **Create a build directory**: From the project's root directory, create a new directory named `build`.

2. **Generate build files with CMake**: Navigate to the newly created `build` directory and use CMake to generate the build files. 

<!--#If you are using CMake GUI:

#- Set the "Where is the source code" field to your project root directory.
#- Set the "Where to build the binaries" field to the `build` directory you just created.
#- Click on `Configure` and `Generate`.

#If you are using command line, navigate to the `build` directory and enter:-->
Using a command line, run the following:

```shell
mkdir build
cd build
cmake ..
make
```

For more detailed instructions, please refer to the official documentation of each library.

## Runnig the server

Since the executed program is an HTTP server, interfacing with is done via HTTP request. To run the HTTP server, run the executable under the ``build`` directory with the name ``springs_assembler_sfml`` (it depends on the Release version, try under RelWithDebInfo). For example in Windows you can run:

```
springs_assembler_sfml --port <PORT_NUMBER>
```

Where port is an optional parameter, which its default value is ``8888``.

## THE REST API Endpoints

The following two endpoints are available:

### `/v1/sanity`

#### Description
This endpoint allows to perform a sanity check, it make sure the REST API is reachable. 

#### HTTP Method
`GET`

#### URL
`http://localhost:8888/v1/sanity`

#### Example Request
```
	 curl -X GET "http://localhost:8888/v1/sanity"
```

#### Example Response
```
Hello World!
```

### `/v1/reconstructions`

#### Description
This endpoint performs a reconstruction for the given pieces using the given pairs of vertices matching. The http request parameters determine the reconstruction parameters while the request body describe the matching pieces.

#### HTTP Method
`POST`

#### URL
`http://localhost:8888/v1/reconstructions`

#### Request Headers
- `Content-Type: application/json`

#### Request Parameters

| Parameter		| Type	| Required	| Default Value | Description	|
|---------------|-------|-----------|---------------|---------------|
|InteractiveOn|Boolean|No|False|If specified, true is set|




#### Example Request
```
	curl --location 'http://localhost:8888/v1/reconstructions?visibilityOn=1' \
	--header 'Content-Type: application/json' \
	--data '{
		"pieces":[
			{
				"id": "square",
				"polygon":[[500,500],[-500,500],[-500,-500],[500,-500]],
				"fixedRotationAngle":-1.398445025086403
			},
			{
				"id": "triangle",
				"polygon":[[500,500],[-500,500],[-500,-500]]
			}
		],
		"matings":[
			{
				"firstPiece":"square",
				"firstPieceLocalCoords": [-500,500],
				"secondPiece":"triangle",
				"secondPieceLocalCoords": [500,500]
			}
			,{
				"firstPiece":"square",
				"firstPieceLocalCoords": [500,500], 
				"secondPiece":"triangle",
				"secondPieceLocalCoords": [-500,500] 
			}
		]
	}
	
	'
```

#### Example Response
```
	{
    "jointsAfterEnableCollision": {
        "springs": [
            {
                "firstPieceId": "square",
                "firstPieceLocalCoords": [
                    -499.9999694824219,
                    499.9999694824219
                ],
                "jointLength": 0.01549101248383522,
                "secondPieceId": "triangle",
                "secondPieceLocalCoords": [
                    499.9999694824219,
                    499.9999694824219
                ]
            },
            {
                "firstPieceId": "square",
                "firstPieceLocalCoords": [
                    499.9999694824219,
                    499.9999694824219
                ],
                "jointLength": 0.015522761270403862,
                "secondPieceId": "triangle",
                "secondPieceLocalCoords": [
                    -499.9999694824219,
                    499.9999694824219
                ]
            }
        ],
        "sumSpringsLength": 0.031013773754239082
    },
    "piecesBeforeEnableCollisionCoords": [
        {
            "coordinates": [
                [
                    578.3419189453125,
                    -406.84222412109375
                ],
                [
                    406.84222412109375,
                    578.3419189453125
                ],
                [
                    -578.3419189453125,
                    406.84222412109375
                ],
                [
                    -406.84222412109375,
                    -578.3419189453125
                ]
            ],
            "pieceId": "square"
        },
        {
            "coordinates": [
                [
                    415.2707824707031,
                    583.6858520507813
                ],
                [
                    587.7718505859375,
                    -401.32330322265625
                ],
                [
                    1572.781494140625,
                    -228.82269287109375
                ]
            ],
            "pieceId": "triangle"
        }
    ],
    "piecesFinalCoords": [
        {
            "coordinates": [
                [
                    578.3419189453125,
                    -406.84222412109375
                ],
                [
                    406.84222412109375,
                    578.3419189453125
                ],
                [
                    -578.3419189453125,
                    406.84222412109375
                ],
                [
                    -406.84222412109375,
                    -578.3419189453125
                ]
            ],
            "pieceId": "square"
        },
        {
            "coordinates": [
                [
                    420.83642578125,
                    584.9852294921875
                ],
                [
                    592.3685913085938,
                    -400.1932067871094
                ],
                [
                    1577.5469970703125,
                    -228.6610565185547
                ]
            ],
            "pieceId": "triangle"
        }
    ],
    "piecesFinalTransformations": [
        {
            "pieceId": "square",
            "rotationRadians": -1.3984450101852417,
            "translateVectorX": 0.0,
            "translateVectorY": 0.0
        },
        {
            "pieceId": "triangle",
            "rotationRadians": 1.7431811094284058,
            "translateVectorX": 863.583984375,
            "translateVectorY": -14.623164176940918
        }
    ]
}
```

## Quick Start

<!--For an automatic run of the program with the provided example, open your terminal or command line and navigate to the project directory, then enter the following command:

```shell
./build/2D_puzzle_springs_assembler --puzzleDir data/example/group_39
```

This command will run the project using the puzzle files located in the data/example/group_39 directory.
-->

## Help

Known issue: If you encounter an error related to DLL missing, a workaround is to switch the running mode to `RelWithDebInfo` in CMake, instead of the default `Debug` mode.

