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
class CField01_Grass_Ex final :public CGameObject
{
public:
	explicit CField01_Grass_Ex(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CField01_Grass_Ex(const CField01_Grass_Ex& rhs);
	virtual ~CField01_Grass_Ex() = default;

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
	_float			m_fFrame = 0.f;
private:
	HRESULT SetUp_Components();
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();
public:
	static	CField01_Grass_Ex*			Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg) override;
	virtual void			Free() override;
};

END