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

class CAll_Door final : public CGameObject
{
public:
	explicit CAll_Door(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CAll_Door(const CAll_Door& rhs);
	virtual ~CAll_Door() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float fTimeDelta) override;
	virtual _int	LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
public:
	void			Set_BoolCnt(_bool bCnt) { m_bCnt = bCnt; }
private:
	CTexture*			m_pTextureCom = nullptr;
	CVIBuffer_Rect*		m_pVIBufferCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;

	_float			m_fFrame = 0.f;
	_bool			m_bCnt = false;
	_bool			bSound = true;// »ç¿îµå

	_uint			m_iCnt = 0;
private:
	HRESULT SetUp_Components();
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

public:
	static	CAll_Door*			Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg) override;
	virtual void			Free() override;
};

END