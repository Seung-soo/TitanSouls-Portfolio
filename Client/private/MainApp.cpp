#include "stdafx.h"
#include "..\public\MainApp.h"
#include "GameInstance.h"
#include "Level_Logo.h"
#include "Level_Loading.h"
#include "Renderer.h"
#include "Texture.h"
#include "Camera_Player.h"
#include "UI_LoadingBack.h"
#include "UI_LoadingIcon.h"
#include "UI_LoadingText.h"
#include "Effect_LightLeak.h"
#include "Clear_Effect.h"

LEVEL g_eCurrentLevel = LEVEL_LOGO;
LEVEL g_eNextLevel = LEVEL_END;
bool g_bGoLevel = false;
bool g_bSlimeClear = false;
bool g_bEyeCubeClear = false;
bool g_bYetiClear = false;
bool g_bIceBrainClear = false;
bool g_bColossusClear = false;
bool g_bCameraDirecting = false;
int	 g_iKill = 0;
bool g_bRespawn1Clear = false;
bool g_bRespawn2Clear = false;
bool g_bStartgame = false;
CMainApp::CMainApp()
	:m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CMainApp::NativeConstruct()
{
	if (nullptr == m_pGameInstance)
	{
		MSGBOX("m_pGameInstance is nullptr in CMainApp::NativeConstruct");
		return E_FAIL;
	}

	CGraphic_Device::GRAPHICDESC		GraphicDesc;
	ZeroMemory(&GraphicDesc, sizeof(CGraphic_Device::GRAPHICDESC));

	GraphicDesc.hWnd = g_hWnd;
	GraphicDesc.iWinCX = g_iWinCX;
	GraphicDesc.iWinCY = g_iWinCY;
	GraphicDesc.eScreenMode = CGraphic_Device::TYPE_FULLMODE;

	if (FAILED(m_pGameInstance->Initialize_Engine(g_hInst, GraphicDesc, LEVEL_END, &m_pGraphic_Device)))
	{
		MSGBOX("Failed Initialize_Engine in CMainApp::NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(DefaultSetting()))
	{
		MSGBOX("DefaultSetting returned E_FAIL in CMainApp::NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(Ready_Prototype_Component()))
	{
		MSGBOX("Ready_Prototype_Component returned E_FAIL in CMainApp::NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(Ready_Prototype_GameObject()))
	{
		MSGBOX("Ready_Prototype_GameObject returned E_FAIL in CMainApp::NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(OpenLevel(LEVEL_LOGO)))
	{
		MSGBOX("OpenLevel returned E_FAIL in CMainApp::NativeConstruct");
		return E_FAIL;
	}

	return S_OK;
}

_int CMainApp::Tick(_float fTimeDelta)
{
	m_pGraphic_Device->ShowCursor(false);
	if (nullptr == m_pGameInstance)
	{
		MSGBOX("m_pGameInstance is nullptr in CMainApp::Tick");
		return -1;
	}

	if (0 > m_pGameInstance->Tick_Engine(fTimeDelta))
	{
		MSGBOX("0 > m_pGameInstance->Tick_Engine in CMainApp::Tick");
		return -1;
	}

	return 0;
}

HRESULT CMainApp::Render()
{
	if (nullptr == m_pGameInstance)
	{
		MSGBOX("m_pGameInstance is nullptr in CMainApp::Render");
		return E_FAIL;
	}

	m_pGameInstance->Render_Begin();

	m_pRenderer->Render();

	

	m_pGameInstance->Render_Level();
	m_pGameInstance->Render_End();
	return S_OK;
}

HRESULT CMainApp::OpenLevel(LEVEL eNextLevel)
{
	if (nullptr == m_pGameInstance)
	{
		MSGBOX("m_pGameInstance is nullptr in CMainApp::OpenLevel");
		return E_FAIL;
	}

	CLevel*	pLevel = nullptr;

	switch (eNextLevel)
	{
	case LEVEL_LOGO:
		pLevel = CLevel_Logo::Create(m_pGraphic_Device);
		break;
	case LEVEL_FIELD:
		pLevel = CLevel_Loading::Create(m_pGraphic_Device, LEVEL_FIELD);
		eNextLevel = LEVEL_LOADING;
		break;
	}

	if (nullptr == pLevel)
	{
		MSGBOX("pLevel is nullptr in CMainApp::OpenLevel");
		return E_FAIL;
	}

	if (FAILED(m_pGameInstance->OpenLevel(eNextLevel, pLevel)))
	{
		MSGBOX("m_pGameInstance::OpenLevel returned the E_FAIL in CMainApp::OpenLevel");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CMainApp::DefaultSetting()
{
	if (nullptr == m_pGraphic_Device)
	{
		MSGBOX("m_pGraphic_Device is nullptr in CMainApp::DefaultSetting");
		return E_FAIL;
	}

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, false);


	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_GameObject()
{
	if (nullptr == m_pGameInstance)
	{
		MSGBOX("m_pGameInstance is nullptr in CMainApp::Ready_Prototype_GameObject");
		return E_FAIL;
	}
	//////////////////////////////////////////////
	// 여기에 프로토타입 게임 오브젝트 추가

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Player"), CCamera_Player::Create(m_pGraphic_Device))))
	{
		MSGBOX("m_pGameInstance->Add_Prototype returned E_FAIL in CMainApp::Ready_Prototype_GameObject");
		return E_FAIL;
	}
	// 검정배경
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject(Loading_Back)"), CUI_LoadingBack::Create(m_pGraphic_Device)))) {
		MSGBOX("m_pGameInstance->Add_Prototype returned E_FAIL in CMainApp::Ready_Prototype_GameObject(Loading_Icon)");
		return E_FAIL;
	}

	// 사슴
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject(Loading_Icon)"), CUI_LoadingIcon::Create(m_pGraphic_Device)))) {
		MSGBOX("m_pGameInstance->Add_Prototype returned E_FAIL in CMainApp::Ready_Prototype_GameObject(Loading_Icon)");
		return E_FAIL;
	}
	// 로딩글자
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject(Loading_Text)"), CUI_LoadingText::Create(m_pGraphic_Device)))) {
		MSGBOX("m_pGameInstance->Add_Prototype returned E_FAIL in CMainApp::Ready_Prototype_GameObject(Text)");
		return E_FAIL;
	}

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_LightLeak"), CEffect_LightLeak::Create(m_pGraphic_Device)))) {
		MSGBOX("m_pGameInstance->Add_Prototype returned E_FAIL in CMainApp::Ready_Prototype_GameObject(Effect_LightLeak)");
		return E_FAIL;
	}

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_White"), CClear_Effect::Create(m_pGraphic_Device)))) {
		MSGBOX("m_pGameInstance->Add_Prototype returned E_FAIL in CMainApp::Ready_Prototype_GameObject(CClear_Effect)");
		return E_FAIL;
	}

	//if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Dynamic"), CCamera_Dynamic::Create(m_pGraphic_Device))))
	//{
	//	MSGBOX("m_pGameInstance->Add_Prototype returned E_FAIL in CMainApp::Ready_Prototype_GameObject");
	//	return E_FAIL;
	//}
	//////////////////////////////////////////////


	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_Component()
{
	if (nullptr == m_pGameInstance)
	{
		MSGBOX("m_pGameInstance is nullptr in CMainApp::Ready_Prototype_Component");
		return E_FAIL;
	}
	//////////////////////////////////////////////
	// 여기에 프로토타입 컴포넌트 추가

	// 랜더러
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), m_pRenderer = CRenderer::Create(m_pGraphic_Device))))
	{
		MSGBOX("m_pGameInstance->Add_prototype returned E_FAIL in CMainApp::Ready_Prototype_Component(Renderer)");
		return E_FAIL;
	}

	// 렉트
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), CVIBuffer_Rect::Create(m_pGraphic_Device))))
	{
		MSGBOX("m_pGameInstance->Add_prototype returned E_FAIL in CMainApp::Ready_Prototype_Component(VIBuffer_Rect)");
		return E_FAIL;
	}

	// 렉트Z 컴포넌트 원형 생성
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect_Z"), CVIBuffer_Rect_Z::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CMainApp::Loading_For_Field(Component_VIBuffer_Rect_Z)");
		return E_FAIL;
	}

	// 렉트칼라 컴포넌트 원형 생성
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Color"), CVIBuffer_Rect_Color::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CMainApp::Loading_For_Field(Component_VIBuffer_Color)");
		return E_FAIL;
	}

	// 큐브 컴포넌트 원형 생성
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"), CVIBuffer_Cube::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CMainApp::Loading_For_Field(Component_VIBuffer_Cube)");
		return E_FAIL;
	}

	// 트랜스폼
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), CTransform::Create(m_pGraphic_Device))))
	{
		MSGBOX("m_pGameInstance->Add_prototype returned E_FAIL in CMainApp::Ready_Prototype_Component(Transform)");
		return E_FAIL;
	}

	// 텍스쳐
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/player.png")))))
	{
		MSGBOX("m_pGameInstance->Add_prototype returned E_FAIL in CMainApp::Ready_Prototype_Component(Texture)");
		return E_FAIL;
	}

	// 콜라이더 스피어
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"), CCollider_Sphere::Create(m_pGraphic_Device))))
	{
		MSGBOX("m_pGameInstance->Add_prototype returned E_FAIL in CMainApp::Ready_Prototype_Component(Collider_Sphere)");
		return E_FAIL;
	}

	// 콜라이더 렉트
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Rect"), CCollider_Rect::Create(m_pGraphic_Device))))
	{
		MSGBOX("m_pGameInstance->Add_prototype returned E_FAIL in CMainApp::Ready_Prototype_Component(Collider_Rect)");
		return E_FAIL;
	}

	// 콜라이더 렉트Z
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Rect_Z"), CCollider_Rect_Z::Create(m_pGraphic_Device))))
	{
		MSGBOX("m_pGameInstance->Add_prototype returned E_FAIL in CMainApp::Ready_Prototype_Component(Collider_Rect_Z)");
		return E_FAIL;
	}
	//////////////////////////////////////////////
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_LogoUI"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/UI/an.png")))))
	{
		MSGBOX("m_pGameInstance->Add_prototype returned E_FAIL in CMainApp::Ready_Prototype_Component(Texture)");
		return E_FAIL;
	}

	//////////////////////////////////////////////logo 0, 1, 2
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_LogoUI00"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/UI/UI_Logo00.png")))))
	{
		MSGBOX("m_pGameInstance->Add_prototype returned E_FAIL in CMainApp::Ready_Prototype_Component(Texture)");
		return E_FAIL;
	}

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_LogoUI01"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/UI/UI_Logo01.png")))))
	{
		MSGBOX("m_pGameInstance->Add_prototype returned E_FAIL in CMainApp::Ready_Prototype_Component(Texture)");
		return E_FAIL;
	}

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_LogoUI02"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/UI/UI_Logo02.png")))))
	{
		MSGBOX("m_pGameInstance->Add_prototype returned E_FAIL in CMainApp::Ready_Prototype_Component(Texture)");
		return E_FAIL;
	}

	// 검은 화면 텍스쳐
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_forLoadingBack"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/UI/BackUtility.png"))))) {
		MSGBOX("m_pGameInstance->Add_prototype returned E_FAIL in CMainApp::Ready_Prototype_Component(Texture for Loading)");
		return E_FAIL;
	}

	// 사슴과 로딩텍스트 텍스쳐
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_forLoading"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/UI/animals,loding.png"))))) {
		MSGBOX("m_pGameInstance->Add_prototype returned E_FAIL in CMainApp::Ready_Prototype_Component(Texture for Loading)");
		return E_FAIL;
	}
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_forMainMenu"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/UI/Text.png"))))) {
		MSGBOX("m_pGameInstance->Add_prototype returned E_FAIL in CMainApp::Ready_Prototype_Component(Texture for MainMenu)");
		return E_FAIL;
	}

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_forMainMenuSelect"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/UI/testUI.png"))))) {
		MSGBOX("m_pGameInstance->Add_prototype returned E_FAIL in CMainApp::Ready_Prototype_Component(Texture for MainMenu)");
		return E_FAIL;
	}

	Safe_AddRef(m_pRenderer);

	return S_OK;
}

CMainApp* CMainApp::Create()
{
	CMainApp*	pInstance = new CMainApp();

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Create CMainApp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainApp::Free()
{
	Safe_Release(m_pRenderer);
	Safe_Release(m_pGraphic_Device);
	Safe_Release(m_pGameInstance);

	CGameInstance::Release_Engine();
}
