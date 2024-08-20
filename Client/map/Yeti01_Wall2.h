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
class CYeti01_Wall2 final : public CGameObject
{
public:
	explicit CYeti01_Wall2(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CYeti01_Wall2(const CYeti01_Wall2& rhs);
	virtual ~CYeti01_Wall2() = default;

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
	static CYeti01_Wall2* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END

