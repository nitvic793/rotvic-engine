# DirectX 11 Game Engine

[![Build status](https://ci.appveyor.com/api/projects/status/l56arbrrf5mpbp5p?svg=true)](https://ci.appveyor.com/project/nitvic793/rotvic-engine)

## Solution Structure

1. Engine - Contains the game engine framework. Consists of a rendering system with DirectX underneath and other interfaces which can be extended for a game. 
2. Game.Core - Implements the engine interfaces and the game logic. 
3. Game - The main application project. Initializes the game.
4. Engine.Tests - Contains all engine related tests.
4. Game.Core.Tests - Contains all game logic related tests.