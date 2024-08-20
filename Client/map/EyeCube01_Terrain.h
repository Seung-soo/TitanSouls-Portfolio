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
class CEyeCube01_Terrain final : public CGameObject
{
public:
	explicit CEyeCube01_Terrain(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CEyeCube01_Terrain(const CEyeCube01_Terrain& rhs);
	virtual ~CEyeCube01_Terrain() = default;

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
	HRESULT			SetUp_Components();
	HRESULT			SetUp_RenderState();
	HRESULT			Release_RenderState();
public:
	static CEyeCube01_Terrain* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};
END

