#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CColossus01_Dark01 final : public CGameObject
{
private:
	explicit CColossus01_Dark01(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CColossus01_Dark01(const CColossus01_Dark01& rhs);
	virtual ~CColossus01_Dark01() = default;

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
private:
	HRESULT SetUp_Components();
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();
public:
	static	CColossus01_Dark01*			Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg) override;
	virtual void			Free() override;
};

END