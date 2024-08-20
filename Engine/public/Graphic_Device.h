#pragma once

#include "Base.h"

BEGIN(Engine)

class CGraphic_Device final : public CBase
{
	DECLARE_SINGLETON(CGraphic_Device)
public:
	enum SCREENMODE { TYPE_FULLMODE, TYPE_WINMODE, TYPE_END };
public:
	typedef struct tagGraphicDesc
	{
		SCREENMODE		eScreenMode;
		HWND			hWnd;
		_uint			iWinCX, iWinCY;
	}GRAPHICDESC;
public:
	CGraphic_Device();
	virtual ~CGraphic_Device() = default;
public:
	LPDIRECT3DDEVICE9	Get_Device() { return m_pDevice; }
	LPD3DXSPRITE		Get_Sprite() { return m_pSprite; }
	LPD3DXFONT			Get_Font() { return m_pFont; }
public:
	HRESULT				InitDevice(const GRAPHICDESC& GraphicDesc, LPDIRECT3DDEVICE9* ppOut);
	void				SetParameters(D3DPRESENT_PARAMETERS& d3dpp, const GRAPHICDESC& GraphicDesc);

	void				Render_Begin();
	void				Render_End();
private:
	LPDIRECT3D9			m_p3D;
	LPDIRECT3DDEVICE9	m_pDevice;

	LPD3DXSPRITE		m_pSprite;
	LPD3DXFONT			m_pFont;
public:
	virtual void	Free() override;
};

END