# 2D Puzzle Springs Assembler

This project is a tool for reconstructing 2D jigsaw puzzles made of polygonal shapes. Each puzzle piece is treated as a rigid body, and the project simulates springs connecting pairs of pieces. A pair of mounting anchors is called a "mating," and the complete set of matings is provided as input. The system aims to minimize elastic energy to achieve convergence, ultimately outputting the final positions of the pieces. To facilitate integration with various third parties, the simulator interfaces via HTTP requests and responses. Reconstructions can be executed in silent mode, visual mode (for debugging), and interactive mode.


<!--The input is the polygonal representation of the shapes, and pairs of vertices to be tighten. 

It simulates each puzzle piece as a rigid body, and for a given input, it mounts springs between vertices of different pieces 

This is a C++ project to solve 2D puzzles for convex and concave polygons. It acts as a physical simulation, with each piece considered a rigid body. Springs are connected between them to create an assembly. To ease the integration to multiple third parties, the simulator runs as an HTTP server, responding in the reconstruction information and optionally visual demonstartion of it. 

TODO: describe here it is for simple polygons, having vertices... 

multiple running modes: interactive, silent, visual,... Offon, Off...-->

<!--![Demo springs](https://github.com/user-attachments/assets/34ae33eb-1e41-467d-b963-4418a0968270)-->

<p align="center">
	<img src="https://github.com/user-attachments/assets/07bafee3-0d37-4cec-abea-644f06b61b42" alt="Demo springs" style="transform: rotate(90deg);">
</p>	

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
Once the server is up http requests are sent to it. For quick start you can use curl or postman using http request described below. 

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
|InteractiveOn|Boolean|No|false|Execute the reconstruction in interative mode| <!--If specified, true is set-->
|visibilityOn|Boolean|No|false|If the parameter InteractiveOn is not specified, a window visualalizing the is opened|
|collideOff|Boolean|No|false|If the parameter InteractiveOn is not specified, the overlapping between between the pieces during the reconstruction is enabled|
|boardSize|string|No|small|Specifies the board size in which the reconstruction lives. Allowed values are small,medium, and large|
|boardHeight|Int|No|None|If the parameter boardSize is not specified, this parameter specifies custom height of the board. Recommended values are between 10 to 80. Please refer for Box2d documentation for more details on allowed size of the bodies.|
|boardWidth|Int|No|No|If the parameter boardSize is not specified, this parameter specifies custom width of the board. Recommended values are between 10 to 80. Please refer for Box2d documentation for more details on allowed size of the bodies.|
|screenHeight|Int|No|1380|If the parameteres InteractiveOn or visibilityOn are specified, specified the screen height which visualizes the reconstruction|
|screenWidth|Int|No|1380|If the parameteres InteractiveOn or visibilityOn are specified, specified the screen Width which visualizes the reconstruction|
|unFixingPiece|Boolean|No|false|Specifying this parameter makes all the pieces moving pieces. Otherwise, one piece is seleceted to be fixed (nailed) at the center of the board|
|seedInitialPositions|Int|No|None|Specifies the seed number for generating the random positions. Otherwise, all the pieces are positioned at the board center|
<!--|disableJointsCollide|Boolean||||-->
<!--|isDrawOnlyPolygons|||||-->
<!--|finalScreenShotPath|||||-->
<!--||||||-->


#### Request Body
The request body contains the description of the pieces and the springs connecting between them. Every spring connects between two vertices of two different pieces. The request body should be a JSON object containing the following fields:
```json
{
    "pieces": [
        {
            "id": "string",
            "polygon": [[number, number], ...],
            "fixedRotationAngle": number
        },
        ...
    ],
    "matings": [
        {
            "firstPiece": "string",
            "firstPieceLocalCoords": [number, number],
            "secondPiece": "string",
            "secondPieceLocalCoords": [number, number]
        },
        ...
    ]
}

```

##### `pieces`
- **Type:** Array of Objects
- **Description:** List of pieces with their respective properties.
  
  Each object in the `pieces` array has the following structure:
  
  - `id`
    - **Type:** String
    - **Description:** Unique identifier for the piece.
  - `polygon`
    - **Type:** Array of [number, number]
    - **Description:** Coordinates defining the shape of the piece.
  - `fixedRotationAngle` (optional)
    - **Type:** Number
    - **Description:** Fixed rotation angle of the piece. Optional field.

##### `matings`
- **Type:** Array of Objects
- **Description:** List of mating relationships between pieces. Each spring buckles two vertices of two different pieces.
  
  Each object in the `matings` array has the following structure:
  
  - `firstPiece`
    - **Type:** String
    - **Description:** Identifier of the first piece in the mating.
  - `firstPieceLocalCoords`
    - **Type:** [number, number]
    - **Description:** Local coordinates of the spring mount on the first piece.
  - `secondPiece`
    - **Type:** String
    - **Description:** Identifier of the second piece in the mating.
  - `secondPieceLocalCoords`
    - **Type:** [number, number]
    - **Description:** Local coordinates of the spring mount on the second piece.




#### Example Request
```shell
	curl --location 'http://localhost:8888/v1/reconstructions?unFixingPiece=1&visibilityOn=1&collideOff=1&boardSize=small' \
--header 'Content-Type: application/json' \
--data '{
    "pieces":[
        {
            "id": "triangle1",
            "polygon":[[0,0],[1000,1500],[1500,-500]],
            "fixedRotationAngle":-1.398445025086403
        },
        {
            "id": "triangle2",
            "polygon":[[0,0],[1000,1500],[-2500,-500]]
        },
        {
            "id": "triangle3",
            "polygon":[[0,0],[1500,-500],[-2500,-500]]
        }
        
    ],
    "matings":[
        {
            "firstPiece":"triangle1",
            "firstPieceLocalCoords": [0,0],
            "secondPiece":"triangle2",
            "secondPieceLocalCoords": [0,0]
        },
        {
            "firstPiece":"triangle1",
            "firstPieceLocalCoords": [1000,1500],
            "secondPiece":"triangle2",
            "secondPieceLocalCoords": [1000,1500]
        },
        {
            "firstPiece":"triangle2",
            "firstPieceLocalCoords": [0,0],
            "secondPiece":"triangle3",
            "secondPieceLocalCoords": [0,0]
        },
        {
            "firstPiece":"triangle2",
            "firstPieceLocalCoords": [-2500,-500],
            "secondPiece":"triangle3",
            "secondPieceLocalCoords": [-2500,-500]
        },
        {
            "firstPiece":"triangle1",
            "firstPieceLocalCoords": [0,0],
            "secondPiece":"triangle3",
            "secondPieceLocalCoords": [0,0]
        },
        {
            "firstPiece":"triangle1",
            "firstPieceLocalCoords": [1500,-500],
            "secondPiece":"triangle3",
            "secondPieceLocalCoords": [1500,-500]
        }
        
    ]
}

'
```

#### Response 
```json
{
    "jointsAfterEnableCollision": {
        "springs": [
            {
                "firstPieceId": "string",
                "firstPieceLocalCoords": [number, number],
                "jointLength": number,
                "secondPieceId": "string",
                "secondPieceLocalCoords": [number, number]
            },
            ...
        ],
        "sumSpringsLength": number
    },
    "piecesBeforeEnableCollisionCoords": [
        {
            "coordinates": [[number, number], ...],
            "pieceId": "string"
        },
        ...
    ],
    "piecesFinalCoords": [
        {
            "coordinates": [[number, number], ...],
            "pieceId": "string"
        },
        ...
    ],
    "piecesFinalTransformations": [
        {
            "pieceId": "string",
            "rotationRadians": number,
            "translateVectorX": number,
            "translateVectorY": number
        },
        ...
    ]
}
```

##### `jointsAfterEnableCollision`
- **Type:** Object
- **Description:** Contains details about the joints after enabling collision.
  
  - `springs`
    - **Type:** Array of Objects
    - **Description:** List of spring connections between pieces.
      
      Each object in the `springs` array has the following structure:
      
      - `firstPieceId`
        - **Type:** String
        - **Description:** Identifier of the first piece.
      - `firstPieceLocalCoords`
        - **Type:** Array of [number, number]
        - **Description:** Local coordinates on the first piece.
      - `jointLength`
        - **Type:** Number
        - **Description:** Length of the joint.
      - `secondPieceId`
        - **Type:** String
        - **Description:** Identifier of the second piece.
      - `secondPieceLocalCoords`
        - **Type:** Array of [number, number]
        - **Description:** Local coordinates on the second piece.
  
  - `sumSpringsLength`
    - **Type:** Number
    - **Description:** Total length of all springs.

##### `piecesBeforeEnableCollisionCoords`
- **Type:** Array of Objects
- **Description:** Coordinates of pieces before enabling collision.
  
  Each object in the `piecesBeforeEnableCollisionCoords` array has the following structure:
  
  - `coordinates`
    - **Type:** Array of [number, number]
    - **Description:** Coordinates defining the shape of the piece.
  - `pieceId`
    - **Type:** String
    - **Description:** Identifier of the piece.

##### `piecesFinalCoords`
- **Type:** Array of Objects
- **Description:** Final coordinates of pieces at the end of the reconstruction.
  
  Each object in the `piecesFinalCoords` array has the following structure:
  
  - `coordinates`
    - **Type:** Array of [number, number]
    - **Description:** Coordinates defining the shape of the piece.
  - `pieceId`
    - **Type:** String
    - **Description:** Identifier of the piece.

##### `piecesFinalTransformations`
- **Type:** Array of Objects
- **Description:** Final transformations applied to each piece at the end of the reconstruction.
  
  Each object in the `piecesFinalTransformations` array has the following structure:
  
  - `pieceId`
    - **Type:** String
    - **Description:** Identifier of the piece.
  - `rotationRadians`
    - **Type:** Number
    - **Description:** Rotation angle in radians.
  - `translateVectorX`
    - **Type:** Number
    - **Description:** Translation vector on the X-axis.
  - `translateVectorY`
    - **Type:** Number
    - **Description:** Translation vector on the Y-axis.



#### Example Response
```json
	{
    "jointsAfterEnableCollision": {
        "springs": [
            {
                "firstPieceId": "triangle1",
                "firstPieceLocalCoords": [
                    0.0,
                    0.0
                ],
                "jointLength": 0.01890582963824272,
                "secondPieceId": "triangle2",
                "secondPieceLocalCoords": [
                    0.0,
                    0.0
                ]
            },
            {
                "firstPieceId": "triangle1",
                "firstPieceLocalCoords": [
                    999.9999389648438,
                    1500.0
                ],
                "jointLength": 0.018003148958086967,
                "secondPieceId": "triangle2",
                "secondPieceLocalCoords": [
                    999.9999389648438,
                    1500.0
                ]
            },
            {
                "firstPieceId": "triangle2",
                "firstPieceLocalCoords": [
                    0.0,
                    0.0
                ],
                "jointLength": 0.026041222736239433,
                "secondPieceId": "triangle3",
                "secondPieceLocalCoords": [
                    0.0,
                    0.0
                ]
            },
            {
                "firstPieceId": "triangle2",
                "firstPieceLocalCoords": [
                    -2500.0,
                    -499.9999694824219
                ],
                "jointLength": 0.01999364234507084,
                "secondPieceId": "triangle3",
                "secondPieceLocalCoords": [
                    -2500.0,
                    -499.9999694824219
                ]
            },
            {
                "firstPieceId": "triangle1",
                "firstPieceLocalCoords": [
                    0.0,
                    0.0
                ],
                "jointLength": 0.015546763315796852,
                "secondPieceId": "triangle3",
                "secondPieceLocalCoords": [
                    0.0,
                    0.0
                ]
            },
            {
                "firstPieceId": "triangle1",
                "firstPieceLocalCoords": [
                    1500.0,
                    -499.9999694824219
                ],
                "jointLength": 0.020058179274201393,
                "secondPieceId": "triangle3",
                "secondPieceLocalCoords": [
                    1500.0,
                    -499.9999694824219
                ]
            }
        ],
        "sumSpringsLength": 0.11854878067970276
    },
    "piecesBeforeEnableCollisionCoords": [
        {
            "coordinates": [
                [
                    0.0,
                    0.0
                ],
                [
                    1649.2757568359375,
                    -727.935302734375
                ],
                [
                    -235.3429718017578,
                    -1563.526123046875
                ]
            ],
            "pieceId": "triangle1"
        },
        {
            "coordinates": [
                [
                    -0.41866299510002136,
                    9.743690490722656
                ],
                [
                    1641.6376953125,
                    -734.33349609375
                ],
                [
                    -898.3973999023438,
                    2395.877685546875
                ]
            ],
            "pieceId": "triangle2"
        },
        {
            "coordinates": [
                [
                    8.678913116455078,
                    4.904747009277344
                ],
                [
                    -241.9295196533203,
                    -1556.2471923828125
                ],
                [
                    -889.3909301757813,
                    2391.00390625
                ]
            ],
            "pieceId": "triangle3"
        }
    ],
    "piecesFinalCoords": [
        {
            "coordinates": [
                [
                    0.0,
                    0.0
                ],
                [
                    1649.2757568359375,
                    -727.935302734375
                ],
                [
                    -235.3429718017578,
                    -1563.526123046875
                ]
            ],
            "pieceId": "triangle1"
        },
        {
            "coordinates": [
                [
                    8.512496948242188,
                    16.88098907470703
                ],
                [
                    1657.4229736328125,
                    -711.881103515625
                ],
                [
                    -911.6358032226563,
                    2394.553466796875
                ]
            ],
            "pieceId": "triangle2"
        },
        {
            "coordinates": [
                [
                    -14.6965970993042,
                    5.070685863494873
                ],
                [
                    -254.55711364746094,
                    -1557.768798828125
                ],
                [
                    -929.1633911132813,
                    2384.934326171875
                ]
            ],
            "pieceId": "triangle3"
        }
    ],
    "piecesFinalTransformations": [
        {
            "pieceId": "triangle1",
            "rotationRadians": -1.3984450101852417,
            "translateVectorX": 0.0,
            "translateVectorY": 0.0
        },
        {
            "pieceId": "triangle2",
            "rotationRadians": -1.3989464044570923,
            "translateVectorX": 251.43336486816406,
            "translateVectorY": 566.517822265625
        },
        {
            "pieceId": "triangle3",
            "rotationRadians": -1.4013348817825317,
            "translateVectorX": -399.47222900390625,
            "translateVectorY": 277.41192626953125
        }
    ]
}
```

<!--
## Quick Start

For an automatic run of the program with the provided example, open your terminal or command line and navigate to the project directory, then enter the following command:

```shell
./build/2D_puzzle_springs_assembler --puzzleDir data/example/group_39
```

This command will run the project using the puzzle files located in the data/example/group_39 directory.
-->


## Help

Known issue: If you encounter an error related to DLL missing, a workaround is to switch the running mode to `RelWithDebInfo` in CMake, instead of the default `Debug` mode.

