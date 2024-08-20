#pragma once

#include "Client_Defines.h"
#include "AlphaUI.h"

BEGIN(Client)

class CDarkScreen final : public CAlphaUI
{
private:
	explicit CDarkScreen(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CDarkScreen(const CDarkScreen& rhs);
	virtual ~CDarkScreen() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float fTimeDelta) override;
	virtual _int	LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
public:
	void			Set_Alpha(_int iAlpha) { m_iAlpha = iAlpha; }
	void			Set_SoulEat(_bool bSoulEat) { m_bSoulEat = bSoulEat; }
private:
	_int			m_iAlpha = 150;
	_bool			m_bSoulEat = false;
protected:
	virtual HRESULT SetUp_Components();
public:
	static CDarkScreen*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg) override;
	virtual void			Free() override;
};

END