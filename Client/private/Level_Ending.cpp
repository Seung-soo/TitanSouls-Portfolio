#include "stdafx.h"
#include "Level_Ending.h"
#include "GameInstance.h"
#include "Camera.h"
#include "Level_Loading.h"
#include "IceBrain01_Ceiling.h"
#include "IceBrain01_Terrain.h"
#include "IceBrain01_Wall1.h"
#include "IceBrain01_Wall2.h"
#include "VIBuffer_Wall.h"
#include "Brother.h"

CLevel_Ending::CLevel_Ending(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel(pGraphic_Device)
{
}

// 필드 레벨이 시작할 때 불러오고 호출해야하는 것들
HRESULT CLevel_Ending::NativeConstruct()
{
	if (0 > __super::NativeConstruct())		// CLevel의 초기화 함수를 호출
	{
		MSGBOX("CLevel::NativeConstruct returned the E_FAIL in CLevel_Ending::NativeConstruct");
		return E_FAIL;
	}


	if (FAILED(Ready_Layer_Terrain(TEXT("Layer_Terrain"))))
	{
		MSGBOX("Ready_Layer_Terrain returned E_FAIL in CLevel_Ending::NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_Wall(TEXT("Layer_Wall"))))
	{
		MSGBOX("Ready_Layer_Wall returned E_FAIL in CLevel_Ending::NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_Brother(TEXT("Layer_Brother"))))
	{
		MSGBOX("Ready_Layer_IceBrain returned E_FAIL in CLevel_Ending::NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_Sky(TEXT("Layer_Sky"))))
	{
		MSGBOX("Ready_Layer_Sky returned E_FAIL in CLevel_Ending::NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_Door(TEXT("Layer_Door"))))
	{
		MSGBOX("Ready_Layer_Door returned E_FAIL in CLevel_Ending::NativeConstruct");
		return E_FAIL;
	}


	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	static_cast<CTransform*>(pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform")))->Set_State(CTransform::STATE_POSITION, _float3(16.f, 0.5f, 3.f));
	static_cast<CTransform*>(pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform")))->LookAtXZ(_float3(16.f, 0.5f, 5.f));

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

// 필드 레벨에서 계속 갱신해줘야하는 것들
_int CLevel_Ending::Tick(_float fTimeDelta)
{
	if (0 > __super::Tick(fTimeDelta))		// CLevel의 갱신 함수를 호출
	{
		MSGBOX("0 > CLevel::Tick in CLevel_Ending::Tick");
		return -1;
	}

	return 0;
}

_int CLevel_Ending::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))	// CLevel의 늦게 갱신하는 함수를 호출
	{
		MSGBOX("0 > CLevel::LateTick in CLevel_Ending::LateTick");
		return -1;
	}

	return 0;
}

// 필드 레벨에서 그려야하는 것들
HRESULT CLevel_Ending::Render()
{
	if (0 > __super::Render())				// CLevel의 그리는 함수를 호출
	{
		MSGBOX("CLevel::Render returned the E_FAIL in CLevel_Ending::Render");
		return E_FAIL;
	}


	return S_OK;
}


HRESULT CLevel_Ending::Ready_Layer_Terrain(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_ENDING, TEXT("Prototype_Component_VIBuffer_Terrain_IceBrain01"), CVIBuffer_Terrain::Create(m_pGraphic_Device, 33, 33))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_IceBrain(Prototype_Component_VIBuffer_Terrain_IceBrain01)");
		return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_ENDING, TEXT("Prototype_Component_Texture_Terrain_IceBrain01"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Map_IceCube/IceBrain01_Terrain.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_IceBrain(Prototype_Component_Texture_Terrain_IceBrain01)");
		return E_FAIL;
	}

	//if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain_IceBrain01"), CIceBrain01_Terrain::Create(m_pGraphic_Device))))
	//{
	//	MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_IceBrain(Prototype_GameObject_Terrain_IceBrain01)");
	//	return E_FAIL;
	//}

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_ENDING, pLayerTag, TEXT("Prototype_GameObject_Terrain_IceBrain01"))))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Ending::Ready_Layer_Terrain");
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Ending::Ready_Layer_Wall(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_ENDING, TEXT("Prototype_Component_VIBuffer_Wall1_IceBrain01"), CVIBuffer_Wall::Create(m_pGraphic_Device, 33, 33))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_IceBrain(Prototype_Component_VIBuffer_Wall1_IceBrain01)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_ENDING, TEXT("Prototype_Component_Texture_Wall1_IceBrain01"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Map_IceCube/IceBrain01_Wall.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_IceBrain(Prototype_Component_Texture_Wall1_IceBrain01)");
		return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_ENDING, TEXT("Prototype_Component_Texture_Wall2_IceBrain01"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Map_IceCube/IceBrain01_Wall2.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_IceBrain(Prototype_Component_Texture_Wall2_IceBrain01)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_ENDING, TEXT("Prototype_Component_Texture_Wall3_IceBrain01"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Map_IceCube/IceBrain01_Wall3.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_IceBrain(Prototype_Component_Texture_Wall3_IceBrain01)");
		return E_FAIL;
	}
	//if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Wall1_IceBrain01"), CIceBrain01_Wall1::Create(m_pGraphic_Device))))
	//{
	//	MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_IceBrain(Prototype_GameObject_Wall1_IceBrain01)");
	//	return E_FAIL;
	//}
	//if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Wall2_IceBrain01"), CIceBrain01_Wall2::Create(m_pGraphic_Device))))
	//{
	//	MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_IceBrain(Prototype_GameObject_Wall2_IceBrain01)");
	//	return E_FAIL;
	//}
	//if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Ceiling_IceBrain01"), CIceBrain01_Ceiling::Create(m_pGraphic_Device))))
	//{
	//	MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_IceBrain(Prototype_GameObject_Ceiling_IceBrain01)");
	//	return E_FAIL;
	//}

	POSANGLE tPosAngle;
	tPosAngle.vPos = _float3(0.f, 0.f, 32.f);
	tPosAngle.fAngle = 0.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_ENDING, pLayerTag, TEXT("Prototype_GameObject_Wall1_IceBrain01"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Ending::Ready_Layer_Wall");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(32.f, 0.f, 0.f);
	tPosAngle.fAngle = 180.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_ENDING, pLayerTag, TEXT("Prototype_GameObject_Wall2_IceBrain01"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Ending::Ready_Layer_Wall");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(0.f, 0.f, 0.f);
	tPosAngle.fAngle = -90.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_ENDING, pLayerTag, TEXT("Prototype_GameObject_Wall1_IceBrain01"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Ending::Ready_Layer_Wall");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(32.f, 0.f, 32.f);
	tPosAngle.fAngle = 90.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_ENDING, pLayerTag, TEXT("Prototype_GameObject_Wall1_IceBrain01"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Ending::Ready_Layer_Wall");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(0.f, 20.f, 32.f);
	tPosAngle.fAngle = 270.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_ENDING, pLayerTag, TEXT("Prototype_GameObject_Ceiling_IceBrain01"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Ending::Ready_Layer_Wall");
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Ending::Ready_Layer_Brother(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_ENDING, TEXT("Prototype_Component_Texture_Brother"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Brother.png")))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLevel_Ending::Ready_Layer_Brothe(Component_Texture_Brother)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Brother"), CBrother::Create(m_pGraphic_Device))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLevel_Ending::Ready_Layer_Brother(GameObject_Brother)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_GameObject(g_eCurrentLevel, TEXT("Layer_Brother"), TEXT("Prototype_GameObject_Brother"))))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Ending::Ready_Layer_Brother(GameObject_Brother)");
		return E_FAIL;
	}
	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CLevel_Ending::Ready_Layer_Sky(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	// 스카이 텍스쳐 원형
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_ENDING, TEXT("Prototype_Component_Texture_Sky"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/SkyBox/Sky_%d.dds"), 4))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLevel_Ending::Ready_Layer_Brothe(Component_Texture_Sky)");
		return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_ENDING, pLayerTag, TEXT("Prototype_GameObject_Sky"))))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Ending::Ready_Layer_Sky");
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Ending::Ready_Layer_Door(const _tchar * pLayerTag)
{

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	POSANGLESCALE tPosAngleScale;
	tPosAngleScale.vPos = _float3(20.f, 0.f, 0.5f);
	tPosAngleScale.fAngle = 180.f;
	tPosAngleScale.fScale = 8.f;


	if (FAILED(pGameInstance->Add_Prototype(g_eCurrentLevel, TEXT("Prototype_Component_VIBuffer_Wall_Field"), CVIBuffer_Wall::Create(m_pGraphic_Device, 33, 33))))
	{
		MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLoader::Loading_For_Field(Component_VIBuffer_Wall)");
		return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_GameObject(g_eCurrentLevel, pLayerTag, TEXT("Prototype_GameObject_WallHide_Field"), &tPosAngleScale)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Ending::Ready_Layer_Door");
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CLevel_Ending * CLevel_Ending::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_Ending*	pInstance = new CLevel_Ending(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("pInstance->NativeConstruct returned the E_FAIL in CLevel_Ending::Create");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Ending::Free()
{
	__super::Free();
}
