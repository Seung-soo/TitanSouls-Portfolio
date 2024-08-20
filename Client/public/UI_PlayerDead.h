#pragma once
#include "AlphaUI.h"
#include "Client_Defines.h"

BEGIN(Client)
class CUI_PlayerDead final : public CAlphaUI
{
private:
	explicit CUI_PlayerDead(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CUI_PlayerDead(const CUI_PlayerDead& rhs);
	virtual ~CUI_PlayerDead() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float fTimeDelta) override;
	virtual _int	LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
private:
	virtual HRESULT SetUp_Components() override;
private:
	
	_bool			m_bPlayerDead =false;
	//private:
	//HRESULT			SkillCool();
public:
	static CUI_PlayerDead*			Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg) override;
	virtual void			Free() override;
};
END