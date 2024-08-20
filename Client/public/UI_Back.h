#pragma once
#include "AlphaUI.h"
#include "Client_Defines.h"
BEGIN(Client)

class CUI_Back :
	public CAlphaUI
{
private:
	explicit CUI_Back(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CUI_Back(const CUI_Back& rhs);
	virtual ~CUI_Back() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
private:
	_int			m_iAlpha = 255;
	virtual	HRESULT SetUp_Components()override;
	HRESULT			SetUp_RenderState();
public:
	static CUI_Back*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	// CUI을(를) 통해 상속됨
	virtual CGameObject * Clone(void * pArg) override;
	virtual void		Free(void) override;
};
END