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

class CScene_Loading final : public CUI
{
private:
	explicit CScene_Loading(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CScene_Loading(const CScene_Loading& rhs);
	virtual ~CScene_Loading() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float fTimeDelta) override;
	virtual _int	LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
private:

	_float			m_fFrame = 0.f;
	_bool			m_bCnt = false;
	SCENECHANGE		m_tSceneChange;
private:
	virtual HRESULT SetUp_Components() override;
public:
	static CScene_Loading*			Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg) override;
	virtual void			Free() override;
};

END