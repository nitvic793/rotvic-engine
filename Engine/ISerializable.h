#pragma once

#include <ostream>

/// <summary>
/// ISerializable interface class represents a class which can be serialized. 
/// </summary>
class ISerializable
{
public:
	ISerializable() {}
	virtual ~ISerializable() {}

	/// <summary>
	/// Serializes the class into given output stream.
	/// </summary>
	/// <param name="stream"></param>
	virtual void Serialize(std::ostream& stream) = 0;

	/// <summary>
	/// Deserializes the input stream and reconstructs the object. 
	/// </summary>
	/// <param name="stream"></param>
	virtual void Deserialize(std::istream& stream) = 0;
};