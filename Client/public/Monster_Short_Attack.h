#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Monster_Short_Range.h"
BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
class CVIBuffer_Rect_Z;
class CVIBuffer_Cube;
class CCollider_Sphere;
END

BEGIN(Client)

class CMonster_Short_Attack final : public CGameObject
{
private:
	explicit CMonster_Short_Attack(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CMonster_Short_Attack(const CMonster_Short_Attack& rhs);
	virtual ~CMonster_Short_Attack() = default;
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
	_bool		Get_Dead() { return m_bObjectDead; }

private:
	/* �ؽ��� */
	CTexture*			m_pTextureCom = nullptr;

	/* �� */
	CVIBuffer_Rect_Z*		m_pVIBufferCom = nullptr;

	/* ���� (��ġ, ũ��, ȸ��) */
	CTransform*			m_pTransformCom = nullptr;

	/* �׷�����. */
	CRenderer*			m_pRendererCom = nullptr;
	CCollider_Sphere*	m_pColliderCom = nullptr;
	CVIBuffer_Cube*		m_pVIBufferCollCom = nullptr;
private:
	_bool btest = false;
	_float m_fFrame = 0.f;
	_float m_fPetternTimer = 0;

	_bool  m_bJump = false;
	_float m_fJumpPower = 5.f;
	_float m_fJumpy;
	_float m_fTime = 0;

	_float m_fScale = 2.f;

	_uint m_iAttackTimer = 0;

	_int m_iafterimageCount = 0;

	_bool TestStop = false;

	CMonster_Short_Range* m_pmonster;
private:
	HRESULT SetUp_Components();
	HRESULT SetUp_OnTerrain(); // �÷��̾ �ͷ������� �ö󰡰� �ϴ� �Լ�
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();
	HRESULT			Go_Sprites();

	_int	Collision();
public:
	static CMonster_Short_Attack* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END