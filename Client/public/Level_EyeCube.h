#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_EyeCube final : public CLevel
{
public:
	explicit CLevel_EyeCube(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_EyeCube() = default;
public:
	virtual HRESULT	NativeConstruct() override;
	virtual _int	Tick(_float fTimeDelta) override;
	virtual _int	LateTick(_float fTimeDelta) override;
	virtual HRESULT	Render() override;
private:
	//COLOSSUSINFO m_ColoInfo;
	_bool m_bDead = false;
private:
	HRESULT Ready_Layer_Terrain(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Wall(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Sky(const _tchar* pLayerTag);
	HRESULT	Ready_Layer_EyeCube(const _tchar* pLayerTag);
	HRESULT	Ready_Layer_UI(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Accessories(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Door(const _tchar* pLayerTag);
public:
	static	CLevel_EyeCube*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void			Free() override;

};

END
