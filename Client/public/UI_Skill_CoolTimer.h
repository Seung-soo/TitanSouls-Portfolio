#pragma once
#include "AlphaUI.h"
#include "Client_Defines.h"

BEGIN(Client)
class CUI_Skill_CoolTimer final : public CAlphaUI
{
private:
	explicit CUI_Skill_CoolTimer(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CUI_Skill_CoolTimer(const CUI_Skill_CoolTimer& rhs);
	virtual ~CUI_Skill_CoolTimer() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float fTimeDelta) override;
	virtual _int	LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
private:
	virtual HRESULT SetUp_Components() override;
private:
	_float			m_fTimerScale = 0.3f;
	_bool			m_fTimerOn = false;
	_float			m_fTimer = 40.f;
	_float			m_fTestTimer;
	_float			m_fSize;
	_bool			m_bPlay = false;
	_bool			m_bAbleSkill = true;


	_bool			m_iPlayerState = 0;
//private:
	//HRESULT			SkillCool();
public:
	static CUI_Skill_CoolTimer*			Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg) override;
	virtual void			Free() override;
};
END