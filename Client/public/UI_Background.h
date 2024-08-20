#pragma once
#include "UI.h"
#include "Client_Defines.h"
BEGIN(Client)
class CUI_Background :
	public CUI
{
private:
	explicit CUI_Background(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CUI_Background(const CUI_Background& rhs);
	virtual ~CUI_Background() = default;
public:
	virtual	HRESULT	NativeConstruct_Prototype(void) override;
	virtual	HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float fTimeDelta) override;
	virtual _int	LateTick(_float fTimeDelta) override;
	virtual HRESULT	Render(void) override;

private:
	_int			m_iAlpha = 255;
private:
	// CUI을(를) 통해 상속됨
	virtual HRESULT SetUp_Components() override;
public:
	static	CUI_Background*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject *	Clone(void * pArg) override;
	virtual	void			Free(void) override;
};
END