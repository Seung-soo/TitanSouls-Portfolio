#include "stdafx.h"
#include "..\public\UI_LobyBoard.h"
#include "GameInstance.h"

CUI_LobyBoard::CUI_LobyBoard(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUI(pGraphic_Device)
{
}

CUI_LobyBoard::CUI_LobyBoard(const CUI_LobyBoard & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_LobyBoard::NativeConstruct_Prototype(void)
{
	return __super::NativeConstruct_Prototype();
}

HRESULT CUI_LobyBoard::NativeConstruct(void * pArg)
{
	if (FAILED(SetUp_Components())) {
		return E_FAIL;
	}
	m_fX = g_iWinCX * 0.5f;
	m_fY = g_iWinCY * 0.5f;
	m_fSizeX = 512.f;
	m_fSizeY = 512.f;

//	m_eCurrentBoard = *(BOARD*)pArg;
	m_iDepth = 3;

	return __super::NativeConstruct(pArg);
}

_int CUI_LobyBoard::Tick(_float fTimeDelta)
{
	//if (m_eCurrentBoard != MAIN_LOBY)
	//	return -1;
	return __super::Tick(fTimeDelta);
}

_int CUI_LobyBoard::LateTick(_float fTimeDelta)
{
	return __super::LateTick(fTimeDelta);
}

HRESULT CUI_LobyBoard::Render(void)
{
	m_pVIBufferCom->Base_Setting();
	return __super::Render();
}
HRESULT CUI_LobyBoard::SetUp_Components()
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
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_LobyBoard"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
	{
		return E_FAIL;
	}
	return S_OK;
}

CUI_LobyBoard * CUI_LobyBoard::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUI_LobyBoard* pInstance = new CUI_LobyBoard(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct_Prototype())) {
		MSGBOX("Failed to Create Background");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CUI_LobyBoard::Clone(void * pArg)
{
	CGameObject*	pInstance = new CUI_LobyBoard(*this);
	if (FAILED(pInstance->NativeConstruct(pArg))) {
		MSGBOX("Failed to Create UI Background");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_LobyBoard::Free(void)
{
	__super::Free();
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
