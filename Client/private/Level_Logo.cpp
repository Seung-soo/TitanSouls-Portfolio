#include "stdafx.h"
#include "..\public\Level_Logo.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "UI_Logo.h"
#include "UI_Logo00.h"
#include "UI_Logo01.h"
#include "UI_Logo02.h"

CLevel_Logo::CLevel_Logo(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel(pGraphic_Device)
{
}

// �ΰ� ������ ������ �� �ҷ����� ȣ���ؾ��ϴ� �͵�
HRESULT CLevel_Logo::NativeConstruct()
{
	if (0 > __super::NativeConstruct())		// CLevel�� �ʱ�ȭ �Լ��� ȣ��
	{
		MSGBOX("CLevel::NativeConstruct returned the E_FAIL in CLevel_Logo::NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(Ready_Prototype_GameObject()))
	{
		MSGBOX("Ready_Prototype_GameObject returned the E_FAIL in CLevel_Logo::NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_UI(TEXT("Layer_UI"))))
	{
		MSGBOX("Ready_Layer_UI returned the E_FAIL in CLevel_Logo::NativeConstruct");
		return E_FAIL;
	}
	CGameInstance *	pGameInstance = GET_INSTANCE(CGameInstance);

	pGameInstance->PlayBGM(L"Opening.ogg", 0.8f);// ����
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

// �ΰ� �������� ��� ����������ϴ� �͵�
_int CLevel_Logo::Tick(_float fTimeDelta)
{
	if (0 > __super::Tick(fTimeDelta))		// CLevel�� ���� �Լ��� ȣ��
	{
		MSGBOX("0 > CLevel::Tick in CLevel_Logo::Tick");
		return -1;
	}

	CGameInstance *	pGameInstance = GET_INSTANCE(CGameInstance);
	_bool bLogoPlay = static_cast<CUI_Logo00*>(pGameInstance->Get_Front(g_eCurrentLevel, TEXT("Layer_UI00")))->Get_Play();


	if (false == bLogoPlay && 0 == m_iCntLogo) {
		++m_iCntLogo;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOGO, TEXT("Layer_UI01"), TEXT("Prototype_GameObject_UI_Logo01")))) {
			MSGBOX("Failed to Add Prototype UI_LOGO in CLevel_Logo::Ready_Prototype_GameObject");
			return E_FAIL;
		}
	}
	if (false == bLogoPlay && 1 == m_iCntLogo) {
		bLogoPlay = static_cast<CUI_Logo01*>(pGameInstance->Get_Front(g_eCurrentLevel, TEXT("Layer_UI01")))->Get_Play();
	}

	if (false == bLogoPlay && 1 == m_iCntLogo) {
		++m_iCntLogo;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOGO, TEXT("Layer_UI02"), TEXT("Prototype_GameObject_UI_Logo02")))) {
			MSGBOX("Failed to Add Prototype UI_LOGO in CLevel_Logo::Ready_Prototype_GameObject");
			return E_FAIL;
		}
	}
	if (false == bLogoPlay && 2 == m_iCntLogo) {
		if (true == static_cast<CUI_Logo02*>(pGameInstance->Get_Front(g_eCurrentLevel, TEXT("Layer_UI02")))->Get_EndLogo()) {
			CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
			g_eNextLevel = LEVEL_MAIN;
			if (FAILED(pGameInstance->OpenLevel(LEVEL_LOADING, CLevel_Loading::Create(m_pGraphic_Device, LEVEL_MAIN))))
				return E_FAIL;

			RELEASE_INSTANCE(CGameInstance);
		}

	}



	RELEASE_INSTANCE(CGameInstance);

	return 0;
}

_int CLevel_Logo::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))	// CLevel�� �ʰ� �����ϴ� �Լ��� ȣ��
	{
		MSGBOX("0 > CLevel::LateTick in CLevel_Logo::LateTick");
		return -1;
	}

	if (GetKeyState(VK_RETURN) & 0x8000)
	{
		CGameInstance*	pGameInstance = CGameInstance::GetInstance();
		Safe_AddRef(pGameInstance);

		if (FAILED(pGameInstance->OpenLevel(LEVEL_LOADING, CLevel_Loading::Create(m_pGraphic_Device, LEVEL_MAIN))))
			return E_FAIL;

		Safe_Release(pGameInstance);
	}

	return 0;
}

// �ΰ� �������� �׷����ϴ� �͵�
HRESULT CLevel_Logo::Render()
{
	if (0 > __super::Render())		// CLevel�� �׸��� �Լ��� ȣ��
	{
		MSGBOX("CLevel::Render returned the E_FAIL in CLevel_Logo::Render");
		return E_FAIL;
	}

	SetWindowText(g_hWnd, TEXT("�ΰ�����. "));


	return S_OK;
}

// �ΰ��� ����� ���� ������Ʈ ������ ����
HRESULT CLevel_Logo::Ready_Prototype_GameObject()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	// ���⿡
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Logo00"), CUI_Logo00::Create(m_pGraphic_Device))))
	{
		MSGBOX("Failed to Add Prototype UI_LOGO in CLevel_Logo::Ready_Prototype_GameObject");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Logo01"), CUI_Logo01::Create(m_pGraphic_Device))))
	{
		MSGBOX("Failed to Add Prototype UI_LOGO in CLevel_Logo::Ready_Prototype_GameObject");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Logo02"), CUI_Logo02::Create(m_pGraphic_Device))))
	{
		MSGBOX("Failed to Add Prototype UI_LOGO in CLevel_Logo::Ready_Prototype_GameObject");
		return E_FAIL;
	}


	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_UI(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	// ���⿡
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOGO, TEXT("Layer_back"), TEXT("Prototype_GameObject(Loading_Back)")))) {
		MSGBOX("m_pGameInstance->Add_Prototype returned E_FAIL in CLevel_Logo::Ready_Prototype_GameObject(Loading_Back)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOGO, TEXT("Layer_UI00"), TEXT("Prototype_GameObject_UI_Logo00")))) {
		MSGBOX("Failed to Add Prototype UI_LOGO in CLevel_Logo::Ready_Prototype_GameObject");
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

CLevel_Logo * CLevel_Logo::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_Logo*	pInstance = new CLevel_Logo(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("CLevel_Logo::NativeConstruct returned the E_FAIL in CLevel_Logo::Create");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Logo::Free()
{
	__super::Free();
}
