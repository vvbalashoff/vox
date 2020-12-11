// ******************************************************************************
// Filename:    ProjectileManager.cpp
// Project:     Vox
// Author:      Steven Ball
//
// Purpose:
//
// Revision History:
//   Initial Revision - 15/02/15
//
// Copyright (c) 2005-2016, Steven Ball
// ******************************************************************************

#include "ProjectileManager.h"

#include <algorithm>

#include "../Lighting/LightingManager.h"
#include "../VoxGame.h"


ProjectileManager::ProjectileManager(Renderer* pRenderer, ChunkManager* pChunkManager)
{
	m_pRenderer = pRenderer;
	m_pChunkManager = pChunkManager;

	m_numRenderProjectiles = 0;
}

ProjectileManager::~ProjectileManager()
{
	ClearProjectiles();
}

void ProjectileManager::SetLightingManager(LightingManager* pLightingManager)
{
	m_pLightingManager = pLightingManager;
}

void ProjectileManager::SetBlockParticleManager(BlockParticleManager* pBlockParticleManager)
{
	m_pBlockParticleManager = pBlockParticleManager;
}

void ProjectileManager::SetPlayer(Player* pPlayer)
{
	m_pPlayer = pPlayer;
}

void ProjectileManager::SetQubicleBinaryManager(QubicleBinaryManager* pQubicleBinaryManager)
{
	m_pQubicleBinaryManager = pQubicleBinaryManager;
}

// Clearing
void ProjectileManager::ClearProjectiles()
{
	// Projectile list
	m_projectileMutex.lock();
	for(unsigned int i = 0; i < m_vpProjectileList.size(); i++)
	{
		delete m_vpProjectileList[i];
		m_vpProjectileList[i] = 0;
	}
	m_vpProjectileList.clear();
	m_projectileMutex.unlock();

	// Create list
	m_projectileCreateMutex.lock();
	for(unsigned int i = 0; i < m_vpProjectileCreateList.size(); i++)
	{
		delete m_vpProjectileCreateList[i];
		m_vpProjectileCreateList[i] = 0;
	}
	m_vpProjectileCreateList.clear();
	m_projectileCreateMutex.unlock();
}

void ProjectileManager::ClearProjectileOwnerForEnemy(Enemy* pEnemy)
{
	m_projectileMutex.lock();
	for(unsigned int i = 0; i < m_vpProjectileList.size(); i++)
	{
		if(m_vpProjectileList[i]->GetEnemyOwner() == pEnemy)
		{
			m_vpProjectileList[i]->SetOwner(nullptr, nullptr, nullptr);
		}
	}
	m_projectileMutex.unlock();
}

void ProjectileManager::ClearProjectileOwnerForNPC(NPC* pNPC)
{
	m_projectileMutex.lock();
	for(unsigned int i = 0; i < m_vpProjectileList.size(); i++)
	{
		if(m_vpProjectileList[i]->GetNPCOwner() == pNPC)
		{
			m_vpProjectileList[i]->SetOwner(nullptr, nullptr, nullptr);
		}
	}
	m_projectileMutex.unlock();
}

// Creating
Projectile* ProjectileManager::CreateProjectile(vec3 position, vec3 velocity, float rotation, const char* objectFilename, float scale)
{
	Projectile* pNewProjectile = new Projectile(m_pRenderer, m_pChunkManager, m_pQubicleBinaryManager, objectFilename, scale);

	pNewProjectile->SetLightingManager(m_pLightingManager);
	pNewProjectile->SetGameWindow(m_pGameWindow);
	pNewProjectile->SetBlockParticleManager(m_pBlockParticleManager);
	pNewProjectile->SetPlayer(m_pPlayer);

	pNewProjectile->SetPosition(position);
	pNewProjectile->SetVelocity(velocity);
	pNewProjectile->SetRotation(rotation);

	m_projectileCreateMutex.lock();
	m_vpProjectileCreateList.push_back(pNewProjectile);
	m_projectileCreateMutex.unlock();

	return pNewProjectile;
}

// Get projectile
Projectile* ProjectileManager::GetProjectile(int index)
{
	m_projectileMutex.lock();
	Projectile* pProjectile = m_vpProjectileList[index];
	m_projectileMutex.unlock();

	return pProjectile;
}

// Get number of projectiles
int ProjectileManager::GetNumProjectiles()
{
	m_projectileMutex.lock();
	int numProjectiles =  (int)m_vpProjectileList.size();
	m_projectileMutex.unlock();

	return numProjectiles;
}

int ProjectileManager::GetNumRenderProjectiles()
{
	return m_numRenderProjectiles;
}

bool projectile_needs_erasing(Projectile* aProjectile)
{
	bool needsErase = aProjectile->GetErase();

	if(needsErase == true)
	{
		delete aProjectile;
	}

	return needsErase;
}

// Rendering helpers
void ProjectileManager::CalculateWorldTransformMatrix()
{
	m_projectileMutex.lock();
	for (unsigned int i = 0; i < m_vpProjectileList.size(); i++)
	{
		Projectile* pProjectile = m_vpProjectileList[i];

		pProjectile->CalculateWorldTransformMatrix();
	}
	m_projectileMutex.unlock();
}

// Updating
void ProjectileManager::Update(float dt)
{
	// Add any projectiles on the create list to the main list and then clear the create list
	m_projectileCreateMutex.lock();
	for(unsigned int i = 0; i < m_vpProjectileCreateList.size(); i++)
	{
		Projectile* pProjectile = m_vpProjectileCreateList[i];

		m_projectileMutex.lock();
		m_vpProjectileList.push_back(pProjectile);
		m_projectileMutex.unlock();
	}
	m_vpProjectileCreateList.clear();
	m_projectileCreateMutex.unlock();

	// Remove any projectiles that need to be erased
	m_projectileMutex.lock();
	m_vpProjectileList.erase( remove_if(m_vpProjectileList.begin(), m_vpProjectileList.end(), projectile_needs_erasing), m_vpProjectileList.end() );
	m_projectileMutex.unlock();

	// Update projectiles
	m_projectileMutex.lock();
	for(unsigned int i = 0; i < m_vpProjectileList.size(); i++)
	{
		Projectile* pProjectile = m_vpProjectileList[i];

		pProjectile->Update(dt);
	}
	m_projectileMutex.unlock();
}

void ProjectileManager::UpdateProjectileLights(float dt)
{
	m_projectileMutex.lock();
	for(unsigned int i = 0; i < m_vpProjectileList.size(); i++)
	{
		Projectile* pProjectile = m_vpProjectileList[i];

		pProjectile->UpdateProjectileLights(dt);
	}
	m_projectileMutex.unlock();
}

void ProjectileManager::UpdateProjectileParticleEffects(float dt)
{
	m_projectileMutex.lock();
	for(unsigned int i = 0; i < m_vpProjectileList.size(); i++)
	{
		Projectile* pProjectile = m_vpProjectileList[i];

		pProjectile->UpdateProjectileParticleEffects(dt);
	}
	m_projectileMutex.unlock();
}

// Rendering
void ProjectileManager::Render()
{
	m_numRenderProjectiles = 0;

	m_projectileMutex.lock();
	for(unsigned int i = 0; i < m_vpProjectileList.size(); i++)
	{
		Projectile* pProjectile = m_vpProjectileList[i];

		// TODO : Add me back in - render culling
		//if(m_pChunkManager->IsInsideLoader(pProjectile->GetCenter()) == false)
		//{
		//	continue;
		//}

		// NOTE : We should always render the projectiles, even when outisde of frustum, else the explosion particles dont appear in correct place.
		// This is due to the model matrix only being updated properly in the render call, for the qubicle binary, that the explode function uses.
		//if(m_pRenderer->SphereInFrustum(VoxGame::GetInstance()->GetDefaultViewport(), pProjectile->GetCenter(), pProjectile->GetRadius()))
		{
			pProjectile->Render();

			m_numRenderProjectiles++;
		}
	}
	m_projectileMutex.unlock();
}

void ProjectileManager::RenderWeaponTrails()
{
	m_projectileMutex.lock();
	for(unsigned int i = 0; i < m_vpProjectileList.size(); i++)
	{
		Projectile* pProjectile = m_vpProjectileList[i];

		// TODO : Add me back in - render culling
		//if(m_pChunkManager->IsInsideLoader(pProjectile->GetCenter()) == false)
		//{
		//	continue;
		//}

		if(m_pRenderer->SphereInFrustum(VoxGame::GetInstance()->GetDefaultViewport(), pProjectile->GetCenter(), pProjectile->GetRadius()))
		{
			pProjectile->RenderWeaponTrails();
		}
	}
	m_projectileMutex.unlock();
}

void ProjectileManager::RenderDebug()
{
	m_projectileMutex.lock();
	for(unsigned int i = 0; i < m_vpProjectileList.size(); i++)
	{
		Projectile* pProjectile = m_vpProjectileList[i];

		// TODO : Add me back in - render culling
		//if(m_pChunkManager->IsInsideLoader(pProjectile->GetCenter()) == false)
		//{
		//	continue;
		//}

		if(m_pRenderer->SphereInFrustum(VoxGame::GetInstance()->GetDefaultViewport(), pProjectile->GetCenter(), pProjectile->GetRadius()))
		{
			pProjectile->RenderDebug();
		}
	}
	m_projectileMutex.unlock();
}
