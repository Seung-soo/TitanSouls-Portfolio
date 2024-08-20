#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CGameInstance;
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
class CCollider_Sphere;
END

BEGIN(Client)

class CBrother final : public CGameObject
{
private:
	explicit CBrother(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CBrother(const CBrother& rhs);
	virtual ~CBrother() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float fTimeDelta) override;
	virtual _int	LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
public:
	void			Set_GoAlpha(_bool bGoAlpha) { m_bGoAlpha = bGoAlpha; }
private:
	CTexture*			m_pTextureCom = nullptr;
	CVIBuffer_Rect*		m_pVIBufferCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CCollider_Sphere*	m_pColliderCom = nullptr;

private:
	HRESULT			SetUp_Components();
	HRESULT			SetUp_OnTerrain();
	HRESULT			SetUp_RenderState();
	HRESULT			Release_RenderState();

	void			Collision();
private:
	_int			m_iAlpha = 255;
	_bool			m_bGoAlpha = false;
public:
	static CBrother*			Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg) override;
	virtual void			Free() override;
};

END