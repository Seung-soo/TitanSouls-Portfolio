#pragma once
#include "Client_Defines.h"
#include "UI.h"
//#include "AlphaUI.h"
BEGIN(Client)
class CUI_LShiftIcon final : public CUI
{
private:
	explicit CUI_LShiftIcon(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CUI_LShiftIcon(const CUI_LShiftIcon& rhs);
	virtual ~CUI_LShiftIcon() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float fTimeDelta) override;
	virtual _int	LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
private:
	virtual HRESULT SetUp_Components() override;
public:
	static CUI_LShiftIcon*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg) override;
	virtual void			Free() override;
};
END