#pragma once
#include "Client_Defines.h"
#include "UI.h"
//#include "AlphaUI.h"
BEGIN(Client)
class CUI_LBtnIcon final : public CUI
{
private:
	explicit CUI_LBtnIcon(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CUI_LBtnIcon(const CUI_LBtnIcon& rhs);
	virtual ~CUI_LBtnIcon() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float fTimeDelta) override;
	virtual _int	LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
private:
	virtual HRESULT SetUp_Components() override;
public:
	static CUI_LBtnIcon*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg) override;
	virtual void			Free() override;
};
END