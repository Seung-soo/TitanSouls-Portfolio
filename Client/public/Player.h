#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CGameInstance;
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
class CCollider_Sphere;
class CVIBuffer_Cube;
class CVIBuffer_Rect_Color;
END

BEGIN(Client)

class CPlayer final : public CGameObject
{
public:
	enum DIR { DIR_RIGHT, DIR_UP, DIR_LEFT, DIR_DOWN, DIR_DR, DIR_DL, DIR_UL, DIR_UR, DIR_END };
	enum STATE { STATE_IDLE, STATE_WALK, STATE_ROLLING, STATE_SHOT, STATE_PULL, STATE_KNOCKBACK, STATE_FLY, STATE_LIGHTLEAK, STATE_DIE, STATE_END };
	//enum ARROWSTATE { ARROWSTATE_IDLE, ARROWSTATE_SHOOT, ARROWSTATE_COME, ARROWSTATE_END };
private:
	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CPlayer(const CPlayer& rhs);
	virtual ~CPlayer() = default;
public:
	void			Set_Arrow(_bool bHaveArrow, _bool isArrow) { m_bHaveArrow = bHaveArrow; m_isArrow = isArrow; }
	_bool			Get_IsArrow() { return m_isArrow; }
	_bool			Get_HaveArrow() { return m_bHaveArrow; }
	void			Set_ShotPower(_float fShotPower) { m_iShotPower *= fShotPower; }
	_float			Get_ShotPower() { return m_iShotPower; }
	void			Set_MoveEndArrow(_bool bMoveEndArrow) { m_bMoveEndArrow = bMoveEndArrow; }
	void			Set_Die(_bool bDie) { m_bDie = bDie; }
	STATE			Get_State() { return m_eState; }
	void			Set_State(CPlayer::STATE eState) { m_eState = eState; }
	void			Set_KnockBackDir(_float3 vKnockBackDir) { m_vKnockBackDir = vKnockBackDir; }
	void			Set_Fly(_bool bFly) { m_bFly = bFly; }
	void			Set_TurnNum(_int iTurnNum) { m_iTurnNum = iTurnNum; }
	void			Set_SoulStart(_bool bSoulStart) { m_bSoulStart = bSoulStart; }
	void			Set_Dir(DIR eDir) { m_eDir = eDir; }
	_int			Get_RollDelay() { return m_iRollDelay; }
	//PlayerDust 추가
	CTransform* Get_TransForm() { return m_pTransformCom; }
	_uint Get_Dust_Num() { return m_iDustNum; }
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float fTimeDelta) override;
	virtual _int	LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
private:
	CTexture*			m_pTextureCom = nullptr;
	CVIBuffer_Rect*		m_pVIBufferCom = nullptr;
	CVIBuffer_Cube*		m_pVIBufferCollCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CCollider_Sphere*	m_pColliderCom = nullptr;
	CVIBuffer_Rect_Color* m_pTestCom = nullptr;

	_float			m_iLookHeight = 0;
	DIR				m_eDir = DIR_DOWN;
	_bool			m_isArrow = false;
	STATE			m_eState = STATE_IDLE;
	_int			m_iRollDelay = 0;
	_float			m_iShotPower = 0.f;
	_bool			m_bHaveArrow = true;
	_bool			m_bPushE = false;
	_bool			m_bMoveEndArrow = false;
	_bool			m_bDie = false;

	_float3			m_vKnockBackDir;
	_int			m_iKnockBackTime = 30;

	_bool			m_bFly = false;
	_float			m_fFlyFrameTime = 0;
	_int			m_iTurnNum = 0;
	_bool			m_bSoulStart = false;
	_bool			m_bFirstBlood = false;

	_uint			m_iDustNum = 0;
	_bool			m_bRolling_Dust = false;
	_float			m_fDustSec = 0;

private:
	HRESULT			SetUp_Components();
	HRESULT			SetUp_OnTerrain();
	HRESULT			SetUp_RenderState();
	HRESULT			Release_RenderState();

	// 스프라이트에 따라 정점 관리
	HRESULT			Go_Sprites();
	void			Collision();
	void			Go_FrameTurn();

private:	// 플레이어 행동
	HRESULT			Move(_float fTimeDelta);
	HRESULT			Rolling(_float fTimeDelta);
	HRESULT			Shot(CGameInstance* pGameInstance, _float fTimeDelta);
	HRESULT			AimMove(_float fTimeDelta);
	HRESULT			PullArrow(CGameInstance* pGameInstance, _float fTimeDelta);
	HRESULT			Die();
public:
	static CPlayer*			Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg) override;
	virtual void			Free() override;
};

END