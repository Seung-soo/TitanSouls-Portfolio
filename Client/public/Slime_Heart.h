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
class CSlime_Heart final : public CGameObject
{
private:
	explicit CSlime_Heart(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CSlime_Heart(const CSlime_Heart& rhs);
	virtual ~CSlime_Heart() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;

	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	_bool Get_Dead() {
		return m_SlimeDead->bHeartDead;
	}

public:
	CTransform* Get_TransForm() {
		return m_pTransformCom;
	}


	_bool Get_Jump() {
		return m_bJump;
	}

	void	Collision();
private:
	CTexture*			m_pTextureCom = nullptr;
	CVIBuffer_Rect*		m_pVIBufferCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CCollider_Sphere*	m_pColliderCom = nullptr;
	CVIBuffer_Cube*		m_pVIBufferCollCom = nullptr;
private:
	SLIMEDEAD* m_SlimeDead;
	CCamera_Player*		m_pCamera = nullptr;
	_bool m_bfirstJump = false;

	_uint m_iWaterCout = 0;
	_bool m_bWaterCreat = false;


	_bool  m_bJump = false;
	_float m_fJumpPower = 3.f;
	_float m_fJumpy;
	_float m_fTime = 0;

	_float m_fCrushTime = 0.f;

	_bool m_bDead = false;
	//_bool* m_pBossDead = nullptr;

	_float m_fScale = 1.f;
	_int	m_iCollDelay = 60;
	_int	m_iSoulOutTime = 120;
	_bool	m_iOutSoul = false;
private:
	HRESULT SetUp_Components();
	HRESULT SetUp_OnTerrain(); // 플레이어를 터레인위에 올라가게 하는 함수
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();
	HRESULT	Go_Sprites();

public:
	static CSlime_Heart* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END