#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_Logo final : public CLevel
{
public:
	explicit CLevel_Logo(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_Logo() = default;
public:
	virtual HRESULT	NativeConstruct() override;
	virtual _int	Tick(_float fTimeDelta) override;
	virtual _int	LateTick(_float fTimeDelta) override;
	virtual HRESULT	Render() override;
private:
	HRESULT			Ready_Prototype_GameObject();
	HRESULT			Ready_Layer_UI(const _tchar* pLayerTag);
private:
	_bool			m_bLogosEnd = false;
	_bool			bLogo2End = false;
	_int			m_iCntLogo = 0;
public:
	static	CLevel_Logo*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void			Free() override;

};

END