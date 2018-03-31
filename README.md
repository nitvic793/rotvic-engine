# DirectX 11 Game Engine

[![Build status](https://ci.appveyor.com/api/projects/status/l56arbrrf5mpbp5p?svg=true)](https://ci.appveyor.com/project/nitvic793/rotvic-engine)

For more elaborate documentation, check out the [wiki](https://github.com/nitvic793/rotvic-engine/wiki).

## Steps to build

This project requires the Windows 10 SDK installed. 

- Open project Visual Studio 2017
- Set project build in x64 platform.
- Set "Game" as startup project.
- Build and run 

## Known issues

- The physics middleware (ReactPhysics3D) throws up a few warnings when building. Can be safely ignored. 
- There is a slight change the physics middleware might throw an assert exception. This is a known 3rd party issue.  

## Solution Structure

- Engine - Contains the game engine framework. Consists of a rendering system with DirectX underneath and other interfaces which can be extended for a game. 
- Game.Core - Implements the engine interfaces and the game logic. 
- Game - The main application project. Initializes the game.
- Engine.Tests - Contains all engine related tests.
- Game.Core.Tests - Contains all game logic related tests.