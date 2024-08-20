#include "..\public\GameInstance.h"

IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
	: m_pTimer_Manager(CTimer_Manager::GetInstance())
	, m_pGraphic_Device(CGraphic_Device::GetInstance())
	, m_pLevel_Manager(CLevel_Manager::GetInstance())
	, m_pObject_Manager(CObject_Manager::GetInstance())
	, m_pComponent_Manager(CComponent_Manager::GetInstance())
	, m_pInput_Device(CInput_Device::GetInstance())
	, m_pPicking(CPicking::GetInstance())
	, m_pSound_Manager(CSoundMgr::GetInstance())
{
	Safe_AddRef(m_pSound_Manager);
	Safe_AddRef(m_pPicking);
	Safe_AddRef(m_pInput_Device);
	Safe_AddRef(m_pComponent_Manager);
	Safe_AddRef(m_pObject_Manager);
	Safe_AddRef(m_pGraphic_Device);
	Safe_AddRef(m_pTimer_Manager);
	Safe_AddRef(m_pLevel_Manager);
}

HRESULT CGameInstance::Initialize_Engine(HINSTANCE hInstance, const CGraphic_Device::GRAPHICDESC & GraphicDesc, _uint iNumLevels, LPDIRECT3DDEVICE9 * ppOut)
{
	if (nullptr == m_pGraphic_Device)
	{
		MSGBOX("m_pGraphic_Device is nullptr in CGameInstance::Initialize_Engine");
		return E_FAIL;
	}

	if (nullptr == m_pObject_Manager)
	{
		MSGBOX("m_pObject_Manager is nullptr in CGameInstance::Initialize_Engine");
		return E_FAIL;
	}

	if (nullptr == m_pComponent_Manager)
	{
		MSGBOX("m_pComponent_Manager is nullptr in CGameInstance::Initialize_Engine");
		return E_FAIL;
	}

	if (nullptr == m_pSound_Manager)
	{
		MSGBOX("m_pSound_Manager is nullptr in CGameInstance::Initialize_Engine");
		return E_FAIL;
	}

	if (FAILED(m_pGraphic_Device->InitDevice(GraphicDesc, ppOut)))	// 그래픽 디바이스 초기화
	{
		MSGBOX("Graphic_Device initialization failed.");
		return E_FAIL;
	}

	if (FAILED(m_pInput_Device->Ready_Input_Device(hInstance, GraphicDesc.hWnd)))
	{
		MSGBOX("m_pInput_Device->Ready_Input_Device returned E_FAIL in CGameInstance::Initialize_Engine");
		return E_FAIL;
	}

	if (FAILED(m_pObject_Manager->Reserve_Container(iNumLevels)))
	{
		MSGBOX("m_pObject_Manager->Reserve_Container returned E_FAIL in CGameInstance::Initialize_Engine");
		return E_FAIL;
	}

	if (FAILED(m_pComponent_Manager->Reserve_Container(iNumLevels)))
	{
		MSGBOX("m_pComponent_Manager->Reserve_Container returned E_FAIL in CGameInstance::Initialize_Engine");
		return E_FAIL;
	}

	m_pSound_Manager->Initialize();

	//if (FAILED(m_pPicking->NativeConstruct(*ppOut, GraphicDesc.hWnd)))
	//{
	//	MSGBOX("m_pPicking->NativeConstruct returned E_FAIL in CGameInstance::Initialize_Engine");
	//	return E_FAIL;
	//}

	return S_OK;
}

_int CGameInstance::Tick_Engine(_float fTimeDelta)
{
	if (nullptr == m_pLevel_Manager)
	{
		MSGBOX("m_pLevel_Manager is nullptr in CGameInstance::Tick_Engine");
		return -1;
	}

	if (nullptr == m_pObject_Manager)
	{
		MSGBOX("m_pObject_Manager is nullptr in CGameInstance::Tick_Engine");
		return -1;
	}

	if (FAILED(m_pInput_Device->SetUp_InputDeviceState()))
	{
		MSGBOX("m_pInput_Device->SetUp_InputDeviceState returned E_FAIL in CGameInstance::Tick_Engine");
		return -1;
	}

	if (0 > m_pObject_Manager->Tick(fTimeDelta))
	{
		MSGBOX("m_pObject_Manager->Tick returned nagative number in CGameInstance::Tick_Engine");
		return -1;
	}

	if (0 > m_pLevel_Manager->Tick(fTimeDelta))
	{
		MSGBOX("m_pLevel_Manager->Tick returned nagative number in CGameInstance::Tick_Engine");
		return -1;
	}

	//if (FAILED(m_pPicking->Transform_ToWorldSpace()))
	//{
	//	MSGBOX("m_pPicking->Transform_ToWorldSpace returned nagative number in CGameInstance::Tick_Engine");
	//	return -1;
	//}

	if (0 > m_pObject_Manager->LateTick(fTimeDelta))
	{
		MSGBOX("m_pObject_Manager->LateTick returned nagative number in CGameInstance::Tick_Engine");
		return -1;
	}

	if (0 > m_pLevel_Manager->LateTick(fTimeDelta))
	{
		MSGBOX("m_pLevel_Manager->LateTick returned nagative number in CGameInstance::Tick_Engine");
		return -1;
	}

	return 0;
}

HRESULT CGameInstance::Clear_LevelResource(_uint iLevelIndex)
{
	if (nullptr == m_pObject_Manager)
	{
		MSGBOX("m_pObject_Manager is nullptr in CGameInstance::Clear_LevelResource");
		return E_FAIL;
	}

	if (nullptr == m_pComponent_Manager)
	{
		MSGBOX("m_pComponent_Manager is nullptr in CGameInstance::Clear_LevelResource");
		return E_FAIL;
	}

	if (FAILED(m_pComponent_Manager->Clear_LevelComponent(iLevelIndex)))
	{
		MSGBOX("m_pComponent_Manager->Clear_LevelComponent returned E_FAIL in CGameInstance::Clear_LevelResource");
		return E_FAIL;
	}

	if (FAILED(m_pObject_Manager->Clear_LevelObject(iLevelIndex)))
	{
		MSGBOX("m_pObject_Manager->Clear_LevelObject returned E_FAIL in CGameInstance::Clear_LevelResource");
		return E_FAIL;
	}

	return S_OK;
}

void CGameInstance::Render_Begin()
{
	if (nullptr == m_pGraphic_Device)
	{
		MSGBOX("CGameInstance::m_pGraphic_Device is nullptr in CGameInstance::Render_Begin");
		return;
	}

	return m_pGraphic_Device->Render_Begin();
}

void CGameInstance::Render_End()
{
	if (nullptr == m_pGraphic_Device)
	{
		MSGBOX("CGameInstance::m_pGraphic_Device is nullptr in CGameInstance::Render_End");
		return;
	}

	return m_pGraphic_Device->Render_End();
}

_float CGameInstance::Get_TimeDelta(const _tchar* pTimerTag)
{
	if (nullptr == m_pTimer_Manager)
	{
		MSGBOX("CGameInstance::m_pGraphic_Device is nullptr in CGameInstance::Get_TimeDelta");
		return 0.f;
	}

	return m_pTimer_Manager->Get_TimeDelta(pTimerTag);
}

HRESULT CGameInstance::Add_Timer(const _tchar* pTimerTag)
{
	if (nullptr == m_pTimer_Manager)
	{
		MSGBOX("CGameInstance::m_pTimer_Manager is nullptr in CGameInstance::Add_Timer");
		return E_FAIL;
	}

	return m_pTimer_Manager->Add_Timer(pTimerTag);
}

HRESULT CGameInstance::OpenLevel(_uint iNextLevelIndex, CLevel * pNextLevel)
{
	if (nullptr == m_pLevel_Manager)
	{
		MSGBOX("m_pLevel_Manager is nullptr in CGameInstance::OpenLevel");
		return E_FAIL;
	}

	return m_pLevel_Manager->OpenLevel(iNextLevelIndex, pNextLevel);
}

HRESULT CGameInstance::Render_Level()
{
	if (nullptr == m_pLevel_Manager)
	{
		MSGBOX("m_pLevel_Manager is nullptr in CGameInstance::Render_Level");
		return E_FAIL;
	}

	return m_pLevel_Manager->Render();
}

CLayer * CGameInstance::Find_Layer(_uint iLevelIndex, const _tchar * pLayerTag)
{
	if (nullptr == m_pObject_Manager)
	{
		MSGBOX("m_pObject_Manager is nullptr in CGameInstance::Find_Layer");
		return nullptr;
	}

	return m_pObject_Manager->Find_Layer(iLevelIndex, pLayerTag);
}

CComponent * CGameInstance::Get_Component(_uint iLevelIndex, const _tchar * pLayerTag, const _tchar * pComponentTag, _uint iIndex)
{
	if (nullptr == m_pObject_Manager)
	{
		MSGBOX("m_pObject_Manager is nullptr in CGameInstance::Get_Component");
		return nullptr;
	}

	return m_pObject_Manager->Get_Component(iLevelIndex, pLayerTag, pComponentTag, iIndex);
}

HRESULT CGameInstance::Add_Prototype(const _tchar * pPrototypeTag, CGameObject * pPrototype)
{
	if (nullptr == m_pObject_Manager)
	{
		MSGBOX("m_pObject_Manager is nullptr in CGameInstance::Add_Prototype");
		return E_FAIL;
	}

	return m_pObject_Manager->Add_Prototype(pPrototypeTag, pPrototype);
}

HRESULT CGameInstance::Add_GameObject(_uint iLevelIndex, const _tchar * pLayerTag, const _tchar * pPrototypeTag, void * pArg)
{
	if (nullptr == m_pObject_Manager)
	{
		MSGBOX("m_pObject_Manager is nullptr in CGameInstance::Add_GameObject");
		return E_FAIL;
	}

	return m_pObject_Manager->Add_GameObject(iLevelIndex, pLayerTag, pPrototypeTag, pArg);
}

HRESULT CGameInstance::Release_LayerObject(_uint iLevelIndex, const _tchar * pLayerTag)
{
	if (nullptr == m_pObject_Manager)
	{
		MSGBOX("m_pObject_Manager is nullptr in CGameInstance::Release_LayerObject");
		return E_FAIL;
	}

	return m_pObject_Manager->Release_LayerObject(iLevelIndex, pLayerTag);
}

size_t CGameInstance::Get_Size(_uint iLevelIndex, const _tchar * pLayerTag)
{
	if (nullptr == m_pObject_Manager)
	{
		MSGBOX("m_pObject_Manager is nullptr in CGameInstance::Get_size");
		return E_FAIL;
	}

	return m_pObject_Manager->Get_Size(iLevelIndex, pLayerTag);
}

CGameObject * CGameInstance::Get_Front(_uint iLevelIndex, const _tchar * pLayerTag)
{
	if (nullptr == m_pObject_Manager)
	{
		MSGBOX("m_pObject_Manager is nullptr in CGameInstance::Get_Front");
		return nullptr;
	}

	return m_pObject_Manager->Get_Front(iLevelIndex, pLayerTag);
}

HRESULT CGameInstance::Add_Prototype(_uint iLevelIndex, const _tchar * pPrototypeTag, CComponent * pPrototype)
{
	if (nullptr == m_pComponent_Manager)
	{
		MSGBOX("m_pComponent_Manager is nullptr in CGameInstance::Add_Prototype");
		return E_FAIL;
	}

	return m_pComponent_Manager->Add_Prototype(iLevelIndex, pPrototypeTag, pPrototype);
}

CComponent * CGameInstance::Clone_Component(_uint iLevelIndex, const _tchar * pPrototypeTag, void * pArg)
{
	if (nullptr == m_pComponent_Manager)
	{
		MSGBOX("m_pComponent_Manager is nullptr in CGameInstance::Clone_Component");
		return nullptr;
	}

	return m_pComponent_Manager->Clone_Component(iLevelIndex, pPrototypeTag, pArg);
}

_bool CGameInstance::Collision_Sphere(CCollider_Sphere * pDst, CCollider_Sphere * pSrc)
{
	if (nullptr == m_pComponent_Manager)
	{
		MSGBOX("m_pComponent_Manager is nullptr in CGameInstance::Collision_Sphere");
		return nullptr;
	}

	return m_pComponent_Manager->Collision_Sphere(pDst, pSrc);
}

_float3 CGameInstance::Collision_SphereGuard(CCollider_Sphere * pDst, CCollider_Sphere * pSrc)
{
	if (nullptr == m_pComponent_Manager)
	{
		MSGBOX("m_pComponent_Manager is nullptr in CGameInstance::Collision_SphereGuard");
		return _float3();
	}

	return m_pComponent_Manager->Collision_SphereGuard(pDst, pSrc);
}

_float3 CGameInstance::Collision_SphereBounce(CCollider_Sphere * pDst, CCollider_Sphere * pSrc, _float3 vLook)
{
	if (nullptr == m_pComponent_Manager)
	{
		MSGBOX("m_pComponent_Manager is nullptr in CGameInstance::Collision_SphereBounce");
		return _float3();
	}

	return m_pComponent_Manager->Collision_SphereBounce(pDst, pSrc, vLook);
}

_float3 CGameInstance::Collision_SphereToRect(CCollider_Rect * pDst, CCollider_Sphere * pSrc)
{
	if (nullptr == m_pComponent_Manager)
	{
		MSGBOX("m_pComponent_Manager is nullptr in CGameInstance::Collision_SphereToRect");
		return _float3();
	}

	return m_pComponent_Manager->Collision_SphereToRect(pDst, pSrc);
}

_float3 CGameInstance::Collision_SphereToRectZ(CCollider_Rect_Z * pDst, CCollider_Sphere * pSrc)
{
	if (nullptr == m_pComponent_Manager)
	{
		MSGBOX("m_pComponent_Manager is nullptr in CGameInstance::Collision_SphereToRectZ");
		return _float3();
	}

	return m_pComponent_Manager->Collision_SphereToRectZ(pDst, pSrc);
}

_float3 CGameInstance::Collision_SphereToRect_Camera(CCollider_Rect * pDst, CCollider_Sphere * pSrc)
{
	if (nullptr == m_pComponent_Manager)
	{
		MSGBOX("m_pComponent_Manager is nullptr in CGameInstance::Collision_SphereToRect_Camera");
		return _float3();
	}

	return m_pComponent_Manager->Collision_SphereToRect_Camera(pDst, pSrc);
}

_float3 CGameInstance::Collision_SphereToRectArrow(CCollider_Rect * pDst, CCollider_Sphere * pSrc)
{
	if (nullptr == m_pComponent_Manager)
	{
		MSGBOX("m_pComponent_Manager is nullptr in CGameInstance::Collision_SphereToRectArrow");
		return _float3();
	}

	return m_pComponent_Manager->Collision_SphereToRectArrow(pDst, pSrc);
}

_float3 CGameInstance::Collision_SphereToRectArrowFull(CCollider_Rect * pDst, CCollider_Sphere * pSrc)
{
	if (nullptr == m_pComponent_Manager)
	{
		MSGBOX("m_pComponent_Manager is nullptr in CGameInstance::Collision_SphereToRectArrowFull");
		return _float3();
	}

	return m_pComponent_Manager->Collision_SphereToRectArrowFull(pDst, pSrc);
}

_byte CGameInstance::Get_DIKeyState(_ubyte eKeyID)
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Get_DIKeyState(eKeyID);
}

_long CGameInstance::Get_DIMouseMoveState(CInput_Device::MOUSEMOVESTATE eMouseMoveState)
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Get_DIMouseMoveState(eMouseMoveState);
}

_byte CGameInstance::Get_DIMouseButtonState(CInput_Device::MOUSEBUTTONSTATE eMouseButtonState)
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Get_DIMouseButtonState(eMouseButtonState);
}

_bool CGameInstance::Get_Mouse_Up(CInput_Device::MOUSEBUTTONSTATE eMouseButtonState)
{
	if (nullptr == m_pInput_Device)
		return false;

	return m_pInput_Device->Get_Mouse_Up(eMouseButtonState);
}

int CGameInstance::VolumeUp(CSoundMgr::CHANNELID eID, _float _vol)
{
	if (nullptr == m_pSound_Manager)
		return false;

	return m_pSound_Manager->VolumeUp(eID, _vol);
}

int CGameInstance::VolumeDown(CSoundMgr::CHANNELID eID, _float _vol)
{
	if (nullptr == m_pSound_Manager)
		return false;

	return m_pSound_Manager->VolumeDown(eID, _vol);
}

int CGameInstance::BGMVolumeUp(_float _vol)
{
	if (nullptr == m_pSound_Manager)
		return false;

	return m_pSound_Manager->BGMVolumeUp(_vol);
}

int CGameInstance::BGMVolumeDown(_float _vol)
{
	if (nullptr == m_pSound_Manager)
		return false;

	return m_pSound_Manager->BGMVolumeDown(_vol);
}

int CGameInstance::Pause(CSoundMgr::CHANNELID eID)
{
	if (nullptr == m_pSound_Manager)
		return false;

	return m_pSound_Manager->Pause(eID);
}

void CGameInstance::LetsPlaySound(TCHAR * pSoundKey, CSoundMgr::CHANNELID eID, _float _vol)
{
	if (nullptr == m_pSound_Manager)
	{
		MSGBOX("failed PlaySound");
		return;
	}

	return m_pSound_Manager->PlaySound(pSoundKey, eID, _vol);
}

void CGameInstance::PlayBGM(TCHAR* pSoundKey, _float _vol)
{
	if (nullptr == m_pSound_Manager)
	{
		MSGBOX("Failed PlayBGM");
		return;
	}

	return m_pSound_Manager->PlayBGM(pSoundKey, _vol);
}

void CGameInstance::StopSound(CSoundMgr::CHANNELID eID)
{
	if (nullptr == m_pSound_Manager)
	{
		MSGBOX("Failed StopSound");
		return;
	}

	return m_pSound_Manager->StopSound(eID);
}

void CGameInstance::StopAll()
{
	if (nullptr == m_pSound_Manager)
	{
		MSGBOX("Failed StopAll");
		return;
	}

	return m_pSound_Manager->StopAll();
}

void CGameInstance::Release_Engine()
{
	if (0 != CGameInstance::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Delete CGameInstance");

	if (0 != CTimer_Manager::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Delete CTimer_Manager");

	if (0 != CComponent_Manager::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Delete CComponent_Manager");

	if (0 != CSoundMgr::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Delete CSoundMgr");

	if (0 != CObject_Manager::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Delete CObject_Manager");

	if (0 != CLevel_Manager::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Delete CLevel_Manager");

	if (0 != CPicking::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Delete CPicking ");

	if (0 != CInput_Device::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Delete CInput_Device ");

	if (0 != CGraphic_Device::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Delete CGraphic_Device");
}

void CGameInstance::Free()
{
	Safe_Release(m_pSound_Manager);
	Safe_Release(m_pPicking);
	Safe_Release(m_pComponent_Manager);
	Safe_Release(m_pObject_Manager);
	Safe_Release(m_pLevel_Manager);
	Safe_Release(m_pTimer_Manager);
	Safe_Release(m_pInput_Device);
	Safe_Release(m_pGraphic_Device);
}
