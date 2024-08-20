#include "stdafx.h"
#include "..\public\DarkScreen.h"
#include "GameInstance.h"

CDarkScreen::CDarkScreen(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CAlphaUI(pGraphic_Device)
{
}

CDarkScreen::CDarkScreen(const CDarkScreen & rhs)
	: CAlphaUI(rhs)
{
}

HRESULT CDarkScreen::NativeConstruct_Prototype()
{
	return __super::NativeConstruct_Prototype();

}

HRESULT CDarkScreen::NativeConstruct(void * pArg)
{
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_fX = g_iWinCX * 0.5f;
	m_fY = g_iWinCY * 0.5f;
	m_fSizeX = g_iWinCX;
	m_fSizeY = g_iWinCY;

	m_iDepth = 9;
	return __super::NativeConstruct(pArg);
}

_int CDarkScreen::Tick(_float fTimeDelta)
{
	if (true == m_bSoulEat)
		--m_iAlpha;
	if (m_iAlpha < 0)
		return -1;
	return __super::Tick(fTimeDelta);
}

_int CDarkScreen::LateTick(_float fTimeDelta)
{
	return __super::LateTick(fTimeDelta);
}

HRESULT CDarkScreen::Render()
{
	m_pVIBufferCom->Base_Setting();

	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);
	if (m_iAlpha > 255)
		m_pGraphic_Device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, 255, 255, 255));
	else
		m_pGraphic_Device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));
	return __super::Render();
}

HRESULT CDarkScreen::SetUp_Components()
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 0;
	TransformDesc.fRotationPerSec = D3DXToRadian(0.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CDarkScreen::SetUp_Components(Transform)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CDarkScreen::SetUp_Components(Renderer)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CDarkScreen::SetUp_Components(VIBuffer_Rect)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_forLoadingBack"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CDarkScreen::SetUp_Components(Texture)");
		return E_FAIL;
	}

	return S_OK;
}

CDarkScreen * CDarkScreen::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CDarkScreen*	pInstance = new CDarkScreen(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create CDarkScreen in CDarkScreen::Create");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CDarkScreen::Clone(void * pArg)
{
	CGameObject*	pInstance = new CDarkScreen(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create CDarkScreen in CDarkScreen::Clone");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CDarkScreen::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTextureCom);
}
