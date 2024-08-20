#include "stdafx.h"
#include "..\public\UI_LoadingBack.h"
#include "GameInstance.h"
CUI_LoadingBack::CUI_LoadingBack(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUI(pGraphic_Device)
{
}

CUI_LoadingBack::CUI_LoadingBack(const CUI_LoadingBack & rhs)
	:CUI(rhs)
{
}

HRESULT CUI_LoadingBack::NativeConstruct_Prototype(void)
{
	return __super::NativeConstruct_Prototype();
}

HRESULT CUI_LoadingBack::NativeConstruct(void * pArg)
{
	if (FAILED(SetUp_Components()))
		return E_FAIL;


	m_fX = g_iWinCX * 0.5f;
	m_fY = g_iWinCY * 0.5f;
	m_fSizeX = 2048.f;
	m_fSizeY = 2048.f;
	m_iDepth = 3;

	return __super::NativeConstruct(pArg);
}

_int CUI_LoadingBack::Tick(_float fTimeDelta)
{
	return __super::Tick(fTimeDelta);
}

_int CUI_LoadingBack::LateTick(_float fTimeDelta)
{
	return __super::LateTick(fTimeDelta);
}

HRESULT CUI_LoadingBack::Render(void)
{
	//m_pVIBufferCom->Base_Setting();
	return __super::Render();
}

HRESULT CUI_LoadingBack::SetUp_Components()
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 0;
	TransformDesc.fRotationPerSec = D3DXToRadian(0.0f);
	//사본 컴포넌트 추가, 
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Texture */
	//사본 텍스쳐 컴포넌트
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_forLoadingBack"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
	{
		return E_FAIL;
	}
	return S_OK;
}

CUI_LoadingBack * CUI_LoadingBack::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUI_LoadingBack*	pInstance = new CUI_LoadingBack(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create Logo in CUI_LoadingBack::Create");
		Safe_Release(pInstance);
	}

	return pInstance;

}

CGameObject * CUI_LoadingBack::Clone(void * pArg)
{
	CGameObject*	pInstance = new CUI_LoadingBack(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create Logo in CUI_LoadingBack::Clone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_LoadingBack::Free(void)
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
