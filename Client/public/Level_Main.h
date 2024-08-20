#pragma once
#include "Level.h"
#include "Client_Defines.h"
BEGIN(Client)
class CLevel_Main :
	public CLevel
{
public:
	explicit CLevel_Main(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_Main() = default;
public:
	virtual HRESULT	NativeConstruct() override;
	virtual _int	Tick(_float fTimeDelta) override;
	virtual _int	LateTick(_float fTimeDelta) override;
	virtual HRESULT	Render() override;
private:
	HRESULT			Select_MenuBoard(void);
	HRESULT			Ready_Layer_UI_Background(const _tchar* pLayerTag);
	HRESULT			Ready_Layer_UI_Loby(const _tchar* pLayerTag);

	HRESULT			Ready_Layer_UI_Option(const _tchar* pLayerTag);
	HRESULT			Ready_Layer_UI_Exit(const _tchar* pLayerTag);
	//////field에 넣어줄 레이어 시험용으로 메인에 깔아둔당 
	HRESULT			Ready_Layer_UI_Pause(const _tchar* pLayerTag);
	HRESULT			SetUpBoard(BOARD eCurrentMenu, _tchar* pEraseBoardTag);
	void		Move_Icon(BOARD eCurrentMenu);
private:

	BOARD		m_eCurrentMenu = MAIN_LOBY;
	MENUSTATE	m_eSelectBtn, m_eBtnState;
	_int		m_iInputKey = 0;
	_bool		m_bInputStart = false;

	_int		m_iInputBtn = 2;
	_float		m_fInterval = 0.f;
public:
	static	CLevel_Main*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void			Free() override;
};
END