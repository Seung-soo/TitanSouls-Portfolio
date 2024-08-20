#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_Respawn2 final : public CLevel
{
public:
	explicit CLevel_Respawn2(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_Respawn2() = default;
public:
	virtual HRESULT	NativeConstruct() override; 
	virtual _int	Tick(_float fTimeDelta) override;
	virtual _int	LateTick(_float fTimeDelta) override;
	virtual HRESULT	Render() override;
private:
	HRESULT Ready_Layer_Terrain(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Wall(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Sky(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Accessories(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Door(const _tchar* pLayerTag);
	HRESULT Ready_Layer_OpenDoor(const _tchar * pLayerTag);
public:
	static	CLevel_Respawn2*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void			Free() override;
};

END