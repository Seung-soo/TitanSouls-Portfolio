#pragma once
#include "AlphaUI.h"
#include "Client_Defines.h"
BEGIN(Client)
// ���Ժ�, FADE IN, FADE OUT ȿ���� ������ 
// �̰� �������� ������ұ� ��ü���ٰ� ������ұ� 
class CUI_NameACIDNERVE :
	public CAlphaUI
{
private:
	explicit CUI_NameACIDNERVE(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CUI_NameACIDNERVE(const CUI_NameACIDNERVE& rhs);
	virtual ~CUI_NameACIDNERVE() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
private:
	_int			m_iAlpha = 255;
	virtual	HRESULT SetUp_Components()override;
private:
	HRESULT		Go_Sprites(void);
public:
	static CUI_NameACIDNERVE*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	// CUI��(��) ���� ��ӵ�
	virtual CGameObject * Clone(void * pArg) override;
	virtual void		Free(void) override;
};

END