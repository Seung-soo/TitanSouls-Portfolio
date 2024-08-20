#pragma once

#include "Client_Defines.h"
#include "UI.h"
#include "AlphaUI.h"
BEGIN(Client)
class CUI_Skill_Shooting final : public CAlphaUI
{
private:
	explicit CUI_Skill_Shooting(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CUI_Skill_Shooting(const CUI_Skill_Shooting& rhs);
	virtual ~CUI_Skill_Shooting() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float fTimeDelta) override;
	virtual _int	LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
private:
	virtual HRESULT SetUp_Components() override;
	HRESULT			SetUp_RenderState();
private:
	_int			m_iAlpha = 50;
	_bool			m_bHaveArrow = true;
	_bool			m_bSkillOn = true;
	_int			m_iTestInterval = 200;
public:
	static CUI_Skill_Shooting*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg) override;
	virtual void			Free() override;
};
END