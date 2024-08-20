#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_IceBrain final : public CLevel
{
public:
	explicit CLevel_IceBrain(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_IceBrain() = default;
public:
	virtual HRESULT	NativeConstruct() override;
	virtual _int	Tick(_float fTimeDelta) override;
	virtual _int	LateTick(_float fTimeDelta) override;
	virtual HRESULT	Render() override;
private:
	ICEDEAD m_IceDeath;
	BUTTONINFO m_Button;
private:
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Terrain(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Wall(const _tchar* pLayerTag);
	HRESULT	Ready_Layer_Player(const _tchar* pLayerTag);
	HRESULT	Ready_Layer_IceBrain(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Sky(const _tchar* pLayerTag);
	HRESULT Ready_Layer_UI(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Door(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Accessories(const _tchar* pLayerTag);

	HRESULT Ready_Layer_Fire(const _tchar* pLayerTag);
	HRESULT Ready_Layer_FireButton(const _tchar* pLayerTag);


public:
	static	CLevel_IceBrain*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void			Free() override;

};

END