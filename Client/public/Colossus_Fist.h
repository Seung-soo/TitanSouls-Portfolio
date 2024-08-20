#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Camera_Player.h"
BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
class CCollider_Sphere;
class CVIBuffer_Cube;
END

BEGIN(Client)

class CColossus_Fist final : public CGameObject
{
private:
	explicit CColossus_Fist(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CColossus_Fist(const CColossus_Fist& rhs);
	virtual ~CColossus_Fist() = default;
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
	_uint m_iDustNum = 0;

	_bool Shake = false;
	_uint ShakeTime = 0;

	_float m_fFrame = 0.f;
	_bool UpFist = false;
	_float m_fFollowTimer = 0;
	_float m_fUpcount = 0;

	//점프
	_bool  m_bJump = false;
	_float m_fJumpPower = 7.f;
	_float m_fJumpy;
	_float m_fTime = 0;
	//
	_bool WallCrash = false;
	_bool no_Terrain = false;
	_bool m_bAttacking = false;
	_float m_fFistTimer = 31.f;

	_float3 m_fScale = { 8.f,8.f,8.f };
	_float m_fSpeed = 0;
	_bool m_bDead = false;
	_bool m_bAttack = false;
	COLOSSUSINFO* m_pColInfo;
	_bool m_bStandUp = false;


	_float m_fJumpMinus = 0;
private:
	HRESULT SetUp_Components();
	HRESULT SetUp_OnTerrain(); // 플레이어를 터레인위에 올라가게 하는 함수
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();
	HRESULT			Go_Sprites();
	void	Collision_Rect();
public:
	static CColossus_Fist* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END