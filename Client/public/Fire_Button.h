#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect_Z;
class CCollider_Sphere;
class CVIBuffer_Cube;
END

BEGIN(Client)

class CFire_Button final : public CGameObject
{
private:
	explicit CFire_Button(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CFire_Button(const CFire_Button& rhs);
	virtual ~CFire_Button() = default;
public:
	void	Set_OnButton(_bool bButtonOn) { m_buttoninfo->bOnButton = bButtonOn; }
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CTexture*				m_pTextureCom = nullptr;
	CVIBuffer_Rect_Z*		m_pVIBufferCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CCollider_Sphere*	m_pColliderCom = nullptr;
	CVIBuffer_Cube*		m_pVIBufferCollCom = nullptr;

	BUTTONINFO* m_buttoninfo;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_OnTerrain(); // 플레이어를 터레인위에 올라가게 하는 함수
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

private:
	_uint m_fFrame = 0;
	_float m_fScale = 3.f;
	void	Collision();

public:
	static CFire_Button* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END