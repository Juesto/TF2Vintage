//========= Copyright � 1996-2006, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef C_OBJ_DISPENSER_H
#define C_OBJ_DISPENSER_H
#ifdef _WIN32
#pragma once
#endif

#include "c_baseobject.h"
#include "ObjectControlPanel.h"
#include "vgui_controls/RotatingProgressBar.h"

class C_ObjectDispenser : public C_BaseObject
{
	DECLARE_CLASS( C_ObjectDispenser, C_BaseObject );
public:
	DECLARE_CLIENTCLASS();

	C_ObjectDispenser();
	~C_ObjectDispenser();

	int GetUpgradeLevel(void) { return m_iUpgradeLevel; }

	virtual void GetStatusText( wchar_t *pStatus, int iMaxStatusLen );

	int GetMetalAmmoCount() { return m_iAmmoMetal; }

	CUtlVector< CHandle<C_TFPlayer> > m_hHealingTargets;

	virtual void OnDataChanged( DataUpdateType_t updateType );

	virtual void SetDormant( bool bDormant );

	void UpdateEffects( void );

	virtual void UpdateDamageEffects( BuildingDamageLevel_t damageLevel );

	bool m_bUpdateHealingTargets;

private:

	int  m_iAmmoMetal;
	bool m_bStealthed;
	bool m_bStealthedLast;

	bool m_bPlayingSound;

	struct healingtargeteffects_t
	{
		C_BaseEntity		*pTarget;
		CNewParticleEffect	*pEffect;
	};
	CUtlVector<healingtargeteffects_t> m_hHealingTargetEffects;

	CNewParticleEffect *m_pDamageEffects;

private:
	C_ObjectDispenser( const C_ObjectDispenser & ); // not defined, not accessible
};


class CDispenserControlPanel : public CObjectControlPanel
{
	DECLARE_CLASS( CDispenserControlPanel, CObjectControlPanel );

public:
	CDispenserControlPanel( vgui::Panel *parent, const char *panelName );

protected:
	virtual void OnTickActive( C_BaseObject *pObj, C_TFPlayer *pLocalPlayer );

private:
	vgui::RotatingProgressBar *m_pAmmoProgress;
};

class CDispenserControlPanel_Red : public CDispenserControlPanel
{
	DECLARE_CLASS( CDispenserControlPanel_Red, CDispenserControlPanel );

public:
	CDispenserControlPanel_Red( vgui::Panel *parent, const char *panelName ) : CDispenserControlPanel( parent, panelName ) {}
};

class CDispenserControlPanel_Green : public CDispenserControlPanel
{
	DECLARE_CLASS( CDispenserControlPanel_Green, CDispenserControlPanel );

public:
	CDispenserControlPanel_Green( vgui::Panel *parent, const char *panelName) : CDispenserControlPanel(parent, panelName ) {}
};

class CDispenserControlPanel_Yellow : public CDispenserControlPanel
{
	DECLARE_CLASS( CDispenserControlPanel_Yellow, CDispenserControlPanel );

public:
	CDispenserControlPanel_Yellow( vgui::Panel *parent, const char *panelName) : CDispenserControlPanel(parent, panelName ) {}
};

class C_ObjectCartDispenser : public C_ObjectDispenser
{
	DECLARE_CLASS( C_ObjectCartDispenser, C_ObjectDispenser );

public:
	DECLARE_CLIENTCLASS();
};

#endif	//C_OBJ_DISPENSER_H