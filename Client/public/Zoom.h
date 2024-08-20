#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CZoom final : public CUI
{
private:
	explicit CZoom(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CZoom(const CZoom& rhs);
	virtual ~CZoom() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float fTimeDelta) override;
	virtual _int	LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
private:

//	HRESULT			Go_Sprites();

	_float			m_fFrame = 0.f;
	_bool			m_bCnt = false;
	_int			m_iFrameP = 0;

	//SCENECHANGE		m_tSceneChange;
private:
	virtual HRESULT SetUp_Components() override;
public:
	static CZoom*			Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg) override;
	virtual void			Free() override;
};

END