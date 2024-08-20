#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)

class CLoader final : public CBase
{
public:
	explicit CLoader(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLoader() = default;
public:
	CRITICAL_SECTION	Get_CS() { return m_CS; }
	LEVEL				Get_LevelID() const { return m_eLevel; }
	_bool				isFinished() const { return m_isFinished; }
public:
	HRESULT	NativeConstruct(LEVEL eNextLevel);
	HRESULT Loading_For_Main();	// logo main에 가기 전 로드해야 할 것들
	HRESULT	Loading_For_Field();	// 필드 가기전 로드할 것들
	HRESULT	Loading_For_Slime();	// 슬라임 가기전 로드할 것들
	HRESULT	Loading_For_IceBrain();	// 아이스브레인 가기전 로드할 것들
	HRESULT	Loading_For_Colossus();	// 콜로서스 가기전 로드할 것들
	HRESULT	Loading_For_EyeCube(); // 아이큐브 가기전 로드할 것들
	HRESULT Loading_For_Yeti();
	HRESULT Loading_For_Respawn1();
	HRESULT Loading_For_Respawn2();
	HRESULT Loading_For_EndingCredit();

private:
	LPDIRECT3DDEVICE9	m_pGraphic_Device = nullptr;
	LEVEL				m_eLevel = LEVEL_END;
	HANDLE				m_hThread;
	_bool				m_isFinished = false;
	CRITICAL_SECTION	m_CS;
public:
	static CLoader*	Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eNextLevel);
	virtual void	Free() override;
};

END