#pragma once
#include "Core.h"

/// <summary>
/// Serializable Save state class.
/// </summary>
class SaveState : public ISerializable
{
public:
	Vector3f Position;
	std::string EntityName;

	SaveState();
	~SaveState();

	// Inherited via ISerializable
	virtual void Serialize(std::ostream & stream) override;
	virtual void Deserialize(std::istream & stream) override;
};

