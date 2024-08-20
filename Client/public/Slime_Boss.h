#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
class CCollider_Sphere;
class CVIBuffer_Cube;
END

BEGIN(Client)

class CSlime_Boss final : public CGameObject
{
private:
	explicit CSlime_Boss(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CSlime_Boss(const CSlime_Boss& rhs);
	virtual ~CSlime_Boss() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;

	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
public:
	CTransform* Get_TransForm() { return m_pTransformCom; }
	_bool Get_Jump() { return m_bJump; }

private:
	CTexture*			m_pTextureCom = nullptr;
	CVIBuffer_Rect*		m_pVIBufferCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CCollider_Sphere*	m_pColliderCom = nullptr;
	CVIBuffer_Cube*		m_pVIBufferCollCom = nullptr;

private:
	SLIMEDEAD* m_SlimeDead;
	_float m_fFrame = 0.f;
	_bool UpFist = false;
	_float m_fPetternTimer = 0;
	_float m_fSearchTimer = 0;
	_float m_fUpcount = 0;
	_bool m_bSilding = false;
	_float m_fSlidingSpeed = 2.f;
	_bool m_b2Phase = false;
	_float3 m_fSplitPos;
	_float3 m_fSplittagetPos;

	_bool m_bHitReady = false;
	_uint m_iHitTimer = 0;

	_uint m_iWaterCout = 0;
	_bool m_bWaterCreat = false;

	_float3 temppos;

	_bool  m_bJump = false;
	_float m_fJumpPower = 5.f;
	_float m_fJumpy;
	_float m_fTime = 0;

	_float m_fCrushTime = 0.f;
	_bool m_bCruch = false;
	_bool m_bDead = false;
	//_bool* m_pBossDead = nullptr;
	_float m_fCrushScale = 0;
	_bool m_bSplitPattern = false;
	_uint m_iSplitCount = 0;
	_float3 savePos;
	_float m_fScale = 8.f;
	_uint  m_iSmallRank = 4;
	SLIMEINFO m_SlimeInfo;

	_int	m_iCollDelay = 60;
private:
	HRESULT SetUp_Components();
	HRESULT SetUp_OnTerrain(); // 플레이어를 터레인위에 올라가게 하는 함수
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();


private:
	HRESULT Hit();
	void	Collision_Rect();
public:
	static CSlime_Boss* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END