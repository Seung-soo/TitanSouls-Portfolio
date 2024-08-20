#pragma once

#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Engine)
class CCollider_Sphere;

END
BEGIN(Client)
class CPlayer;
class CCamera_Player final : public CCamera
{
private:
	explicit				CCamera_Player(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit				CCamera_Player(const CCamera_Player& rhs);
	virtual					~CCamera_Player() = default;
public:
	virtual					HRESULT NativeConstruct_Prototype();
	virtual					HRESULT NativeConstruct(void* pArg);
	virtual					_int	Tick(_float fTimeDelta);
	virtual					_int	LateTick(_float fTimeDelta);
	virtual					HRESULT Render();

public:
	void					Set_Shake(_bool bX, _bool bY, _bool bZ);
	void					Set_ShakeEX(_bool bX, _bool bY, _bool bZ);
	void					Set_IsArrow(_bool isArrow) { m_isArrow = isArrow; }
	void					Set_PowerEx(_float _Power) { m_fShakePower = _Power; }
	void					Set_Directing(_bool bSoul, _bool bIntro, _bool bInDoor);
	void					Set_Map_Intro(_bool bIntro);
	void					Set_InDoor(_bool bInDoor, LEVEL eNextLevel);
	void					Set_InBigDoor(_bool bInBigDoor, LEVEL eNextLevel);
	void					Set_Ending(_bool bEnding) { m_bEnding = bEnding; }
	void					Set_EndingSoulHit(_bool bEndingSoulHit) { m_bEndingSoulHit = bEndingSoulHit; }
	void					Set_EndingExit(_bool bEndingExit) { m_bEndingExit = bEndingExit; }
public:
	void					Camera_Shake_Horizon(_float fTimeDelta);
	void					Camera_Shake_Vertical(_float fTimeDelta);
	void					Camera_Shake_Z(_float fTimeDelta);
	void					Camera_Stop();
public:
	void					Camera_Shake_Horizon(_float fPower, _float fTimeDelta);
	void					Camera_Shake_Vertical(_float fPower, _float fTimeDelta);
	void					Camera_Shake_Z(_float fPower, _float fTimeDelta);

	void					Camera_Soul_Moving(_float fTimeDelta);
	void					Camera_Map_Intro(_float fTimeDelta);
	void					Camera_InDoor(_float fTimeDelta);
	void					Camera_InBigDoor(_float fTimeDelta);

	void					Camera_EndingStart(_float fTimeDelta);
	void					Camera_EndingSoulHit(_float fTimeDelta);
	void					Camera_EndingExit(_float fTimeDelta);
private:
	_bool					m_isArrow = false;
	_float					m_fMove = 0.f;
	_float					m_fMoveUp = 0.f;
	_float					m_fMoveZ = 0.f;
	_int					m_iShakeDelay = 3;
	_int					m_iShakeDelayUp = 3;
	_int					m_iShakeDelayZ = 3;
	_bool					m_bShakeX = false;
	_bool					m_bShakeY = false;
	_bool					m_bShakeZ = false;
	_bool					m_bShakeXEx = false;
	_bool					m_bShakeYEx = false;
	_bool					m_bShakeZEx = false;
	_bool					m_bSoulMove = false;
	_bool					m_bIntro = false;
	_bool					m_bInDoor = false;
	_bool					m_bInBigDoor = false;
	_bool					m_bEnding = false;
	_bool					m_bEndingSoulHit = false;
	_bool					m_bEndingExit = false;
	_float					m_fShakePower = 0;
	_int					m_iIntroTime = 0;

	_bool					m_bStartLoading = false;

	_int					m_iWalkTime = 0;
	_int					iZoom = 0;
	LEVEL					m_eNextLevel = LEVEL_END;
	CPlayer*				m_pPlayer = nullptr;
private:
	CCollider_Sphere*		m_pColliderCom = nullptr;
public:
	static CCamera_Player*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg);
	virtual void			Free() override;
};

END