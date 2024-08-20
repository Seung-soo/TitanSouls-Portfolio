#pragma once
#include "UI.h"
#include "Client_Defines.h"
BEGIN(Client)
class CUI_SelectIcon final : public CUI
{
	enum BOARD { MAIN_LOBY, MAIN_START, MAIN_OPTION, MAIN_QUIT, MAIN_END };
	enum MENUSTATE { MENU_DEFAULT, MENU_START, MENU_OPTION, MENU_EXIT, MENU_YES, MENU_NO, MENU_END };
private:
	explicit CUI_SelectIcon(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CUI_SelectIcon(const CUI_SelectIcon& rhs);
	virtual ~CUI_SelectIcon() = default;
public:
	virtual    HRESULT NativeConstruct_Prototype(void) override;
	virtual    HRESULT NativeConstruct(void* pArg)override;
	virtual _int    Tick(_float fTimeDelta) override;
	virtual    _int    LateTick(_float fTimeDelta) override;
	virtual HRESULT Render(void) override;
private:
	virtual HRESULT SetUp_Components() override;

	//HRESULT            SetUp_RenderState();
private:
	HRESULT        Go_Sprites(void);
private:
	// 이동용 함수 
	void        Move_Icon(BOARD eCurrentMenu);


	HRESULT            Ready_Layer_UI_Board(BOARD eCurrentMenu, _tchar* pLayerTag);


private:
	_int        m_iInputBtn = 0;
	_float        m_fInterval = 0.f;
	BOARD        m_eCurrentMenu, m_eNextMenu;
	MENUSTATE    m_eSelectBtn, m_eBtnState;

	_float        m_fTime;
	_float        m_fMenuInterval = 0.f;
	_bool        m_bBoardOn = false;
	_bool        m_bLobyOn = true;
	_bool        m_bQuitOn = false;
	_bool        m_bOption = false;
	_bool        m_bOtherOn = false;
	_bool        m_bStartOn = false;
	_tchar*        m_pLayerTag;
	_float        m_fKeyDelay = 10.f;

public:
	static CUI_SelectIcon*        Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*        Clone(void * pArg) override;
	virtual void                Free(void) override;
};
END