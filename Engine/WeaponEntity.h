/// <summary>
/// Author: Rahul SV
/// </summary>


#pragma once
#include "Entity.h"
#include "Mesh.h"
#include "Material.h"
#include "ResourceManager.h"
#include "ThirdPersonPlayerEntity.h"


// Weapon entity class. 


class WeaponEntity :
	public Entity
{

public:
	int boneIndex = 0;
	int weaponIndex = 0;
	int numWeapons = 1;
	std::vector<Mesh*> meshes;			// List of weapon meshes
	std::vector<Material*> materials;	// List of weapon materials
	Entity* playerEntity;  
	
	WeaponEntity(Mesh* m, Material* mat, rp3d::Vector3 position, rp3d::DynamicsWorld* world);
	void AttachToEntity(ThirdPersonPlayerEntity* pEntity,int bIndex);  /* attaches the weapon entity to the bone of a player entity */
	void AddWeapon(Mesh* mesh, Material * material); /* add weapon to list of weapons (mesh+material) */
	void SwitchWeapon(); /* iterate through weapons*/
};