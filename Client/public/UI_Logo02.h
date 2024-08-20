#pragma once
#include "AlphaUI.h"
#include "Client_Defines.h"
BEGIN(Client)
// 도입부, FADE IN, FADE OUT 효과가 들어가야함 
// 이걸 레벨에서 해줘야할까 객체에다가 해줘야할까 
class CUI_Logo02 :
	public CAlphaUI
{
private:
	explicit CUI_Logo02(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CUI_Logo02(const CUI_Logo02& rhs);
	virtual ~CUI_Logo02() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
public:
	void			Set_Alpha(_int iAlpha) { m_iAlpha = iAlpha; }
	_bool			Get_EndLogo() { return m_bEnd; }
private:
	_int			m_iAlpha = 0;
private:
	virtual	HRESULT SetUp_Components()override;
	HRESULT			SetUp_RenderState();
private:
	DWORD alpha = 175;
	
	_bool	m_bEnd = false;
	_bool	m_bOff = false;
	_int	m_iLogoIdx = 0;
public:
	static CUI_Logo02*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	// CUI을(를) 통해 상속됨
	virtual CGameObject * Clone(void * pArg) override;
	virtual void		Free(void) override;
};

END