# DirectX 11 Game Engine

[![Build status](https://ci.appveyor.com/api/projects/status/l56arbrrf5mpbp5p?svg=true)](https://ci.appveyor.com/project/nitvic793/rotvic-engine)

For more elaborate documentation, check out the [wiki](https://github.com/nitvic793/rotvic-engine/wiki).

## Steps to build

This project requires the Windows 10 SDK to be installed. 

- Open project in Visual Studio 2017
- Set project to build in x64 platform.
- Set "Game" as startup project.
- Build and run.

## Known issues

- There are a few known and ticketed issues with the physics middleware that we've done our best to avoid encountering. If there's an assert failure, it's the middleware.  

## Solution Structure

- Engine - Contains the game engine framework. Consists of a rendering system with DirectX underneath and other interfaces which can be extended for a game. 
- Game.Core - Implements the engine interfaces and the game logic. 
- Game - The main application project. Initializes the game.
