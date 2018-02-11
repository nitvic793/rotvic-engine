# Engine

## Steps to Save/Load state

Once the game is started and running, the game state can be saved or loaded. 

- To save the state of the game, press **F5** while the game is running. This will save the current position of the object on the screen.
- To load a previously saved state, press **F6**. If no previously saved state file is found, it will pop up a warning message.

### Save File Structure

- An object's coordinates are stored in one line.
- Should start with name of entity(with no spaces) and followed by X, Y and Z coordinates(float values) space separated. 

An example save file would look like

```
MainEntity 1.0 2.0 0.0
```

### Assumptions

- The game state is always saved in the file "save.dat". 
- Hence, every time an attempt to load the save file is made, it will look for "save.dat".
- Default coordinate values are loaded if there are any parsing errors with the save file. 
