#include "stdafx.h"
#include "..\public\UI_Credit.h"
#include "GameInstance.h"

CUI_Credit::CUI_Credit(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUI(pGraphic_Device)
{
}

CUI_Credit::CUI_Credit(const CUI_Credit & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Credit::NativeConstruct_Prototype(void)
{
	return __super::NativeConstruct_Prototype();
}

HRESULT CUI_Credit::NativeConstruct(void * pArg)
{
	if (FAILED(SetUp_Components())) {
		return E_FAIL;
	}
	m_fX = g_iWinCX * 0.5f-150.f;
	m_fY = g_iWinCY * 0.5f + 1200.f;
	m_fSizeX = 2048.f;
	m_fSizeY = 2048.f;

	m_iDepth = 3;

	return __super::NativeConstruct(pArg);
}

_int CUI_Credit::Tick(_float fTimeDelta)
{
	
	m_fY--;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_fX - g_iWinCX * 0.5f, -m_fY + g_iWinCY * 0.5f, 0.f));
	return __super::Tick(fTimeDelta);
	//m_fX = g_iWinCX * 0.5f - 150.f;
	//m_fY++;
}

_int CUI_Credit::LateTick(_float fTimeDelta)
{
	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_fX, m_fY, 0.f));
	return __super::LateTick(fTimeDelta);
}

HRESULT CUI_Credit::Render(void)
{
	m_pVIBufferCom->Base_Setting();
	return __super::Render();
}
HRESULT CUI_Credit::SetUp_Components()
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
	if (FAILED(__super::Add_Component(g_eCurrentLevel, TEXT("Prototype_Component_Texture_EndingCredit"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
	{
		return E_FAIL;
	}
	return S_OK;
}

CUI_Credit * CUI_Credit::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUI_Credit* pInstance = new CUI_Credit(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct_Prototype())) {
		MSGBOX("Failed to Create Background");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CUI_Credit::Clone(void * pArg)
{
	CGameObject*	pInstance = new CUI_Credit(*this);
	if (FAILED(pInstance->NativeConstruct(pArg))) {
		MSGBOX("Failed to Create UI Background");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_Credit::Free(void)
{
	__super::Free();
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
