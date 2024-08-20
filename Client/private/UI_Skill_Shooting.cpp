#include "stdafx.h"
#include "..\public\UI_Skill_Shooting.h"
#include "GameInstance.h"
#include "Player.h"

CUI_Skill_Shooting::CUI_Skill_Shooting(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CAlphaUI(pGraphic_Device)
{
}

CUI_Skill_Shooting::CUI_Skill_Shooting(const CUI_Skill_Shooting & rhs)
	: CAlphaUI(rhs)
{
}

HRESULT CUI_Skill_Shooting::NativeConstruct_Prototype()
{
	return __super::NativeConstruct_Prototype();
}

HRESULT CUI_Skill_Shooting::NativeConstruct(void * pArg)
{
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_fX = g_iWinCX * 0.5f + 400.f;
	m_fY = g_iWinCY * 0.5f - 280.f;
	m_fSizeX = 128.f;
	m_fSizeY = 128.f;
	m_iDepth = 7;
	//m_bHaveArrow = *(_bool*)pArg;
//	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	//m_bHaveArrow = pGameInstance;
	//static_cast<CPlayer*>(pGameInstance->Find_Layer(LEVEL_STATIC, TEXT("Layer_Player"))->Get_Object().front()->
//	m_bHaveArrow = static_cast<CPlayer*>(pGameInstance->Get_Front(LEVEL_STATIC, TEXT("Layer_Player")))->Get_HaveArrow();
	//CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	//m_bHaveArrow = pGameInstance;
	//static_cast<CPlayer*>(pGameInstance->Find_Layer(LEVEL_STATIC, TEXT("Layer_Player"))->Get_Object().front()->
	//m_bHaveArrow = static_cast<CPlayer*>(pGameInstance->Get_Front(LEVEL_STATIC, TEXT("Layer_Player")))->Get_HaveArrow();


	//RELEASE_INSTANCE(CGameInstance);



//	RELEASE_INSTANCE(CGameInstance);


		
	return __super::NativeConstruct(pArg);
}

_int CUI_Skill_Shooting::Tick(_float fTimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	//m_bHaveArrow = pGameInstance;
	//static_cast<CPlayer*>(pGameInstance->Find_Layer(LEVEL_STATIC, TEXT("Layer_Player"))->Get_Object().front()->
	m_bHaveArrow = static_cast<CPlayer*>(pGameInstance->Get_Front(LEVEL_STATIC, TEXT("Layer_Player")))->Get_HaveArrow();

	
	RELEASE_INSTANCE(CGameInstance);


	return __super::Tick(fTimeDelta);
}

_int CUI_Skill_Shooting::LateTick(_float fTimeDelta)
{
	
	if (true == m_bHaveArrow) {
		m_iAlpha = 255;
	}
	else {
		m_iAlpha = 50;
	}
	return __super::LateTick(fTimeDelta);
}

HRESULT CUI_Skill_Shooting::Render()
{
	m_pVIBufferCom->Base_Setting();
	if (true == g_bCameraDirecting || true == g_bColossusClear)
		return S_OK;
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);
	m_pGraphic_Device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));

	return __super::Render();
}

HRESULT CUI_Skill_Shooting::SetUp_Components()
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 0;
	TransformDesc.fRotationPerSec = D3DXToRadian(0.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CAim::SetUp_Components(Transform)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CAim::SetUp_Components(Renderer)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CAim::SetUp_Components(VIBuffer_Rect)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Skill00"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CAim::SetUp_Components(Texture)");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CUI_Skill_Shooting::SetUp_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);
	return S_OK;
}

CUI_Skill_Shooting * CUI_Skill_Shooting::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUI_Skill_Shooting*	pInstance = new CUI_Skill_Shooting(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create Aim in CAim::Create");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CUI_Skill_Shooting::Clone(void * pArg)
{
	CGameObject*	pInstance = new CUI_Skill_Shooting(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create Aim in CAim::Clone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Skill_Shooting::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
