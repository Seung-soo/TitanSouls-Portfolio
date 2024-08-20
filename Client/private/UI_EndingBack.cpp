#include "stdafx.h"
#include "..\public\UI_EndingBack.h"
#include "GameInstance.h"

CUI_EndingBack::CUI_EndingBack(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUI(pGraphic_Device)
{
}

CUI_EndingBack::CUI_EndingBack(const CUI_EndingBack & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_EndingBack::NativeConstruct_Prototype(void)
{
	return __super::NativeConstruct_Prototype();
}

HRESULT CUI_EndingBack::NativeConstruct(void * pArg)
{
	if (FAILED(SetUp_Components())) {
		return E_FAIL;
	}
	m_fX = g_iWinCX * 0.5f;
	m_fY = g_iWinCY * 0.5f;
	m_fSizeX = 2048.f;
	m_fSizeY = 2048.f;

	m_iDepth = 4;


	return __super::NativeConstruct(pArg);
}

_int CUI_EndingBack::Tick(_float fTimeDelta)
{
	return __super::Tick(fTimeDelta);
}

_int CUI_EndingBack::LateTick(_float fTimeDelta)
{
	return __super::LateTick(fTimeDelta);
}

HRESULT CUI_EndingBack::Render(void)
{
	//m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	//m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
	//m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);
	//m_pGraphic_Device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(m_iAlpha, 255,255,255));
	//m_iAlpha--;
	m_pVIBufferCom->Base_Setting();
	return __super::Render();
}
HRESULT CUI_EndingBack::SetUp_Components()
{
	CTransform::TRANSFORMDESC	TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 0;
	TransformDesc.fRotationPerSec = D3DXToRadian(0.f);
	// add clone component 

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	//사본 텍스쳐 컴포넌트
	if (FAILED(__super::Add_Component(g_eCurrentLevel, TEXT("Prototype_Component_Texture_EndingBack"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
	{
		return E_FAIL;
	}
	return S_OK;
}

CUI_EndingBack * CUI_EndingBack::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUI_EndingBack* pInstance = new CUI_EndingBack(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct_Prototype())) {
		MSGBOX("Failed to Create CUI_EndingBack");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CUI_EndingBack::Clone(void * pArg)
{
	CGameObject*	pInstance = new CUI_EndingBack(*this);
	if (FAILED(pInstance->NativeConstruct(pArg))) {
		MSGBOX("Failed to Create CUI_EndingBack");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_EndingBack::Free(void)
{
	__super::Free();
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
