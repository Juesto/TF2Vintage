//====== Copyright � 1996-2003, Valve Corporation, All rights reserved. =======
//
// Purpose: 
//
//=============================================================================

#ifndef C_TF_PLAYER_H
#define C_TF_PLAYER_H
#ifdef _WIN32
#pragma once
#endif

#include "tf_playeranimstate.h"
#include "c_baseplayer.h"
#include "tf_shareddefs.h"
#include "baseparticleentity.h"
#include "tf_player_shared.h"
#include "c_tf_playerclass.h"
#include "tf_item.h"
#include "props_shared.h"
#include "hintsystem.h"
#include "c_playerattachedmodel.h"
#include "iinput.h"
#include "tf_weapon_medigun.h"
#include "ihasattributes.h"
#include "c_tf_spymask.h"

class C_MuzzleFlashModel;
class C_BaseObject;
class C_TFRagdoll;

extern ConVar tf_medigun_autoheal;
extern ConVar cl_autorezoom;
extern ConVar cl_autoreload;
extern ConVar cl_flipviewmodels;

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class C_TFPlayer : public C_BasePlayer, public IHasAttributes
{
public:

	DECLARE_CLASS( C_TFPlayer, C_BasePlayer );
	DECLARE_CLIENTCLASS();
	DECLARE_PREDICTABLE();
	DECLARE_INTERPOLATION();

	C_TFPlayer();
	~C_TFPlayer();

	static C_TFPlayer *GetLocalTFPlayer();

	virtual void UpdateOnRemove( void );

	virtual void FireGameEvent( IGameEvent *event );

	virtual const QAngle &GetRenderAngles();
	virtual void UpdateClientSideAnimation();
	virtual void SetDormant( bool bDormant );
	virtual void OnPreDataChanged( DataUpdateType_t updateType );
	virtual void OnDataChanged( DataUpdateType_t updateType );
	virtual void ProcessMuzzleFlashEvent();
	virtual void ValidateModelIndex( void );
	virtual int	 ModCalculateObjectCost(int iObjectType, bool bMini = false /*, int iNumberOfObjects, int iTeam, bool bLast = false*/);

	virtual Vector GetObserverCamOrigin( void );
	virtual int DrawModel( int flags );
	virtual bool ShouldDraw( void ) OVERRIDE;
	virtual const Vector &GetRenderOrigin( void ) OVERRIDE;

	virtual bool CreateMove( float flInputSampleTime, CUserCmd *pCmd );

	virtual bool IsAllowedToSwitchWeapons( void );

	virtual void ClientThink();

	// Deal with recording
	virtual void GetToolRecordingState( KeyValues *msg );

	C_TFWeaponBase *GetActiveTFWeapon( void ) const;
	bool		 IsActiveTFWeapon( int iWeaponID );

	virtual void Simulate( void );
	virtual void FireEvent( const Vector &origin, const QAngle &angles, int event, const char *options );
	virtual void PlayStepSound( Vector &vecOrigin, surfacedata_t *psurface, float fvol, bool force );

	void LoadInventory( void );
	void EditInventory( int iSlot, int iWeapon );

	void FireBullet( const FireBulletsInfo_t &info, bool bDoEffects, int nDamageType, int nCustomDamageType = TF_DMG_CUSTOM_NONE );

	void ImpactWaterTrace( trace_t &trace, const Vector &vecStart );

	bool CanAttack( void );

	C_TFPlayerClass *GetPlayerClass( void ) { return &m_PlayerClass; }
	C_TFPlayerClass const *GetPlayerClass( void ) const { return &m_PlayerClass; }
	bool IsPlayerClass( int iClass );
	virtual int GetMaxHealth( void ) const;
	virtual int	GetMaxHealthForBuffing( void ) const;

	float GetLastDamageTime( void ) const { return m_flLastDamageTime; }

	virtual int GetRenderTeamNumber( void );

	bool IsWeaponLowered( void );

	void	AvoidPlayers( CUserCmd *pCmd );

	// Get the ID target entity index. The ID target is the player that is behind our crosshairs, used to
	// display the player's name.
	void UpdateIDTarget();
	int GetIDTarget() const;
	void SetForcedIDTarget( int iTarget );

	void SetAnimation( PLAYER_ANIM playerAnim );

	virtual float GetMinFOV() const;

	virtual int GetVisionFilterFlags( bool bWeaponCheck );

	virtual const QAngle &EyeAngles();

	int GetBuildResources( void );

	// MATTTODO: object selection if necessary
	void SetSelectedObject( C_BaseObject *pObject ) {}

	void GetTeamColor( Color &color );

	virtual void ComputeFxBlend( void );

	// Taunts/VCDs
	virtual bool	StartSceneEvent( CSceneEventInfo *info, CChoreoScene *scene, CChoreoEvent *event, CChoreoActor *actor, C_BaseEntity *pTarget );
	virtual void	CalcView( Vector &eyeOrigin, QAngle &eyeAngles, float &zNear, float &zFar, float &fov );
	bool			StartGestureSceneEvent( CSceneEventInfo *info, CChoreoScene *scene, CChoreoEvent *event, CChoreoActor *actor, C_BaseEntity *pTarget );
	void			TurnOnTauntCam( void );
	void			TurnOffTauntCam( void );
	void			TauntCamInterpolation( void );
	bool			InTauntCam( void ) { return m_bWasTaunting; }
	virtual void	ThirdPersonSwitch( bool bThirdperson );

	void			BuildTransformations( CStudioHdr *pStudioHdr, Vector *pos, Quaternion q[], const matrix3x4_t &cameraTransform, int boneMask, CBoneBitList &boneComputed );

	void			CreateBoneAttachmentsFromWearables( C_TFRagdoll *pRagdoll, bool bDisguised );

	// Minimal Viewmodels
	void			CalcMinViewmodelOffset( void );

	virtual void	InitPhonemeMappings();

	virtual void	GetGlowEffectColor( float *r, float *g, float *b );

	// Gibs.
	void InitPlayerGibs( void );
	void CreatePlayerGibs( const Vector &vecOrigin, const Vector &vecVelocity, float flImpactScale, bool bWearables, bool bBurning, bool bHeadGib, bool bDisguised );
	void DropPartyHat( breakablepropparams_t const &breakParams, Vector const &vecBreakVelocity );
	void DropHat( breakablepropparams_t const &breakParams, Vector const &vecBreakVelocity );

	int	GetObjectCount( void );
	C_BaseObject *GetObject( int index );
	C_BaseObject *GetObjectOfType( int iObjectType, int iObjectMode );
	int GetNumObjects( int iObjectType, int iObjectMode );

	virtual bool ShouldCollide( int collisionGroup, int contentsMask ) const;

	float GetPercentInvisible( void );
	float GetEffectiveInvisibilityLevel( void );	// takes viewer into account

	virtual void AddDecal( const Vector &rayStart, const Vector &rayEnd,
		const Vector &decalCenter, int hitbox, int decalIndex, bool doTrace, trace_t &tr, int maxLODToDecal = ADDDECAL_TO_ALL_LODS );

	virtual void CalcDeathCamView( Vector &eyeOrigin, QAngle &eyeAngles, float &fov );
	virtual Vector GetChaseCamViewOffset( C_BaseEntity *target );

	void ClientPlayerRespawn( void );

	void CreateSaveMeEffect( void );

	virtual bool	IsOverridingViewmodel( void );
	virtual int		DrawOverriddenViewmodel( C_BaseViewModel *pViewmodel, int flags );

	void			SetHealer( C_TFPlayer *pHealer, float flChargeLevel );
	void			GetHealer( C_TFPlayer **pHealer, float *flChargeLevel ) { *pHealer = m_hHealer; *flChargeLevel = m_flHealerChargeLevel; }
	float			MedicGetChargeLevel( void );
	C_BaseEntity *MedicGetHealTarget( void );

	void			StartBurningSound( void );
	void			StopBurningSound( void );
	void			UpdateRecentlyTeleportedEffect( void );

	void			StopViewModelParticles( C_BaseEntity *pEntity );

	bool			CanShowClassMenu( void );

	void			InitializePoseParams( void );
	void			UpdateLookAt( void );
	
	void			UpdatePlayerGlows(void);
	typedef struct
	{
		int m_nEntIndex;
		Color m_clrGlowColor;
		int m_nOffset;
	} player_glow_t;
	CUtlVector< player_glow_t > m_vecEntitiesToDraw;

	bool			IsEnemyPlayer( void );
	void			ShowNemesisIcon( bool bShow );

	CUtlVector<EHANDLE> *GetSpawnedGibs( void ) { return &m_hSpawnedGibs; }

	Vector 			GetClassEyeHeight( void );

	void			ForceUpdateObjectHudState( void );

	bool			GetMedigunAutoHeal( void ) { return tf_medigun_autoheal.GetBool(); }
	bool			ShouldAutoRezoom( void ) { return cl_autorezoom.GetBool(); }
	bool			ShouldAutoReload( void ) { return cl_autoreload.GetBool(); }
	bool			ShouldFlipViewModel( void ) { return cl_flipviewmodels.GetBool(); }

public:
	// Shared functions
	void			TeamFortress_SetSpeed();
	bool			HasItem( void );					// Currently can have only one item at a time.
	void			SetItem( C_TFItem *pItem );
	C_TFItem *GetItem( void );
	bool			IsAllowedToPickUpFlag( void );
	bool			HasTheFlag( int const *pFlagExceptions = NULL, int nNumExceptions = 0 );
	float			GetCritMult( void ) { return m_Shared.GetCritMult(); }

	virtual void	ItemPostFrame( void );

	void			SetOffHandWeapon( C_TFWeaponBase *pWeapon );
	void			HolsterOffHandWeapon( void );

	virtual int GetSkin();
	virtual int GetBody();

	virtual bool		Weapon_ShouldSetLast( C_BaseCombatWeapon *pOldWeapon, C_BaseCombatWeapon *pNewWeapon );
	virtual	bool		Weapon_Switch( C_BaseCombatWeapon *pWeapon, int viewmodelindex = 0 );

	C_WeaponMedigun *GetMedigun( void );
	C_TFWeaponBase *Weapon_OwnsThisID( int iWeaponID );
	C_TFWeaponBase *Weapon_GetWeaponByType( int iType );
	virtual bool		Weapon_SlotOccupied( C_BaseCombatWeapon *pWeapon );
	virtual C_BaseCombatWeapon *Weapon_GetSlot( int slot ) const;
	C_EconEntity *GetEntityForLoadoutSlot( int iSlot );
	C_EconWearable *GetWearableForLoadoutSlot( int iSlot );

	virtual void		GetStepSoundVelocities( float *velwalk, float *velrun );
	virtual void		SetStepSoundTime( stepsoundtimes_t iStepSoundTime, bool bWalking );

	void				ModifyEmitSoundParams( EmitSound_t &params ) OVERRIDE;

	bool	DoClassSpecialSkill( void );
	bool	CanGoInvisible( bool bFeigning = false );

	int		GetMaxAmmo( int iAmmoIndex, int iClassNumber = -1 ) const;

	virtual CAttributeManager *GetAttributeManager() { return &m_AttributeManager; }
	virtual CAttributeContainer *GetAttributeContainer() { return NULL; }
	virtual C_BaseEntity *GetAttributeOwner() { return NULL; }
	virtual CAttributeList *GetAttributeList() { return &m_AttributeList; }
	virtual void ReapplyProvision( void ) { /*Do nothing*/ };

	// Gunslinger
	bool				HasGunslinger( void ) { return m_Shared.m_bGunslinger; }

	C_BaseEntity	*GetGrapplingHookTarget( void ) { return m_hGrapplingHookTarget; }
	void			SetGrapplingHookTarget( CBaseEntity *pTarget, bool bSomething = false );
	
public:
	// Ragdolls.
	virtual C_BaseAnimating *BecomeRagdollOnClient();
	virtual IRagdoll *GetRepresentativeRagdoll() const;
	EHANDLE	m_hRagdoll;
	Vector m_vecRagdollVelocity;

	// Objects
	int CanBuild( int iObjectType, int iObjectMode );
	CUtlVector< CHandle<C_BaseObject> > m_aObjects;

	virtual CStudioHdr *OnNewModel( void );

	void				DisplaysHintsForTarget( C_BaseEntity *pTarget );

	// Shadows
	virtual ShadowType_t ShadowCastType( void );
	virtual void GetShadowRenderBounds( Vector &mins, Vector &maxs, ShadowType_t shadowType );
	virtual void GetRenderBounds( Vector &theMins, Vector &theMaxs );
	virtual bool GetShadowCastDirection( Vector *pDirection, ShadowType_t shadowType ) const;

	IMaterial *GetInvulnMaterial( void ) const { return m_InvulnerableMaterial; }
	bool IsNemesisOfLocalPlayer();
	bool ShouldShowNemesisIcon();

	virtual	IMaterial *GetHeadLabelMaterial( void );

	void UpdateSpyMask(void);
	void UpdateTypingBubble( void );

	void UpdateOverhealEffect( bool bForceHide = false );

	void UpdateDemomanEyeEffect( int iDecapCount );

	bool ShouldDrawSpyAsDisguised( void );

	void CreateBombinomiconHint( void );
	void DestroyBombinomiconHint( void );
	void UpdateHalloweenBombHead( void );

	float m_flInspectTime;
	bool IsInspecting() const;

	int GetCurrency( void ) { return m_nCurrency; }

protected:

	void ResetFlexWeights( CStudioHdr *pStudioHdr );

private:

	void HandleTaunting( void );

	void OnPlayerClassChange( void );
	void UpdatePartyHat( void );

	bool CanLightCigarette( void );

	void InitInvulnerableMaterial( void );

	bool				m_bWasTaunting;
	float				m_flTauntOffTime;
	CameraThirdData_t	m_TauntCameraData;

	QAngle				m_angTauntPredViewAngles;
	QAngle				m_angTauntEngViewAngles;

	CHandle<CBaseAnimating> m_hBombHat;
	CHandle<CBaseAnimating> m_hBombinomicon;
	float				m_flCreateBombHeadAt;

public:

	Vector				m_vecPlayerColor;

private:

	C_TFPlayerClass		m_PlayerClass;

	// ID Target
	int					m_iIDEntIndex;
	int					m_iForcedIDTarget;

	CNewParticleEffect *m_pTeleporterEffect;
	bool				m_bToolRecordingVisibility;

	int					m_iOldState;
	int					m_iOldSpawnCounter;

	// Healer
	CHandle<C_TFPlayer>	m_hHealer;
	float				m_flHealerChargeLevel;
	int					m_iOldHealth;

	CNetworkVar( int, m_iPlayerModelIndex );

	// blinking
	//CountdownTimer m_blinkTimer;

	// Look At
	/*
	int m_headYawPoseParam;
	int m_headPitchPoseParam;
	float m_headYawMin;
	float m_headYawMax;
	float m_headPitchMin;
	float m_headPitchMax;
	float m_flLastBodyYaw;
	float m_flCurrentHeadYaw;
	float m_flCurrentHeadPitch;
	*/

	// Spy cigarette smoke
	bool m_bCigaretteSmokeActive;

	// Medic callout particle effect
	CNewParticleEffect *m_pSaveMeEffect;
	CNewParticleEffect *m_pTypingEffect;

	bool m_bUpdateObjectHudState;

	bool m_bOldCustomModelVisible;

public:

	CTFPlayerShared m_Shared;

// Called by shared code.
public:

	void DoAnimationEvent( PlayerAnimEvent_t event, int nData = 0 );

	CTFPlayerAnimState *m_PlayerAnimState;

	QAngle	m_angEyeAngles;
	CInterpolatedVar< QAngle >	m_iv_angEyeAngles;

	CNetworkHandle( C_TFItem, m_hItem );

	CNetworkHandle( C_TFWeaponBase, m_hOffHandWeapon );
	CNetworkHandle( C_BaseEntity, m_hGrapplingHookTarget );

	int				m_iOldPlayerClass;	// Used to detect player class changes
	bool			m_bIsDisplayingNemesisIcon;

	int				m_nForceTauntCam;
	bool			m_bTyping;

	int				m_iSpawnCounter;

	bool			m_bSaveMeParity;
	bool			m_bOldSaveMeParity;

	int				m_nOldWaterLevel;
	float			m_flWaterEntryTime;
	bool			m_bWaterExitEffectActive;

	float			m_flHeadScale;

	CMaterialReference	m_InvulnerableMaterial;

	// Overheal
	CNewParticleEffect *m_pOverhealEffect;

	// Burning
	CSoundPatch *m_pBurningSound;
	CNewParticleEffect *m_pBurningEffect;
	float				m_flBurnEffectStartTime;
	float				m_flBurnEffectEndTime;

	// Jumping
	CSoundPatch *m_pJumpSound;


	CNewParticleEffect *m_pDisguisingEffect;
	float m_flDisguiseEffectStartTime;
	float m_flDisguiseEndEffectStartTime;

	EHANDLE					m_hFirstGib;
	CUtlVector<EHANDLE>		m_hSpawnedGibs;

	int				m_iOldTeam;
	int				m_iOldClass;
	int				m_iOldDisguiseTeam;
	int				m_iOldDisguiseClass;

	bool			m_bDisguised;
	int				m_iPreviousMetal;

	EHANDLE			m_hOldActiveWeapon;

	CNewParticleEffect *m_pDemoEyeEffect;

	int GetNumActivePipebombs( void );

	int				m_iSpyMaskBodygroup;

	bool			m_bUpdatePartyHat;
	CHandle<C_PlayerAttachedModel>	m_hPartyHat;
	
	CHandle<C_TFSpyMask> m_hSpyMask;

	CAttributeManager m_AttributeManager;

	// Overheal particle fix for spies
	int				m_iOldOverhealTeamNum;

	// MvM Currency
	int				m_nCurrency;
	int				m_nOldCurrency;

private:

	float m_flWaterImpactTime;
	float m_flLastDamageTime;

	// Gibs.
	CUtlVector<breakmodel_t>	m_aGibs;

	C_TFPlayer( const C_TFPlayer & );
};

inline C_TFPlayer *ToTFPlayer( C_BaseEntity *pEntity )
{
	if ( !pEntity || !pEntity->IsPlayer() )
		return NULL;

	Assert( dynamic_cast<C_TFPlayer *>( pEntity ) != 0 );
	return static_cast<C_TFPlayer *>( pEntity );
}

#endif // C_TF_PLAYER_H
