#pragma once
#include "AlphaUI.h"
#include "Client_Defines.h"
BEGIN(Client)
// 도입부, FADE IN, FADE OUT 효과가 들어가야함 
// 이걸 레벨에서 해줘야할까 객체에다가 해줘야할까 
class CUI_NameICEBRAIN :
	public CAlphaUI
{
private:
	explicit CUI_NameICEBRAIN(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CUI_NameICEBRAIN(const CUI_NameICEBRAIN& rhs);
	virtual ~CUI_NameICEBRAIN() = default;
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
	static CUI_NameICEBRAIN*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	// CUI을(를) 통해 상속됨
	virtual CGameObject * Clone(void * pArg) override;
	virtual void		Free(void) override;
};

END