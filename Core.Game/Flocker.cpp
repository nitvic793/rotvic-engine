#include "Flocker.h"
// Code in this file inspired by previous personal (Trevor Walden) projects, and those were in turn utilizations of Reynolds' steering algorithms
// All code in this file written by Trevor Walden


Flocker::Flocker()
{
}


Flocker::~Flocker()
{
}

void Flocker::Init(Entity* parent, Camera* follow, rp3d::Vector3* centroidFor, rp3d::Vector3* centroidPos, std::map<std::string, Entity*>* entitymap)
{
	gameObject = parent;
	followObject = follow;
	centroidForward = centroidFor;
	centroidPosition = centroidPos;
	entities = entitymap;
	steeringForce = rp3d::Vector3(0,0,0); // Initialize the steering force
	desiredSeparation = gameObject->GetScale().x * 2.0f;  // Separation based on size
	sPartial = rp3d::Vector3(0, 0, 0);
	sTotal = rp3d::Vector3(0, 0, 0);
	arrivalDistSq = std::powf(arrivalDist, 2.0f); // Square once and never again

}

void Flocker::Update(float deltaTime)
{
	steeringForce = rp3d::Vector3(0, 0, 0); // Reset the steering force

	desired.x = followObject->GetPosition().x - gameObject->GetPosition().x; // Use arrival
	desired.y = followObject->GetPosition().y - gameObject->GetPosition().y;
	desired.z = followObject->GetPosition().z - gameObject->GetPosition().z;
	sepDist = desired.lengthSquare(); // Get the comparative distance to the point
	desired.normalize();

	if (sepDist <= arrivalDistSq) // When encroaching on the set area around the point,
	{
		desired = desired * (maxSpeed * (std::sqrtf(sepDist) / arrivalDist)); // Scale the desired vector by the distance mapped to speed (equation = map(sD,0,aD,0,mS))
	}
	else
	{
		desired = desired * maxSpeed; // If not encroaching, scale the vector to maxSpeed
	}
	steer = desired - gameObject->GetLinearVelocity(); // Take the desired velocity we just calculated and subtract the current velocity
	steeringForce = followWeight * steer; // Add this total arrival/follow force to the total steering force

	{
		desired = (*centroidForward) - gameObject->GetPosition(); // Get the vector between there and here
		desired.normalize();
		desired = desired * maxSpeed; // Scale the vector to maxSpeed
		steer = desired - gameObject->GetLinearVelocity(); // Take the desired velocity we just calculated and subtract the current velocity
	}
	steeringForce += alignWeight *  steer; // Alignment, move with the flock (seek the average direction)
	{
		desired = (*centroidPosition) - gameObject->GetPosition(); // Get the vector between there and here
		desired.normalize();
		desired = desired * maxSpeed; // Scale the vector to maxSpeed
		steer = desired - gameObject->GetLinearVelocity(); // Take the desired velocity we just calculated and subtract the current velocity
	}
	steeringForce += cohereWeight * steer; // Cohesion, stay with the flock (seek the average/center position)

	sTotal = rp3d::Vector3(0, 0, 0); // Separation, space out so the flock isn't just a tight bunch
	int count = 0; // Create a counter for averaging the force vectors
	std::ostringstream flockerindex;
	for (int i = 1; i < 6; i++) // Separation code, check against all flockers to see which ones need separaing from
	{
		flockerindex = std::ostringstream();
		flockerindex << "Flocker" << i;
		sepDist = (gameObject->GetPosition() - (*entities)[flockerindex.str()]->GetPosition()).length(); // Get the distance between vehicles
		if ((sepDist > 0) && (sepDist < desiredSeparation)) // If distance between them is more than 0 (same vehicle) and less than the desired distance,
		{
			sPartial = gameObject->GetPosition() - (*entities)[flockerindex.str()]->GetPosition(); // Then get a flee force,
			sPartial.normalize();
			sPartial = sPartial / sepDist; // Scale it,
			sTotal += sPartial; // And add it to the total
			count++; // Indicate that one more vehicle has to be separated from
		}

		if (count > 0) // Only do extra calculations if there are vehicles to avoid
		{
			sTotal = sTotal / count; // Average the force and scale to max speed
			sTotal.normalize();
			sTotal = sTotal * maxSpeed;
			sTotal -= gameObject->GetLinearVelocity();
			steeringForce += separateWeight * sTotal; // Apply to the steering force (I messed this up last time, I was applying directly to the acceleration)
		}
	}
	XMVECTOR s = XMVectorSet(steeringForce.x, steeringForce.y, steeringForce.z, 0);
	XMVector3ClampLength(s, .01, maxForce);
	steeringForce.x = XMVectorGetX(s);
	steeringForce.y = XMVectorGetY(s);
	steeringForce.z = XMVectorGetZ(s);
	//rp3d::Vector3 acceleration = steeringForce / 15; // Apply the force, accounting for mass
	gameObject->ApplyForce(steeringForce); // Apply all forces to the acceleration as 1 force (ultimate force) in ApplyForce()
	//if (gameObject->GetLinearVelocity() != rp3d::Vector3(0,0,0)) // This rotates the vehicle to face its new direction (if the vehicle should do this)
	//{
	//	rp3d::Vector3 vec = gameObject->GetLinearVelocity();
	//	vec.normalize();
	//	XMVECTOR v = XMVectorSet(vec.x, vec.y, vec.z,0);
	//	XMVECTOR o = XMVectorSet(0, 0, 0, 0);
	//	v = XMVectorLerp(o,v, 0.0f);
	//	gameObject->SetRotation(XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v));
	//}

}
