#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CCollider_Rect;
END

BEGIN(Client)
class CVIBuffer_Wall;
class CRespawn01_Ceiling final : public CGameObject
{
private:
	explicit CRespawn01_Ceiling(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CRespawn01_Ceiling(const CRespawn01_Ceiling& rhs);
	virtual ~CRespawn01_Ceiling() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float fTimeDelta) override;
	virtual _int	LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
private:
	CTexture*				m_pTextureCom = nullptr;
	CVIBuffer_Wall*			m_pVIBufferCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CCollider_Rect*			m_pColliderCom = nullptr;
private:
	HRESULT SetUp_Components();
public:
	static CRespawn01_Ceiling* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
