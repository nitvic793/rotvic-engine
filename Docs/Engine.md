# Engine


### Introduction

DirectX11 based Game engine. This document aims to provide an overall idea of how the game is implemented.

### Solution Structure

- *Engine* - Contains the game engine framework. Consists of a rendering system with DirectX underneath and other interfaces which can be extended for a game. 
- *Game.Core* - Implements engine interfaces and the game logic. 
- *Game* - The main application project. Initializes the game.
- *Engine.Tests* - Contains all engine related tests.
- *Game.Core.Tests* - Contains all game logic related tests.


### Engine Structure

The engine interfaces and class structure will be defined here along with other required details to understand the architecture of the engine. 

#### Core Game and System Core

The Core Game class loads and initializes required resources, windows and binds DirectX to the main render target. Once initialization is complete, it enters into the main game loop. 

#### IGame Interface

An abstract class which represents the game instance. This interface will contain require an concrete implementation of the game logic as an instance of a concrete IGame class binds with the Core Game instance. So, a typical game implementation will look like

```C++
auto game = new CoreGame("config.json");
game->Initialize(hInstance, nCmdShow);
auto gInstance = Game::CreateInstance(); //Where "Game" is an implementation of IGame
game->Bind(gInstance);
gInstance->Initialize();
game->Run();
```

#### Entity (Game Object)

An entity represents any object that will be part of the game instance or game "scene". Any game instance will contain a map of entities referred to by strings. So, `entities["myEntity"]` will represent an object in the game instance with the name "myEntity". The entity object can have multiple properties which can be mutated or transformed throughout the lifetime of the game instance. 

For example, `entities["myEntity"]->SetPosition(Vector3(1,1,1))` sets the position of the object in the game world. The entity class can be inherited and tweaked to define more custom behavior if required. 

#### Rendering Details

- Meshes: Each entity can have a mesh associated with it. This mesh represents the 3D model of the entity.
- Materials: Each entity can also have a material associated with it. It uses the default shaders and textures if custom textures and shaders are not provided. A material can be applied to any number of entities
- Lights: Currently only Point Light and Directional Light are supported. 
- Skybox: Uses a cubemap to represent a sky and uses it's own shaders to render the skybox. 

#### Input

##### Mouse

The engine provides basic mouse input capabilities. The Mouse class is a singleton instance which can be retrieved by calling `Mouse::GetInstance()`. The class also provides a callback registration for mouse events such as MouseUp(), MouseDown(), MouseWheel() and MouseMove(). An example registration would look like

```
mouse->RegisterOnMouseMoveCallback([&](WPARAM param, int x, int y)
{
	//Do logic here...
});
```

##### Keyboard

The engine provides basic keyboard input capabilities. The Keyboard class is a singleton instance which can be retrieved by calling `Keyboard::GetInstance()`. It also exposes functions to check if a key is pressed or not and also allows action binding with multiple keys. A sample action binding would look like

```
keyboard->AddAction("up", { Up,W }); //Where Up and W are enum values
```
Hence, the it can be checked if an action is pressed by calling `keyboard->IsActionPressed("up")` which will return true if the keys *W* or the Up arrow are pressed. Calling `keyboard->AddAction()` again with the same action name will replace the existing keys with the given keys. 

*Note: Combo keys are not supported yet.* 

#### Resource Manager

The resource manager is initialized by the Core Game class. The resource manager loads all needed resources up front. All resources such as Meshes, Materials, Shaders, Textures, Samplers etc are managed by the resource manager class. All resources are stored in key-value pairs using STL maps. Hence, retrieving a resource becomes as trivial as `resource->GetMesh("cube")`. 

Currently, the resources are not loaded from an external file. It will be implemented in the next iteration. 

#### Terrain

Derived from entity class, represents a terrain in-game. Loads a heightmap from BMP file and loads the height values from the file. A terrain object can be treated as any other entity. Hence, A terrain object can be added to the Game's entity lists by calling `AddEntity(terrainObject)`. 

#### DebugDraw 

#### In-Game Console

