/// <summary>
/// Author: Nitish Victor
/// </summary>

#pragma once
#include <DirectXMath.h>

/// <summary>
/// UI Component type
/// </summary>
enum ComponentType
{
	UI_TEXT,
	UI_IMAGE
};

/// <summary>
/// Base UI Component class.
/// </summary>
/// <remarks>
/// All UI components such Text, Images, Buttons etc. should inherit from this class.
/// </remarks>
class UIComponent
{
	DirectX::XMFLOAT3 mPosition;
public:
	/// <summary>
	/// Set position of component on screen.
	/// </summary>
	/// <param name="position">Position Vector</param>
	/// <remarks>
	/// The UI system uses a coordinate system where the top left corner of the screen starts with (0,0) 
	/// and bottom right of the screen ends at (ScreenWidth, ScreenHeight)
	/// </remarks>
	void SetPosition(DirectX::XMFLOAT3 position);

	/// <summary>
	/// Get position of component on screen.
	/// </summary>
	/// <returns>Position vector</returns>
	/// <remarks>
	/// The UI system uses a coordinate system where the top left corner of the screen starts with (0,0) 
	/// and bottom right of the screen ends at (ScreenWidth, ScreenHeight)
	/// </remarks>
	DirectX::XMFLOAT3 GetPosition();

	UIComponent();
	virtual ComponentType GetType() const = 0;
	virtual ~UIComponent();
};

