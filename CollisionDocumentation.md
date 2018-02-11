# Engine

## Collision Player Entity - Custom Implementation

Once the game is started and running, the player "object" can be moved around using the arrow keys or by clicking anywhere on screen.

- If the player object encounters a static object on screen, it should stop. 
- Player object and other static objects will not overlap. 

## Collision2D System

### ICollision2D Interface 

Any 2D entity which requires collision detection should inherit the abstract class ICollision2D and implement the OnCollision(GameEntity*) pure virtual function. This function will be invoked whenever a collision is detected with the entity with the colliding entity as a parameter. 

### QuadTree

If there are a lot of objects requiring collision detection, a quad tree may be initialized to optimize the broadphase collision detection. The quad tree must be initialized using the Collision2D::InitializeQuadTree() method. Once the quad tree is initialized, the static objects in the level should be inserted into the tree. Any object which actively moves around should not be inserted into the level. Dynamic objects are NOT supported yet. 

### Collision2D::IsColliding Method

The IsColliding method takes two Rect objects as parameters and returns true if the two objects are colliding. This uses AABB collision detection. Can be used at the user's discretion. 
