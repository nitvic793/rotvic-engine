
## To run the tests

1. Open the solution using Visual Studio 2017. 
2. Go to Test->Default Processor Architecture and set it to x64 instead of x86. 
3. After building the solution once, open Test->Windows->Test Explorer and run the tests by clicking on "Run All" in the Test Explorer panel. 

Also ensure the other projects are "x64" to avoid any issues.


## Run the game
- Run the project "Game". Use arrow keys to move the triangle. 

## Solution Structure

1. Engine - Contains the game engine framework. Consists of a rendering system with DirectX underneath and other interfaces which can be extended for a game. 
2. Game.Core - Implements the engine interfaces and the game logic. 
3. Game - The main application project. Initializes the game.
4. Engine.Tests - Contains all engine related tests.
4. Game.Core.Tests - Contains all game logic related tests.