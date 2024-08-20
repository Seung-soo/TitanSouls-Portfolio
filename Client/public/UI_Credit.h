#pragma once
#include "UI.h"
#include "Client_Defines.h"
BEGIN(Client)
class CUI_Credit :
	public CUI
{
private:
	explicit CUI_Credit(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CUI_Credit(const CUI_Credit& rhs);
	virtual ~CUI_Credit() = default;
public:
	virtual	HRESULT	NativeConstruct_Prototype(void) override;
	virtual	HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float fTimeDelta) override;
	virtual _int	LateTick(_float fTimeDelta) override;
	virtual HRESULT	Render(void) override;
private:
	// CUI��(��) ���� ��ӵ�
	virtual HRESULT SetUp_Components() override;
public:
	static	CUI_Credit*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject *	Clone(void * pArg) override;
	virtual	void			Free(void) override;
};
END