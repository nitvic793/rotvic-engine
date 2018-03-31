# Debug Draw

The engine has a basic debug draw system which can be invoked anywhere in the system. The draw functions usually just queue up draw actions, and only enter the render system after all the game entities are rendered. 

## Instantiation

The debug draw is a singleton instance which is initialized by CoreGame. It uses different "Debug" shaders to draw the debug draw shapes. Although the Debug Draw system is a singleton, the instance itself need not be directly required to draw any debug draw shapes. Instead, `DebugDraw::Draw()` is a public static method which can be invoked anywhere it is required. 

## Implementation

The Debug Draw system uses separate shaders and vertex data to represent the shapes. Each draw call queues up a draw for the given shape. When Render() is called on DebugDraw, it uses DirectX's `PrimitiveBatch` to draw all the queued up shapes before resetting the render pipeline. 

Every draw call can also have a "group" associated with it. If no group is specified, the group will be "default". The given groups can be enabled or disabled according to requirements. 

## Drawing a Shape

To draw a shape, first include `DebugDraw.h` in the source file. For example, the update function of an entity or of an IGame instance is as good a place as any. The following sample shows how to draw a box.

``` C++
void Game::Update(float deltaTime)
{
	Box box;
	box.color = XMFLOAT4(1, 1, 1, 1); //RGBA Values
	box.bounding.Center = XMFLOAT3(2, 1, 0);
	box.bounding.Extents = XMFLOAT3(1, 1, 1);
    
    DebugDraw::Draw<Box>(box);
}
```

If you want to specify a group for the shape, just pass it on as the next parameter.

```
DebugDraw::Draw<Box>(box, "Collision");
```

## Toggling Groups

The debug draw instance can be retrieved by calling `DebugDraw::GetInstance()`. The debug draw groups can be enabled or disabled by calling the function like so

```
debugDraw->SetGroupActive("Collision", active); // Where active is a boolean
```
This ensures that even subsequent debug draw calls with the group "Collision" will not be queued even if Draw() is called with given group name. i.e. if "Collision" group is disabled, a call such as `DebugDraw::Draw<Box>(box, "Collision")` will not queue a draw call. 

## Available Shapes

### Ray

```
	Ray ray;
	ray.color = XMFLOAT4(1, 1, 1, 1);
	ray.origin = XMFLOAT3(0, 0, 0);
	ray.direction = XMFLOAT3(0, 1, 0);
	ray.length = 2.f;
```
### Sphere
```
	Sphere sphere;
	sphere.color = XMFLOAT4(1, 1, 1, 1);
	sphere.bounding.Center = XMFLOAT3(-1, 0, 0);
	sphere.bounding.Radius = 0.5;
```
### Box
```
	Box box;
	box.color = XMFLOAT4(1, 1, 1, 1);
	box.bounding.Center = XMFLOAT3(2, 1, 0);
	box.bounding.Extents = XMFLOAT3(1, 1, 1);
```
### Frustum
```
	Frustum fr;
	fr.color = XMFLOAT4(1, 1, 1, 1);
	fr.bounding.Origin = XMFLOAT3(-4, 0, 1);
```
### Grid
```
	Grid grid = Grid::GetDefaultGrid();
```