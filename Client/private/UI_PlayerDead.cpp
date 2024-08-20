#include "stdafx.h"
#include "..\public\UI_PlayerDead.h"
#include "GameInstance.h"
#include "Player.h"

CUI_PlayerDead::CUI_PlayerDead(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CAlphaUI(pGraphic_Device)
{
}

CUI_PlayerDead::CUI_PlayerDead(const CUI_PlayerDead & rhs)
	: CAlphaUI(rhs)
{
}

HRESULT CUI_PlayerDead::NativeConstruct_Prototype()
{
	return __super::NativeConstruct_Prototype();
}

HRESULT CUI_PlayerDead::NativeConstruct(void * pArg)
{
	if (FAILED(SetUp_Components()))
		return E_FAIL;


	
	m_fX = g_iWinCX * 0.5f - 550.f;
	m_fY = g_iWinCY * 0.5f - 280.f;
	m_fSizeX = 128.f;
	m_fSizeY = 128.f;
	//	m_fSize = 0.f;
	m_iDepth = 6;
	// 
	//m_pTransformCom->Scaled(_float3(0, 0, 0.f));
	return __super::NativeConstruct(pArg);
}
// m_fsizeX랑 m_fSizeY로 스케일 값을 수정해야 한다아..? 

_int CUI_PlayerDead::Tick(_float fTimeDelta)
{

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	_int m_iPlayerState = static_cast<CPlayer*>(pGameInstance->Get_Front(LEVEL_STATIC, TEXT("Layer_Player")))->Get_State();
	RELEASE_INSTANCE(CGameInstance);


	if (m_iPlayerState != 8) {
		return -1;
	}
	else {			// 플레이어가 죽었을때 띄울 이미지
		m_fSizeX =128.f;
		m_fSizeY = 128.f;
		m_pTransformCom->Scaled(_float3(m_fSizeX, m_fSizeY, 0.f));
	}


	return __super::Tick(fTimeDelta);
	
}

_int CUI_PlayerDead::LateTick(_float fTimeDelta)
{

	if (0 > __super::LateTick(fTimeDelta))
	{
		MSGBOX("0 > __super::Tick in CAlphaUI::Tick");
		return -1;
	}

	return _int();
}

HRESULT CUI_PlayerDead::Render()
{
	if (true == g_bCameraDirecting || true == g_bColossusClear)
		return S_OK;

	m_pVIBufferCom->Base_Setting();

	return __super::Render();
}

HRESULT CUI_PlayerDead::SetUp_Components()
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

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_PlayerDead"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CAim::SetUp_Components(Texture)");
		return E_FAIL;
	}

	return S_OK;
}

CUI_PlayerDead * CUI_PlayerDead::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUI_PlayerDead*	pInstance = new CUI_PlayerDead(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create Aim in CAim::Create");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CUI_PlayerDead::Clone(void * pArg)
{
	CGameObject*	pInstance = new CUI_PlayerDead(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create Aim in CAim::Clone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_PlayerDead::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
