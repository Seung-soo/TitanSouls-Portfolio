#include "stdafx.h"
#include "..\public\UI_Skill_CoolTimer.h"
#include "GameInstance.h"
#include "Player.h"

CUI_Skill_CoolTimer::CUI_Skill_CoolTimer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CAlphaUI(pGraphic_Device)
{
}

CUI_Skill_CoolTimer::CUI_Skill_CoolTimer(const CUI_Skill_CoolTimer & rhs)
	: CAlphaUI(rhs)
{
}

HRESULT CUI_Skill_CoolTimer::NativeConstruct_Prototype()
{
	return __super::NativeConstruct_Prototype();
}

HRESULT CUI_Skill_CoolTimer::NativeConstruct(void * pArg)
{
	if (FAILED(SetUp_Components()))
		return E_FAIL;


	m_fTestTimer = m_fTimer;
	m_fX = g_iWinCX * 0.5f + 550.f;
	m_fY = g_iWinCY * 0.5f - 280.f;
	m_fSizeX = 128.f;
	m_fSizeY = 128.f;
//	m_fSize = 0.f;
	m_iDepth = 5;
	// 
	//m_pTransformCom->Scaled(_float3(0, 0, 0.f));
	return __super::NativeConstruct(pArg);
}
// m_fsizeX랑 m_fSizeY로 스케일 값을 수정해야 한다아..? 

_int CUI_Skill_CoolTimer::Tick(_float fTimeDelta)
{
	/*
	if (0 > __super::Tick(fTimeDelta))
	{
		MSGBOX("0 > __super::Tick in CAlphaUI::Tick");
		return -1;
	}*/
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	//_int m_iPlayerState = static_cast<CPlayer*>(pGameInstance->Get_Front(LEVEL_STATIC, TEXT("Layer_Player")))->Get_State();
	_int m_iPlayerRollDelay = 0; 
	if (false == g_bColossusClear)
		m_iPlayerRollDelay = static_cast<CPlayer*>(pGameInstance->Get_Front(LEVEL_STATIC, TEXT("Layer_Player")))->Get_RollDelay();
	RELEASE_INSTANCE(CGameInstance);
	
	
	//if (m_iPlayerState != 2) {
	//	return -1;
	//}
	//else {			// 구르기중일떄 
		//m_fSizeX -= (128.f / 40.f);
		//m_fSizeY -= (128.f / 40.f);
		m_fSizeX = (128.f / 40.f) * m_iPlayerRollDelay;
		m_fSizeY = (128.f / 40.f) * m_iPlayerRollDelay;
		m_pTransformCom->Scaled(_float3(m_fSizeX, m_fSizeY, 0.f));
	//}

	if (m_bPlay == true) {
	//	m_fSize -= (128.f / 40.f);
		//m_pTransformCom->Scaled(_float3(m_fSize, m_fSize, 0.f));
	//	if (m_fSize <= 0) {
	//		return -1;
	//	}
	}
	//	m_bPlay = true;
	//}
	//else {
	//	m_bPlay = false;
	//}

	//if(m_bPlay == true) {
	//	m_fSizeX = 128.f;
	//	m_fSizeY = 128.f;
	//	//m_fSize = 128.f;
	//}
	//else {
	//	m_fSizeX = 0.f;
	//	m_fSizeY = 0.f;
	//}
	//m_pTransformCom->Scaled(_float3(m_fSizeX, m_fSizeY, 0.f));
	
	/*
	if (m_iPlayerState == 2) {
		m_fSizeX = 128.f;
		m_fSizeY = 128.f;
	}
	
	else {
	m_fSize = 0.f;
	}
	*/
	/*
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	_int m_iPlayerState = static_cast<CPlayer*>(pGameInstance->Get_Front(LEVEL_STATIC, TEXT("Layer_Player")))->Get_State();
	RELEASE_INSTANCE(CGameInstance);

	*/
	// 스킬 쓸수있을때
	/*
	if(iPlayerState == 2){
		
	
	}
	if (m_bAbleSkill == true) {
		m_fSize = 0.f;
	}
	else {
	
	}
	*/
	/*
	if(m_bAbleSkill == true){
		
		m_pTransformCom->Scaled(_float3(m_fSize, m_fSize, 0.f));
		if (GetAsyncKeyState(VK_LSHIFT) & 1) {
			m_fSize = 128.f;
			m_fTimerOn = true;
			m_bAbleSkill = false;
			m_pTransformCom->Scaled(_float3(m_fSize, m_fSize, 0.f));
		}
	}
	*/
	//쿨타임 이미지가 덮어씌워주고 해당 이프문에 진입하지 않도록 m_fTimerOn을 false로 바꿔준다 
	// 그리도 쿨타임 이미지를 크기를 늘려 스킬 이미지를 덮어 버리도록 하고 스케일 값이 줄어 드는 이프문으로 진입하기 위해 m_bPlay를 true로 바꿔준다
	
	
	
	//////////////이상하게 동작함 반대로 함!!!
	/*
	if (m_fTimerOn == true) {
		m_fSize = 128.f;
		//m_pTransformCom->Scaled(_float3(m_fSize, m_fSize, 0.f));
		m_fTimerOn = false;
		m_bPlay = true;
	}

	// 스케일 값이 계속 줄어드는 구간 
	if (m_bPlay == true) {
		m_fSize -= (128.f/40.f);
		//m_pTransformCom->Scaled(_float3(m_fSize, m_fSize, 0.f));
		if (m_fSize <= 0) {
			m_fSize = 0.f;
			//m_pTransformCom->Scaled(_float3(m_fSize, m_fSize, 0.f));
			m_bPlay = false;		// 스케일 값이 다 줄어들었다 ( 쿨이 다 돌았다, 다시 스킬을 쓸 수 있는 상태
			m_bAbleSkill = true;
		}
	}
	m_pTransformCom->Scaled(_float3(m_fSize, m_fSize, 0.f));
	*/
	///////////////////////////////////////////

	/*
	CGameInstance* pGameinstance = GET_INSTANCE(CGameInstance);
	if (pGameinstance->Get_DIKeyState(DIK_LSHIFT)&0x80) {
		m_fTimerOn = true;
	}
	RELEASE_INSTANCE(CGameInstance);
	*/
	
	//m_fSizeX = 50.f;
	//m_fSizeY = 50.f;

	//m_pTransformCom->Scaled(_float3(m_fSize, m_fSize,0.f));
	/*
	if (GetAsyncKeyState('H') & 1) {
		m_fTimerOn = false;
	}
	if (true == m_fTimerOn) {
		m_fSizeX += m_fTimerScale;
		m_fSizeY += m_fTimerScale;
	}*/
	//
	/*
		if (GetAsyncKeyState('H') & 0x8000) {
			m_fTimerOn = true;
			m_bPlayOn = true;
			//m_fSize = 128.f;
			m_fSizeX = 128.f;
			m_fSizeY = 128.f;
		}
	*/
	//m_pTransformCom->Scaled(_float3(m_fSizeX, m_fSizeY, 0.f));
	/*
	m_pTransformCom->Scaled(_float3(m_fSizeX, m_fSizeY, 0.f));
	if (true == m_fTimerOn) {
		--m_fTimer;
	}
	*/
	//pGameInstance->Add_GameObject(g_eCurrentLevel, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Effect_LightLeak"));
	
	return __super::Tick(fTimeDelta);
	//return _int();
}

_int CUI_Skill_CoolTimer::LateTick(_float fTimeDelta)
{

	

		//m_fSize = 0.f;
	
	//m_pTransformCom->Scaled(_float3(m_fSize, m_fSize, 0.f));
	if (0 > __super::LateTick(fTimeDelta))
	{
		MSGBOX("0 > __super::Tick in CAlphaUI::Tick");
		return -1;
	}

	







	/*
	if (true == m_fTimerOn) {
		m_pTransformCom->Scaled(_float3(128, 128, 0.f));

	}
	*/

	/*
	if (true == m_bPlayOn) {
		--m_fTimer;
	}
	if (true == m_fTimerOn) {
		if (m_fTimer <=0 ) {
			m_fTimer = 40.f;
			m_fTimerOn = false;
		}
	}
	*/
	//return __super::LateTick(fTimeDelta);

	return _int();
}

HRESULT CUI_Skill_CoolTimer::Render()
{
	m_pVIBufferCom->Base_Setting();
	if (true == g_bCameraDirecting || true == g_bColossusClear)
		return S_OK;
	return __super::Render();
}

HRESULT CUI_Skill_CoolTimer::SetUp_Components()
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

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Skill99"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CAim::SetUp_Components(Texture)");
		return E_FAIL;
	}

	return S_OK;
}

CUI_Skill_CoolTimer * CUI_Skill_CoolTimer::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUI_Skill_CoolTimer*	pInstance = new CUI_Skill_CoolTimer(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create Aim in CAim::Create");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CUI_Skill_CoolTimer::Clone(void * pArg)
{
	CGameObject*	pInstance = new CUI_Skill_CoolTimer(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create Aim in CAim::Clone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Skill_CoolTimer::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
