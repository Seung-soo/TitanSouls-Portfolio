#include "stdafx.h"
#include "..\public\Loader.h"
#include "GameInstance.h"
#include "Include_Loader.h"
#include "LightLeak.h"

CLoader::CLoader(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device(pGraphic_Device)
{
	Safe_AddRef(m_pGraphic_Device);
}

_uint APIENTRY ThreadFunction(void* pArg)
{
	CLoader*	pLoader = (CLoader*)pArg;

	EnterCriticalSection(&pLoader->Get_CS());



	switch (pLoader->Get_LevelID())
	{
	case LEVEL_MAIN:
		pLoader->Loading_For_Main();
		break;
	case LEVEL_FIELD:
		pLoader->Loading_For_Field();
		break;
	case LEVEL_SLIME:
		pLoader->Loading_For_Slime();
		break;
	case LEVEL_ICEBRAIN:
		pLoader->Loading_For_IceBrain();
		break;
	case LEVEL_COLOSUSS:
		pLoader->Loading_For_Colossus();
		break;
	case LEVEL_EYECUBE:
		pLoader->Loading_For_EyeCube();
		break;
	case LEVEL_YETI:
		pLoader->Loading_For_Yeti();
		break;
	case LEVEL_RESPAWN1:
		pLoader->Loading_For_Respawn1();
		break;
	case LEVEL_RESPAWN2:
		pLoader->Loading_For_Respawn2();
		break;
	case LEVEL_CREDIT:
		pLoader->Loading_For_EndingCredit();
		break;
	}

	LeaveCriticalSection(&pLoader->Get_CS());

	return 0;
}

HRESULT CLoader::NativeConstruct(LEVEL eNextLevel)
{
	m_isFinished = false;
	InitializeCriticalSection(&m_CS);
	g_eCurrentLevel = LEVEL_LOADING;
	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, ThreadFunction, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	m_eLevel = eNextLevel;
	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);

	pGameInstance->StopAll();
	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CLoader::Loading_For_Main(void)
{

	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);
	// create prototype component 
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAIN, TEXT("Prototype_Component_Texture_UIBackground"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/UI/MainBackground.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For Main(Component Texture)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_White"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Particle/White.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For Main(Component Texture)");
		return E_FAIL;
	}
	// create prototype gameobject
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UIBackground"), CUI_Background::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Main(Prototype_GameObject_UIBackground)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MainMenu_Start"), CUI_StartText::Create(m_pGraphic_Device)))) {
		MSGBOX("m_pGameInstance->Add_Prototype returned E_FAIL in CMainApp::Ready_Prototype_GameObject(Text)");
		return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MainMenu_Option"), CUI_OptionText::Create(m_pGraphic_Device)))) {
		MSGBOX("m_pGameInstance->Add_Prototype returned E_FAIL in CMainApp::Ready_Prototype_GameObject(Text)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MainMenu_Quit"), CUI_QuitText::Create(m_pGraphic_Device)))) {
		MSGBOX("m_pGameInstance->Add_Prototype returned E_FAIL in CMainApp::Ready_Prototype_GameObject(Text)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MainMenu_Select"), CUI_SelectIcon::Create(m_pGraphic_Device)))) {
		MSGBOX("m_pGameInstance->Add_Prototype returned E_FAIL in CMainApp::Ready_Prototype_GameObject(Text)");
		return E_FAIL;
	}

	// 맵이동 효과 로딩

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_SceneLoading"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/UI/Loading%d.png"), 19))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Main(Prototype_Texture_SceneLoading)");
		return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_SceneLoading"), CScene_Loading::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(GameObject_SceneLoading)");
		return E_FAIL;
	}

	// 0309추가
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Zoom"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/UI/zoom0%d.png"), 7))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Main(Prototype_Component_Texture_Zoom)");
		return E_FAIL;
	}
	// 0309추가
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Zoom"), CZoom::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(Prototype_GameObject_Zoom)");
		return E_FAIL;
	}

	// board texture 
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_LobyBoard"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/UI/UI_LobyBoard.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For Main(Component Texture)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_ExitBoard"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/UI/UI_ExitBoard.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For Main(Component Texture)");
		return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Text"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/UI/UI_Text.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For Main(Component Texture)");
		return E_FAIL;
	}


	//////////game object

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_LobyBoard"), CUI_LobyBoard::Create(m_pGraphic_Device)))) {
		MSGBOX("m_pGameInstance->Add_Prototype returned E_FAIL in CMainApp::Ready_Prototype_GameObject(Text)");
		return E_FAIL;
	}


	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_ExitBoard"), CUI_ExitBoard::Create(m_pGraphic_Device)))) {
		MSGBOX("m_pGameInstance->Add_Prototype returned E_FAIL in CMainApp::Ready_Prototype_GameObject(Text)");
		return E_FAIL;
	}
	// yes text
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_ExitMenu_Yes"), CUI_YesText::Create(m_pGraphic_Device)))) {
		MSGBOX("m_pGameInstance->Add_Prototype returned E_FAIL in CMainApp::Ready_Prototype_GameObject(Text)");
		return E_FAIL;
	}
	// no text
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_ExitMenu_No"), CUI_NoText::Create(m_pGraphic_Device)))) {
		MSGBOX("m_pGameInstance->Add_Prototype returned E_FAIL in CMainApp::Ready_Prototype_GameObject(Text)");
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);
	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Field()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_FIELD, TEXT("Prototype_Component_Texture_UIBackground"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/UI/MainLogo.png")))))
	//{
	//	MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For Main(Component Texture)");
	//	return E_FAIL;
	//}

	// 필드 부속품들 텍스쳐 원형 선언
	pGameInstance->Add_Prototype(LEVEL_FIELD, TEXT("Prototype_Component_Texture_Field01_Ceiling"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Field01Cailing.png")));
	pGameInstance->Add_Prototype(LEVEL_FIELD, TEXT("Prototype_Component_Texture_Field01_Grass00"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Field01_Grass00.png")));
	pGameInstance->Add_Prototype(LEVEL_FIELD, TEXT("Prototype_Component_Texture_Field01_Grass01"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Field01_Grass01.png")));
	pGameInstance->Add_Prototype(LEVEL_FIELD, TEXT("Prototype_Component_Texture_Field01_Grass02_Down"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Grass02_%d.png"), 3));
	pGameInstance->Add_Prototype(LEVEL_FIELD, TEXT("Prototype_Component_Texture_Field01_Water"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Water.png")));
	pGameInstance->Add_Prototype(LEVEL_FIELD, TEXT("Prototype_Component_Texture_Field01_Light"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Light.png")));
	pGameInstance->Add_Prototype(LEVEL_FIELD, TEXT("Prototype_Component_Texture_Field01_Dark"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Dark.png")));
	pGameInstance->Add_Prototype(LEVEL_FIELD, TEXT("Prototype_Component_Texture_Field01_Tree"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Tree.png")));
	pGameInstance->Add_Prototype(LEVEL_FIELD, TEXT("Prototype_Component_Texture_Field01_Grass04"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Grass04_%d.png"), 3));
	pGameInstance->Add_Prototype(LEVEL_FIELD, TEXT("Prototype_Component_Texture_Field01_Light2"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Light2_%d.png"), 4));

	//Handrall
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_FIELD, TEXT("Prototype_Component_Texture_Handrail"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/Untitled-3.dds")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(Prototype_Component_Texture_Handrail)");
		return E_FAIL;
	}
	pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Handrall_Field01"), CField01_Handrail::Create(m_pGraphic_Device));

	//Ceiling
	pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Field01_Ceiling"), CField01_Ceiling::Create(m_pGraphic_Device));

	//Grass
	pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Field01_Grass00"), CField01_Grass::Create(m_pGraphic_Device));
	pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Field01_Grass_Up"), CField01_Grass_Up::Create(m_pGraphic_Device));
	pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Field01_Water"), CField01_Water::Create(m_pGraphic_Device));

	pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Field01_Light"), CField01_Light::Create(m_pGraphic_Device));
	pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Field01_Dark"), CField01_Dark::Create(m_pGraphic_Device));
	pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Field01_Grass_Down"), CField01_Grass_Down::Create(m_pGraphic_Device));
	pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Field01_Tree"), CField01_Tree::Create(m_pGraphic_Device));
	pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Field01_Grass04"), CField01_Grass_Ex::Create(m_pGraphic_Device));
	pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Field01_Light2"), CField01_Light2::Create(m_pGraphic_Device));

	//Rect Door ++

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Door1_Field"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Field01_Door0.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(Component_Texture_Terrain_Field)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Door2_Field"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Field01_Door1.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(Component_Texture_Terrain_Field)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Door1_Field"), CField01_Door1::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(Prototype_GameObject_Door1_Field)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Door2_Field"), CField01_Door2::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(Prototype_GameObject_Door2_Field)");
		return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_AllDoor"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/All_Door.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(Prototype_Component_Texture_AllDoor)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_AllDoor"), CAll_Door::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(Prototype_GameObject_AllDoor)");
		return E_FAIL;
	}


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_AllDoor2"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/All_Door2.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(Prototype_Component_Texture_AllDoor2)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_AllDoor2"), CAll_Door2::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(Prototype_GameObject_AllDoor2)");
		return E_FAIL;
	}

	// 터레인 컴포넌트 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_FIELD, TEXT("Prototype_Component_VIBuffer_Terrain_Field"), CVIBuffer_Terrain::Create(m_pGraphic_Device, 33, 33))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(Component_VIBuffer_Terrain_Field)");
		return E_FAIL;
	}
	// 터레인 텍스쳐 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_FIELD, TEXT("Prototype_Component_Texture_Terrain_Field"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Field01.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(Component_Texture_Terrain_Field)");
		return E_FAIL;
	}
	// 터레인 오브젝트 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain_Field"), CTerrain_Field01::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(GameObject_Terrain_Field)");
		return E_FAIL;
	}


	// 벽 컴포넌트 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_FIELD, TEXT("Prototype_Component_VIBuffer_Wall_Field"), CVIBuffer_Wall::Create(m_pGraphic_Device, 33, 33))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(Component_VIBuffer_Wall)");
		return E_FAIL;
	}
	// 벽 양옆 텍스쳐 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_FIELD, TEXT("Prototype_Component_Texture_WallSide_Field"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Field01Wall.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(Component_Texture_Wall_Field)");
		return E_FAIL;
	}
	// 정면 벽 텍스쳐 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_FIELD, TEXT("Prototype_Component_Texture_WallFront_Field"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Field01Door.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(Component_Texture_WallFront_Field)");
		return E_FAIL;
	}
	// 후면 벽 텍스쳐 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_FIELD, TEXT("Prototype_Component_Texture_WallBack_Field"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Field01DoorBack.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(Component_Texture_Terrain)");
		return E_FAIL;
	}
	// 벽 양옆 오브젝트 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_WallSide_Field"), CWall_Side_Field::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(GameObject_WallSide)");
		return E_FAIL;
	}
	// 벽 정면 오브젝트 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_WallFront_Field"), CWall_Field01_1::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(GameObject_WallFront)");
		return E_FAIL;
	}
	// 벽 뒤 오브젝트 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_WallBack_Field"), CWall_Field01_2::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(GameObject_WallBack)");
		return E_FAIL;
	}

	// 투명벽 오브젝트 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_WallHide_Field"), CWall_Hide::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(GameObject_WallHide)");
		return E_FAIL;
	}

	// 플레이어 텍스쳐 원형
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/player.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(Component_Texture_Player)");
		return E_FAIL;
	}
	// 플레이어 오브젝트 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player"), CPlayer::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(GameObject_Player)");
		return E_FAIL;
	}

	// 화살 텍스쳐 원형
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Arrow"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/player.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(Component_Texture_Arrow)");
		return E_FAIL;
	}
	// 화살 오브젝트 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Arrow"), CArrow::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(GameObject_CArrow)");
		return E_FAIL;
	}

	// 화살 라이트이펙트 텍스쳐 원형
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ArrowLight"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Particle/LightEffect.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(Component_Texture_Arrow)");
		return E_FAIL;
	}
	// 화살 라이트 이펙트 오브젝트 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ArrowLight"), CArrowLight::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(GameObject_CArrowLight)");
		return E_FAIL;
	}


	// 스카이 텍스쳐 원형
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_FIELD, TEXT("Prototype_Component_Texture_Sky"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/SkyBox/Sky_%d.dds"), 4))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(Component_Texture_Sky)");
		return E_FAIL;
	}
	// 스카이 오브젝트 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Sky"), CSky::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(GameObject_Sky)");
		return E_FAIL;
	}

	// 에임 텍스쳐 원형
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Aim"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Aim.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(Component_Texture_Aim)");
		return E_FAIL;
	}
	// 에임 오브젝트 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Aim"), CAim::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(GameObject_Aim)");
		return E_FAIL;
	}

	// 애로우파이어 오브젝트 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ArrowFire"), CArrowFire::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(GameObject_ArrowFire)");
		return E_FAIL;
	}

	// 블러드 텍스쳐 원형
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Blood"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Particle/Blood.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(Component_Texture_Blood)");
		return E_FAIL;
	}

	// 블러드 오브젝트 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Blood"), CBlood::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(GameObject_Blood)");
		return E_FAIL;
	}

	// 소울 텍스쳐 원형
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Soul"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Particle/Effect_Soul.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(Component_Texture_Soul)");
		return E_FAIL;
	}
	// 소울 오브젝트 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Soul"), CSoul::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(GameObject_Soul)");
		return E_FAIL;
	}
	// 소울 잔상 오브젝트 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Soul_AfterImage"), CSoul_AfterImage::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(GameObject_CSoul_AfterImage)");
		return E_FAIL;
	}
	// 소울먹방이펙트 오브젝트 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_EatSouls"), CEffect_EatSouls::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(GameObject_Effect_EatSouls)");
		return E_FAIL;
	}

	// 화이트스크린 오브젝트 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_WhiteScreen"), CWhiteScreen::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(GameObject_WhiteScreen)");
		return E_FAIL;
	}

	// 다크스크린 오브젝트 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_DarkScreen"), CDarkScreen::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(GameObject_DarkScreen)");
		return E_FAIL;
	}

	// Prototype Create for UI Skill Texture
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Skill00"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/UI/UI_Skill_00.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(Component_Texture_SkillIcon00)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Skill01"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/UI/UI_Skill_01.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(Component_Texture_SkillIcon01)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Skill99"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/UI/UI_Skill_99.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(Component_Texture_SkillIcon02)");
		return E_FAIL;
	}

	////////////////////////////////////prototype Texture for btn icon
	//포탈 아이콘
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_PotalIcon"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/UI/UI_PotalIcon(InGame).png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(Component_Texture_UI_PotalIcon)");
		return E_FAIL;
	}



	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_LShiftIcon"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/UI/UI_LShiftIcon(InGame).png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(Component_Texture_UI_LShiftIcon)");
		return E_FAIL;
	}


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_LButtonIcon"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/UI/UI_LButtonIcon(InGame).png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(Component_Texture_UI_LButtonIcon)");
		return E_FAIL;
	}


	/////////////////////////////버튼 아이콘 오브젝트

	// Prototype Create for UI Skill Object
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_SkillArrow"), CUI_Skill_Shooting::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(GameObject_SkillIcon00)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_SkillRolling"), CUI_Skill_Rolling::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(GameObject_SkillIcon01)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_SkillCoolingTimer"), CUI_Skill_CoolTimer::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(GameObject_SkillIcon99)");
		return E_FAIL;
	}


	//////////////////////////////////////////////////////////////////////////


	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_PotalIcon"), CUI_PotalBtnIcon::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(GameObject_UI_PotalIcon)");
		return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_LShiftIcon"), CUI_LShiftIcon::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(GameObject_UI_LShiftIcon)");
		return E_FAIL;
	}


	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_LButtonIcon"), CUI_LBtnIcon::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(GameObject_UI_LButtonIcon)");
		return E_FAIL;
	}

	// UI for boss name ui

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_BossName"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/UI/InGameUI_BossName.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(Component_UIBack)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UIBack"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/UI/InGameUI_Back.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(Component_UIBack)");
		return E_FAIL;
	}

	// UI Object for Boss Name
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UIBack"), CUI_Back::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(GameObject_EffectEmpty)");
		return E_FAIL;
	}
	// UI Object for Boss Name
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UIName_Slime"), CUI_NameACIDNERVE::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(GameObject_EffectEmpty)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UIName_Colosuss"), CUI_NameCOLOSUSS::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(GameObject_EffectEmpty)");
		return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UIName_EyeCube"), CUI_NameEYECUBE::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(GameObject_EffectEmpty)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UIName_IceBrain"), CUI_NameICEBRAIN::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(GameObject_EffectEmpty)");
		return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UIName_Yeti"), CUI_NameYETI::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(GameObject_EffectEmpty)");
		return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UIName_Empty"), CUI_NameEmpty::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(GameObject_EffectEmpty)");
		return E_FAIL;
	}

	// 인게임 플레이어 상태(죽었는지 살았는지) 텍스쳐
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_PlayerState"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/UI/UI_PlayerAlive.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(Component_Texture_UI_PlayerState)");
		return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_PlayerDead"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/UI/UI_PlayerDead.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(Component_Texture_UI_PlayerDead)");
		return E_FAIL;
	}

	// 프로토타입 오브젝트


	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_PlayerAlive"), CUI_PlayerState::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(GameObject_UI_LButtonIcon)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_PlayerDead"), CUI_PlayerDead::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(GameObject_UI_LButtonIcon)");
		return E_FAIL;
	}

	// 플레이어 대쉬먼지 텍스쳐 원형
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_PlayerDust"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Player_Dust.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(Prototype_Component_Texture_PlayerDust)");
		return E_FAIL;
	}
	// 플레이어 대쉬먼지 오브젝트 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_PlayerDust"), CPlayer_Dust::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(Prototype_GameObject_PlayerDust)");
		return E_FAIL;
	}

	// 화살 Rect 텍스쳐 원형
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ArrowRect"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Arrow_Rect.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(Prototype_Component_Texture_ArrowRect)");
		return E_FAIL;
	}
	// 화살 Rect 오브젝트 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ArrowRect"), CArrow_Rect::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(Prototype_GameObject_ArrowRect)");
		return E_FAIL;
	}

	// 화살 Pull 텍스쳐 원형
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ArrowPull"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Arrow_Effect.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(Prototype_Component_Texture_ArrowPull)");
		return E_FAIL;
	}
	// 화살 Pull 오브젝트 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ArrowPull"), CArrow_Pull::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(Prototype_GameObject_ArrowPull)");
		return E_FAIL;
	}

	//++
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_WallDark"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/WallDark.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(Prototype_Component_Texture_WallDark)");
		return E_FAIL;
	}

	//++
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_FIELD, TEXT("Prototype_Component_VIBuffer_Terrain2_Field"), CVIBuffer_Terrain::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Field_Height.bmp")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Respawn1(Prototype_Component_VIBuffer_Terrain2_Field)");
		return E_FAIL;
	}
	//++
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_FIELD, TEXT("Prototype_Component_Texture_Terrain2_Field"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Terrian2_Field01.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Respawn1(Prototype_Component_Texture_Terrain2_Field)");
		return E_FAIL;
	}

	//++
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_CField01_Ceiling2"), CField01_Ceiling2::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Respawn1(Prototype_GameObject_CField01_Ceiling2)");
		return E_FAIL;
	}
	//++
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain2_Field"), CTerrain2_Field01::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Respawn1(Prototype_GameObject_Terrain2_Field)");
		return E_FAIL;
	}

	//++
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_CField01_WallDark"), CField01_WallDark::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Respawn1(Prototype_GameObject_CField01_Ceiling2)");
		return E_FAIL;
	}

	// 통로 게임 오브젝트들
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain1_Respawn1"), CRespawn01_Terrain::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Respawn1(Prototype_GameObject_Terrain1_Respawn1)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain2_Respawn1"), CRespawn01_Terrain2::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Respawn1(Prototype_GameObject_Terrain2_Respawn1)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Wall1_Respawn01"), CRespawn01_Wall1::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Slime(Prototype_GameObject_Wall1_Respawn01)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Ceiling_Respawn1"), CRespawn01_Ceiling::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Slime(Prototype_GameObject_Ceiling_Respawn1)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Grass1_Respawn01"), CRespawn01_Grass1::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Respawn1(Prototype_GameObject_Grass1_Respawn01)");
		return E_FAIL;
	}
	pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Respawn01_Grass3"), CRespawn01_Grass3::Create(m_pGraphic_Device));
	pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Respawn01_Grass02"), CRespawn01_Grass2::Create(m_pGraphic_Device));
	pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Respawn01_Grass04"), CRespawn01_Grass4::Create(m_pGraphic_Device));
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_WallA_Respawn01"), CRespawn01_WallA::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Respawn1(Prototype_GameObject_WallA_Respawn01)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_WallB_Respawn01"), CRespawn01_WallB::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Respawn1(Prototype_GameObject_WallB_Respawn01)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Ice1_Respawn02"), CRespawn02_Ice1::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Respawn1(Prototype_GameObject_WallA_Respawn01)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Wall1_Respawn02"), CRespawn02_Wall1::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Slime(Prototype_GameObject_Wall1_Respawn01)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Ceiling01_Respawn2"), CRespawn02_Ceiling1::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Colossus(Prototype_GameObject_Ceiling01_Yeti01)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain_Respawn2"), CRespawn02_Terrain::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Respawn1(Prototype_GameObject_Terrain1_Respawn1)");
		return E_FAIL;
	}
	//0311추가
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_IceWall_Respawn02"), CRespawn02_IceWall::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(Prototype_GameObject_IceWall_Respawn02)");
		return E_FAIL;
	}
	//+++
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_CeilingYeti_Respawn2"), CRespawn02_CeilingYeti::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Colossus(Prototype_GameObject_Ceiling01_Yeti01)");
		return E_FAIL;
	}
	//+++
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_WallA_Respawn02"), CRespawn02_WallA::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Respawn1(Prototype_GameObject_WallA_Respawn02)");
		return E_FAIL;
	}
	//+++
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_WallB_Respawn02"), CRespawn02_WallB::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Respawn1(Prototype_GameObject_WallB_Respawn02)");
		return E_FAIL;
	}
	//+++
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_WallC_Respawn02"), CRespawn02_WallC::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Respawn1(Prototype_GameObject_WallB_Respawn02)");
		return E_FAIL;
	}
	//+++
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_WallD_Respawn02"), CRespawn02_WallD::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Respawn1(Prototype_GameObject_WallB_Respawn02)");
		return E_FAIL;
	}
	//+++
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain2_Respawn2"), CRespawn02_Terrain2::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Respawn1(Prototype_GameObject_Terrain2_Respawn1)");
		return E_FAIL;
	}

	// 슬라임 오브젝트들
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Mist_Slime01"), CSlime01_Mist::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Slime(Prototype_GameObject_Light_Slime01)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Light_Slime01"), CSlime01_Light::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Slime(Prototype_GameObject_Light_Slime01)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Wall2_Slime01"), CSlime01_Wall2::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Slime(Prototype_GameObject_Wall2_Slime01)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Wall_Slime01"), CSlime01_Wall::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Slime(Prototype_GameObject_Wall_Slime01)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain_Slime01"), CSlime01_Terrain::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Slime(GameObject_Slime_Water)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Ceiling_Slime01"), CSlime01_Ceiling::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Slime(Prototype_GameObject_Ceiling_Slime01)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Pipe1_Slime01"), CSlime01_Pipe01::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Slime(Prototype_GameObject_Light_Slime01)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Pipe2_Slime01"), CSlime01_Pipe02::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Slime(Prototype_GameObject_Light_Slime01)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Skull_Slime01"), CSlime01_Skull::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Slime(Prototype_GameObject_Skull_Slime01)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Legiron_Slime01"), CSlime01_Legiron::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Slime(Prototype_GameObject_Skull_Slime01)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Flask01_Slime01"), CSlime01_Flask1::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Slime(Prototype_GameObject_Flask01_Slime01)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Flask02_Slime01"), CSlime01_Flask2::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Slime(Prototype_GameObject_Flask02_Slime01)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Flask03_Slime01"), CSlime01_Flask3::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Slime(Prototype_GameObject_Flask03_Slime01)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Water_drop_Slime01"), CSlime01_Water_drop::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Slime(Prototype_GameObject_Water_drop_Slime01)");
		return E_FAIL;
	}
	// 벽 오브젝트 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Wall"), CWall::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(GameObject_Wall)");
		return E_FAIL;
	}
	// 슬라임 오브젝트 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Slime"), CSlime::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Slime(GameObject_Slime)");
		return E_FAIL;
	}
	// 슬라임 보스 오브젝트 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Slime_Boss"), CSlime_Boss::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Slime(GameObject_Slime_Boss)");
		return E_FAIL;
	}
	// 슬라임 심장 오브젝트 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Slime_Heart"), CSlime_Heart::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Slime(GameObject_Slime_Heart)");
		return E_FAIL;
	}
	// 슬라임 액체 오브젝트 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Slime_Water"), CSlime_Water::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Slime(GameObject_Slime_Water)");
		return E_FAIL;
	}

	// 슬라임 보스 그림자 오브젝트 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Slime_Boss_Shadow"), CSlime_Boss_Shadow::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Slime(Prototype_GameObject_Slime_Boss_Shadow)");
		return E_FAIL;
	}
	// 슬라임 그림자 오브젝트 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Slime_Shadow"), CSlime_Shadow::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Slime(Prototype_GameObject_Slime_Shadow)");
		return E_FAIL;
	}


	// 슬라임 심장 그림자 오브젝트 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Slime_Heart_Shadow"), CSlime_Heart_Shadow::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Slime(Prototype_GameObject_Slime_Heart_Shadow)");
		return E_FAIL;
	}

	// 슬라임 워터 미니 오브젝트 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Slime_water_mini"), CSlime_water_mini::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Slime(Prototype_GameObject_Slime_water_mini)");
		return E_FAIL;
	}

	// 심장 워터 미니 오브젝트 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Heart_water_mini"), CHeart_water_mini::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Slime(Prototype_GameObject_Heart_water_mini)");
		return E_FAIL;
	}
	// 심장 워터 오브젝트 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Heart_water"), CHeart_Water::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Slime(Prototype_GameObject_Heart_water)");
		return E_FAIL;
	}
	// 아이스브레인 오브젝트들
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain_IceBrain01"), CIceBrain01_Terrain::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_IceBrain(Prototype_GameObject_Terrain_IceBrain01)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Wall1_IceBrain01"), CIceBrain01_Wall1::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_IceBrain(Prototype_GameObject_Wall1_IceBrain01)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Wall2_IceBrain01"), CIceBrain01_Wall2::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_IceBrain(Prototype_GameObject_Wall2_IceBrain01)");
		return E_FAIL;
	}
	//+++
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Wall3_IceBrain01"), CIceBrain01_Wall3::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_IceBrain(Prototype_GameObject_Wall3_IceBrain01)");
		return E_FAIL;
	}

	//+++
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Wall4_IceBrain01"), CIceBrain01_Wall4::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_IceBrain(Prototype_GameObject_Wall4_IceBrain01)");
		return E_FAIL;
	}

	//+++
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Ceiling_IceBrain01"), CIceBrain01_Ceiling::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_IceBrain(Prototype_GameObject_Ceiling_IceBrain01)");
		return E_FAIL;
	}
	// 아이스 브레인 오브젝트 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Ice_Brain"), CIce_Brain::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_IceBrain(GameObject_Ice_Brain)");
		return E_FAIL;
	}
	// 브레인 오브젝트 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Brain"), CBrain::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_IceBrain(GameObject_Brain)");
		return E_FAIL;
	}

	// 브레인 그림자 오브젝트 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Brain_Shadow"), CBrain_Shadow::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_IceBrain(GameObject_Brain)");
		return E_FAIL;
	}

	// 아이스 그림자 오브젝트 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Ice_Shadow"), CIce_Shadow::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_IceBrain(Prototype_GameObject_Ice_Shadow)");
		return E_FAIL;
	}

	// 파이어 버튼 오브젝트 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Fire_Button"), CFire_Button::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_IceBrain(Prototype_GameObject_Fire_Button)");
		return E_FAIL;
	}

	// 파이어 오브젝트 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Fire"), CFire::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_IceBrain(Prototype_GameObject_Fire)");
		return E_FAIL;
	}

	// 브레인 워터 미니 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Brain_water_mini"), CBrain_water_mini::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_IceBrain(Prototype_GameObject_Brain_water_mini)");
		return E_FAIL;
	}
	// 브레인 워터  원형 생성
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Brain_water"), CBrain_Water::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_IceBrain(Prototype_GameObject_Brain_water)");
		return E_FAIL;
	}
	// 콜로서스 오브젝트들
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain_Colossus01"), CColossus01_Terrain::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Colossus(Prototype_GameObject_Terrain_IceBrain01)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_TerrainEx_Colossus01"), CColossus01_TerrainEx::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Colossus(Prototype_GameObject_Terrain_IceBrain01)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Wall1_Colossus01"), CColossus01_Wall1::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Colossus(Prototype_GameObject_Wall1_Colossus01)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Wall2_Colossus01"), CColossus01_Wall2::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Colossus(Prototype_GameObject_Wall2_Colossus01)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_WallEX_Colossus01"), CColossus01_WallEx::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Colossus(Prototype_GameObject_Wall2_Colossus01)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_CeilingEx_Colossus01"), CColossus01_CeilingEx::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Colossus(Prototype_GameObject_Terrain_IceBrain01)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_CeilingEx2_Colossus01"), CColossus01_CeilingEx02::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Colossus(Prototype_GameObject_CeilingEx2_Colossus01)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Dark01_Colossus01"), CColossus01_Dark01::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Colossus(Prototype_GameObject_Dark01_Colossus01)");
		return E_FAIL;
	}
	pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Colossus01_Grass_Down"), CColossus01_Grass01::Create(m_pGraphic_Device));

	// 콜로서스 본체 오브젝트 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Colossus"), CColossus::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Colossus_Fist(GameObject_Colossus_Fist)");
		return E_FAIL;
	}


	// 콜로서스 왼주먹 오브젝트 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Colossus_Fist"), CColossus_Fist::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Colossus_Fist(GameObject_Colossus_Fist)");
		return E_FAIL;
	}


	// 콜로서스 왼주먹 오브젝트 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Colossus_Fist_Right"), CColossus_Fist_Right::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Colossus_Fist(GameObject_Colossus_Fist)");
		return E_FAIL;
	}

	// 콜로서스 왼주먹 그림자 오브젝트 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ColossusFist_Left_Shadow"), CColossusFist_Left_Shadow::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_CColossus(GameObject_Colossus_Fist_Shadow)");
		return E_FAIL;
	}
	// 콜로서스 오른주먹 그림자 오브젝트 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ColossusFist_Right_Shadow"), CColossusFist_Right_Shadow::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_CColossus(GameObject_Colossus_Fist_Shadow)");
		return E_FAIL;
	}

	// 콜로서스 왼주먹 잔상 오브젝트 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ColossusFist_Left_afterimage"), CColosuss_Left_Fist_afterimage::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_CColossus(Prototype_GameObject_ColossusFist_Left_afterimage)");
		return E_FAIL;
	}

	// 콜로서스 오른주먹 잔상 오브젝트 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ColossusFist_Right_afterimage"), CColosuss_Right_Fist_afterimage::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_CColossus(Prototype_GameObject_ColossusFist_Right_afterimage)");
		return E_FAIL;
	}

	// 콜로서스 먼지 오브젝트 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Colossus_Dust"), CColosuss_Fist_Dust::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_CColossus(Prototype_GameObject_Colossus_Dust)");
		return E_FAIL;
	}
	// 종유석 오브젝트 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Jongyu"), CJongyu::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_CColossus(Prototype_GameObject_Jongyu)");
		return E_FAIL;
	}
	// 종유석 그림자 오브젝트 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Jongyu_Shadow"), CJongyu_Shadow::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_CColossus(Prototype_GameObject_Jongyu_Shadow)");
		return E_FAIL;
	}

	// 종유석 부서짐 오브젝트 원형 생성 0312
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Jongyu_Break"), CJongyu_Break_Effect::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_CColossus(Prototype_GameObject_Jongyu_Shadow)");
		return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_EndDoorRight"), CEndDoorRight::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(Prototype_GameObject_EndDoorRight)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_EndDoorLeft"), CEndDoorLeft::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(Prototype_GameObject_EndDoorLeft)");
		return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_EndDoor"), CEndDoor::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(Prototype_GameObject_EndDoor)");
		return E_FAIL;
	}


	// 아이큐브 오브젝트들
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain_EyeCube01"), CEyeCube01_Terrain::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_EyeCube(Prototype_GameObject_Terrain_EyeCube01)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain2_EyeCube01"), CEyeCube01_Terrain2::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Colossus(Prototype_GameObject_Terrain2_EyeCube01)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain3_EyeCube01"), CEyeCube01_Terrain3::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Colossus(Prototype_GameObject_Terrain3_EyeCube01)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Wall1_EyeCube01"), CEyeCube_Wall1::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_EyeCube(Prototype_GameObject_Wall1_EyeCube01)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Wall2_EyeCube01"), CEyeCube_Wall2::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_EyeCube(Prototype_GameObject_Wall2_EyeCube01)");
		return E_FAIL;
	}


	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Ceiling_EyeCube01"), CEyeCube_Ceiling::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_EyeCube(Prototype_GameObject_Wall2_EyeCube01)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Wallex_EyeCube01"), CEyeCube01_Wallex::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_EyeCube(Prototype_GameObject_Wallex_EyeCube01)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Wallex2_EyeCube01"), CEyeCube01_Wallex2::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Colossus(Prototype_GameObject_Wallex2_EyeCube01)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Chess1_EyeCube01"), CEyeCube01_Chess1::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_EyeCube(Prototype_GameObject_Chess1_EyeCube01)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Chess2_EyeCube01"), CEyeCube01_Chess2::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_EyeCube(Prototype_GameObject_Chess2_EyeCube01)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Chess3_EyeCube01"), CEyeCube01_Chess3::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_EyeCube(Prototype_GameObject_Chess3_EyeCube01)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Chess4_EyeCube01"), CEyeCube01_Chess4::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_EyeCube(Prototype_GameObject_Chess4_EyeCube01)");
		return E_FAIL;
	}
	// 아이큐브 오브젝트 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_EyeCube"), CEyeCube::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_EyeCuBe(GameObject_EyeCube)");
		return E_FAIL;
	}
	// 아이큐브 그림자 오브젝트 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_EyeCube_Shadow"), CEyeCube_Shadow::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_EyeCuBe(GameObject_EyeCube_Shadow)");
		return E_FAIL;
	}

	// 아이큐브 레이저 오브젝트 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_EyeCube_Rayzer"), CEyeCube_Rayzer::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_EyeCuBe(GameObject_EyeCube_Rayzer)");
		return E_FAIL;
	}
	// 아이큐브 세로 오브젝트 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_EyeCube_Rayzer_Vertical"), CEyeCube_Rayzer_Vertical::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_EyeCuBe(Prototype_GameObject_EyeCube_Rayzer_Vertical)");
		return E_FAIL;
	}


	// 예티 오브젝트들
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain_Yeti01"), CYeti01_Terrain::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Yeti(Prototype_GameObject_Terrain_Yeti01)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Wall1_Yeti01"), CYeti01_Wall1::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Yeti(Prototype_GameObject_Wall1_Yeti01)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Wall2_Yeti01"), CYeti01_Wall2::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Yeti(Prototype_GameObject_Wall2_Yeti01)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Ceiling01_Yeti01"), CYeti01_Ceiling01::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Colossus(Prototype_GameObject_Ceiling01_Yeti01)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Ceiling02_Yeti01"), CYeti01_Ceiling02::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Colossus(Prototype_GameObject_Ceiling01_Yeti01)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain3_Yeti01"), CYeti01_Terrain03::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Colossus(Prototype_GameObject_Ceiling01_Yeti01)");
		return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain02_Yeti01"), CYeti01_Terrain02::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Colossus(Prototype_GameObject_Terrain02_Yeti01)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Ice03_Yeti01"), CYeti01_Ice03::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Colossus(Prototype_GameObject_Ice03_Yeti01)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ice04_Yeti01"), CYeti01_Ice04::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Colossus(Prototype_GameObject_ice04_Yeti01)");
		return E_FAIL;
	}
	// 예티 오브젝트 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Yeti"), CYeti::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Yeti(GameObject_Yeti)");
		return E_FAIL;
	}


	// 예티 스노우볼 오브젝트 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Yeti_SnowBall"), CYeti_SnowBall::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Yeti(GameObject_Yeti_SnowBall)");
		return E_FAIL;
	}

	// 고드름 오브젝트 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Icicle"), CIcicle::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Yeti(GameObject_Icicle)");
		return E_FAIL;
	}

	// 고드름 그림자 오브젝트 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Icicle_Shadow"), CIcicle_Shadow::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Yeti(GameObject_Icicle_Shadow)");
		return E_FAIL;
	}
	// 예티 그림자 오브젝트 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Yeti_Shadow"), CYeti_Shadow::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Yeti(GameObject_Yeti_Shadow)");
		return E_FAIL;
	}
	// 스노우볼 그림자 오브젝트 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_SnowBall_Shadow"), CSnowBall_Shadow::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Yeti(GameObject_SnowBall_Shadow)");
		return E_FAIL;
	}

	// 스노우볼 잔상 오브젝트 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_SnowBall_afterimage"), CSnowBall_afterimage::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Yeti(GameObject_SnowBall_afterimage)");
		return E_FAIL;
	}

	// 예티 잔상 오브젝트 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Yeti_afterimage"), CYeti_afterimage::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Yeti(Prototype_GameObject_Yeti_afterimage)");
		return E_FAIL;
	}

	// 예티 먼지 오브젝트 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Yeti_Dust"), CYeti_Dust::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Yeti(Prototype_GameObject_Yeti_Dust)");
		return E_FAIL;
	}

	// 예티 파티클 오브젝트 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Yeti_Paticle"), CYeti_Snow_Dust::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Yeti(Prototype_GameObject_Yeti_Paticle)");
		return E_FAIL;
	}
	// 예티 스노우볼 파티클 빅 오브젝트 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Yeti_Paticle_Big"), CYeti_Snow_Dust_Big::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Yeti(Prototype_GameObject_Yeti_Paticle_Big)");
		return E_FAIL;
	}

	// 예티 스노우볼 부서짐 파티클  오브젝트 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Yeti_SnowBall_Break_Effect"), CYeti_Snow_Break_Effect::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Yeti(Prototype_GameObject_Yeti_SnowBall_Break_Effect)");
		return E_FAIL;
	}

	// 예티 점프 먼지 오브젝트 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Yeti_Jump_Dust"), CYeti_Jump_Dust::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Yeti(Prototype_GameObject_Yeti_Jump_Dust)");
		return E_FAIL;
	}
	// 고드름 부서짐 이펙트 오브젝트 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Icicle_Break_Effect"), CIcicle_Break_Effect::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Yeti(Prototype_GameObject_Icicle_Break_Effect)");
		return E_FAIL;
	}

	// 원거리 몬스터 오브젝트 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Archor"), CMonster_Archor::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Yeti(Prototype_GameObject_Monster_Archor)");
		return E_FAIL;
	}

	// 원거리 몬스터 총알 오브젝트 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Archor_Buller"), CArchor_Bullet::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Yeti(Prototype_GameObject_Monster_Archor_Buller)");
		return E_FAIL;
	}

	// 근거리 몬스터 오브젝트 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Short_Range"), CMonster_Short_Range::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Yeti(Prototype_GameObject_Monster_Short_Range)");
		return E_FAIL;
	}
	// 근거리 몬스터 총알 오브젝트 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Short_Bullet"), CMonster_Short_Attack::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Yeti(Prototype_GameObject_Monster_Short_Bullet)");
		return E_FAIL;
	}
	/*-*/
	//0311추가
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_FIELD, TEXT("Prototype_Component_Texture_DoorLight_Field01"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Door_Light.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(Prototype_Component_Texture_DoorLight_Field01)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_DoorLight_Field01"), CField01_DoorLight::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(Prototype_GameObject_DoorLight_Field01)");
		return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Torch"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Torch.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(Prototype_Component_Texture_Torch)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Torch"), CTorch::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(Prototype_GameObject_Torch)");
		return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_TorchFire"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Arrow_Rect.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(Prototype_Component_Texture_Torch)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_TorchFire"), CTorch_Fire::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(Prototype_GameObject_Torch)");
		return E_FAIL;
	}


	RELEASE_INSTANCE(CGameInstance);

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Slime()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	// 벽 컴포넌트 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SLIME, TEXT("Prototype_Component_VIBuffer_Wall_Field"), CVIBuffer_Wall::Create(m_pGraphic_Device, 33, 33))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(Component_VIBuffer_Wall)");
		return E_FAIL;
	}
	//Terrain
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SLIME, TEXT("Prototype_Component_VIBuffer_Terrain_Slime01"), CVIBuffer_Terrain::Create(m_pGraphic_Device, 33, 33))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(Prototype_Component_VIBuffer_Terrain_Slime01)");
		return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SLIME, TEXT("Prototype_Component_Texture_Terrain_Slime01"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Map_Slime/Slime01_Terrain.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Slime(Prototype_Component_Texture_Terrain_Slime01)");
		return E_FAIL;
	}


	// Wall

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SLIME, TEXT("Prototype_Component_VIBuffer_Wall_Slime01"), CVIBuffer_Wall::Create(m_pGraphic_Device, 33, 33))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(Prototype_Component_VIBuffer_Wall_Slime01)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SLIME, TEXT("Prototype_Component_Texture_Wall_Slime01"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Map_Slime/Slime01_Wall1.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Slime(Prototype_Component_Texture_Wall_Slime01)");
		return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SLIME, TEXT("Prototype_Component_Texture_Wall2_Slime01"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Map_Slime/Slime01_Wall2.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Slime(Prototype_Component_Texture_Wall2_Slime01)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SLIME, TEXT("Prototype_Component_Texture_Ceiling_Slime01"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Map_Slime/Slime01_Ceiling.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Slime(Prototype_Component_Texture_Ceiling_Slime01)");
		return E_FAIL;
	}

	//Rect

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SLIME, TEXT("Prototype_Component_VIBuffer_Rect_Slime"), CVIBuffer_Rect::Create(m_pGraphic_Device))))
	{
		MSGBOX("m_pGameInstance->Add_prototype returned E_FAIL in CLoader::Loading_For_Slime(Prototype_Component_VIBuffer_Rect_Slime)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SLIME, TEXT("Prototype_Component_Texture_Light_Slime01"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Map_Slime/Slime01_Light.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Slime(Prototype_Component_Texture_Light_Slime01)");
		return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SLIME, TEXT("Prototype_Component_Texture_Mist_Slime01"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Map_Slime/Slime01_Mist%d.png"), 4))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Slime(Prototype_Component_Texture_Light_Slime01)");
		return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SLIME, TEXT("Prototype_Component_Texture_Pipe1_Slime01"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Map_Slime/Slime01_Pipe1.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Slime(Prototype_Component_Texture_Pipe1_Slime01)");
		return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SLIME, TEXT("Prototype_Component_Texture_Pipe2_Slime01"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Map_Slime/Slime01_Pipe2.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Slime(Prototype_Component_Texture_Light_Slime01)");
		return E_FAIL;
	}
	
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SLIME, TEXT("Prototype_Component_Texture_Skull_Slime01"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Map_Slime/Slime01_Skull.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Slime(Prototype_Component_Texture_Skull_Slime01)");
		return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SLIME, TEXT("Prototype_Component_Texture_Legiron_Slime01"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Map_Slime/Slime01_Legiron%d.png"), 4))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Slime(Prototype_Component_Texture_Skull_Slime01)");
		return E_FAIL;
	}



	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SLIME, TEXT("Prototype_Component_Texture_Flask01_Slime01"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Map_Slime/Slime01_Flask1.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Slime(Prototype_Component_Texture_Flask01_Slime01)");
		return E_FAIL;
	}
	
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SLIME, TEXT("Prototype_Component_Texture_Flask02_Slime01"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Map_Slime/Slime01_Flask2.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Slime(Prototype_Component_Texture_Flask02_Slime01)");
		return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SLIME, TEXT("Prototype_Component_Texture_Flask03_Slime01"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Map_Slime/Slime01_Flask3.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Slime(Prototype_Component_Texture_Flask03_Slime01)");
		return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SLIME, TEXT("Prototype_Component_Texture_Water_drop_Slime01"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Map_Slime/Slime01_water_drop%d.png"), 12))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Slime(Prototype_Component_Texture_Water_drop_Slime01)");
		return E_FAIL;
	}

	// 스카이 텍스쳐 원형
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SLIME, TEXT("Prototype_Component_Texture_Sky"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/SkyBox/Sky_%d.dds"), 4))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(Component_Texture_Sky)");
		return E_FAIL;
	}


	// 슬라임 텍스쳐 원형
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SLIME, TEXT("Prototype_Component_Texture_Slime"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Boss/Slime.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Slime(Component_Texture_Slime)");
		return E_FAIL;
	}

	// 슬라임 그림자 텍스쳐 원형
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SLIME, TEXT("Prototype_Component_Texture_Slime_Shadow"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Boss/Slime_Shadow.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Slime(Component_Texture_Slime_Shadow)");
		return E_FAIL;
	}

	//  심장 미니  텍스쳐 원형
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SLIME, TEXT("Prototype_Component_Texture_Heart_mini"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Boss/Heart_Water_mini.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_IceBrain(Component_Texture_Ice)");
		return E_FAIL;
	}
	//  심장 워터 텍스쳐 원형
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SLIME, TEXT("Prototype_Component_Texture_Heart_Water"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Boss/Heart_Water.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_IceBrain(Component_Texture_Ice)");
		return E_FAIL;
	}

	// 슬라임 심장 그림자 텍스쳐 원형
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SLIME, TEXT("Prototype_Component_Texture_Slime_Heart_Shadow"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Boss/Slime_Heart_Shadow.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Slime(Component_Texture_Slime_Heart_Shadow)");
		return E_FAIL;
	}

	// 슬라임 심장 원형
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SLIME, TEXT("Prototype_Component_Texture_Slime_Heart"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Boss/Slime_Heart.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Slime(Component_Texture_Slime_Heart)");
		return E_FAIL;
	}

	// 슬라임 액체 원형
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SLIME, TEXT("Prototype_Component_Texture_Slime_Water"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Boss/Slime_Water.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Slime(Component_Texture_Slime_Water)");
		return E_FAIL;
	}


	RELEASE_INSTANCE(CGameInstance);

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_IceBrain()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	// 벽 컴포넌트 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_ICEBRAIN, TEXT("Prototype_Component_VIBuffer_Wall_Field"), CVIBuffer_Wall::Create(m_pGraphic_Device, 33, 33))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(Component_VIBuffer_Wall)");
		return E_FAIL;
	}

	// 스카이 텍스쳐 원형
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_ICEBRAIN, TEXT("Prototype_Component_Texture_Sky"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/SkyBox/Sky_%d.dds"), 4))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_IceBrain(Component_Texture_Sky)");
		return E_FAIL;
	}

	// 파이어버튼 텍스쳐 원형
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_ICEBRAIN, TEXT("Prototype_Component_Texture_FireButton"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Boss/Button_Fire%d.png"), 2))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_IceBrain(Component_Texture_FireButton)");
		return E_FAIL;
	}
	// 파이어 텍스쳐 원형
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_ICEBRAIN, TEXT("Prototype_Component_Texture_Fire"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Boss/Fire%d.png"), 3))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_IceBrain(Component_Texture_FireButton)");
		return E_FAIL;
	}


	//Terrain
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_ICEBRAIN, TEXT("Prototype_Component_VIBuffer_Terrain_IceBrain01"), CVIBuffer_Terrain::Create(m_pGraphic_Device, 33, 33))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_IceBrain(Prototype_Component_VIBuffer_Terrain_IceBrain01)");
		return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_ICEBRAIN, TEXT("Prototype_Component_Texture_Terrain_IceBrain01"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Map_IceCube/IceBrain01_Terrain.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_IceBrain(Prototype_Component_Texture_Terrain_IceBrain01)");
		return E_FAIL;
	}
	

	// Wall

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_ICEBRAIN, TEXT("Prototype_Component_VIBuffer_Wall1_IceBrain01"), CVIBuffer_Wall::Create(m_pGraphic_Device, 33, 33))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_IceBrain(Prototype_Component_VIBuffer_Wall1_IceBrain01)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_ICEBRAIN, TEXT("Prototype_Component_Texture_Wall1_IceBrain01"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Map_IceCube/IceBrain01_Wall.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_IceBrain(Prototype_Component_Texture_Wall1_IceBrain01)");
		return E_FAIL;
	}
	
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_ICEBRAIN, TEXT("Prototype_Component_Texture_Wall2_IceBrain01"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Map_IceCube/IceBrain01_Wall2.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_IceBrain(Prototype_Component_Texture_Wall2_IceBrain01)");
		return E_FAIL;
	}

	//+++
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_ICEBRAIN, TEXT("Prototype_Component_Texture_Wall3_IceBrain01"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Map_IceCube/IceBrain01_Wall3.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_IceBrain(Prototype_Component_Texture_Wall3_IceBrain01)");
		return E_FAIL;
	}
	

	// 브레인 텍스쳐 원형
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_ICEBRAIN, TEXT("Prototype_Component_Texture_Brain"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Boss/Brain.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_IceBrain(Component_Texture_Brain)");
		return E_FAIL;
	}


	// 브레인그림자 텍스쳐 원형
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_ICEBRAIN, TEXT("Prototype_Component_Texture_Brain_Shadow"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Boss/Brain_Shadow.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_IceBrain(Component_Texture_FireButton)");
		return E_FAIL;
	}

	// 아이스 텍스쳐 원형
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_ICEBRAIN, TEXT("Prototype_Component_Texture_Ice"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/Boss/Ice.dds")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_IceBrain(Component_Texture_Ice)");
		return E_FAIL;
	}
	// 아이스 그림자 텍스쳐 원형
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_ICEBRAIN, TEXT("Prototype_Component_Texture_Ice_Shadow"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Boss/IceShadow.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_IceBrain(Component_Texture_Ice)");
		return E_FAIL;
	}

	// 브레인 워터 미니 텍스쳐 원형
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_ICEBRAIN, TEXT("Prototype_Component_Texture_Brain_water_mini"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Boss/Heart_Water_mini.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_IceBrain(Component_Texture_Ice)");
		return E_FAIL;
	}
	// 브레인 워터 텍스쳐 원형
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_ICEBRAIN, TEXT("Prototype_Component_Texture_Brain_water"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Boss/Heart_Water.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_IceBrain(Component_Texture_Ice)");
		return E_FAIL;
	}
	

	RELEASE_INSTANCE(CGameInstance);

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Colossus()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	// 벽 컴포넌트 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_COLOSUSS, TEXT("Prototype_Component_VIBuffer_Wall_Field"), CVIBuffer_Wall::Create(m_pGraphic_Device, 33, 33))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(Component_VIBuffer_Wall)");
		return E_FAIL;
	}

	// 스카이 박스 텍스쳐 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_COLOSUSS, TEXT("Prototype_Component_Texture_Sky"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/SkyBox/Sky_%d.dds"), 4))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Colossus(Component_Texture_Sky)");
		return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_COLOSUSS, TEXT("Prototype_Component_VIBuffer_Wall1_IceBrain01"), CVIBuffer_Wall::Create(m_pGraphic_Device, 33, 33))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_IceBrain(Prototype_Component_VIBuffer_Wall1_IceBrain01)");
		return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_COLOSUSS, TEXT("Prototype_Component_Texture_Wall3_IceBrain01"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Map_IceCube/IceBrain01_Wall3.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_IceBrain(Prototype_Component_Texture_Wall3_IceBrain01)");
		return E_FAIL;
	}

	//Terrain
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_COLOSUSS, TEXT("Prototype_Component_VIBuffer_Terrain_Colossus01"), CVIBuffer_Terrain::Create(m_pGraphic_Device, 33, 33))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Colossus(Prototype_Component_VIBuffer_Terrain_Colossus01)");
		return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_COLOSUSS, TEXT("Prototype_Component_Texture_Terrain_Colossus01"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Map_Colossus/Colossus01_Terrain.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Colossus(Prototype_Component_Texture_Terrain_Colossus01)");
		return E_FAIL;
	}
	

	//TerrainEx

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_COLOSUSS, TEXT("Prototype_Component_VIBuffer_TerrainEx_Colossus01"), CVIBuffer_Ceiling::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Map_Colossus/Colossus01_TerrainExHight.bmp")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Colossus(Prototype_Component_VIBuffer_Terrain_Colossus01)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_COLOSUSS, TEXT("Prototype_Component_Texture_TerrainEx01_Colossus01"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Map_Colossus/Colossus01_TerrainEx.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Colossus(Prototype_Component_Texture_Terrain_Colossus01)");
		return E_FAIL;
	}
	

	// Wall

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_COLOSUSS, TEXT("Prototype_Component_VIBuffer_Wall1_Colossus01"), CVIBuffer_Wall::Create(m_pGraphic_Device, 33, 33))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Colossus(Prototype_Component_VIBuffer_Wall1_Colossus01)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_COLOSUSS, TEXT("Prototype_Component_Texture_Wall1_Colossus01"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Map_Colossus/Colossus01_Wall.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Colossus(Prototype_Component_VIBuffer_Wall1_Colossus01)");
		return E_FAIL;
	}
	
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_COLOSUSS, TEXT("Prototype_Component_Texture_Wall2_Colossus01"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Map_Colossus/Colossus01_Wall2.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Colossus(Prototype_Component_Texture_Wall2_Colossus01)");
		return E_FAIL;
	}

	

	// Rect

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_COLOSUSS, TEXT("Prototype_Component_VIBuffer_CeilingEx_Colossus01"), CVIBuffer_Ceiling::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Map_Colossus/Colossus01_CeilingExHight.bmp")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Colossus(Prototype_Component_VIBuffer_Terrain_Colossus01)");
		return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_COLOSUSS, TEXT("Prototype_Component_Texture_CeilingEx_Colossus01"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Map_Colossus/Colossus01_CeilingEx.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Colossus(Prototype_Component_Texture_Terrain_Colossus01)");
		return E_FAIL;
	}
	

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_COLOSUSS, TEXT("Prototype_Component_VIBuffer_CeilingEx2_Colossus01"), CVIBuffer_Ceiling::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Map_Colossus/Colossus01_CeilingEx2Hight.bmp")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Colossus(Prototype_Component_VIBuffer_CeilingEx2_Colossus01)");
		return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_COLOSUSS, TEXT("Prototype_Component_Texture_CeilingEx2_Colossus01"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Map_Colossus/Colossus01_CeilingEx02.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Colossus(Prototype_Component_Texture_CeilingEx2_Colossus01)");
		return E_FAIL;
	}
	

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_COLOSUSS, TEXT("Prototype_Component_VIBuffer_Dark01_Colossus01"), CVIBuffer_Ceiling::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Map_Colossus/Colossus01_Dark01Hight.bmp")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Colossus(Prototype_Component_VIBuffer_Dark01_Colossus01)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_COLOSUSS, TEXT("Prototype_Component_Texture_Dark01_Colossus01"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Map_Colossus/Colossus01_Dark.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Colossus(Prototype_Component_Texture_Dark01_Colossus01)");
		return E_FAIL;
	}
	pGameInstance->Add_Prototype(LEVEL_COLOSUSS, TEXT("Prototype_Component_Texture_Colossus01_Grass02"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Grass02_%d.png"), 3));
	

	// 엔딩 문
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_COLOSUSS, TEXT("Prototype_Component_Texture_EndDoor"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Ending/EndDoor.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Slime(Prototype_Component_Texture_EndDoor)");
		return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_COLOSUSS, TEXT("Prototype_Component_Texture_EndDoorLeft"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Ending/EndDoorLeft.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Slime(Prototype_Component_Texture_EndDoorLeft)");
		return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_COLOSUSS, TEXT("Prototype_Component_Texture_EndDoorRight"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Ending/EndDoorRight.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Slime(Prototype_Component_Texture_EndDoorRight)");
		return E_FAIL;
	}

	

	// 콜로서스 피스트 텍스쳐 원형
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_COLOSUSS, TEXT("Prototype_Component_Texture_Colossus_Fist"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Boss/Colossus_Fist.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Colossus_Fist(Component_Texture_Colossus)");
		return E_FAIL;
	}
	// 콜로서스 피스트 오른쪽 텍스쳐 원형
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_COLOSUSS, TEXT("Prototype_Component_Texture_Colossus_Fist_Right"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Boss/Colossus_Fist_Right.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Colossus_Fist_Right(Component_Texture_Colossus)");
		return E_FAIL;
	}
	// 콜로서스 피스트 그림자 텍스쳐 원형
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_COLOSUSS, TEXT("Prototype_Component_Texture_Colossus_Fist_Shadow"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Boss/Colossus_Fist_Shadow.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Colossus_Fist(Component_Texture_Colossus)");
		return E_FAIL;
	}

	// 콜로서스 본체  텍스쳐 원형
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_COLOSUSS, TEXT("Prototype_Component_Texture_Colossus"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Boss/Colossus.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Colossus_Fist(Component_Texture_Colossus)");
		return E_FAIL;
	}

	// 콜로서스 먼지  텍스쳐 원형
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_COLOSUSS, TEXT("Prototype_Component_Texture_Colossus_Dust"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Boss/Dust.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Colossus_Fist(Component_Texture_Colossus)");
		return E_FAIL;
	}

	// 종유석  텍스쳐 원형
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_COLOSUSS, TEXT("Prototype_Component_Texture_Jongyu"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Boss/Jongyu.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Colossus_Fist(Prototype_Component_Texture_Jongyu)");
		return E_FAIL;
	}
	// 종유석 그림자  텍스쳐 원형
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_COLOSUSS, TEXT("Prototype_Component_Texture_Jongyu_Shadow"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Boss/Icicle_Shadow.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Colossus_Fist(Prototype_Component_Texture_Jongyu)");
		return E_FAIL;
	}

	// 종유석 부서짐  텍스쳐 원형
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_COLOSUSS, TEXT("Prototype_Component_Texture_Jongyu_Break"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Boss/Jungyu_Break_Sprite.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Colossus_Fist(Prototype_Component_Texture_Jongyu)");
		return E_FAIL;
	}
	

	RELEASE_INSTANCE(CGameInstance);

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_EyeCube()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	// 벽 컴포넌트 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_EYECUBE, TEXT("Prototype_Component_VIBuffer_Wall_Field"), CVIBuffer_Wall::Create(m_pGraphic_Device, 33, 33))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(Component_VIBuffer_Wall)");
		return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_EYECUBE, TEXT("Prototype_Component_Texture_Sky"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/SkyBox/Sky_%d.dds"), 4))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_EyeCube(Component_Texture_Sky)");
		return E_FAIL;
	}

	// 추가 된 것 =======================================================================================================================================
	//===================================================================================================================================================

	//Terrain
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_EYECUBE, TEXT("Prototype_Component_VIBuffer_Terrain_EyeCube01"), CVIBuffer_Terrain::Create(m_pGraphic_Device, 33, 33))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_EyeCube(Prototype_Component_VIBuffer_Terrain_EyeCube01)");
		return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_EYECUBE, TEXT("Prototype_Component_Texture_Terrain_EyeCube01"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Map_EyeCube/EyeCube01_Terrain.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_EyeCube(Prototype_Component_Texture_Terrain_EyeCube01)");
		return E_FAIL;
	}
	

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_EYECUBE, TEXT("Prototype_Component_Texture_Terrain2_EyeCube01"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Map_EyeCube/EyeCube01_Terrain2.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Colossus(Prototype_Component_Texture_Terrain2_EyeCube01)");
		return E_FAIL;
	}
	

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_EYECUBE, TEXT("Prototype_Component_Texture_Terrain3_EyeCube01"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Map_EyeCube/EyeCube01_Terrain3.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Colossus(Prototype_Component_Texture_Terrain3_EyeCube01)");
		return E_FAIL;
	}
	

	// Wall

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_EYECUBE, TEXT("Prototype_Component_VIBuffer_Wall1_EyeCube01"), CVIBuffer_Wall::Create(m_pGraphic_Device, 33, 33))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_EyeCube(Prototype_Component_VIBuffer_Wall1_EyeCube01)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_EYECUBE, TEXT("Prototype_Component_Texture_Wall1_EyeCube01"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Map_EyeCube/EyeCube01_Wall1.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_EyeCube(Prototype_Component_Texture_Wall1_EyeCube01)");
		return E_FAIL;
	}
	
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_EYECUBE, TEXT("Prototype_Component_Texture_Wall2_EyeCube01"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Map_EyeCube/EyeCube01_Wall2.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_EyeCube(Prototype_Component_Texture_Wall2_EyeCube01)");
		return E_FAIL;
	}
	


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_EYECUBE, TEXT("Prototype_Component_Texture_Wallex_EyeCube01"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Map_EyeCube/EyeCube01_Wallex.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_EyeCube(Prototype_Component_Texture_Wallex_EyeCube01)");
		return E_FAIL;
	}
	

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_EYECUBE, TEXT("Prototype_Component_Texture_Wallex2_EyeCube01"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Map_EyeCube/EyeCube01_Wallex.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Colossus(Prototype_Component_Texture_Wallex2_EyeCube01)");
		return E_FAIL;
	}


	// Chess

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_EYECUBE, TEXT("Prototype_Component_Texture_Chess1_EyeCube01"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Map_EyeCube/EyeCube_Chess1.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_EyeCube(Prototype_Component_Texture_Chess1_EyeCube01)");
		return E_FAIL;
	}


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_EYECUBE, TEXT("Prototype_Component_Texture_Chess2_EyeCube01"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Map_EyeCube/EyeCube_Chess2.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_EyeCube(Prototype_Component_Texture_Chess2_EyeCube01)");
		return E_FAIL;
	}


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_EYECUBE, TEXT("Prototype_Component_Texture_Chess3_EyeCube01"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Map_EyeCube/EyeCube_Chess3.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_EyeCube(Prototype_Component_Texture_Chess3_EyeCube01)");
		return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_EYECUBE, TEXT("Prototype_Component_Texture_Chess4_EyeCube01"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Map_EyeCube/EyeCube_Chess4.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_EyeCube(Prototype_Component_Texture_Ches4_EyeCube01)");
		return E_FAIL;
	}

	// 아이큐브 텍스쳐 원형
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_EYECUBE, TEXT("Prototype_Component_Texture_EyeCube"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/Boss/TestBlack%d.dds"), 4))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Colossus_Fist(Component_Texture_EyeCube)");
		return E_FAIL;
	}

	// 아이큐브 레이저 원형
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_EYECUBE, TEXT("Prototype_Component_Texture_EyeCube_Rayzer"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/Boss/Rayzer.dds")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Colossus_Fist(Component_Texture_EyeCube)");
		return E_FAIL;
	}

	// 아이큐브 그림자 텍스쳐 원형
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_EYECUBE, TEXT("Prototype_Component_Texture_EyeCube_Shadow"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Boss/CubeShadow.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Colossus_Fist(Component_Texture_EyeCube_Shadow)");
		return E_FAIL;
	}

	// 아이큐브 레이저 큐브 컴포넌트 원형생성
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_EYECUBE, TEXT("Prototype_Component_VIBuffer_Cube_Rayzer"), CVIBuffer_Cube_Rayzer::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_EyeCube(Prototype_Component_Cube_Rayzer)");
		return E_FAIL;
	}




	RELEASE_INSTANCE(CGameInstance);

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Yeti()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	// 벽 컴포넌트 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_YETI, TEXT("Prototype_Component_VIBuffer_Wall_Field"), CVIBuffer_Wall::Create(m_pGraphic_Device, 33, 33))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(Component_VIBuffer_Wall)");
		return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_YETI, TEXT("Prototype_Component_Texture_Sky"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/SkyBox/Sky_%d.dds"), 4))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Yeti(Component_Texture_Sky)");
		return E_FAIL;
	}

	// 추가 된 것 =======================================================================================================================================
	//===================================================================================================================================================

	//Terrain
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_YETI, TEXT("Prototype_Component_VIBuffer_Terrain_Yeti01"), CVIBuffer_Terrain::Create(m_pGraphic_Device, 33, 33))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Yeti(Prototype_Component_VIBuffer_Terrain_Yeti01)");
		return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_YETI, TEXT("Prototype_Component_Texture_Terrain_Yeti01"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Map_Yeti/Yeti_Terrain.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Yeti(Prototype_Component_Texture_Terrain_Yeti01)");
		return E_FAIL;
	}
	
	


	// Wall

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_YETI, TEXT("Prototype_Component_VIBuffer_Wall1_Yeti01"), CVIBuffer_Wall::Create(m_pGraphic_Device, 33, 33))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Yeti(Prototype_Component_VIBuffer_Wall1_Yeti01)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_YETI, TEXT("Prototype_Component_Texture_Wall1_Yeti01"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Map_Yeti/Yeti_Wall1.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Yeti(Prototype_Component_Texture_Wall1_Yeti01)");
		return E_FAIL;
	}
	
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_YETI, TEXT("Prototype_Component_Texture_Wall2_Yeti01"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Map_Yeti/Yeti_Wall00.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Yeti(Prototype_Component_Texture_Wall2_Yeti01)");
		return E_FAIL;
	}
	

	// Ceiling
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_YETI, TEXT("Prototype_Component_VIBuffer_Ceiling01_Yeti01"), CVIBuffer_Ceiling::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Map_Yeti/Sprite-0001.bmp")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Colossus(Prototype_Component_VIBuffer_Ceiling01_Yeti01)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_YETI, TEXT("Prototype_Component_Texture_Ceiling01_Yeti01"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Map_Yeti/Yeti01_Ice1.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Colossus(Prototype_Component_Texture_Ceiling01_Yeti01)");
		return E_FAIL;
	}
	

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_YETI, TEXT("Prototype_Component_VIBuffer_Ceiling02_Yeti01"), CVIBuffer_Ceiling::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Map_Yeti/Sprite-0002.bmp")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Colossus(Prototype_Component_VIBuffer_Ceiling01_Yeti01)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_YETI, TEXT("Prototype_Component_Texture_Ceiling02_Yeti01"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Map_Yeti/Yeti01_Ice1.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Colossus(Prototype_Component_Texture_Ceiling01_Yeti01)");
		return E_FAIL;
	}


	//
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_YETI, TEXT("Prototype_Component_VIBuffer_Terrain3_Yeti01"), CVIBuffer_Ceiling::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Map_Yeti/Sprite-0003.bmp")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Colossus(Prototype_Component_VIBuffer_Ceiling01_Yeti01)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_YETI, TEXT("Prototype_Component_Texture_Terrain3_Yeti01"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Map_Yeti/Yeti01_Ice1.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Colossus(Prototype_Component_Texture_Ceiling01_Yeti01)");
		return E_FAIL;
	}

	//Rect

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_YETI, TEXT("Prototype_Component_Texture_Terrain02_Yeti01"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Map_Yeti/Yeti_Terrain2.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Colossus(Prototype_Component_Texture_Terrain02_Yeti01)");
		return E_FAIL;
	}


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_YETI, TEXT("Prototype_Component_Texture_Ice03_Yeti01"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Map_Yeti/Yeti01_Ice03.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Colossus(Prototype_Component_Texture_Ice03_Yeti01)");
		return E_FAIL;
	}


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_YETI, TEXT("Prototype_Component_Texture_Ice04_Yeti01"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Map_Yeti/Yeti01_Ice04.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Colossus(Prototype_Component_Texture_Ice04_Yeti01)");
		return E_FAIL;
	}


	// 예티 텍스쳐 원형
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_YETI, TEXT("Prototype_Component_Texture_Yeti"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Boss/Yeti.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Yeti(Component_Texture_Yeti)");
		return E_FAIL;
	}

	// 예티 스노우볼 텍스쳐 원형
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_YETI, TEXT("Prototype_Component_Texture_Yeti_SnowBall"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Boss/Yeti_SnowBall.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Yeti(Component_Texture_Yeti_SnowBall)");
		return E_FAIL;
	}

	// 고드름 텍스쳐 원형
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_YETI, TEXT("Prototype_Component_Texture_Icicle"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Boss/Icicle.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Yeti(Component_Texture_Icicle)");
		return E_FAIL;
	}

	// 고드름 그림자 텍스쳐 원형
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_YETI, TEXT("Prototype_Component_Texture_Icicle_Shadow"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Boss/Icicle_Shadow.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Yeti(Component_Texture_Icicle_Shadow)");
		return E_FAIL;
	}
	// 예티 그림자 텍스쳐 원형
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_YETI, TEXT("Prototype_Component_Texture_Yeti_Shadow"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Boss/yeti_Shadow.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Yeti(Component_Texture_Yeti_Shadow)");
		return E_FAIL;
	}

	// 예티 먼지 텍스쳐 원형
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_YETI, TEXT("Prototype_Component_Texture_Yeti_Dust"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Boss/Ice_Dust.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Yeti(Component_Texture_Yeti_Shadow)");
		return E_FAIL;
	}

	// 예티 파티클 텍스쳐 원형
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_YETI, TEXT("Prototype_Component_Texture_Yeti_Paticle"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Boss/Ice_Paticle.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Yeti(Component_Texture_Yeti_Shadow)");
		return E_FAIL;
	}

	// 고드름 부서짐 텍스쳐 원형
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_YETI, TEXT("Prototype_Component_Texture_Icicle_Break"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Boss/Icicle_Break_Sprite.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Yeti(Component_Texture_Yeti_Shadow)");
		return E_FAIL;
	}







	RELEASE_INSTANCE(CGameInstance);
	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Respawn1()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	// 벽 컴포넌트 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_RESPAWN1, TEXT("Prototype_Component_VIBuffer_Wall_Field"), CVIBuffer_Wall::Create(m_pGraphic_Device, 33, 33))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(Component_VIBuffer_Wall)");
		return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_RESPAWN1, TEXT("Prototype_Component_Texture_Sky"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/SkyBox/Sky_%d.dds"), 4))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Respawn1(Component_Texture_Sky)");
		return E_FAIL;
	}

	//Terrain

	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_RESPAWN1, TEXT("Prototype_Component_VIBuffer_Terrain_Field"), CVIBuffer_Terrain::Create(m_pGraphic_Device, 33, 33))))
	//{
	//	MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(Component_VIBuffer_Terrain_Field)");
	//	return E_FAIL;
	//}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_RESPAWN1, TEXT("Prototype_Component_VIBuffer_Terrain1_Respawn1"), CVIBuffer_Terrain::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Map_Respawn1/Respawn1_Height.bmp")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Respawn1(Prototype_Component_VIBuffer_Terrain1_Respawn1)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_RESPAWN1, TEXT("Prototype_Component_Texture_Terrain1_Respawn1"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Map_Respawn1/Respawn01_Terrain01.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Respawn1(Prototype_Component_Texture_Terrain1_Respawn1)");
		return E_FAIL;
	}



	if (FAILED(pGameInstance->Add_Prototype(LEVEL_RESPAWN1, TEXT("Prototype_Component_VIBuffer_Terrain2_Respawn1"), CVIBuffer_Ceiling::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Map_Respawn1/Respawn1_Height2.bmp")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Respawn1(Prototype_Component_VIBuffer_Terrain2_Respawn1)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_RESPAWN1, TEXT("Prototype_Component_Texture_Terrain2_Respawn1"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Map_Respawn1/Respawn01_Terrain02.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Respawn1(Prototype_Component_Texture_Terrain2_Respawn1)");
		return E_FAIL;
	}


	// Wall

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_RESPAWN1, TEXT("Prototype_Component_VIBuffer_Wall1_Respawn01"), CVIBuffer_Wall::Create(m_pGraphic_Device, 33, 33))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(Prototype_Component_VIBuffer_Wall1_Respawn01)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_RESPAWN1, TEXT("Prototype_Component_Texture_Wall1_Respawn01"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Map_Respawn1/Respawn01_Wall1.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Slime(Prototype_Component_Texture_Wall1_Respawn01)");
		return E_FAIL;
	}


	//Ceiling
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_RESPAWN1, TEXT("Prototype_Component_VIBuffer_Ceiling_Respawn1"), CVIBuffer_Wall::Create(m_pGraphic_Device, 65, 65))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(Prototype_Component_VIBuffer_Ceiling_Respawn1)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_RESPAWN1, TEXT("Prototype_Component_Texture_Ceiling_Respawn1"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Map_Respawn1/Respawn01_Ceiling.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Slime(Prototype_Component_Texture_Ceiling_Respawn1)");
		return E_FAIL;
	}


	//Rect

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_RESPAWN1, TEXT("Prototype_Component_Texture_Grass1_Respawn01"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Map_Respawn1/Respawn01_Grass1.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Slime(Prototype_Component_Texture_Grass1_Respawn01)");
		return E_FAIL;
	}


	pGameInstance->Add_Prototype(LEVEL_RESPAWN1, TEXT("Prototype_Component_Texture_Respawn01_Grass03"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Grass02_%d.png"), 3));

	pGameInstance->Add_Prototype(LEVEL_RESPAWN1, TEXT("Prototype_Component_Texture_Respawn01_Grass02"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Field01_Grass01.png")));

	pGameInstance->Add_Prototype(LEVEL_RESPAWN1, TEXT("Prototype_Component_Texture_Respawn01_Grass04"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Grass04_%d.png"), 3));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_RESPAWN1, TEXT("Prototype_Component_Texture_WallA_Respawn01"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Map_Respawn1/Respawn01_WallA.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Slime(Prototype_Component_Texture_WallA_Respawn01)");
		return E_FAIL;
	}


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_RESPAWN1, TEXT("Prototype_Component_Texture_WallB_Respawn01"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Map_Respawn1/Respawn01_WallB.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Slime(Prototype_Component_Texture_WallA_Respawn01)");
		return E_FAIL;
	}

	// 근거리 몬스터 텍스쳐 원형
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_RESPAWN1, TEXT("Prototype_Component_Texture_Monster_Sword"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Boss/Sword.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Yeti(Prototype_Component_Texture_Monster_Archor)");
		return E_FAIL;
	}

	// 근거리 몬스터  공격 텍스쳐 원형
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_RESPAWN1, TEXT("Prototype_Component_Texture_Monster_SWORD_Bullet"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Boss/Sword_Bullet.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Yeti(Prototype_Component_Texture_Monster_Archor)");
		return E_FAIL;
	}


	RELEASE_INSTANCE(CGameInstance);
	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Respawn2()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	// 벽 컴포넌트 원형 생성
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_RESPAWN2, TEXT("Prototype_Component_VIBuffer_Wall_Field"), CVIBuffer_Wall::Create(m_pGraphic_Device, 33, 33))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(Component_VIBuffer_Wall)");
		return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_RESPAWN2, TEXT("Prototype_Component_Texture_Sky"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/SkyBox/Sky_%d.dds"), 4))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Respawn1(Component_Texture_Sky)");
		return E_FAIL;
	}
	// Terrain

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_RESPAWN2, TEXT("Prototype_Component_VIBuffer_Terrain1_Respawn2"), CVIBuffer_Terrain::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Map_Respawn2/Respawn2_Height.bmp")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Respawn1(Prototype_Component_VIBuffer_Terrain1_Respawn1)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_RESPAWN2, TEXT("Prototype_Component_Texture_Terrain1_Respawn2"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Map_Respawn2/Respawn02_Terrain.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Respawn1(Prototype_Component_Texture_Terrain1_Respawn1)");
		return E_FAIL;
	}


	// Ceiling


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_RESPAWN2, TEXT("Prototype_Component_VIBuffer_Ceiling1_Respawn2"), CVIBuffer_Ceiling::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Map_Respawn2/Respawn2_Height2.bmp")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Respawn1(Prototype_Component_VIBuffer_Terrain2_Respawn1)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_RESPAWN2, TEXT("Prototype_Component_Texture_Ceiling1_Respawn2"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Map_Respawn2/Respawn02_Ceiling1.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Colossus(Prototype_Component_Texture_Ceiling01_Yeti01)");
		return E_FAIL;
	}

	//+++
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_RESPAWN2, TEXT("Prototype_Component_VIBuffer_CeilingYeti_Respawn2"), CVIBuffer_Ceiling::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Map_Respawn2/Respawn2_YetiHeight2.bmp")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Respawn1(Prototype_Component_VIBuffer_Terrain2_Respawn1)");
		return E_FAIL;
	}
	//+++
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_RESPAWN2, TEXT("Prototype_Component_Texture_CeilingYeti_Respawn2"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Map_Respawn2/Respawn02_Ceiling1.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Colossus(Prototype_Component_Texture_Ceiling01_Yeti01)");
		return E_FAIL;
	}
	


	// Wall

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_RESPAWN2, TEXT("Prototype_Component_VIBuffer_Wall1_Respawn02"), CVIBuffer_Wall::Create(m_pGraphic_Device, 33, 33))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(Prototype_Component_VIBuffer_Wall1_Respawn01)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_RESPAWN2, TEXT("Prototype_Component_Texture_Wall1_Respawn02"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Map_Respawn1/Respawn01_Wall1.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Slime(Prototype_Component_Texture_Wall1_Respawn01)");
		return E_FAIL;
	}

	//Rect

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_RESPAWN2, TEXT("Prototype_Component_Texture_Ice1_Respawn02"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Map_Respawn2/Respawn02_Ice1.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Slime(Prototype_Component_Texture_WallA_Respawn01)");
		return E_FAIL;
	}

	

	//+++
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_RESPAWN2, TEXT("Prototype_Component_Texture_WallA_Respawn02"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Map_Respawn2/Respawn02_WallA.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Slime(Prototype_Component_Texture_WallA_Respawn02)");
		return E_FAIL;
	}

	//+++
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_RESPAWN2, TEXT("Prototype_Component_Texture_WallB_Respawn02"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Map_Respawn2/Respawn02_WallB.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Slime(Prototype_Component_Texture_WallB_Respawn02)");
		return E_FAIL;
	}
	

	//+++
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_RESPAWN2, TEXT("Prototype_Component_Texture_WallC_Respawn02"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Map_Respawn2/Respawn02_WallC.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Slime(Prototype_Component_Texture_WallB_Respawn02)");
		return E_FAIL;
	}
	

	//+++
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_RESPAWN2, TEXT("Prototype_Component_VIBuffer_Terrain2_Respawn2"), CVIBuffer_Ceiling::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Map_Respawn1/Respawn1_Height2.bmp")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Respawn1(Prototype_Component_VIBuffer_Terrain2_Respawn1)");
		return E_FAIL;
	}

	//+++
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_RESPAWN2, TEXT("Prototype_Component_Texture_Terrain2_Respawn2"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Map_Respawn2/Respawn02_Terrain02.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Respawn1(Prototype_Component_Texture_Terrain2_Respawn1)");
		return E_FAIL;
	}
	//0311추가
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_RESPAWN2, TEXT("Prototype_Component_Texture_IceWall_Respawn02"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Map_Respawn2/Respawn02_IceWall.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(Prototype_Component_Texture_IceWall_Respawn02)");
		return E_FAIL;
	}


	// 원거리 몬스터 텍스쳐 원형
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_RESPAWN2, TEXT("Prototype_Component_Texture_Monster_WIZARD"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Boss/WIZARD.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Yeti(Prototype_Component_Texture_Monster_Archor)");
		return E_FAIL;
	}
	// 원거리 몬스터  공격 텍스쳐 원형
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_RESPAWN2, TEXT("Prototype_Component_Texture_Monster_WIZARD_Bullet"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Boss/Wizard_Bullet.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Yeti(Prototype_Component_Texture_Monster_Archor)");
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);
	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_EndingCredit()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_CREDIT, TEXT("Prototype_Component_Texture_EndingBack"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/UI/UI_EndingBackground.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(Component_Texture_Terrain)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_CREDIT, TEXT("Prototype_Component_Texture_EndingCredit"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/UI/UI_EndingCredit.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(Component_Texture_Terrain)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_EndingBack"), CUI_EndingBack::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(GameObject_EndingBack)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_EndingCredit"), CUI_Credit::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(GameObject_EndingCredit)");
		return E_FAIL;
	}
	RELEASE_INSTANCE(CGameInstance);
	m_isFinished = true;

	return S_OK;
}

CLoader * CLoader::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eNextLevel)
{
	CLoader* pInstance = new CLoader(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct(eNextLevel)))
	{
		MSGBOX("pInstance::NativeConstruct returned E_FAIL in CLoader::Create");
		Safe_Release(pInstance);
	}


	return pInstance;
}

void CLoader::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
	DeleteObject(m_hThread);
	DeleteCriticalSection(&m_CS);
	Safe_Release(m_pGraphic_Device);
}
