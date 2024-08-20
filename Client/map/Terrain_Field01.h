#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Terrain;

END

BEGIN(Client)

class CTerrain_Field01 final : public CGameObject
{
private:
	explicit CTerrain_Field01(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CTerrain_Field01(const CTerrain_Field01& rhs);
	virtual ~CTerrain_Field01() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CTexture*				m_pTextureCom = nullptr;
	CVIBuffer_Terrain*		m_pVIBufferCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
private:
	HRESULT SetUp_Components();
	HRESULT			SetUp_RenderState();
	HRESULT			Release_RenderState();
public:
	static CTerrain_Field01* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END