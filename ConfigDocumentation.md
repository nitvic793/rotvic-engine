# Engine

## Config File Structure

The config file is expected to be in JSON format with the name "config.json" in the root directory of the application. The Config file contains:

- **width**: The width of the window.
- **height**: The height of the window. 
- **speed**: Speed of the movement object in the game.
- **mesh**: The mesh file to load. 

Sample config might look like below:

```json
{
    "width" : "1280",
    "height" : "720",
    "title" : "DX Game",
    "speed" : "20.0",
    "mesh": "mesh.msh"
}
```
### Loading Mechanism

- The engine loads the "config.json" on initialization. 
- The config file is saved in a STL map in memory where key-value pairs are both of std::string types. 
- The config map is made available to the game instance too. 
- The game instance loads the speed value from the config map. If an invalid value is detected, the speed value defaults to 20.0
- The game instance also loads the mesh according to the given config data. The engine should throw an exception if the mesh file is unable to load. 

### Config file Specifications

- All config values are expected to be in string literal format. 
- The width and height of the screen need to follow one of the supported aspect ratios(4:3, 16:9, 16:10). 
- The mesh file indicated in the config file needs to be present in the root directory of the application along with the config file. 
- The mesh file will need to adhere to the Mesh File specification described below.

## Mesh File Structure

The mesh file structure supported by the Engine is a simplified form of the Wavefront OBJ file format. Only vertices and indices can be described in the file. 

A vertex may be described in one line and should always start with the letter 'v' and contain the X, Y and Z coordinates in decimal single space separated. For example
```
v 0.0 1.0 0.0
```

The indices which indicate the winding order for the vertices should start with the letter 'f' followed by the order of the vertices in integer format. For example
```
f 1 2 3
```

Hence, a 3 vertex mesh file would look like
```
v 0.0 1.0 0.0
v 1.5 -1.0 0.0
v -1.5 -1.0 0.0
f 1 2 3
``` 

## Sample Config and Mesh Files

- The main game project under the folder "RotvicEngine" contains a sample config.json and two mesh files - mesh.msh and mesh2.msh. 
- On a fresh build of the solution in "Debug" mode, the post build script should copy the files to the output directory. 
- In case the files are not copied, copy the files to output folder where the application is generated.  



