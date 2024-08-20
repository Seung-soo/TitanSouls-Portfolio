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
class CBrain final : public CGameObject
{
private:
	explicit CBrain(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CBrain(const CBrain& rhs);
	virtual ~CBrain() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;

	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
public:
	CTransform* Get_TransForm() { return m_pTransformCom; }
	void			Set_Dead(_bool bDead) { m_IceDead->bBrainDead = bDead; }
private:
	CTexture*			m_pTextureCom = nullptr;
	CVIBuffer_Rect*		m_pVIBufferCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CCollider_Sphere*	m_pColliderCom = nullptr;
	CVIBuffer_Cube*		m_pVIBufferCollCom = nullptr;
private:
	_float m_fFrame = 0.f;
	_bool UpFist = false;
	_float m_fPetternTimer = 0;
	_float m_fSearchTimer = 0;
	_float m_fUpcount = 0;
	_bool m_bSilding = false;
	_float m_fSlidingSpeed = 2.f;
	_bool m_b2Phase = false;

	_uint m_iWaterCout = 0;
	_bool m_bWaterCreat = false;

	_bool Shake = false;
	_uint ShakeTime = 0;

	_bool  m_bJump = false;
	_float m_fJumpPower = 5.f;
	_float m_fJumpy;
	_float m_fTime = 0;

	_float m_fCrushTime = 0.f;

	_bool m_bDead = false;
	_bool* m_pBossDead = nullptr;
	ICEDEAD* m_IceDead = nullptr;

	_float m_fminusScaleTime = 0;
	_float m_fminusScale = 0;
	_float m_fScale = 4.f;
	CCamera_Player*		m_pCamera = nullptr;
	_int	m_iSoulOutTime = 120;
	_bool	m_iOutSoul = false;
private:
	HRESULT SetUp_Components();
	HRESULT SetUp_OnTerrain(); // 플레이어를 터레인위에 올라가게 하는 함수
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();
	HRESULT	Go_Sprites();
	void	Collision_Rect();
public:
	static CBrain* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END