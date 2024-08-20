#pragma once
#include "UI.h"
#include "Client_Defines.h"
BEGIN(Client)
class CUI_NoText final : public CUI
{
private:
	explicit CUI_NoText(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CUI_NoText(const CUI_NoText& rhs);
	virtual ~CUI_NoText() = default;
public:
	virtual	HRESULT NativeConstruct_Prototype(void) override;
	virtual	HRESULT NativeConstruct(void* pArg)override;
	virtual _int	Tick(_float fTimeDelta) override;
	virtual	_int	LateTick(_float fTimeDelta) override;
	virtual HRESULT Render(void) override;
private:
	virtual HRESULT SetUp_Components() override;
	HRESULT			SetUp_RenderState();
private:
	HRESULT		Go_Sprites(void);
private:
	BOARD	m_eCurrentBoard;
public:
	static CUI_NoText*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*		Clone(void * pArg) override;
	virtual void				Free(void) override;
};
END