// ******************************************************************************
// Filename:    Projectile.h
// Project:     Vox
// Author:      Steven Ball
//
// Purpose:
//
// Revision History:
//   Initial Revision - 15/02/16
//
// Copyright (c) 2005-2011, Steven Ball
// ******************************************************************************

#pragma once

#include "../models/VoxelWeapon.h"
#include "../blocks/ChunkManager.h"
#include "../Particles/BlockParticleManager.h"

class LightingManager;
class GameWindow;
class NPC;
class Enemy;
class Player;


enum eProjectileHitboxType
{
	eProjectileHitboxType_Sphere = 0,
	eProjectileHitboxType_Cube,
};

class Projectile
{
public:
	/* Public methods */
	Projectile(Renderer* pRenderer, ChunkManager* pChunkManager, QubicleBinaryManager* pQubicleBinaryManager, const char* objectFilename, float scale);
	~Projectile();

	void SetLightingManager(LightingManager* pLightingManager);
	void SetGameWindow(GameWindow* pGameWindow);
	void SetBlockParticleManager(BlockParticleManager* pBlockParticleManager);
	void SetPlayer(Player* pPlayer);

	// Unloading
	void UnloadEffectsAndLights() const;

	// Erase flag
	bool GetErase() const;
	void SetErase(bool erase);

	// Setup
	void LoadItem(const char* objectFilename);

	// Accessors / Setters
	void SetPosition(const vec3& pos);
	const vec3& GetPosition() const;
	void SetVelocity(const vec3& vel);
	const vec3& GetVelocity() const;
	void SetRotation(float rot);
	float GetRotation() const;
	float GetScale() const;
	float GetRadius() const;
	void UpdateRadius();
	vec3 GetCenter() const;
	vec3 GetForwardVector() const;
	vec3 GetRightVector() const;
	vec3 GetUpVector() const;
	void SetGravityDirection(const vec3& dir);
	void SetGravityMultiplier(float multiplier);
	void SetProjectileCurveParams(const vec3& forward, const vec3& target, float curveTime);
	void SetWorldCollisionEnabled(bool enabled);

	// Projectile type
	void SetProjectileType(bool attackEnemies, bool attackPlayer, bool attackNPCs);
	bool CanAttackEnemies() const;
	bool CanAttackPlayer() const;
	bool CanAttackNPCs() const;
	void SetReturnToPlayer(bool returnToPlayer);
	bool IsReturnToPlayer() const;
	void SetExplodingProjectile(bool exploding, float radius);

	void SetOwner(Player* pPlayer, NPC* pNPC, Enemy* pEnemy);
	Player* GetPlayerOwner() const;
	NPC* GetNPCOwner() const;
	Enemy* GetEnemyOwner() const;

	// Catching
	bool CanCatch() const;
	void PlayerCatch();

	// Explode
	void Explode();

	// Grid
	void UpdateGridPosition();
	Chunk* GetCachedGridChunkOrFromPosition(const vec3& pos) const;

	// Rendering Helpers
	void CalculateWorldTransformMatrix();

	// Updating
	void Update(float dt);
	void UpdateProjectileLights(float dt);
	void UpdateProjectileParticleEffects(float dt);

	// Rendering
	void Render();
	void RenderWeaponTrails();
	void RenderDebug();
	void RenderDebugReturnCurve();
	void RenderForwardDebug();

protected:
	/* Protected methods */
	static void _RightCurveTimerFinished(void *apData);
	void RightCurveTimerFinished();

private:
	/* Private methods */

public:
	/* Public members */

protected:
	/* Protected members */

private:
	/* Private members */
	Renderer* m_pRenderer;
	ChunkManager* m_pChunkManager;
	LightingManager* m_pLightingManager;
	GameWindow* m_pGameWindow;
	BlockParticleManager* m_pBlockParticleManager;
	Player* m_pPlayer;
	QubicleBinaryManager* m_pQubicleBinaryManager;

	// Erase flag
	bool m_erase;

	// Projectile radius
	float m_radius;

	// 	Scale
	float m_renderScale;

	// Projectile position and movement variables
	vec3 m_position;
	vec3 m_previousPosition;
	vec3 m_velocity;
	vec3 m_gravityDirection;
	float m_gravityMultiplier;

	// Forward vector
	vec3 m_forward;

	// World matrix
	Matrix4x4 m_worldMatrix;

	// Grid position
	int m_gridPositionX;
	int m_gridPositionY;
	int m_gridPositionZ;

	// Cached grid chunk
	Chunk* m_pCachedGridChunk;

	// What type of projectile are we
	bool m_attackEnemies;
	bool m_attackPlayer;
	bool m_attackNPCs;

	// World collision flag
	bool m_worldCollisionEnabled;

	// Explosion
	bool m_explodeWorld;
	float m_explosionRadius;

	// Return to player flag
	bool m_returnToPlayer;
	float m_catchTimer;
	float m_curveTimer;
	float m_curveTime;
	bool m_rightCurve;
	bool m_returningDirectToPlayer;

	// Return to player curve
	vec3 m_bezierStart_Right;
	vec3 m_bezierEnd_Right;
	vec3 m_bezierControl_Right;
	vec3 m_bezierStart_Left;
	vec3 m_bezierEnd_Left;
	vec3 m_bezierControl_Left;

	// Projectile owner
	Player* m_pOwnedPlayer;
	NPC* m_pOwnedNPC;
	Enemy* m_pOwnedEnemy;

	// Voxel projectile
	VoxelWeapon* m_pVoxeProjectile;
};
