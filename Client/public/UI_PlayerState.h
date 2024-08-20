#pragma once
#include "AlphaUI.h"
#include "Client_Defines.h"

BEGIN(Client)
class CUI_PlayerState final : public CAlphaUI
{
private:
	explicit CUI_PlayerState(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CUI_PlayerState(const CUI_PlayerState& rhs);
	virtual ~CUI_PlayerState() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float fTimeDelta) override;
	virtual _int	LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
private:
	virtual HRESULT SetUp_Components() override;
public:
	static CUI_PlayerState*			Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*			Clone(void* pArg) override;
	virtual void					Free() override;
};
END