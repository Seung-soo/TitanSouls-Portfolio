#include "stdafx.h"
#include "..\public\UI_Back.h"
#include "GameInstance.h"

CUI_Back::CUI_Back(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CAlphaUI(pGraphic_Device)
{
}

CUI_Back::CUI_Back(const CUI_Back & rhs)
	: CAlphaUI(rhs)
{
}

HRESULT CUI_Back::NativeConstruct_Prototype()
{
	return __super::NativeConstruct_Prototype();
}

HRESULT CUI_Back::NativeConstruct(void * pArg)
{
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_fX = g_iWinCX * 0.5f;
	m_fY = g_iWinCY * 0.5f+310.f;
	m_fSizeX = 1280.f;
	m_fSizeY = 80.f;

	m_iDepth = 2;


	return __super::NativeConstruct(pArg);
}

_int CUI_Back::Tick(_float fTimeDelta)
{
	m_iAlpha -= 3;
	if (m_iAlpha <= 0)
		return -1;
	
	return __super::Tick(fTimeDelta);
}

_int CUI_Back::LateTick(_float fTimeDelta)
{
	
	return __super::LateTick(fTimeDelta);
}

HRESULT CUI_Back::Render()
{
	m_pVIBufferCom->Base_Setting();
	/**/
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);
	m_pGraphic_Device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));

	return __super::Render();
}
HRESULT CUI_Back::SetUp_Components()
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
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UIBack"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CUI_Back::SetUp_RenderState()
{

	/**/
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);
	return S_OK;
}


CUI_Back * CUI_Back::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUI_Back*	pInstance = new CUI_Back(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create Back in CUI_Back::Create");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CUI_Back::Clone(void * pArg)
{
	CGameObject*	pInstance = new CUI_Back(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create Back in CUI_Back::Clone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Back::Free(void)
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
