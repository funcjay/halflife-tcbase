// env_music (FMOD), written by Shepard

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "gamerules.h"
#include "player.h"
#include "weapons.h"

class CMusic : public CPointEntity
{
public:
	void Spawn() override;
	void Use(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value) override;
};

LINK_ENTITY_TO_CLASS(env_music, CMusic);

void CMusic::Spawn()
{
	pev->solid = SOLID_NOT;
	pev->movetype = MOVETYPE_NONE;
	SET_MODEL(ENT(pev), STRING(pev->model));
}

extern int gmsgFMOD;

void CMusic::Use(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value)
{
	MESSAGE_BEGIN(MSG_ALL, gmsgFMOD);
		WRITE_STRING((char*)STRING(pev->target));
		WRITE_COORD(pev->speed);
		WRITE_BYTE((pev->spawnflags & 1) ? TRUE : FALSE);
	MESSAGE_END();
}
