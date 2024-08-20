#pragma once
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

class CFire final : public CGameObject
{
private:
	explicit CFire(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CFire(const CFire& rhs);
	virtual ~CFire() = default;
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

	_bool Get_Dead() {
		return m_bDead;
	}

	_bool Get_ButtonOn() { return m_buttoninfo->bOnButton; }
private:
	/* �ؽ��� */
	CTexture*			m_pTextureCom = nullptr;

	/* �� */
	CVIBuffer_Rect*		m_pVIBufferCom = nullptr;

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
	_float m_fJumpPower = 0.f;
	_float m_fJumpy;
	_float m_fTime = 0;

	//_float m_fScale = 2.f;

	_uint m_iDeleteCount = 0;
	_bool m_bDead = false;

	_bool TestStop = false;
	BUTTONINFO* m_buttoninfo;
	//_uint m_fFrame = 0;
	_float m_fScale = 3.f;


private:
	HRESULT SetUp_Components();
	HRESULT SetUp_OnTerrain(); // �÷��̾ �ͷ������� �ö󰡰� �ϴ� �Լ�
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

	void	Collision();
public:
	static CFire* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END