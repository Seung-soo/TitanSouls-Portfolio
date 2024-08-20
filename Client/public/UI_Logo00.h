#pragma once
#include "AlphaUI.h"
#include "Client_Defines.h"
BEGIN(Client)
// ���Ժ�, FADE IN, FADE OUT ȿ���� ������ 
// �̰� �������� ������ұ� ��ü���ٰ� ������ұ� 
class CUI_Logo00 :
	public CAlphaUI
{
private:
	explicit CUI_Logo00(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CUI_Logo00(const CUI_Logo00& rhs);
	virtual ~CUI_Logo00() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
public:
	void			Set_Alpha(_int iAlpha) { m_iAlpha = iAlpha; }
	_bool			Get_EndLogo() { return m_bEnd; }
	_bool			Get_Play() { return m_bPlay; }
private:
	_int			m_iAlpha = 0;
private:
	virtual	HRESULT SetUp_Components()override;
	HRESULT			SetUp_RenderState();
private:
	DWORD alpha = 175;
	
	_bool	m_bEnd = false;
	_bool	m_bOff = false;
	_bool	m_bPlay = true;
	_int	m_iLogoIdx = 0;
public:
	static CUI_Logo00*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	// CUI��(��) ���� ��ӵ�
	virtual CGameObject * Clone(void * pArg) override;
	virtual void		Free(void) override;
};

END