#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Torch.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
END

BEGIN(Client)
class CTorch_Fire final : public CGameObject
{
public:
	explicit CTorch_Fire(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CTorch_Fire(const CTorch_Fire& rhs);
	virtual ~CTorch_Fire() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float fTimeDelta) override;
	virtual _int	LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
private:
	CTexture*			m_pTextureCom = nullptr;
	CVIBuffer_Rect*		m_pVIBufferCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;

	_int m_iAlpha = 255;
	_float m_fScale = 1.f;

	_float fFrame = 0.f;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();
public:
	static	CTorch_Fire*			Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg) override;
	virtual void			Free() override;
};

END