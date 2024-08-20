#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
class CVIBuffer_Cube;
class CCollider_Sphere;
END

BEGIN(Client)
class CCamera_Player;
class CYeti final : public CGameObject
{
public:
	enum STATE { STATE_IDLE, STATE_ROLLING, STATE_SHOT, STATE_END };

private:
	explicit CYeti(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CYeti(const CYeti& rhs);
	virtual ~CYeti() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	CTransform* Get_TransForm() {
		return m_pTransformCom;
	}

	FRAME Get_Frame() {
		return m_tFrame;
	}
	_uint Get_Dust_Num() {
		return m_iDustNum;
	}


	_float Get_Angle() { return m_fAngle; }

	_bool	Get_Clear() { return m_bClear; }
	void	Set_Clear(_bool bClear) { m_bClear = bClear; }

private:
	HRESULT ComParePlayerPos();

private:
	/* 텍스쳐 */
	CTexture*			m_pTextureCom = nullptr;

	/* 모델 */
	CVIBuffer_Rect*		m_pVIBufferCom = nullptr;

	/* 상태 (위치, 크기, 회전) */
	CTransform*			m_pTransformCom = nullptr;

	/* 그려진다. */
	CRenderer*			m_pRendererCom = nullptr;
	CCollider_Sphere*	m_pColliderCom = nullptr;
	CVIBuffer_Cube*		m_pVIBufferCollCom = nullptr;
private:
	_bool btest = false;
	_float m_fFrame = 0.f;
	_uint m_iDustNum = 0;

	_bool  m_bJump = false;
	_float m_fJumpPower = 5.f;
	_float m_fJumpy;
	_float m_fTime = 0;

	STATE			m_eState = STATE_IDLE;

	_bool Shake = false;
	_uint ShakeTime = 0;

	_bool  m_bSnowBallPatter = false;
	_float m_fSnowBallPetternTimer = 0; //패턴 
	_float m_fSnowBallSrowTimer = 0; //던지는 간격
	_int m_iSrowCount = 0;// 던진횟수


	_bool m_bRollingPattern = false;
	_bool m_bRollingScan = false;
	_int m_iRollingCount = 0;// 구른횟수

	_int m_iRolingWaitTime = 0;
	_bool m_bWaiting = false;

	_float m_fScale = 4.f;

	_int m_iWallBoundCount = 0;

	_float m_fAngle = 0.f;

	_bool TestStop = false;

	_bool m_bClear = false;

	CCamera_Player*		m_pCamera = nullptr;
	_int	m_iSoulOutTime = 120;
	_bool	m_iOutSoul = false;
private:
	HRESULT SetUp_Components();
	HRESULT SetUp_OnTerrain(); // 플레이어를 터레인위에 올라가게 하는 함수
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

	HRESULT			Go_Sprites();
	void	Collision_Rect();
public:
	static CYeti* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END