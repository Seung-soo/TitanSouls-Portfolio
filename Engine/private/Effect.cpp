#include "..\public\Effect.h"
#include "GameInstance.h"

CEffect::CEffect(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CEffect::CEffect(const CEffect & rhs)
	: CGameObject(rhs)
{
}

HRESULT CEffect::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		MSGBOX("__super::NativeConstruct_Prototype returned E_FAIL in CEffect::NativeConstruct_Prototype");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CEffect::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		MSGBOX("__super::NativeConstruct returned E_FAIL in CEffect::NativeConstruct");
		return E_FAIL;
	}

	return S_OK;
}

_int CEffect::Tick(_float fTimeDelta)
{
	if (0 > __super::Tick(fTimeDelta))
	{
		MSGBOX("0 > __super::Tick in CEffect::Tick");
		return -1;
	}

	return _int();
}

_int CEffect::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
	{
		MSGBOX("0 > __super::LateTick in CEffect::LateTick");
		return -1;
	}


	return _int();
}

HRESULT CEffect::Render()
{
	return S_OK;
}

HRESULT CEffect::Add_Component(_uint iLevelIndex, const _tchar * pPrototypeTag, const _tchar * pComponentTag, CComponent ** pOut, void * pArg)
{
	if (FAILED(__super::Add_Component(iLevelIndex, pPrototypeTag, pComponentTag, pOut, pArg)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CEffect::Add_Component");
		return E_FAIL;
	}
	return S_OK;
}


void CEffect::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
