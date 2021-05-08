/***
*
*	Copyright (c) 1996-2001, Valve LLC. All rights reserved.
*	
*	This product contains software technology licensed from Id 
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*	All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/
#ifndef WEAPONS_H
#define WEAPONS_H

#include "effects.h"

class CBasePlayer;

// Contact Grenade / Timed grenade / Satchel Charge
class CGrenade : public CBaseMonster
{
public:
	void Spawn() override;

	typedef enum { SATCHEL_DETONATE = 0, SATCHEL_RELEASE } SATCHELCODE;

	static CGrenade *ShootTimed( entvars_t *pevOwner, Vector vecStart, Vector vecVelocity, float time );
	static CGrenade *ShootContact( entvars_t *pevOwner, Vector vecStart, Vector vecVelocity );
	static CGrenade *ShootSatchelCharge( entvars_t *pevOwner, Vector vecStart, Vector vecVelocity );
	static void UseSatchelCharges( entvars_t *pevOwner, SATCHELCODE code );

	void Explode( Vector vecSrc, Vector vecAim );
	void Explode( TraceResult *pTrace, int bitsDamageType );
	void EXPORT Smoke();

	void EXPORT BounceTouch( CBaseEntity *pOther );
	void EXPORT SlideTouch( CBaseEntity *pOther );
	void EXPORT ExplodeTouch( CBaseEntity *pOther );
	void EXPORT DangerSoundThink();
	void EXPORT PreDetonate();
	void EXPORT Detonate();
	void EXPORT DetonateUse( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value );
	void EXPORT TumbleThink();

	virtual void BounceSound();
    int	BloodColor() override { return DONT_BLEED; }
	void Killed( entvars_t *pevAttacker, int iGib ) override;

	BOOL m_fRegisteredSound;// whether or not this grenade has issued its DANGER sound to the world sound list yet.
};


// constant items
#define ITEM_HEALTHKIT	1
#define ITEM_BATTERY	2

#define WEAPON_NONE		0
#define WEAPON_CROWBAR	1

#define MAX_WEAPONS	32


#define MAX_NORMAL_BATTERY	100


// weapon weight factors (for auto-switching)   (-1 = noswitch)
#define NOSWITCH_WEIGHT	-1
#define CROWBAR_WEIGHT	0

// the maximum amount of ammo each weapon's clip can hold
#define WEAPON_NOCLIP	-1

// bullet types
typedef	enum
{
	BULLET_NONE = 0,
	BULLET_PLAYER_CROWBAR, // crowbar swipe

	BULLET_TANK_9MM,
	BULLET_TANK_MP5,
	BULLET_TANK_12MM,
} Bullet;


#define ITEM_FLAG_SELECTONEMPTY		1
#define ITEM_FLAG_NOAUTORELOAD		2
#define ITEM_FLAG_NOAUTOSWITCHEMPTY	4
#define ITEM_FLAG_LIMITINWORLD		8
#define ITEM_FLAG_EXHAUSTIBLE		16 // A player can totally exhaust their ammo supply and lose this weapon

#define WEAPON_IS_ONTARGET 0x40

typedef struct
{
	int		iSlot;
	int		iPosition;
	const char	*pszAmmo1;	// ammo 1 type
	int		iMaxAmmo1;		// max ammo 1
	const char	*pszAmmo2;	// ammo 2 type
	int		iMaxAmmo2;		// max ammo 2
	const char	*pszName;
	int		iMaxClip;
	int		iId;
	int		iFlags;
	int		iWeight;// this value used to determine this weapon's importance in autoselection.
} ItemInfo;

typedef struct
{
	const char *pszName;
	int iId;
} AmmoInfo;

extern int giAmmoIndex;

void AddAmmoNameToAmmoRegistry(const char* szAmmoname);

// Items that the player has in their inventory that they can use
class CBasePlayerItem : public CBaseAnimating
{
public:
    void SetObjectCollisionBox() override;

    int		Save( CSave &save ) override;
    int		Restore( CRestore &restore ) override;
	
	static	TYPEDESCRIPTION m_SaveData[];

	virtual int AddToPlayer( CBasePlayer *pPlayer );	// return TRUE if the item you want the item added to the player inventory
	virtual int AddDuplicate( CBasePlayerItem *pItem ) { return FALSE; }	// return TRUE if you want your duplicate removed from world
	void EXPORT DestroyItem();
	void EXPORT DefaultTouch( CBaseEntity *pOther );	// default weapon touch
	void EXPORT FallThink ();// when an item is first spawned, this think is run to determine when the object has hit the ground.
	void EXPORT Materialize();// make a weapon visible and tangible
	void EXPORT AttemptToMaterialize();  // the weapon desires to become visible and tangible, if the game rules allow for it
	CBaseEntity* Respawn () override;// copy a weapon
	void FallInit();
	void CheckRespawn();
	virtual int GetItemInfo(ItemInfo *p) { return 0; }	// returns 0 if struct not filled out
	virtual BOOL CanDeploy() { return TRUE; }
	virtual BOOL Deploy( )								// returns is deploy was successful
		 { return TRUE; }

	virtual BOOL CanHolster() { return TRUE; }// can this weapon be put away right now?
	virtual void Holster();
	virtual void UpdateItemInfo() {}

	virtual void ItemPreFrame()	{}		// called each frame by the player PreThink
	virtual void ItemPostFrame() {}		// called each frame by the player PostThink

	virtual void Drop();
	virtual void Kill();
	virtual void AttachToPlayer ( CBasePlayer *pPlayer );

	virtual int PrimaryAmmoIndex() { return -1; }
	virtual int SecondaryAmmoIndex() { return -1; }

	virtual int UpdateClientData( CBasePlayer *pPlayer ) { return 0; }

	virtual CBasePlayerItem *GetWeaponPtr() { return NULL; }

	static ItemInfo ItemInfoArray[ MAX_WEAPONS ];
	static AmmoInfo AmmoInfoArray[ MAX_AMMO_SLOTS ];

	CBasePlayer	*m_pPlayer;
	CBasePlayerItem *m_pNext;
	int		m_iId;												// WEAPON_???

	virtual int iItemSlot() { return 0; }			// return 0 to MAX_ITEMS_SLOTS, used in hud

	int			iItemPosition() { return ItemInfoArray[ m_iId ].iPosition; }
	const char	*pszAmmo1()	{ return ItemInfoArray[ m_iId ].pszAmmo1; }
	int			iMaxAmmo1()	{ return ItemInfoArray[ m_iId ].iMaxAmmo1; }
	const char	*pszAmmo2()	{ return ItemInfoArray[ m_iId ].pszAmmo2; }
	int			iMaxAmmo2()	{ return ItemInfoArray[ m_iId ].iMaxAmmo2; }
	const char	*pszName()	{ return ItemInfoArray[ m_iId ].pszName; }
	int			iMaxClip()	{ return ItemInfoArray[ m_iId ].iMaxClip; }
	int			iWeight()		{ return ItemInfoArray[ m_iId ].iWeight; }
	int			iFlags()		{ return ItemInfoArray[ m_iId ].iFlags; }

	// int		m_iIdPrimary;										// Unique Id for primary ammo
	// int		m_iIdSecondary;										// Unique Id for secondary ammo
};


// inventory items that 
class CBasePlayerWeapon : public CBasePlayerItem
{
public:
    int		Save( CSave &save ) override;
    int		Restore( CRestore &restore ) override;
	
	static	TYPEDESCRIPTION m_SaveData[];

	// generic weapon versions of CBasePlayerItem calls
	int AddToPlayer( CBasePlayer *pPlayer ) override;
	int AddDuplicate( CBasePlayerItem *pItem ) override;

	virtual int ExtractAmmo( CBasePlayerWeapon *pWeapon ); //{ return TRUE; }			// Return TRUE if you can add ammo to yourself when picked up
	virtual int ExtractClipAmmo( CBasePlayerWeapon *pWeapon );// { return TRUE; }			// Return TRUE if you can add ammo to yourself when picked up

	virtual int AddWeapon() { ExtractAmmo( this ); return TRUE; }	// Return TRUE if you want to add yourself to the player

	// generic "shared" ammo handlers
	BOOL AddPrimaryAmmo( int iCount, char *szName, int iMaxClip, int iMaxCarry );
	BOOL AddSecondaryAmmo( int iCount, char *szName, int iMaxCarry );

	void UpdateItemInfo() override {}	// updates HUD state

	int m_iPlayEmptySound;
	int m_fFireOnEmpty;		// True when the gun is empty and the player is still holding down the
							// attack key(s)
	virtual BOOL PlayEmptySound();
	virtual void ResetEmptySound();

	virtual void SendWeaponAnim( int iAnim, int skiplocal = 1, int body = 0 );  // skiplocal is 1 if client is predicting weapon animations

    BOOL CanDeploy() override;
	virtual BOOL IsUseable();
	BOOL DefaultDeploy(const char *szViewModel, const char *szWeaponModel, int iAnim, const char *szAnimExt, int skiplocal = 0, int body = 0 );
	int DefaultReload( int iClipSize, int iAnim, float fDelay, int body = 0 );

	void ItemPostFrame() override;	// called each frame by the player PostThink
	// called by CBasePlayerWeapons ItemPostFrame()
	virtual void PrimaryAttack() {}				// do "+ATTACK"
	virtual void SecondaryAttack() {}			// do "+ATTACK2"
	virtual void Reload() {}						// do "+RELOAD"
	virtual void WeaponIdle() {}					// called when no buttons pressed
	int UpdateClientData( CBasePlayer *pPlayer ) override;		// sends hud info to client dll, if things have changed
	virtual void RetireWeapon();
	virtual BOOL ShouldWeaponIdle() {return FALSE; }
	void Holster() override;
	virtual BOOL UseDecrement() { return FALSE; }
	
	int	PrimaryAmmoIndex() override;
	int	SecondaryAmmoIndex() override;

	void PrintState();

	CBasePlayerItem *GetWeaponPtr() override { return (CBasePlayerItem *)this; }
	float GetNextAttackDelay( float delay );

	float m_flPumpTime;
	int		m_fInSpecialReload;									// Are we in the middle of a reload for the shotguns
	float	m_flNextPrimaryAttack;								// soonest time ItemPostFrame will call PrimaryAttack
	float	m_flNextSecondaryAttack;							// soonest time ItemPostFrame will call SecondaryAttack
	float	m_flTimeWeaponIdle;									// soonest time ItemPostFrame will call WeaponIdle
	int		m_iPrimaryAmmoType;									// "primary" ammo index into players m_rgAmmo[]
	int		m_iSecondaryAmmoType;								// "secondary" ammo index into players m_rgAmmo[]
	int		m_iClip;											// number of shots left in the primary weapon clip, -1 it not used
	int		m_iClientClip;										// the last version of m_iClip sent to hud dll
	int		m_iClientWeaponState;								// the last version of the weapon state sent to hud dll (is current weapon, is on target)
	int		m_fInReload;										// Are we in the middle of a reload;

	int		m_iDefaultAmmo;// how much ammo you get when you pick up this weapon as placed by a level designer.
	
	// hle time creep vars
	float	m_flPrevPrimaryAttack;
	float	m_flLastFireTime;			

};


class CBasePlayerAmmo : public CBaseEntity
{
public:
    void Spawn() override;
	void EXPORT DefaultTouch( CBaseEntity *pOther ); // default weapon touch
	virtual BOOL AddAmmo( CBaseEntity *pOther ) { return TRUE; }

	CBaseEntity* Respawn() override;
	void EXPORT Materialize();
};


extern DLL_GLOBAL	short	g_sModelIndexLaser;// holds the index for the laser beam
extern DLL_GLOBAL	const char *g_pModelNameLaser;

extern DLL_GLOBAL	short	g_sModelIndexLaserDot;// holds the index for the laser beam dot
extern DLL_GLOBAL	short	g_sModelIndexFireball;// holds the index for the fireball
extern DLL_GLOBAL	short	g_sModelIndexSmoke;// holds the index for the smoke cloud
extern DLL_GLOBAL	short	g_sModelIndexWExplosion;// holds the index for the underwater explosion
extern DLL_GLOBAL	short	g_sModelIndexBubbles;// holds the index for the bubbles model
extern DLL_GLOBAL	short	g_sModelIndexBloodDrop;// holds the sprite index for blood drops
extern DLL_GLOBAL	short	g_sModelIndexBloodSpray;// holds the sprite index for blood spray (bigger)

extern void ClearMultiDamage();
extern void ApplyMultiDamage(entvars_t* pevInflictor, entvars_t* pevAttacker );
extern void AddMultiDamage( entvars_t *pevInflictor, CBaseEntity *pEntity, float flDamage, int bitsDamageType);

extern void DecalGunshot( TraceResult *pTrace, int iBulletType );
extern void SpawnBlood(Vector vecSpot, int bloodColor, float flDamage);
extern int DamageDecal( CBaseEntity *pEntity, int bitsDamageType );
extern void RadiusDamage( Vector vecSrc, entvars_t *pevInflictor, entvars_t *pevAttacker, float flDamage, float flRadius, int iClassIgnore, int bitsDamageType );

typedef struct 
{
	CBaseEntity		*pEntity;
	float			amount;
	int				type;
} MULTIDAMAGE;

extern MULTIDAMAGE gMultiDamage;


#define LOUD_GUN_VOLUME			1000
#define NORMAL_GUN_VOLUME		600
#define QUIET_GUN_VOLUME		200

#define	BRIGHT_GUN_FLASH		512
#define NORMAL_GUN_FLASH		256
#define	DIM_GUN_FLASH			128

#define BIG_EXPLOSION_VOLUME	2048
#define NORMAL_EXPLOSION_VOLUME	1024
#define SMALL_EXPLOSION_VOLUME	512

#define	WEAPON_ACTIVITY_VOLUME	64

#define VECTOR_CONE_1DEGREES	Vector( 0.00873, 0.00873, 0.00873 )
#define VECTOR_CONE_2DEGREES	Vector( 0.01745, 0.01745, 0.01745 )
#define VECTOR_CONE_3DEGREES	Vector( 0.02618, 0.02618, 0.02618 )
#define VECTOR_CONE_4DEGREES	Vector( 0.03490, 0.03490, 0.03490 )
#define VECTOR_CONE_5DEGREES	Vector( 0.04362, 0.04362, 0.04362 )
#define VECTOR_CONE_6DEGREES	Vector( 0.05234, 0.05234, 0.05234 )
#define VECTOR_CONE_7DEGREES	Vector( 0.06105, 0.06105, 0.06105 )
#define VECTOR_CONE_8DEGREES	Vector( 0.06976, 0.06976, 0.06976 )
#define VECTOR_CONE_9DEGREES	Vector( 0.07846, 0.07846, 0.07846 )
#define VECTOR_CONE_10DEGREES	Vector( 0.08716, 0.08716, 0.08716 )
#define VECTOR_CONE_15DEGREES	Vector( 0.13053, 0.13053, 0.13053 )
#define VECTOR_CONE_20DEGREES	Vector( 0.17365, 0.17365, 0.17365 )

#ifdef CLIENT_DLL
bool bIsMultiplayer ();
void LoadVModel ( const char *szViewModel, CBasePlayer *m_pPlayer );
#endif

#endif // WEAPONS_H
