// env_fog, written by Solokiller

//=======================
// ClientFog
//=======================
#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "CClientFog.h"

extern int gmsgSetFog;

const int SF_FOG_STARTON = 1;

LINK_ENTITY_TO_CLASS(env_fog, CClientFog);

TYPEDESCRIPTION CClientFog::m_SaveData[] =
{
	DEFINE_FIELD(CClientFog, m_fActive, FIELD_BOOLEAN),
	DEFINE_FIELD(CClientFog, m_iStartDist, FIELD_INTEGER),
	DEFINE_FIELD(CClientFog, m_iEndDist, FIELD_INTEGER),
};

IMPLEMENT_SAVERESTORE(CClientFog, CBaseEntity);

CClientFog* CClientFog::FogCreate()
{
	CClientFog* pFog = GetClassPtr((CClientFog*)NULL);
	pFog->pev->classname = MAKE_STRING("env_fog");
	pFog->Spawn();

	return pFog;
}

void CClientFog::KeyValue(KeyValueData* pkvd)
{
	if (FStrEq(pkvd->szKeyName, "startdist"))
	{
		m_iStartDist = atoi(pkvd->szValue);
		pkvd->fHandled = TRUE;
	}
	else if (FStrEq(pkvd->szKeyName, "enddist"))
	{
		m_iEndDist = atoi(pkvd->szValue);
		pkvd->fHandled = TRUE;
	}
	else
		CBaseEntity::KeyValue(pkvd);
}

void CClientFog::Spawn()
{
	pev->effects |= EF_NODRAW;

	if (FStringNull(pev->targetname))
		pev->spawnflags |= SF_FOG_STARTON;

	//Delay fog startup to the next frame
	pev->nextthink = gpGlobals->time + 0.01;

	SetThink(&CClientFog::FogThink);
}

void CClientFog::FogThink()
{
	if (!(pev->spawnflags & SF_FOG_STARTON))
	{
		m_fActive = FALSE;
	}
	else
	{
		m_fActive = TRUE;
		EnableForAll();
	}

	SetThink(NULL);
}

void CClientFog::Use(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value)
{
	if (!m_fActive)
	{
		m_fActive = TRUE;
		EnableForAll();
	}
	else
	{
		m_fActive = FALSE;
		SetFogAll(g_vecZero, 0, 0);
	}
}

void CClientFog::CheckFogForClient(edict_t* pClient)
{
	//TODO: currently only one fog entity can exist in a map
	//This can be updated to use a fog manager object that keeps track of which fog entity is active to automatically change fog settings
	CClientFog* pFog = (CClientFog*)UTIL_FindEntityByClassname(NULL, "env_fog");

	if (pFog && pFog->m_fActive == TRUE)
	{
		pFog->SetFog(pClient, pFog->pev->rendercolor, pFog->m_iStartDist, pFog->m_iEndDist);
	}
	else
	{
		SetFog(pClient, g_vecZero, 0, 0);
	}
}

void CClientFog::EnableForAll()
{
	SetFogAll(pev->rendercolor, m_iStartDist, m_iEndDist);
}

static void InternalSetFog(edict_t* pClient, const Vector& color, float startDistance, float endDistance)
{
	MESSAGE_BEGIN(pClient ? MSG_ONE : MSG_ALL, gmsgSetFog, NULL, pClient);
		WRITE_SHORT(color.x);
		WRITE_SHORT(color.y);
		WRITE_SHORT(color.z);
		WRITE_SHORT(startDistance);
		WRITE_SHORT(endDistance);
	MESSAGE_END();
}

void CClientFog::SetFog(edict_t* pClient, const Vector& color, float startDistance, float endDistance)
{
	if (pClient == NULL)
	{
		ALERT(at_error, "CClientFog::SetFog called with NULL client\n");
		return;
	}

	InternalSetFog(pClient, color, startDistance, endDistance);
}

void CClientFog::SetFogAll(const Vector& color, float startDistance, float endDistance)
{
	InternalSetFog(NULL, color, startDistance, endDistance);
}
