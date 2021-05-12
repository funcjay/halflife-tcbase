// env_fog, written by Solokiller

//=======================
// ClientFog
//=======================
class CClientFog : public CBaseEntity
{
public:
	void Spawn();
	void KeyValue(KeyValueData* pkvd);
	void EXPORT FogThink();
	void Use(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value);
	float m_iStartDist;
	float m_iEndDist;
	BOOL m_fActive;
	virtual int Save(CSave& save);
	virtual int Restore(CRestore& restore);
	static TYPEDESCRIPTION m_SaveData[];

	// Checks if there's a fog entity active whose settings needs sending. Otherwise, tells the client to disable fog
	static void CheckFogForClient(edict_t* pClient);

private:
	void EnableForAll();

	// Sets the fog settings for the given client
	static void SetFog(edict_t* pClient, const Vector& color, float startDistance, float endDistance);

	// Sets the fog settings for all clients
	static void SetFogAll(const Vector& color, float startDistance, float endDistance);

public:
	static CClientFog* FogCreate();
};
