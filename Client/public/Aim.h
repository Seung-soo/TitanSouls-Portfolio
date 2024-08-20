#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CAim final : public CUI
{
private:
	explicit CAim(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CAim(const CAim& rhs);
	virtual ~CAim() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float fTimeDelta) override;
	virtual _int	LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
private:
	virtual HRESULT SetUp_Components() override;
public:
	static CAim*			Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg) override;
	virtual void			Free() override;
};

END