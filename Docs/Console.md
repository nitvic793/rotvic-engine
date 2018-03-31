# Console Sub-System

The game engine has an in-game console which can be leveraged for debug purposes. 

## Instantiation

The console is instantiated with the instantiation of CoreGame where the core game provides the input context to the console system as it requires the windows message pump to send in `WM_CHAR` events to the console for input processing. Console is a singleton, hence can be retrieved by calling `Console::GetInstance()`.  

## In-built commands

The engine has a few predefined console commands which can be used as necessary. Custom commands can also be defined by the developer as they see fit. 

### Rebind

`Rebind actionName key1 key2 ...`

This command rebinds any action which is defined in game. For example, `Rebind up W I J` rebinds the action "up" to keys *W*, *I* and *J*. The command expects the keys to be exactly as defined. The complete key-map can be found [here](https://github.com/nitvic793/rotvic-engine/blob/master/Engine/Keyboard.cpp)

### SetDebugDraw

`SetDebugDraw 0` disables Debug Draw. \
`SetDebugDraw 1` enables Debug Draw

### Instantiate

`Instantiate meshName identifierName`

Instantiates a given mesh as an entity and adds it to the game instance. The given mesh name should be present in the resource list and preloaded. The identifierName can be used for manipulating this entity later on. For example, `Instantiate cube myCube`

### Set Functions

`SetPosition identifierName x y z` Sets the position of given entity where x,y and z are int/float values.\
`SetRotation identifierName x y z` Sets the Rotation of given entity where x,y and z are euler angles represented in int/float values.\
`SetMaterial identifierName materialName` Sets the material of given entity with given material name. The material should be present in the resource list and preloaded.\
`SetScale identifierName x y z` Sets the position of given entity where x,y and z are int/float values.

### ToggleDebug

`ToggleDebug groupName 0` to disable debug draw for given group name.\
`ToggleDebug groupName 1` to enable debug draw for given group name.

## Enabling Console

The console is rendered only if the `bool Console::enabled` variable is true. The draw order is such that the console is rendered only in the end after drawing all entities and the debug draw system. The console can be toggled by `console->enabled != console->enabled;`

## Registering a command

The console system allows registration of custom commands when and where required. So, a command can be registered by

``` C++
console->RegisterCommand("CustomCommand", [&](std::vector<std::string> params)
{
	//Process command here
});
```

where `params` is a list of parameters passed to the command. So, for example calling `CustomCommand x y z` will fill the params vector accordingly. 
