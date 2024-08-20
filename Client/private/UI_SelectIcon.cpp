#include "stdafx.h"
#include "..\public\UI_SelectIcon.h"
#include "GameInstance.h"
#include "Level_Loading.h"
CUI_SelectIcon::CUI_SelectIcon(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUI(pGraphic_Device)
{
}

CUI_SelectIcon::CUI_SelectIcon(const CUI_SelectIcon & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_SelectIcon::NativeConstruct_Prototype()
{
	return __super::NativeConstruct_Prototype();
}

HRESULT CUI_SelectIcon::NativeConstruct(void * pArg)
{
	if (FAILED(SetUp_Components()))
		return E_FAIL;
	//m_IInputBtnCnt = *(_int*)pArg;
	m_tFrame.iFrameStartX = 2.f;
	m_tFrame.iFrameEndX = 2.f;
	m_tFrame.iFrameCurrentX = 2.f;
	m_tFrame.iFrameStartY = 0.f;

	m_tFrame.iFrameHeight = 128.f;
	m_tFrame.iFrameWidth = 128.f;
	m_tFrame.iTexHeight = 512.f;
	m_tFrame.iTexWidth = 512.f;

	m_fX = g_iWinCX * 0.5f;
	m_fY = g_iWinCY * 0.5f;
	m_fSizeX = 256.f;
	m_fSizeY = 128.f;

	m_iDepth = 0;

	m_pLayerTag = TEXT("Layer_Loby");
	//m_eCurrentMenu = *(BOARD*)pArg;
	m_eCurrentMenu = MAIN_LOBY;
	m_eNextMenu = MAIN_LOBY;
	m_bStartOn = *(_bool*)pArg;
	Ready_Layer_UI_Board(m_eCurrentMenu, TEXT("Layer_Loby"));
	return __super::NativeConstruct(pArg);
}

_int CUI_SelectIcon::Tick(_float fTimeDelta)
{

	m_fSpriteSpeed = 1.f;
	Go_Frame(m_fSpriteSpeed, fTimeDelta);
	if (0 > --m_fKeyDelay) {
		m_fKeyDelay = 0.f;
	}


	Move_Icon(m_eCurrentMenu);

	if (GetKeyState(VK_RETURN) & 0x8000)
	{
		if (!m_fKeyDelay) {
			m_fKeyDelay = 10.f;
			if (MAIN_LOBY == m_eCurrentMenu) {
				if (m_bBoardOn == false) {
					if (MENU_EXIT == m_eBtnState) {
						m_eNextMenu = MAIN_QUIT;

						m_fInterval = 0.f;
						Ready_Layer_UI_Board(m_eNextMenu, TEXT("Layer_Loby"));
						m_bBoardOn = true;

					}
				}
				if (MENU_START == m_eBtnState) {
					g_bStartgame = true;

				}
			}
			if (MAIN_QUIT == m_eCurrentMenu) {
				if (MENU_YES == m_eBtnState) {
					DestroyWindow(g_hWnd);
				}
				if (m_bOtherOn == false) {
					if (MENU_NO == m_eBtnState) {
						m_eNextMenu = MAIN_LOBY;
						m_fInterval = 0.f;
						Ready_Layer_UI_Board(m_eNextMenu, TEXT("Layer_Exit"));
						m_bOtherOn = true;

					}
				}
			}
		}

	}
	//m_bBoardOn = false;
	/*

	if (GetKeyState(VK_ESCAPE) & 0x8000) {
	if (m_eCurrentMenu == MAIN_LOBY) {
	m_eNextMenu = MAIN_QUIT;
	Ready_Layer_UI_Board(m_eNextMenu, TEXT("Layer_Loby"));
	}
	if (m_eCurrentMenu == MAIN_QUIT) {
	m_eNextMenu = MAIN_LOBY;
	Ready_Layer_UI_Board(m_eNextMenu, TEXT("Layer_Exit"));
	}
	}
	*/

	return __super::Tick(fTimeDelta);
}

_int CUI_SelectIcon::LateTick(_float fTimeDelta)
{
	//Move_Icon(m_eCurrentMenu); 
	//0xfffffffffffffffffffffffff

	return __super::LateTick(fTimeDelta);
}

HRESULT CUI_SelectIcon::Render()
{

	if (FAILED(Go_Sprites())) {
		MSGBOX("Failed to call Go_Sprites in CEffect_CatchArrow");
		return E_FAIL;
	}
	return __super::Render();
}
HRESULT CUI_SelectIcon::SetUp_Components()
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
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_forMainMenuSelect"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
	{
		return E_FAIL;
	}
	return S_OK;
}


HRESULT CUI_SelectIcon::Go_Sprites(void)
{
	if (FAILED(m_pVIBufferCom->Go_Sprites(m_tFrame))) {
		MSGBOX("m_pVIBufferCom->Go_Sprites(m_tFrame) in CUI_SelectIcon");
		return E_FAIL;
	}
	return S_OK;
}

void CUI_SelectIcon::Move_Icon(BOARD eCurrentMenu)
{
	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);
	if (GetAsyncKeyState('W') & 1) {
		++m_iInputBtn;
	}
	if (GetAsyncKeyState('S') & 1) {
		--m_iInputBtn;
	}

	if (m_iInputBtn % 2 == 0) {

		m_fInterval = 0.f;
		//m_eBtnState = MENU_START;
	}
	else {
		m_fInterval = 50.f;
		//m_eBtnState = MENU_EXIT;
	}
	m_fY = g_iWinCY * 0.5f + m_fInterval;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_fX - g_iWinCX * 0.5f, -m_fY + g_iWinCY * 0.5f, 0.f));




	if (MAIN_LOBY == eCurrentMenu) {
		switch ((int)m_fInterval)
		{
		case 50:
			//m_fInterval = 50.f;
			m_eBtnState = MENU_EXIT;
			break;

		case 0:
			//m_fInterval = 0.f;

			m_eBtnState = MENU_START;
			break;
		}
	}
	if (MAIN_QUIT == eCurrentMenu) {
		switch ((int)m_fInterval)
		{
		case 50:
			//	m_fInterval = 50.f;
			m_eBtnState = MENU_NO;
			break;
		case 0:
			//m_fInterval = 0.f;
			m_eBtnState = MENU_YES;
			break;
		}
	}


	//if(GetKeyState(VK_ESCAPE)&0x8)

	RELEASE_INSTANCE(CGameInstance);

}

HRESULT CUI_SelectIcon::Ready_Layer_UI_Board(BOARD eCurrentMenu, _tchar* pLayerTag)
{
	m_bBoardOn = false;
	m_bOtherOn = false;
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	//
	// 
	if (m_eCurrentMenu != m_eNextMenu) {

		if (nullptr != pGameInstance->Find_Layer(LEVEL_MAIN, pLayerTag))
			pGameInstance->Release_LayerObject(LEVEL_MAIN, pLayerTag);
	}
	if (eCurrentMenu == MAIN_LOBY) {
		m_eCurrentMenu = MAIN_LOBY;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_MAIN, TEXT("Layer_Loby"), TEXT("Prototype_GameObject_UI_LobyBoard")))) {
			MSGBOX("Failed to Add Prototype UI_Background in CLevel_Main::Ready_Prototype_GameObject");
			return E_FAIL;
		}
		// 스타트
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_MAIN, TEXT("Layer_Loby"), TEXT("Prototype_GameObject_MainMenu_Start")))) {
			MSGBOX("Failed to Add Prototype UI_Background in CLevel_Main::Ready_Prototype_GameObject");
			return E_FAIL;
		}
		/*
		// 옵션
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_MAIN, TEXT("Layer_Loby"), TEXT("Prototype_GameObject_MainMenu_Option")))) {
		MSGBOX("Failed to Add Prototype UI_Background in CLevel_Main::Ready_Prototype_GameObject");
		return E_FAIL;
		}
		*/
		// 종료 
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_MAIN, TEXT("Layer_Loby"), TEXT("Prototype_GameObject_MainMenu_Quit")))) {
			MSGBOX("Failed to Add Prototype UI_Background in CLevel_Main::Ready_Prototype_GameObject");
			return E_FAIL;
		}

	}
	if (eCurrentMenu == MAIN_QUIT) {
		m_eCurrentMenu = MAIN_QUIT;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_MAIN, TEXT("Layer_Exit"), TEXT("Prototype_GameObject_UI_ExitBoard"), &eCurrentMenu))) {
			MSGBOX("Failed to Add Prototype UI_Background in CLevel_Main::Ready_Prototype_GameObject");
			return E_FAIL;
		}
		// 네
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_MAIN, TEXT("Layer_Exit"), TEXT("Prototype_GameObject_UI_ExitMenu_Yes"), &eCurrentMenu))) {
			MSGBOX("Failed to Add Prototype UI_Background in CLevel_Main::Ready_Prototype_GameObject");
			return E_FAIL;
		}

		// 아니요
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_MAIN, TEXT("Layer_Exit"), TEXT("Prototype_GameObject_UI_ExitMenu_No"), &eCurrentMenu))) {
			MSGBOX("Failed to Add Prototype UI_Background in CLevel_Main::Ready_Prototype_GameObject");
			return E_FAIL;
		}

	}
	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

CUI_SelectIcon * CUI_SelectIcon::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUI_SelectIcon*	pInstance = new CUI_SelectIcon(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create Logo in CUI_LoadingIcon::Create");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CUI_SelectIcon::Clone(void * pArg)
{
	CGameObject*	pInstance = new CUI_SelectIcon(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create Logo in CUI_Logo::Clone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_SelectIcon::Free(void)
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}