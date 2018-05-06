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
	std::vector<Mesh*> meshes;
	std::vector<Material*> materials;
	Entity* playerEntity;
	
	WeaponEntity(Mesh* m, Material* mat, rp3d::Vector3 position, rp3d::DynamicsWorld* world);
	void AttachToEntity(ThirdPersonPlayerEntity* pEntity,int bIndex);
	void AddWeapon(Mesh* mesh, Material * material);
	void SwitchWeapon();
};