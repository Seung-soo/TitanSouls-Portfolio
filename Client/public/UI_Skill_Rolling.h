#pragma once
#include "AlphaUI.h"
#include "Client_Defines.h"

BEGIN(Client)
class CUI_Skill_Rolling final : public CAlphaUI
{
private:
	explicit CUI_Skill_Rolling(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CUI_Skill_Rolling(const CUI_Skill_Rolling& rhs);
	virtual ~CUI_Skill_Rolling() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float fTimeDelta) override;
	virtual _int	LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
private:
	virtual HRESULT SetUp_Components() override;
public:
	static CUI_Skill_Rolling*			Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*			Clone(void* pArg) override;
	virtual void					Free() override;
};
END