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
class CField01_DoorLight final : public CGameObject
{
public:
	explicit CField01_DoorLight(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CField01_DoorLight(const CField01_DoorLight& rhs);
	virtual ~CField01_DoorLight() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float fTimeDelta) override;
	virtual _int	LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
public:
	void	Set_Light(_bool bLight) { m_bLight = bLight; }
private:
	CTexture*			m_pTextureCom = nullptr;
	CVIBuffer_Rect*		m_pVIBufferCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;

	_bool				m_bLight = false;
	_float				m_fSizeX = 0.001f;
	_float				m_fSizeY = 10.f;
	_float				fSize_Frame = 0.f;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();
public:
	static	CField01_DoorLight*			Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg) override;
	virtual void			Free() override;
};

END