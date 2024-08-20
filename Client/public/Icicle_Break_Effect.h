#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Icicle.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
class CCollider_Sphere;
class CVIBuffer_Cube;
END

BEGIN(Client)

class CIcicle_Break_Effect final : public CGameObject
{
private:
	explicit CIcicle_Break_Effect(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CIcicle_Break_Effect(const CIcicle_Break_Effect& rhs);
	virtual ~CIcicle_Break_Effect() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;

	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
public:
	CTransform* Get_TransForm() { return m_pTransformCom; }
	_bool Get_Jump() { return m_bJump; }
	_bool Get_Dead() { return m_bDead; }
private:
	CTexture*			m_pTextureCom = nullptr;
	CVIBuffer_Rect*		m_pVIBufferCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	//CCollider_Sphere*	m_pColliderCom = nullptr;
	CVIBuffer_Cube*		m_pVIBufferCollCom = nullptr;
private:
	_float m_fFrame = 0.f;

	
	_float3 temppos;
	_bool  m_bJump = false;
	_float m_fJumpPower = 4.f;
	_float m_fJumpy;
	_float m_fTime = 0;

	_float m_fCrushTime = 0.f;
	_bool m_bSplitPattern = true;
	_uint m_iSplitCount = 0;
	_bool m_bDead = false;
	//_bool* m_pBossDead = nullptr;
	
	_float3 savePos;

	_float m_fScale = 1.5f;
	SLIMEINFO* m_SlimeInfo;
	SLIMEINFO m_SlimeInfo_Mini;

	CIcicle* m_pSlime;

	_int	m_iCollDelay = 60;
private:
	HRESULT SetUp_Components();
	HRESULT SetUp_OnTerrain(); // 플레이어를 터레인위에 올라가게 하는 함수
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();
	HRESULT			Go_Sprites();

	//HRESULT Hit();

	//void	Collision_Rect();
public:
	static CIcicle_Break_Effect* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END