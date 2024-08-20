#pragma once
#include "../../core/inc/fmod.h"
#include "Base.h"

#define SOUND_MAX 1.0f
#define SOUND_MIN 0.0f
#define SOUND_DEFAULT 0.5f
#define SOUND_WEIGHT 0.1f

BEGIN(Engine)

class CSoundMgr final : public CBase
{
	DECLARE_SINGLETON(CSoundMgr)
//public:
//	static CSoundMgr* Get_Instance()
//	{
//		if (nullptr == m_pInstance)
//			m_pInstance = new CSoundMgr;
//
//		return m_pInstance;
//	}
//	static void Destroy_Instance()
//	{
//		if (m_pInstance)
//		{
//			delete m_pInstance;
//			m_pInstance = nullptr;
//		}
//	}
public:
	enum CHANNELID { BGM, PLAYER, PLAYER_EFFECT, PLAYER_SHOT, MONSTER1, MONSTER2, MONSTER3, MONSTER4, MONSTER5, KILL_MONSTER, EFFECT, UI, SYSTEM_EFFECT, SYSTEM_EFFECT2, SYSTEM_EFFECT3, MAXCHANNEL };

private:
	CSoundMgr();
	virtual ~CSoundMgr() = default;

public:
	void Initialize();

public:
	int  VolumeUp(CHANNELID eID, _float _vol);
	int  VolumeDown(CHANNELID eID, _float _vol);
	int  BGMVolumeUp(_float _vol);
	int  BGMVolumeDown(_float _vol);
	int  Pause(CHANNELID eID);
	void PlaySound(TCHAR* pSoundKey, CHANNELID eID, _float _vol);
	void PlayBGM(TCHAR* pSoundKey, _float _vol);
	void StopSound(CHANNELID eID);
	void StopAll();

private:
	float m_volume = SOUND_DEFAULT;
	float m_BGMvolume = SOUND_DEFAULT;
	FMOD_BOOL m_bool;

private:
	void LoadSoundFile();

private:
	//static CSoundMgr* m_pInstance;
	// 사운드 리소스 정보를 갖는 객체 
	map<TCHAR*, FMOD_SOUND*> m_mapSound;
	// FMOD_CHANNEL : 재생하고 있는 사운드를 관리할 객체 
	FMOD_CHANNEL* m_pChannelArr[MAXCHANNEL];
	// 사운드 ,채널 객체 및 장치를 관리하는 객체 
	FMOD_SYSTEM* m_pSystem;
	_bool		m_bPause = false;
public:
	virtual void	Free() override;
};

END