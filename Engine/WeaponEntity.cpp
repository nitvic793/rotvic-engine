/// <summary>
/// Author: Rahul SV
/// </summary>



#include "WeaponEntity.h"


WeaponEntity::WeaponEntity(Mesh* m, Material* mat, rp3d::Vector3 position, rp3d::DynamicsWorld* world)
	:Entity(m, mat, position, world)
{
	meshes.push_back(m);
	materials.push_back(mat);
}

/* attaches the weapon entity to the bone of a player entity */
void WeaponEntity::AttachToEntity(ThirdPersonPlayerEntity* pEntity, int bIndex)
{
	playerEntity = pEntity;
	boneIndex = bIndex;
}

/* add weapon to list of weapons (mesh+material) */
void WeaponEntity::AddWeapon(Mesh * mesh, Material * material)
{
	meshes.push_back(mesh);
	materials.push_back(material);
	numWeapons++;
}

/* iterate through weapons*/
void WeaponEntity::SwitchWeapon()
{
	++weaponIndex;
	if (weaponIndex == numWeapons)
	{
		weaponIndex = 0;
	}

	this->mesh = meshes[weaponIndex];
	this->material = materials[weaponIndex];
}
