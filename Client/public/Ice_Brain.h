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

class CIce_Brain final : public CGameObject
{
private:
	explicit CIce_Brain(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CIce_Brain(const CIce_Brain& rhs);
	virtual ~CIce_Brain() = default;
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

	_bool Get_Jump() {
		return m_bJump;
	}
	_bool Get_Search() {
		return m_bSearch;
	}
	_bool Get_Dead() {
		return m_bDead;
	}
	_bool Get_Hir() {
		return m_bHit;
	}
	_float Get_Scale() {
		return m_fScale;
	}

	void Set_Phase(_bool bPhase) { m_b2Phase = bPhase; }
private:
	CTexture*			m_pTextureCom = nullptr;
	CVIBuffer_Cube*		m_pVIBufferCom = nullptr;
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
	_float m_fSlidingSpeed = 3.f;
	_float m_fJumpSlidingSpeed = 1.f;
	_bool m_bHit = false;
	_bool Shake = false;
	_uint ShakeTime = 0;

	_bool m_b2Phase = false;
	_bool m_bSearch = false;
	_bool  m_bJump = false;
	_float m_fJumpPower = 5.f;
	_float m_fJumpy;
	_float m_fTime = 0;

	_uint m_iSearchCount = 0;
	_bool m_bJumpPattern = false;
	_bool m_bJumpSliding = false;

	_bool m_bDead = false;
	ICEDEAD* m_IceDead = nullptr;
	_float m_fScale = 4.f;
	_float m_fIceSmallCount = 0;
	_bool m_bStart = false;
private:
	HRESULT Hit();
private:
	HRESULT SetUp_Components();
	HRESULT SetUp_OnTerrain(); // 플레이어를 터레인위에 올라가게 하는 함수
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

	void	Collision_Rect();
public:
	static CIce_Brain* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END