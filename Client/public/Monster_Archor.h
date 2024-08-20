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

class CMonster_Archor final : public CGameObject
{
private:
	explicit CMonster_Archor(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CMonster_Archor(const CMonster_Archor& rhs);
	virtual ~CMonster_Archor() = default;
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
	CCollider_Sphere*	m_pColliderCom = nullptr;
	CVIBuffer_Cube*		m_pVIBufferCollCom = nullptr;
private:
	enum STATE { STATE_IDLE, STATE_WALK, STATE_ATTACK, STATE_DEAD, STATE_END };
	_bool m_bAttacking = false;
	_bool m_bShotBullet = false;

	STATE m_estate;
	STATE m_esavestate;

	_float m_fFrame = 0.f;

	_float m_fPetternTimer = 0;
	_bool m_bHitReady = false;
	_uint m_iHitTimer = 0;

	_bool m_bAttack = false;
	_uint m_iAttackTimer = 0;

	_uint m_iBulletTimer = 0;
	_uint m_iShotWait = 0;


	_bool  m_bJump = false;
	_float m_fJumpPower = 2.f;
	_float m_fJumpy;
	_float m_fTime = 0;

	_bool m_bDead = false;
	//_bool* m_pBossDead = nullptr;
	_float3 savePos;

	_float m_fScale = 4.f;
	_int	m_iCollDelay = 60;
private:
	HRESULT SetUp_Components();
	HRESULT SetUp_OnTerrain(); // 플레이어를 터레인위에 올라가게 하는 함수
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();
	HRESULT			Go_Sprites();


	void	Collision_Rect();
public:
	static CMonster_Archor* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END