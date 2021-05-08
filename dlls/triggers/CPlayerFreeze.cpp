// player_freeze, ported from SoHL by Shepard

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "gamerules.h"
#include "player.h"
#include "weapons.h"

class CPlayerFreeze : public CBaseDelay
{
public:
	void Spawn() override;
	void Use(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value) override;
};

LINK_ENTITY_TO_CLASS(player_freeze, CPlayerFreeze);

void CPlayerFreeze::Spawn()
{
	pev->solid = SOLID_NOT;
	pev->movetype = MOVETYPE_NONE;
	SET_MODEL(ENT(pev), STRING(pev->model));
}

void CPlayerFreeze::Use(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value)
{
	if (!pActivator || !(pActivator->pev->flags & FL_CLIENT) || !ShouldToggle(useType, (pActivator->pev->flags & FL_FROZEN)))
		return;

	if (pActivator->pev->flags & FL_FROZEN)
		((CBasePlayer*)((CBaseEntity*)pActivator))->EnableControl(TRUE);
	else
		((CBasePlayer*)((CBaseEntity*)pActivator))->EnableControl(FALSE);
}
