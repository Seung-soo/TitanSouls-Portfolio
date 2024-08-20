#pragma once
#include "Level.h"
#include "Client_Defines.h"
BEGIN(Client)
class CLevel_EndingCredit :
	public CLevel
{
public:
	explicit CLevel_EndingCredit(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_EndingCredit() = default;
public:
	virtual HRESULT	NativeConstruct() override;
	virtual _int	Tick(_float fTimeDelta) override;
	virtual _int	LateTick(_float fTimeDelta) override;
	virtual HRESULT	Render() override;
private:

	HRESULT			Ready_Layer_UI_Background(const _tchar* pLayerTag);

	HRESULT			Ready_Layer_UI_Credit(const _tchar* pLayerTag);



public:
	static	CLevel_EndingCredit*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void			Free() override;
};
END