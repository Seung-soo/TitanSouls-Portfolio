
#pragma once

#include "Client_Defines.h"
#include "Effect.h"
#include "LightLeak.h"

BEGIN(Engine)
class CGameInstance;
class CTransform;
END

BEGIN(Client)
class CCamera_Player;
class CEffect_LightLeak final : public CEffect
{
private:
	explicit CEffect_LightLeak(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CEffect_LightLeak(const CEffect_LightLeak& rhs);
	virtual ~CEffect_LightLeak() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float fTimeDelta) override;
	virtual _int	LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
public:
	_uint			Get_Dir() { return m_iDir; }
private:
	CTransform*		m_pTransformCom = nullptr;
private:
	_int			m_iDelay = 30;
	_int			m_iMakeLightTime = 30;
	CCamera_Player* m_pCameraPlayer = nullptr;
	_uint			m_iDir = 0;
	_uint			m_iAlpha = 0;
	_bool			m_bDown = false;
	_bool			m_bEndStart = true;
private:
	HRESULT			SetUp_Components();
public:
	static CEffect_LightLeak*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*			Clone(void* pArg) override;
	virtual void					Free() override;
};

END