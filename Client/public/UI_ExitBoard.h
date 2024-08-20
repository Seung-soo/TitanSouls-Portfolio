#pragma once
#include "UI.h"
#include "Client_Defines.h"
BEGIN(Client)
class CUI_ExitBoard :
	public CUI
{
private:
	explicit CUI_ExitBoard(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CUI_ExitBoard(const CUI_ExitBoard& rhs);
	virtual ~CUI_ExitBoard() = default;
public:
	virtual	HRESULT	NativeConstruct_Prototype(void) override;
	virtual	HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float fTimeDelta) override;
	virtual _int	LateTick(_float fTimeDelta) override;
	virtual HRESULT	Render(void) override;
private:
	// CUI을(를) 통해 상속됨
	virtual HRESULT SetUp_Components() override;
private:
	BOARD	m_eCurrentBoard;
public:
	static	CUI_ExitBoard*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject *	Clone(void * pArg) override;
	virtual	void			Free(void) override;
};
END