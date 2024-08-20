#include "stdafx.h"
#include "..\Public\Colossus.h"
#include "GameInstance.h"
#include "Camera_Player.h"
#include "DarkScreen.h""

CColossus::CColossus(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CColossus::CColossus(const CColossus & rhs)
	: CGameObject(rhs)
{
}

HRESULT CColossus::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		MSGBOX("__super::NativeConstruct_Prototype returned E_FAIL in CColossus::NativeConstruct_Prototype");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CColossus::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		MSGBOX("__super::NativeConstruct returned E_FAIL in CColossus::NativeConstruct");
		return E_FAIL;
	}

	// 컴포넌트 세팅
	if (FAILED(SetUp_Components()))
	{
		MSGBOX("SetUp_Components returned E_FAIL in CColossus::NativeConstruct");
		return E_FAIL;
	}
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	m_pCamera = static_cast<CCamera_Player*>(pGameInstance->Get_Front(LEVEL_STATIC, TEXT("Layer_Camera")));
	RELEASE_INSTANCE(CGameInstance);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(15.f, -3.f, 30.5f));
	m_pTransformCom->Scaled(_float3(16.f, 16.f, 16.f));
	m_pColInfo = (COLOSSUSINFO*)pArg;
	m_pColInfo->bRightAttacking = false;
	m_pColInfo->bLeftAttacking = false;
	m_pColInfo->bWallCrash = false;
	m_pColInfo->bRightWallCrash = false;
	m_pColInfo->bLeftWallCrash = false;



	m_tFrame.iFrameStartY = 1.f;
	m_tFrame.iFrameCurrentX = 0.f;
	m_tFrame.iFrameStartX = 0.f;
	m_tFrame.iFrameEndX = 0.f;
	m_tFrame.iFrameHeight = 128.f;
	m_tFrame.iFrameWidth = 128.f;
	m_tFrame.iTexHeight = 384;
	m_tFrame.iTexWidth = 384.f;
	m_fSpriteSpeed = 1;

	CCollider_Rect::RECTDESC RectDesc;
	RectDesc.ObjectMatrix = m_pTransformCom->Get_WorldMatrix();
	RectDesc.fX = 16.f;
	RectDesc.fY = 16.f;
	m_pColliderCom->Set_RectDesc(RectDesc);

	//_float3* vpPos = (_float3*)pArg;
	////m_SlimeInfo = (SLIMEINFO*)pArg;
	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, *vpPos);

	return S_OK;
}

_int CColossus::Tick(_float fTimeDelta)
{

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (m_bSoulOut == true) {
		m_pTransformCom->Go_Down(fTimeDelta * 0.2f);
	}

	if (true == m_bClear)
	{
		if (pGameInstance->Get_DIKeyState(DIK_Q) & 0x80)
		{
			--m_iSoulOutTime;
			if (m_iSoulOutTime == 0)
			{
				if (m_tFrame.iFrameStartY = 1.f) {
					m_tFrame.iFrameStartY = 1.f;
					m_tFrame.iFrameCurrentX = 0.f;
					m_tFrame.iFrameEndX = 0.f;
					m_bSoulOut = true;
				}
				pGameInstance->Add_GameObject(g_eCurrentLevel, TEXT("Layer_EatSouls"), TEXT("Prototype_GameObject_Effect_EatSouls"), &m_pTransformCom->Get_State(CTransform::STATE_POSITION));
				static_cast<CTransform*>(pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform")))->LookAtXZ(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
				static_cast<CDarkScreen*>(pGameInstance->Get_Front(g_eCurrentLevel, TEXT("Layer_DarkScreen")))->Set_SoulEat(true);
				pGameInstance->Release_LayerObject(LEVEL_STATIC, TEXT("Layer_Arrow"));
				m_iOutSoul = true;
			}
			if (false == m_iOutSoul)
			{
				if (119 == m_iSoulOutTime)
					pGameInstance->LetsPlaySound(L"HeavyDrumLoop.ogg", CSoundMgr::SYSTEM_EFFECT, 0.5f);
				static_cast<CTransform*>(pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform")))->LookTurn(_float3(0.f, 1.f, 0.f), m_pTransformCom->Get_State(CTransform::STATE_POSITION), fTimeDelta * 10.f);
				m_pCamera->Set_Shake(true, false, false);
			}
			pGameInstance->VolumeDown(CSoundMgr::SYSTEM_EFFECT, 0.002f);
		}
		else
			m_iSoulOutTime = 120;
		RELEASE_INSTANCE(CGameInstance);
		return 0;
	}

	if (0 > __super::Tick(fTimeDelta))
	{
		MSGBOX("0 > __super::Tick in CColossus::Tick");
		return -1;
	}




	CTransform* m_pPlayerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"));
	_float3 vpos = m_pPlayerTransform->Get_State(CTransform::STATE_POSITION);
	_float3 MyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	if (m_bCry == true) {
		m_iCryTime++;
		static_cast<CCamera_Player*>(pGameInstance->Get_Front(LEVEL_STATIC, TEXT("Layer_Camera")))->Set_Shake(true, true, false);
		pGameInstance->LetsPlaySound(L"Colossus_Roar.ogg", CSoundMgr::MONSTER2, 1.0f);
	}
	if (m_bUpStanding == false) {

		if (true == m_bStart) {
			if (m_bJump == false) {
				m_fJumpy = m_pTransformCom->Get_State(CTransform::STATE_POSITION).y;
				m_bJump = true;
			}
		}
		if (m_bJump == true) {
			MyPos.y = m_fJumpy + (m_fJumpPower * m_fTime - 3.8f * m_fTime * m_fTime * 0.5f);
			m_fTime += 0.05f;
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, MyPos);
			pGameInstance->LetsPlaySound(L"Colossus_Rise.ogg", CSoundMgr::MONSTER1, 1.0f);
		}
	}
	if (m_bUpStanding) {
		if (m_iCryTime > 100) {
			m_bCry = false;
			m_bAttackOn = true;
			m_iCryTime = 0;
		}
		if (m_bCry == true) {
			m_tFrame.iFrameStartY = 2.f;
			m_tFrame.iFrameStartX = 0.f;
			m_tFrame.iFrameCurrentX = m_tFrame.iFrameStartX;
			m_tFrame.iFrameEndX = m_tFrame.iFrameStartX;
		}
		if (m_bCry == false && m_bAttackOn == true) {
			m_tFrame.iFrameStartY = 0.f;
			m_tFrame.iFrameStartX = 0.f;
			m_tFrame.iFrameCurrentX = m_tFrame.iFrameStartX;
			m_tFrame.iFrameEndX = m_tFrame.iFrameStartX;
		}



		if (m_pColInfo->FistCount > 4) {
			m_bCry = true;
			if (m_pColInfo->bRightFistAttack == true &&
				m_pColInfo->bLeftFistAttack == false) {
				m_pColInfo->bRightWallCrash = true;
				m_pColInfo->FistCount = 0;
			}
			else if (m_pColInfo->bRightFistAttack == false &&
				m_pColInfo->bLeftFistAttack == true) {
				m_pColInfo->bLeftWallCrash = true;

			}
		}




		else if (m_bAttackOn == true && m_pColInfo->bLeftWallCrash == false && m_pColInfo->bRightWallCrash == false) {
			if (MyPos.x < vpos.x && m_pColInfo->bLeftAttacking == false) {
				m_pColInfo->bRightFistAttack = true;
				m_pColInfo->bLeftFistAttack = false;
			}
			else if (vpos.x < MyPos.x  && m_pColInfo->bRightAttacking == false)
			{
				m_pColInfo->bRightFistAttack = false;
				m_pColInfo->bLeftFistAttack = true;
			}

			if (m_bUp == false && m_fUpDownTimer < 50) {
				m_pTransformCom->Go_Up(fTimeDelta * 0.2f);
				m_fUpDownTimer++;
			}
			else if (m_fUpDownTimer >= 50) {
				m_bUp = true;
			}

			if (m_bUp == true && m_fUpDownTimer >= 0) {
				m_pTransformCom->Go_Down(fTimeDelta * 0.2f);
				m_fUpDownTimer--;
			}
			else if (m_fUpDownTimer < 0)
				m_bUp = false;
		}
	}
	if (m_bClear == false)
		Go_FrameEnd(m_fSpriteSpeed, fTimeDelta);
	if (m_iOutSoul == true) {
		Go_FrameEnd(1.f, fTimeDelta);
	}
	RELEASE_INSTANCE(CGameInstance);

	return _int();
}

_int CColossus::LateTick(_float fTimeDelta)
{
	if (true == g_bColossusClear)
		return -1;
	if (0 > __super::LateTick(fTimeDelta))
	{
		MSGBOX("0 > __super::LateTick in CColossus::LateTick");
		return -1;
	}

	if (nullptr == m_pRendererCom)
	{
		MSGBOX("m_pRendererCom is nullptr in CColossus::LateTick");
		return -1;
	}
	if (m_bUpStanding == false) {
		if (FAILED(SetUp_OnTerrain()))
		{
			MSGBOX("SetUp_OnTerrain returned E_FAIL in CColossus::LateTick");
			return -1;
		}
	}

	CCollider_Sphere::SPHEREDESC SphereDesc;
	SphereDesc.ObjectMatrix = m_pTransformCom->Get_WorldMatrix();
	SphereDesc.fRadius = 0.05f;
	SphereDesc.fDist_From_Obj = _float3(0.f, -0.055f, 0.03f);
	m_pWeaknessCom->Set_SphereDesc(SphereDesc);
	m_pWeaknessCom->Tick();

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

HRESULT CColossus::Render()
{
	if (nullptr == m_pVIBufferCom)
	{
		MSGBOX("m_pVIBufferCom is nullptr in CColossus::Render")
			return E_FAIL;
	}

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
	{
		MSGBOX("m_pTransformCom->Bind_OnGraphicDevice returned E_FAIL in CColossus::Render");
		return E_FAIL;
	}
	if (FAILED(Go_Sprites()))
	{
		MSGBOX("Go_Sprites returned E_FAIL in CYeti::Tick");
		return E_FAIL;
	}
	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice()))
	{
		MSGBOX("m_pTextureCom->Bind_OnGraphicDevice in CColossus::Render");
		return E_FAIL;
	}


	if (FAILED(SetUp_RenderState()))
	{
		MSGBOX("SetUp_RenderState returned E_FAIL in CColossus::Render");
		return E_FAIL;
	}

	m_pVIBufferCom->Render();

	if (FAILED(Release_RenderState()))
	{
		MSGBOX("Release_RenderState returned E_FAIL in CColossus::Render");
		return E_FAIL;
	}

	//#ifdef _DEBUG
	//	CCollider_Sphere::SPHEREDESC tSphereDesc = m_pWeaknessCom->Get_SphereDesc();
	//	m_pVIBufferCollCom->MakeHitBox(tSphereDesc);
	//	m_pGraphic_Device->SetTexture(0, FALSE);
	//	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	//	m_pVIBufferCollCom->Render();
	//	m_pVIBufferCollCom->BaseSetting();
	//	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	//
	//#endif // _DEBUG
	//
	return S_OK;
}

HRESULT CColossus::SetUp_Components()
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.0f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CColossus::SetUp_Components(Transform)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CColossus::SetUp_Components(Renderer)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CColossus::SetUp_Components(VIBuffer_Rect)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_COLOSUSS, TEXT("Prototype_Component_Texture_Colossus"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CColossus::SetUp_Components(Texture)");
		return E_FAIL;
	}

	CCollider_Rect::RECTDESC RectDesc;
	RectDesc.ObjectMatrix = m_pTransformCom->Get_WorldMatrix();
	RectDesc.fX = 16.f;
	RectDesc.fY = 16.f;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Rect"), TEXT("Com_Collider"), (CComponent**)&m_pColliderCom, &RectDesc)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CColossus::SetUp_Components(Collider_Rect)");
		return E_FAIL;
	}

	CCollider_Sphere::SPHEREDESC SphereDesc;
	SphereDesc.ObjectMatrix = m_pTransformCom->Get_WorldMatrix();
	SphereDesc.fRadius = 0.05f;
	SphereDesc.fDist_From_Obj = _float3(0.f, -0.055f, 0.04f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"), TEXT("Com_Weak"), (CComponent**)&m_pWeaknessCom, &SphereDesc)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CColossus::SetUp_Components(Collider_Sphere)");
		return E_FAIL;
	}

	//#ifdef _DEBUG
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"), TEXT("Com_VIBuffer_Coll"), (CComponent**)&m_pVIBufferCollCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CColossus::SetUp_Components(VIBuffer_Cube)");
		return E_FAIL;
	}


	return S_OK;
}


HRESULT CColossus::SetUp_OnTerrain()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	CVIBuffer_Terrain* m_pVIBuffer_Terrain = (CVIBuffer_Terrain*)pGameInstance->Get_Component(g_eCurrentLevel, TEXT("Layer_Terrain"), TEXT("Com_VIBuffer"));
	if (m_pVIBuffer_Terrain == nullptr)
	{
		MSGBOX("pVIBuffer_Terrain is nullptr in CColossus::SetUp_OnTerrain");
		return E_FAIL;
	}
	_float3 vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	vPosition.y = m_pVIBuffer_Terrain->Compute_Y(vPosition) + 5.f;
	if (m_bJump == true && m_pTransformCom->Get_State(CTransform::STATE_POSITION).y < vPosition.y
		&& m_fTime > 3) {
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
		m_bUpStanding = true;
		m_pColInfo->bStandUp = true;
		m_bCry = true;
		//m_pColInfo->bRightAttacking == true;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}


HRESULT CColossus::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
	{
		MSGBOX("m_pGraphic_Device is nullptr in CColossus::SetUp_RenderState");
		return E_FAIL;
	}

	//알파 테스팅
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 50);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);



	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);




	return S_OK;
}

HRESULT CColossus::Release_RenderState()
{
	if (nullptr == m_pGraphic_Device)
	{
		MSGBOX("m_pGraphic_Device is nullptr in CColossus::Release_RenderState");
		return E_FAIL;
	}
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);

	//m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pVIBufferCom->Base_Setting();

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	return S_OK;
}

HRESULT CColossus::Go_Sprites()
{
	if (FAILED(m_pVIBufferCom->Go_Sprites(m_tFrame)))
	{
		MSGBOX("m_pVIBufferCom->Go_Sprites returned E_FAIL in CYeti::Go_Sprites");
		return E_FAIL;
	}
	return S_OK;
}

CColossus * CColossus::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CColossus*	pInstance = new CColossus(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create Slime_Water in CColossus::Create");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CColossus::Clone(void* pArg)
{
	/* 새로운객체를 복제하여 생성한다. */
	CColossus*	pInstance = new CColossus(*this);


	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create Slime_Water in CColossus::Clone");
		Safe_Release(pInstance);
	}
	return pInstance;
}



void CColossus::Free()
{
	__super::Free();

	Safe_Release(m_pWeaknessCom);
	Safe_Release(m_pVIBufferCollCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
