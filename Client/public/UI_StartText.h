#pragma once

#include "UI.h"
#include "Client_Defines.h"
BEGIN(Client)
class CUI_StartText final : public CUI
{
private:
	explicit CUI_StartText(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CUI_StartText(const CUI_StartText& rhs);
	virtual ~CUI_StartText() = default;
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
public:
	static CUI_StartText*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*		Clone(void * pArg) override;
	virtual void				Free(void) override;
};
END