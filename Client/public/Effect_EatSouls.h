#pragma once

#include "Client_Defines.h"
#include "Effect.h"
#include "Soul.h"

BEGIN(Engine)
class CGameInstance;
class CTransform;
END

BEGIN(Client)
class CPlayer;

class CEffect_EatSouls final : public CEffect
{
private:
	explicit CEffect_EatSouls(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CEffect_EatSouls(const CEffect_EatSouls& rhs);
	virtual ~CEffect_EatSouls() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float fTimeDelta) override;
	virtual _int	LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
private:
	CTransform*		m_pTransformCom = nullptr;
private:
	CPlayer*		m_pPlayer = nullptr;
	_float3			m_vRight;
	_float3			m_vLook;
	_float3			m_vAxis[8];
	_float3			m_vCenterPoint;

	_uint			m_iMakeTime = 8;
	_int			m_iDelay = 0;
private:
	HRESULT			SetUp_Components();
public:
	static CEffect_EatSouls*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*			Clone(void* pArg) override;
	virtual void					Free() override;
};

END