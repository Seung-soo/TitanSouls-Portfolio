#include "stdafx.h"
#include "..\public\Player.h"
#include "GameInstance.h"
#include "Camera_Player.h"
#include "Level_Loading.h"
#include "Arrow.h"
#include "All_Door.h"
#include "All_Door2.h"
#include "Field01_DoorLight.h"
CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CPlayer::CPlayer(const CPlayer & rhs)
	: CGameObject(rhs)
{
}

HRESULT CPlayer::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		MSGBOX("__super::NativeConstruct_Prototype returned E_FAIL in CPlayer::NativeConstruct_Prototype");
		return E_FAIL;
	}

	m_tFrame.iFrameCurrentX = 0.f;
	m_tFrame.iFrameEndX = 5.f;
	m_tFrame.iFrameHeight = 16.f;
	m_tFrame.iFrameWidth = 16.f;
	m_tFrame.iFrameStartY = 1.f;
	m_tFrame.iTexHeight = 512.f;
	m_tFrame.iTexWidth = 512.f;

	m_iLookHeight = 0;
	m_eDir = DIR_DOWN;
	m_isArrow = false;
	m_eState = STATE_IDLE;
	m_iRollDelay = 0;
	m_iShotPower = 0.f;
	m_bHaveArrow = true;
	m_bPushE = false;
	m_bMoveEndArrow = false;
	m_bDie = false;

	m_vKnockBackDir;
	m_iKnockBackTime = 30;

	m_bFly = false;
	m_fFlyFrameTime = 0;
	m_iTurnNum = 0;
	m_bSoulStart = false;
	m_bFirstBlood = false;

	m_iDustNum = 0;
	m_bRolling_Dust = false;
	m_fDustSec = 0;

	return S_OK;
}

HRESULT CPlayer::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		MSGBOX("__super::NativeConstruct returned E_FAIL in CPlayer::NativeConstruct");
		return E_FAIL;
	}

	// 컴포넌트 세팅
	if (FAILED(SetUp_Components()))
	{
		MSGBOX("SetUp_Components returned E_FAIL in CPlayer::NativeConstruct");
		return E_FAIL;
	}

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(16.f, 0.f, 2.f));
	// 플레이어 크기 조절
	m_pTransformCom->Scaled(_float3(1.f, 1.f, 1.f));


	return S_OK;
}

_int CPlayer::Tick(_float fTimeDelta)
{


	if (0 > __super::Tick(fTimeDelta))
	{
		MSGBOX("0 > __super::Tick in CPlayer::Tick");
		return -1;
	}
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (pGameInstance->Get_DIKeyState(DIK_9) & 0x80)
	{
		m_bDie = false;
		m_eState = STATE_IDLE;
		m_tFrame.iFrameStartX = 0.f;
		m_tFrame.iFrameCurrentX = 0.f;
		m_tFrame.iFrameEndX = 5.f;
	}

	// 죽으면 아무것도 조작안됌
	if (STATE_DIE == m_eState)
	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Layer_PlayerDead"), TEXT("Prototype_GameObject_UI_PlayerDead")))) {
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Field::Ready_Layer_UI");
			return E_FAIL;
		}
		if (false == m_bFirstBlood)
		{
			pGameInstance->Add_GameObject(g_eCurrentLevel, TEXT("Layer_Blood"), TEXT("Prototype_GameObject_Blood"));
			m_bFirstBlood = true;
		}
		if (true == m_bFirstBlood && 0 == pGameInstance->Get_Size(g_eCurrentLevel, TEXT("Layer_Blood")) && nullptr == pGameInstance->Find_Layer(g_eCurrentLevel, TEXT("Layer_SceneLoading")))
		{
			//pGameInstance->Add_GameObject(g_eCurrentLevel, TEXT("Layer_White"), TEXT("Prototype_GameObject_Effect_WhiteScreen"));
			SCENECHANGE tSceneChange;
			tSceneChange.bCrease = false;
			switch (g_eCurrentLevel)
			{
			case LEVEL_RESPAWN1:
				tSceneChange.iLevelIndex = LEVEL_RESPAWN1;
				pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Layer_SceneLoading"), TEXT("Prototype_GameObject_SceneLoading"), &tSceneChange);
				break;
			case LEVEL_RESPAWN2:
				tSceneChange.iLevelIndex = LEVEL_RESPAWN2;
				pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Layer_SceneLoading"), TEXT("Prototype_GameObject_SceneLoading"), &tSceneChange);

				break;
			case LEVEL_SLIME:
				tSceneChange.iLevelIndex = LEVEL_RESPAWN1;
				pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Layer_SceneLoading"), TEXT("Prototype_GameObject_SceneLoading"), &tSceneChange);

				break;
			case LEVEL_ICEBRAIN:
				tSceneChange.iLevelIndex = LEVEL_RESPAWN2;
				pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Layer_SceneLoading"), TEXT("Prototype_GameObject_SceneLoading"), &tSceneChange);

				break;
			case LEVEL_COLOSUSS:
				tSceneChange.iLevelIndex = LEVEL_RESPAWN2;
				pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Layer_SceneLoading"), TEXT("Prototype_GameObject_SceneLoading"), &tSceneChange);

				break;
			case LEVEL_EYECUBE:
				tSceneChange.iLevelIndex = LEVEL_RESPAWN1;
				pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Layer_SceneLoading"), TEXT("Prototype_GameObject_SceneLoading"), &tSceneChange);

				break;
			case LEVEL_YETI:
				tSceneChange.iLevelIndex = LEVEL_RESPAWN2;
				pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Layer_SceneLoading"), TEXT("Prototype_GameObject_SceneLoading"), &tSceneChange);

				break;
			}
		}
		if (true == m_bFirstBlood && g_bGoLevel == true)
		{
			NativeConstruct_Prototype();
			if (nullptr != pGameInstance->Find_Layer(LEVEL_STATIC, TEXT("Layer_Arrow")))
				pGameInstance->Release_LayerObject(LEVEL_STATIC, TEXT("Layer_Arrow"));
			pGameInstance->OpenLevel(LEVEL_LOADING, CLevel_Loading::Create(m_pGraphic_Device, g_eNextLevel));
			g_bGoLevel = false;

		}

		RELEASE_INSTANCE(CGameInstance);
		return S_OK;
	}
	else
	{
		m_bFirstBlood = false;
		g_bGoLevel = false;
	}




	if (STATE_KNOCKBACK == m_eState && 0 < m_iKnockBackTime)
	{
		m_pTransformCom->Go_Vector(m_vKnockBackDir * 3.f, fTimeDelta);
		m_iKnockBackTime--;
	}
	else if (0 >= m_iKnockBackTime)
	{
		m_eState = STATE_IDLE;
		m_iKnockBackTime = 30;
	}

	if (true == m_isArrow && false == m_bHaveArrow)
	{
		if (pGameInstance->Find_Layer(LEVEL_STATIC, TEXT("Layer_Arrow"))->Get_Object().empty())
		{
			m_isArrow = false;
			m_bHaveArrow = true;
		}
	}

	if (false == g_bCameraDirecting)
	{
		CTransform::TRANSFORMDESC	tTransformDesc = m_pTransformCom->Get_TransformDesc();

		if (STATE_IDLE == m_eState || STATE_WALK == m_eState)
		{
			m_eState = STATE_IDLE;
			Move(fTimeDelta);
		}

		m_tFrame.iFrameStartY = (_float)m_eDir;
		m_fSpriteSpeed = 1.f;


		if (STATE_KNOCKBACK != m_eState)
		{
			Rolling(fTimeDelta);
			Shot(pGameInstance, fTimeDelta);
			PullArrow(pGameInstance, fTimeDelta);
		}

		AimMove(fTimeDelta);
	}

	if (true == m_bDie)
	{
		if (false == m_bFirstBlood)
		{
			static_cast<CTransform*>(pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Camera"), TEXT("Com_Transform")))->Go_BackWard(fTimeDelta * 5.f);

			pGameInstance->Add_GameObject(g_eCurrentLevel, TEXT("Layer_Blood"), TEXT("Prototype_GameObject_Blood"));
			m_bFirstBlood = true;
		}
		m_eState = STATE_DIE;
		Die();
	}

	if (true == m_bFly)
	{
		if (m_eState != STATE_FLY)
			m_tFrame.iFrameCurrentX = 3.f;
		if (STATE_LIGHTLEAK != m_eState)
			m_eState = STATE_FLY;
		else
			m_tFrame.iFrameCurrentX = 3.f;
		m_tFrame.iFrameStartX = 0.f;

		m_tFrame.iFrameEndX = 7.f;
		m_tFrame.iFrameStartY = 17.f;

		if (m_iTurnNum > 3)
			m_tFrame.iFrameStartY = 16.f;

	}


	if (m_eState == STATE_IDLE)
	{
		m_tFrame.iFrameStartX = 0.f;
		m_tFrame.iFrameEndX = 5.f;
		m_tFrame.iFrameStartY = _float(m_eDir);
	}

	RELEASE_INSTANCE(CGameInstance);

	return _int();
}

_int CPlayer::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
	{
		MSGBOX("0 > __super::LateTick in CPlayer::LateTick");
		return -1;
	}

	if (nullptr == m_pRendererCom)
	{
		MSGBOX("m_pRendererCom is nullptr in CPlayer::LateTick");
		return -1;
	}

	if (m_bFly == false)
		if (FAILED(SetUp_OnTerrain()))
		{
			MSGBOX("SetUp_OnTerrain returned E_FAIL in CPlayer::LateTick");
			return -1;
		}

	CCollider_Sphere::SPHEREDESC SphereDesc;
	SphereDesc.ObjectMatrix = m_pTransformCom->Get_WorldMatrix();
	SphereDesc.fRadius = 0.3f;
	SphereDesc.fDist_From_Obj = _float3(0.f, 0.f, 0.f);
	m_pColliderCom->Set_SphereDesc(SphereDesc);
	m_pColliderCom->Tick();

	//////////알파소팅Z값 주기///////////
	_float4x4 WorldMatrix;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &WorldMatrix);
	D3DXMatrixInverse(&WorldMatrix, nullptr, &WorldMatrix);
	_float3 vRight;
	_float3 vUp = _float3(0.f, 1.f, 0.f);
	_float3 vLook = *(_float3*)WorldMatrix.m[2];
	vLook.y = 0.f;
	D3DXVec3Normalize(&vLook, &vLook);

	memcpy(&WorldMatrix.m[1][0], &vUp, sizeof(_float3));
	memcpy(&WorldMatrix.m[2][0], &vLook, sizeof(_float3));
	D3DXMatrixInverse(&WorldMatrix, nullptr, &WorldMatrix);

	_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	D3DXVec3TransformCoord(&vPos, &vPos, &WorldMatrix);
	m_fAlphaZ = vPos.z;
	///////////////////////////////////

	if (m_iTurnNum > 3 || m_tFrame.iFrameStartY == 16.f)
	{

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHA, this);
	}
	else
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	// 프레임 움직임
	if (STATE_SHOT == m_eState)
		Go_FrameEnd(m_fSpriteSpeed, fTimeDelta * 2.f);
	else if (STATE_PULL == m_eState)
		Go_FrameEnd(m_fSpriteSpeed, fTimeDelta * 4.f);
	else if (STATE_DIE == m_eState)
		Go_FrameEnd(m_fSpriteSpeed, fTimeDelta);
	else if (STATE_FLY == m_eState)
	{
		m_fFlyFrameTime += 8.f * fTimeDelta;
		if (1 < m_fFlyFrameTime)
		{
			Go_FrameTurn();
			m_fFlyFrameTime = 0.f;
		}
	}
	else
		Go_Frame(m_fSpriteSpeed, fTimeDelta);

	if (STATE_IDLE == m_eState)
		m_tFrame.iFrameCurrentX = 0.f;

	Collision();

	return _int();
}

HRESULT CPlayer::Render()
{
	if (nullptr == m_pVIBufferCom)
	{
		MSGBOX("m_pVIBufferCom is nullptr in CPlayer::Render")
			return E_FAIL;
	}

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
	{
		MSGBOX("m_pTransformCom->Bind_OnGraphicDevice returned E_FAIL in CPlayer::Render");
		return E_FAIL;
	}

	if (FAILED(Go_Sprites()))
	{
		MSGBOX("Go_Sprites returned E_FAIL in CPlayer::Tick");
		return E_FAIL;
	}

	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice()))
	{
		MSGBOX("m_pTextureCom->Bind_OnGraphicDevice in CPlayer::Render");
		return E_FAIL;
	}


	if (FAILED(SetUp_RenderState()))
	{
		MSGBOX("SetUp_RenderState returned E_FAIL in CPlayer::Render");
		return E_FAIL;
	}

	m_pVIBufferCom->Render();

	if (FAILED(Release_RenderState()))
	{
		MSGBOX("Release_RenderState returned E_FAIL in CPlayer::Render");
		return E_FAIL;
	}
	//#ifdef _DEBUG
	//	CCollider_Sphere::SPHEREDESC tSphereDesc = m_pColliderCom->Get_SphereDesc();
	//	m_pVIBufferCollCom->MakeHitBox(tSphereDesc);
	//	m_pGraphic_Device->SetTexture(0, FALSE);
	//	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	//	m_pVIBufferCollCom->Render();
	//	m_pVIBufferCollCom->BaseSetting();
	//	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	//
	//#endif // _DEBUG

		//m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		//m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		//m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		//m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		//m_pTestCom->Render();
		//m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	return S_OK;

}

HRESULT CPlayer::SetUp_Components()
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = PLAYER_MOVESPEED;
	TransformDesc.fRotationPerSec = D3DXToRadian(PLAYER_ROTATIONSPEED);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CPlayer::SetUp_Components(Transform)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CPlayer::SetUp_Components(Renderer)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CPlayer::SetUp_Components(VIBuffer_Rect)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CPlayer::SetUp_Components(Texture)");
		return E_FAIL;
	}

	CCollider_Sphere::SPHEREDESC SphereDesc;
	SphereDesc.ObjectMatrix = m_pTransformCom->Get_WorldMatrix();
	SphereDesc.fRadius = 0.3f;
	SphereDesc.fDist_From_Obj = _float3(0.f, 0.f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"), TEXT("Com_Collider"), (CComponent**)&m_pColliderCom, &SphereDesc)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CPlayer::SetUp_Components(Collider_Sphere)");
		return E_FAIL;
	}

	//#ifdef _DEBUG
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"), TEXT("Com_VIBuffer_Coll"), (CComponent**)&m_pVIBufferCollCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CPlayer::SetUp_Components(VIBuffer_Cube)");
		return E_FAIL;
	}
	//#endif // DEBUG

		//if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Color"), TEXT("Com_VIBuffer_Color"), (CComponent**)&m_pTestCom)))
		//{
		//	MSGBOX("__super::Add_Component returned E_FAIL in CPlayer::SetUp_Components(VIBuffer_Color)");
		//	return E_FAIL;
		//}

	return S_OK;
}

HRESULT CPlayer::SetUp_OnTerrain()
{
	if (LEVEL_LOADING == g_eCurrentLevel)
		return S_OK;

	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);


	CVIBuffer_Terrain*	pVIBuffer_Terrain = (CVIBuffer_Terrain*)pGameInstance->Get_Component(g_eCurrentLevel, TEXT("Layer_Terrain"), TEXT("Com_VIBuffer"));
	if (nullptr == pVIBuffer_Terrain)
	{
		MSGBOX("pVIBuffer_Terrain is nullptr in CPlayer::SetUp_OnTerrain");
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}

	_float3	vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	CTransform* pTerrainTransform = (CTransform*)pGameInstance->Get_Component(g_eCurrentLevel, TEXT("Layer_Terrain"), TEXT("Com_Transform"));
	if (nullptr == pTerrainTransform)
	{
		MSGBOX("pTerrainTrasform is nullptr in CPlayer::SetUp_OnTerrain");
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}

	_float4x4	WorldMatrixInverse = pTerrainTransform->Get_WorldMatrixInverse();

	_float3		vLocalPos;
	D3DXVec3TransformCoord(&vLocalPos, &vPosition, &WorldMatrixInverse);


	D3DXVec3TransformCoord(&vPosition, &vLocalPos, &pTerrainTransform->Get_WorldMatrix());

	if (g_eCurrentLevel == LEVEL_RESPAWN1 || g_eCurrentLevel == LEVEL_RESPAWN2)
		vPosition.y = pVIBuffer_Terrain->Compute_Y_Big(vLocalPos) + 0.5f;
	else
		vPosition.y = pVIBuffer_Terrain->Compute_Y(vLocalPos) + 0.5f;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CPlayer::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
	{
		MSGBOX("m_pGraphic_Device is nullptr in CPlayer::SetUp_RenderState");
		return E_FAIL;
	}
	if (g_eCurrentLevel == LEVEL_COLOSUSS && true == g_bColossusClear)
	{
		m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TFACTOR);
		m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TEXTURE);
		m_pGraphic_Device->SetRenderState(D3DRS_TEXTUREFACTOR, 0x00000000);
	}


	//m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	//m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
	//m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);
	//m_pGraphic_Device->SetRenderState(D3DRS_TEXTUREFACTOR, 0xa0ffffff);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);

	if (m_tFrame.iFrameStartY == 16.f)
	{
		// 이건 알파 블랜딩 할 때 복붙해서 사용할 것
		m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
	else
	{

		//알파 테스팅
		m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 0);
		m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	}
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);

	if (LEVEL_COLOSUSS == g_eCurrentLevel && true == g_bColossusClear)
		m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	return S_OK;
}

HRESULT CPlayer::Release_RenderState()
{
	if (nullptr == m_pGraphic_Device)
	{
		MSGBOX("m_pGraphic_Device is nullptr in CPlayer::Release_RenderState");
		return E_FAIL;
	}
	if (LEVEL_COLOSUSS == g_eCurrentLevel && true == g_bColossusClear)
		m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);


	//m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pVIBufferCom->Base_Setting();

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	return S_OK;
}

HRESULT CPlayer::Go_Sprites()
{
	if (FAILED(m_pVIBufferCom->Go_Sprites(m_tFrame)))
	{
		MSGBOX("m_pVIBufferCom->Go_Sprites returned E_FAIL in CPlayer::Go_Sprites");
		return E_FAIL;
	}
	return S_OK;
}

void CPlayer::Collision()
{
	if (LEVEL_LOADING == g_eCurrentLevel)
		return;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	if (g_bCameraDirecting == false)
		for (_uint i = 0; i < pGameInstance->Get_Size(g_eCurrentLevel, TEXT("Layer_Wall")); i++)
		{
			_float3 vPos = pGameInstance->Collision_SphereToRect((CCollider_Rect*)pGameInstance->Get_Component(g_eCurrentLevel, TEXT("Layer_Wall"), TEXT("Com_Collider"), i), m_pColliderCom);
			if (vPos != _float3(0.f, 0.f, 0.f))
			{
				m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
				CCollider_Sphere::SPHEREDESC SphereDesc;
				SphereDesc.ObjectMatrix = m_pTransformCom->Get_WorldMatrix();
				SphereDesc.fRadius = 0.3f;
				SphereDesc.fDist_From_Obj = _float3(0.f, 0.f, 0.f);
				m_pColliderCom->Set_SphereDesc(SphereDesc);
				m_pColliderCom->Tick();
				if (LEVEL_YETI == g_eCurrentLevel && STATE_KNOCKBACK == m_eState)
					m_bDie = true;
			}
		}
	if (LEVEL_FIELD == g_eCurrentLevel)
	{
		_float3 vPos = pGameInstance->Collision_SphereToRect((CCollider_Rect*)pGameInstance->Get_Component(g_eCurrentLevel, TEXT("Layer_Door"), TEXT("Com_Collider")), m_pColliderCom);
		if (vPos != _float3(0.f, 0.f, 0.f))
		{
			if (false == g_bCameraDirecting)
				if (FAILED(pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Layer_PotalButton"), TEXT("Prototype_GameObject_UI_PotalIcon"))))
				{
					MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Field::Ready_Layer_UI");
					return;
				}
			if (pGameInstance->Get_DIKeyState(DIK_SPACE) & 0x80)
			{
				m_eState = STATE_IDLE;
				m_eDir = DIR_UP;
				pGameInstance->Release_LayerObject(LEVEL_STATIC, TEXT("Layer_PotalButton"));
				static_cast<CField01_DoorLight*>(pGameInstance->Get_Front(g_eCurrentLevel, TEXT("Layer_Light")))->Set_Light(true);
				m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(16.f, 0.5f, 31.5f));
				m_pTransformCom->LookAtXZ(_float3(16.f, 0.5f, 46.0f));
				_float3 vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
				D3DXVec3Normalize(&vLook, &vLook);

				_float3 vPlayerPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);


				_float3 CameraPos = vPlayerPos - (vLook * 2);
				CameraPos += _float3(0.f, 1.2f, 0.f);

				static_cast<CTransform*>(pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Camera"), TEXT("Com_Transform")))->Set_State(CTransform::STATE_POSITION, CameraPos);
				static_cast<CTransform*>(pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Camera"), TEXT("Com_Transform")))->LookAt(vPlayerPos + _float3(0.f, 1.2f, 0.f));

				g_bCameraDirecting = true;
				static_cast<CCamera_Player*>(pGameInstance->Get_Front(LEVEL_STATIC, TEXT("Layer_Camera")))->Set_InBigDoor(true, LEVEL_RESPAWN1);
				static_cast<CAll_Door*>(pGameInstance->Get_Front(g_eCurrentLevel, TEXT("Layer_BigDoorLeft")))->Set_BoolCnt(true);
				static_cast<CAll_Door*>(pGameInstance->Get_Front(g_eCurrentLevel, TEXT("Layer_BigDoorRight")))->Set_BoolCnt(true);
				//m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(9.f, 0.5f, 3.f));
				//pGameInstance->OpenLevel(LEVEL_LOADING, CLevel_Loading::Create(m_pGraphic_Device, LEVEL_RESPAWN1));
				RELEASE_INSTANCE(CGameInstance);
				return;
			}
		}
		else
			pGameInstance->Release_LayerObject(LEVEL_STATIC, TEXT("Layer_PotalButton"));

	}
	else if (LEVEL_RESPAWN1 == g_eCurrentLevel)
	{
		_uint ColCount = 0;
		_float3 vPos = pGameInstance->Collision_SphereToRect((CCollider_Rect*)pGameInstance->Get_Component(g_eCurrentLevel, TEXT("Layer_Door"), TEXT("Com_Collider"), 0), m_pColliderCom);

		if (false == g_bSlimeClear)
		{
			if (vPos != _float3(0.f, 0.f, 0.f))
			{
				++ColCount;
				if (false == g_bCameraDirecting && (pGameInstance->Find_Layer(LEVEL_STATIC, TEXT("Layer_PotalButton")) == nullptr || pGameInstance->Get_Size(LEVEL_STATIC, TEXT("Layer_PotalButton")) == 0))
					if (FAILED(pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Layer_PotalButton"), TEXT("Prototype_GameObject_UI_PotalIcon"))))
					{
						MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Field::Ready_Layer_UI");
						return;
					}
				if (pGameInstance->Get_DIKeyState(DIK_SPACE) & 0x80)
				{
					pGameInstance->Release_LayerObject(LEVEL_STATIC, TEXT("Layer_PotalButton"));
					m_eState = STATE_IDLE;
					m_eDir = DIR_UP;
					m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(13.6f, 0.5f, 46.0f));
					m_pTransformCom->LookAtXZ(_float3(10.f, 0.5f, 46.0f));
					_float3 vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
					D3DXVec3Normalize(&vLook, &vLook);

					_float3 vPlayerPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);


					_float3 CameraPos = vPlayerPos - (vLook * 2);
					CameraPos += _float3(0.f, 0.3f, 0.f);

					static_cast<CTransform*>(pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Camera"), TEXT("Com_Transform")))->Set_State(CTransform::STATE_POSITION, CameraPos);
					static_cast<CTransform*>(pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Camera"), TEXT("Com_Transform")))->LookAt(vPlayerPos + _float3(0.f, 0.7f, 0.f));

					g_bCameraDirecting = true;
					static_cast<CCamera_Player*>(pGameInstance->Get_Front(LEVEL_STATIC, TEXT("Layer_Camera")))->Set_InDoor(true, LEVEL_SLIME);
					static_cast<CAll_Door*>(pGameInstance->Get_Front(g_eCurrentLevel, TEXT("Layer_OpenDoor")))->Set_BoolCnt(true);
					//pGameInstance->OpenLevel(LEVEL_LOADING, CLevel_Loading::Create(m_pGraphic_Device, LEVEL_SLIME));
					RELEASE_INSTANCE(CGameInstance);
					return;

				}
			}
		}
		
		if (false == g_bEyeCubeClear)
		{
			vPos = pGameInstance->Collision_SphereToRect((CCollider_Rect*)pGameInstance->Get_Component(g_eCurrentLevel, TEXT("Layer_Door"), TEXT("Com_Collider"), 1), m_pColliderCom);
			if (vPos != _float3(0.f, 0.f, 0.f))
			{
				++ColCount;

				if (false == g_bCameraDirecting)
					if (FAILED(pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Layer_PotalButton"), TEXT("Prototype_GameObject_UI_PotalIcon"))))
					{
						MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Field::Ready_Layer_UI");
						return;
					}
				if (pGameInstance->Get_DIKeyState(DIK_SPACE) & 0x80)
				{
					m_eState = STATE_IDLE;
					m_eDir = DIR_UP;
					m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(57.5f, 0.5f, 40.0f));
					m_pTransformCom->LookAtXZ(_float3(60.f, 0.5f, 40.0f));
					_float3 vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
					D3DXVec3Normalize(&vLook, &vLook);

					_float3 vPlayerPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);


					_float3 CameraPos = vPlayerPos - (vLook * 2);
					CameraPos += _float3(0.f, 0.3f, 0.f);

					static_cast<CTransform*>(pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Camera"), TEXT("Com_Transform")))->Set_State(CTransform::STATE_POSITION, CameraPos);
					static_cast<CTransform*>(pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Camera"), TEXT("Com_Transform")))->LookAt(vPlayerPos + _float3(0.f, 0.7f, 0.f));

					g_bCameraDirecting = true;
					static_cast<CCamera_Player*>(pGameInstance->Get_Front(LEVEL_STATIC, TEXT("Layer_Camera")))->Set_InDoor(true, LEVEL_EYECUBE);
					static_cast<CAll_Door*>(pGameInstance->Get_Front(g_eCurrentLevel, TEXT("Layer_OpenDoor2")))->Set_BoolCnt(true);
					RELEASE_INSTANCE(CGameInstance);
					return;
				}
			}
		}
		

		vPos = pGameInstance->Collision_SphereToRect((CCollider_Rect*)pGameInstance->Get_Component(g_eCurrentLevel, TEXT("Layer_Door"), TEXT("Com_Collider"), 2), m_pColliderCom);
		if (vPos != _float3(0.f, 0.f, 0.f))
		{
			if (true == g_bSlimeClear && true == g_bEyeCubeClear)
			{
				++ColCount;

				if (false == g_bCameraDirecting)
					if (FAILED(pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Layer_PotalButton"), TEXT("Prototype_GameObject_UI_PotalIcon"))))
					{
						MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Field::Ready_Layer_UI");
						return;
					}
				if (pGameInstance->Get_DIKeyState(DIK_SPACE) & 0x80)
				{
					m_eState = STATE_IDLE;
					m_eDir = DIR_UP;
					pGameInstance->Release_LayerObject(LEVEL_STATIC, TEXT("Layer_PotalButton"));

					m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(35.f, 0.5f, 55.5f));
					m_pTransformCom->LookAtXZ(_float3(35.f, 0.5f, 60.0f));
					_float3 vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
					D3DXVec3Normalize(&vLook, &vLook);

					_float3 vPlayerPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);


					_float3 CameraPos = vPlayerPos - (vLook * 2);
					CameraPos += _float3(0.f, 0.3f, 0.f);

					static_cast<CTransform*>(pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Camera"), TEXT("Com_Transform")))->Set_State(CTransform::STATE_POSITION, CameraPos);
					static_cast<CTransform*>(pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Camera"), TEXT("Com_Transform")))->LookAt(vPlayerPos + _float3(0.f, 0.7f, 0.f));

					g_bCameraDirecting = true;
					static_cast<CCamera_Player*>(pGameInstance->Get_Front(LEVEL_STATIC, TEXT("Layer_Camera")))->Set_InDoor(true, LEVEL_RESPAWN2);
					static_cast<CAll_Door*>(pGameInstance->Get_Front(g_eCurrentLevel, TEXT("Layer_BigDoorLeft")))->Set_BoolCnt(true);
					static_cast<CAll_Door*>(pGameInstance->Get_Front(g_eCurrentLevel, TEXT("Layer_BigDoorRight")))->Set_BoolCnt(true);
					//m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(32.f, 0.5f, 3.f));
					//pGameInstance->OpenLevel(LEVEL_LOADING, CLevel_Loading::Create(m_pGraphic_Device, LEVEL_RESPAWN2));
					RELEASE_INSTANCE(CGameInstance);
					return;
				}
			}
		}
		if (0 == ColCount)
			pGameInstance->Release_LayerObject(LEVEL_STATIC, TEXT("Layer_PotalButton"));
	}
	else if (LEVEL_RESPAWN2 == g_eCurrentLevel)
	{
		_uint ColCount = 0;

		_float3 vPos = pGameInstance->Collision_SphereToRect((CCollider_Rect*)pGameInstance->Get_Component(g_eCurrentLevel, TEXT("Layer_Door"), TEXT("Com_Collider"), 0), m_pColliderCom);
		if (false == g_bYetiClear)
		{
			if (vPos != _float3(0.f, 0.f, 0.f))
			{

				++ColCount;
				if (false == g_bCameraDirecting && (pGameInstance->Find_Layer(LEVEL_STATIC, TEXT("Layer_PotalButton")) == nullptr || pGameInstance->Get_Size(LEVEL_STATIC, TEXT("Layer_PotalButton")) == 0))
					if (FAILED(pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Layer_PotalButton"), TEXT("Prototype_GameObject_UI_PotalIcon"))))
					{
						MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Field::Ready_Layer_UI");
						return;
					}
				if (pGameInstance->Get_DIKeyState(DIK_SPACE) & 0x80)
				{
					m_eState = STATE_IDLE;
					m_eDir = DIR_UP;
					pGameInstance->Release_LayerObject(LEVEL_STATIC, TEXT("Layer_PotalButton"));

					m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(17.f, 0.5f, 41.5f));
					m_pTransformCom->LookAtXZ(_float3(17.f, 0.5f, 43.5f));
					_float3 vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
					D3DXVec3Normalize(&vLook, &vLook);

					_float3 vPlayerPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);


					_float3 CameraPos = vPlayerPos - (vLook * 2);
					CameraPos += _float3(0.f, 0.3f, 0.f);

					static_cast<CTransform*>(pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Camera"), TEXT("Com_Transform")))->Set_State(CTransform::STATE_POSITION, CameraPos);
					static_cast<CTransform*>(pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Camera"), TEXT("Com_Transform")))->LookAt(vPlayerPos + _float3(0.f, 0.7f, 0.f));

					g_bCameraDirecting = true;
					static_cast<CCamera_Player*>(pGameInstance->Get_Front(LEVEL_STATIC, TEXT("Layer_Camera")))->Set_InDoor(true, LEVEL_YETI);
					static_cast<CAll_Door2*>(pGameInstance->Get_Front(g_eCurrentLevel, TEXT("Layer_OpenDoor2")))->Set_BoolCnt(true);
					//pGameInstance->OpenLevel(LEVEL_LOADING, CLevel_Loading::Create(m_pGraphic_Device, LEVEL_SLIME));
					RELEASE_INSTANCE(CGameInstance);
					return;

				}
			}
		}
		
		if (false == g_bIceBrainClear)
		{
			vPos = pGameInstance->Collision_SphereToRect((CCollider_Rect*)pGameInstance->Get_Component(g_eCurrentLevel, TEXT("Layer_Door"), TEXT("Com_Collider"), 1), m_pColliderCom);
			if (vPos != _float3(0.f, 0.f, 0.f))
			{
				++ColCount;
				if (false == g_bCameraDirecting && (pGameInstance->Find_Layer(LEVEL_STATIC, TEXT("Layer_PotalButton")) == nullptr || pGameInstance->Get_Size(LEVEL_STATIC, TEXT("Layer_PotalButton")) == 0))
					if (FAILED(pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Layer_PotalButton"), TEXT("Prototype_GameObject_UI_PotalIcon"))))
					{
						MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Field::Ready_Layer_UI");
						return;
					}
				if (pGameInstance->Get_DIKeyState(DIK_SPACE) & 0x80)
				{
					pGameInstance->Release_LayerObject(LEVEL_STATIC, TEXT("Layer_PotalButton"));
					m_eState = STATE_IDLE;
					m_eDir = DIR_UP;
					m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(47.f, 0.5f, 41.5f));
					m_pTransformCom->LookAtXZ(_float3(47.f, 0.5f, 43.5f));
					_float3 vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
					D3DXVec3Normalize(&vLook, &vLook);

					_float3 vPlayerPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);


					_float3 CameraPos = vPlayerPos - (vLook * 2);
					CameraPos += _float3(0.f, 0.3f, 0.f);

					static_cast<CTransform*>(pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Camera"), TEXT("Com_Transform")))->Set_State(CTransform::STATE_POSITION, CameraPos);
					static_cast<CTransform*>(pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Camera"), TEXT("Com_Transform")))->LookAt(vPlayerPos + _float3(0.f, 0.7f, 0.f));

					g_bCameraDirecting = true;
					static_cast<CCamera_Player*>(pGameInstance->Get_Front(LEVEL_STATIC, TEXT("Layer_Camera")))->Set_InDoor(true, LEVEL_ICEBRAIN);
					static_cast<CAll_Door*>(pGameInstance->Get_Front(g_eCurrentLevel, TEXT("Layer_OpenDoor")))->Set_BoolCnt(true);
					//pGameInstance->OpenLevel(LEVEL_LOADING, CLevel_Loading::Create(m_pGraphic_Device, LEVEL_SLIME));
					RELEASE_INSTANCE(CGameInstance);
					return;

				}
			}
		}
		

		vPos = pGameInstance->Collision_SphereToRect((CCollider_Rect*)pGameInstance->Get_Component(g_eCurrentLevel, TEXT("Layer_Door"), TEXT("Com_Collider"), 2), m_pColliderCom);
		if (vPos != _float3(0.f, 0.f, 0.f))
		{
			if (true == g_bIceBrainClear && true == g_bYetiClear)
			{
				++ColCount;
				if (false == g_bCameraDirecting)
					if (FAILED(pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Layer_PotalButton"), TEXT("Prototype_GameObject_UI_PotalIcon"))))
					{
						MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Field::Ready_Layer_UI");
						return;
					}
				if (pGameInstance->Get_DIKeyState(DIK_SPACE) & 0x80)
				{
					pGameInstance->Release_LayerObject(LEVEL_STATIC, TEXT("Layer_PotalButton"));
					m_eState = STATE_IDLE;
					m_eDir = DIR_UP;
					m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(32.f, 9.f, 62.5f));
					m_pTransformCom->LookAtXZ(_float3(32.f, 9.f, 63.5f));
					_float3 vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
					D3DXVec3Normalize(&vLook, &vLook);

					_float3 vPlayerPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);


					_float3 CameraPos = vPlayerPos - (vLook * 2);
					CameraPos += _float3(0.f, 0.3f, 0.f);

					static_cast<CTransform*>(pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Camera"), TEXT("Com_Transform")))->Set_State(CTransform::STATE_POSITION, CameraPos);
					static_cast<CTransform*>(pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Camera"), TEXT("Com_Transform")))->LookAt(vPlayerPos + _float3(0.f, 0.7f, 0.f));

					g_bCameraDirecting = true;
					static_cast<CCamera_Player*>(pGameInstance->Get_Front(LEVEL_STATIC, TEXT("Layer_Camera")))->Set_InDoor(true, LEVEL_COLOSUSS);
					RELEASE_INSTANCE(CGameInstance);
					return;

				}
			}
		}
		if (0 == ColCount)
			pGameInstance->Release_LayerObject(LEVEL_STATIC, TEXT("Layer_PotalButton"));

	}
	if (LEVEL_ICEBRAIN == g_eCurrentLevel)
	{
		_float3 vPosition = pGameInstance->Collision_SphereToRect((CCollider_Rect*)pGameInstance->Get_Component(g_eCurrentLevel, TEXT("Layer_Door"), TEXT("Com_Collider"), 0), m_pColliderCom);
		if (vPosition != _float3(0.f, 0.f, 0.f))
		{
			if (true == g_bIceBrainClear)
			{
				if (false == g_bCameraDirecting)
					if (FAILED(pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Layer_PotalButton"), TEXT("Prototype_GameObject_UI_PotalIcon"))))
					{
						MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Field::Ready_Layer_UI");
						return;
					}
				if (pGameInstance->Get_DIKeyState(DIK_SPACE) & 0x80)
				{
					pGameInstance->Release_LayerObject(LEVEL_STATIC, TEXT("Layer_PotalButton"));
					m_eState = STATE_IDLE;
					m_eDir = DIR_UP;
					m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(47.f, 0.5f, 41.5f));
					pGameInstance->OpenLevel(LEVEL_LOADING, CLevel_Loading::Create(m_pGraphic_Device, LEVEL_RESPAWN2));
					RELEASE_INSTANCE(CGameInstance);
					return;
				}
			}

		}
		else
			pGameInstance->Release_LayerObject(LEVEL_STATIC, TEXT("Layer_PotalButton"));

		m_pColliderCom->Set_Radius(0.7f);

		_float3 vPos;
		if (_float3(0.f, 0.f, 0.f) != (vPos = pGameInstance->Collision_SphereGuard((CCollider_Sphere*)pGameInstance->Get_Component(LEVEL_ICEBRAIN, TEXT("Layer_Ice"), TEXT("Com_Collider")), m_pColliderCom)))
		{
			m_bDie = true;
		}
		m_pColliderCom->Set_Radius(0.3f);
	}
	else if (LEVEL_YETI == g_eCurrentLevel)
	{
		_float3 vPosition = pGameInstance->Collision_SphereToRect((CCollider_Rect*)pGameInstance->Get_Component(g_eCurrentLevel, TEXT("Layer_Door"), TEXT("Com_Collider"), 0), m_pColliderCom);
		if (vPosition != _float3(0.f, 0.f, 0.f))
		{
			if (true == g_bYetiClear)
			{
				if (false == g_bCameraDirecting)
					if (FAILED(pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Layer_PotalButton"), TEXT("Prototype_GameObject_UI_PotalIcon"))))
					{
						MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Field::Ready_Layer_UI");
						return;
					}
				if (pGameInstance->Get_DIKeyState(DIK_SPACE) & 0x80)
				{
					pGameInstance->Release_LayerObject(LEVEL_STATIC, TEXT("Layer_PotalButton"));
					m_eState = STATE_IDLE;
					m_eDir = DIR_UP;
					m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(17.f, 0.5f, 41.5f));
					pGameInstance->OpenLevel(LEVEL_LOADING, CLevel_Loading::Create(m_pGraphic_Device, LEVEL_RESPAWN2));
					RELEASE_INSTANCE(CGameInstance);
					return;
				}
			}

		}
		else
			pGameInstance->Release_LayerObject(LEVEL_STATIC, TEXT("Layer_PotalButton"));

		_float3 vPos;
		if (_float3(0.f, 0.f, 0.f) != (vPos = pGameInstance->Collision_SphereGuard((CCollider_Sphere*)pGameInstance->Get_Component(LEVEL_YETI, TEXT("Layer_Yeti"), TEXT("Com_Collider")), m_pColliderCom)))
		{
			m_bDie = true;
			RELEASE_INSTANCE(CGameInstance);
			return;
			//m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
		}


	}
	else if (LEVEL_SLIME == g_eCurrentLevel)
	{
		_float3 vPos = pGameInstance->Collision_SphereToRect((CCollider_Rect*)pGameInstance->Get_Component(g_eCurrentLevel, TEXT("Layer_Door"), TEXT("Com_Collider"), 0), m_pColliderCom);
		if (vPos != _float3(0.f, 0.f, 0.f))
		{
			if (true == g_bSlimeClear)
			{
				if (false == g_bCameraDirecting)
					if (FAILED(pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Layer_PotalButton"), TEXT("Prototype_GameObject_UI_PotalIcon"))))
					{
						MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Field::Ready_Layer_UI");
						return;
					}
				if (pGameInstance->Get_DIKeyState(DIK_SPACE) & 0x80)
				{
					pGameInstance->Release_LayerObject(LEVEL_STATIC, TEXT("Layer_PotalButton"));
					m_eState = STATE_IDLE;
					m_eDir = DIR_UP;
					m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(15.5f, 0.f, 46.0f));
					pGameInstance->OpenLevel(LEVEL_LOADING, CLevel_Loading::Create(m_pGraphic_Device, LEVEL_RESPAWN1));
					RELEASE_INSTANCE(CGameInstance);
					return;
				}
			}

		}
		else
			pGameInstance->Release_LayerObject(LEVEL_STATIC, TEXT("Layer_PotalButton"));


		if ((nullptr != pGameInstance->Find_Layer(LEVEL_SLIME, TEXT("Layer_Slime_Boss")) && 0 < pGameInstance->Get_Size(LEVEL_SLIME, TEXT("Layer_Slime_Boss"))) && true == pGameInstance->Collision_Sphere((CCollider_Sphere*)pGameInstance->Get_Component(LEVEL_SLIME, TEXT("Layer_Slime_Boss"), TEXT("Com_Collider")), m_pColliderCom))
		{
			m_bDie = true;
			RELEASE_INSTANCE(CGameInstance);
			return;
		}
		if ((nullptr != pGameInstance->Find_Layer(LEVEL_SLIME, TEXT("Layer_Slime_Heart")) && 0 < pGameInstance->Get_Size(LEVEL_SLIME, TEXT("Layer_Slime_Heart"))) && true == pGameInstance->Collision_Sphere((CCollider_Sphere*)pGameInstance->Get_Component(LEVEL_SLIME, TEXT("Layer_Slime_Heart"), TEXT("Com_Collider")), m_pColliderCom))
		{
			m_bDie = true;
			RELEASE_INSTANCE(CGameInstance);
			return;
		}
		if (nullptr != pGameInstance->Find_Layer(LEVEL_SLIME, TEXT("Layer_Slime")) && 0 < pGameInstance->Get_Size(LEVEL_SLIME, TEXT("Layer_Slime")))
		{
			for (size_t i = 0; i < pGameInstance->Get_Size(LEVEL_SLIME, TEXT("Layer_Slime")); i++)
			{
				if (true == pGameInstance->Collision_Sphere((CCollider_Sphere*)pGameInstance->Get_Component(LEVEL_SLIME, TEXT("Layer_Slime"), TEXT("Com_Collider")), m_pColliderCom))
				{
					m_bDie = true;
					RELEASE_INSTANCE(CGameInstance);
					return;
				}
			}
		}
	}
	else if (LEVEL_EYECUBE == g_eCurrentLevel)
	{
		_float3 vPosition = pGameInstance->Collision_SphereToRect((CCollider_Rect*)pGameInstance->Get_Component(g_eCurrentLevel, TEXT("Layer_Door"), TEXT("Com_Collider"), 0), m_pColliderCom);
		if (vPosition != _float3(0.f, 0.f, 0.f))
		{
			if (true == g_bEyeCubeClear)
			{
				if (false == g_bCameraDirecting)
					if (FAILED(pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Layer_PotalButton"), TEXT("Prototype_GameObject_UI_PotalIcon"))))
					{
						MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Field::Ready_Layer_UI");
						return;
					}
				if (pGameInstance->Get_DIKeyState(DIK_SPACE) & 0x80)
				{
					pGameInstance->Release_LayerObject(LEVEL_STATIC, TEXT("Layer_PotalButton"));
					m_eState = STATE_IDLE;
					m_eDir = DIR_UP;
					m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(55.5f, 0.5f, 40.f));
					pGameInstance->OpenLevel(LEVEL_LOADING, CLevel_Loading::Create(m_pGraphic_Device, LEVEL_RESPAWN1));
					RELEASE_INSTANCE(CGameInstance);
					return;
				}
			}

		}
		else
			pGameInstance->Release_LayerObject(LEVEL_STATIC, TEXT("Layer_PotalButton"));

		m_pColliderCom->Set_Radius(0.7f);

		_float3 vPos;
		if (_float3(0.f, 0.f, 0.f) != (vPos = pGameInstance->Collision_SphereGuard((CCollider_Sphere*)pGameInstance->Get_Component(LEVEL_EYECUBE, TEXT("Layer_EyeCube"), TEXT("Com_Collider")), m_pColliderCom)))
		{
			if (static_cast<CTransform*>(pGameInstance->Get_Component(LEVEL_EYECUBE, TEXT("Layer_EyeCube"), TEXT("Com_Transform")))->Get_State(CTransform::STATE_POSITION).y > 2.f)
			{

				m_bDie = true;
				m_pColliderCom->Set_Radius(0.3f);
				RELEASE_INSTANCE(CGameInstance);
				return;
			}
			if (m_bDie == false)
				m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
		}
		m_pColliderCom->Set_Radius(0.3f);

	}
	else if (LEVEL_COLOSUSS == g_eCurrentLevel && false == g_bColossusClear)
	{
		_float3 vPos;
		if (_float3(0.f, 0.f, 0.f) != (vPos = pGameInstance->Collision_SphereGuard((CCollider_Sphere*)pGameInstance->Get_Component(LEVEL_COLOSUSS, TEXT("Layer_Clossus_Fist_Left"), TEXT("Com_Collider")), m_pColliderCom)))
		{
			m_bDie = true;
			RELEASE_INSTANCE(CGameInstance);
			return;
		}
		if (_float3(0.f, 0.f, 0.f) != (vPos = pGameInstance->Collision_SphereGuard((CCollider_Sphere*)pGameInstance->Get_Component(LEVEL_COLOSUSS, TEXT("Layer_Clossus_Fist_Right"), TEXT("Com_Collider")), m_pColliderCom)))
		{
			m_bDie = true;
			RELEASE_INSTANCE(CGameInstance);
			return;
		}
	}



	RELEASE_INSTANCE(CGameInstance);
}

void CPlayer::Go_FrameTurn()
{
	switch (_int(m_tFrame.iFrameCurrentX))
	{
	case 3:
		m_tFrame.iFrameCurrentX = 4.f;
		break;
	case 4:
		m_tFrame.iFrameCurrentX = 0.f;
		break;
	case 0:
		m_tFrame.iFrameCurrentX = 7.f;
		break;
	case 7:
		m_tFrame.iFrameCurrentX = 1.f;
		break;
	case 1:
		m_tFrame.iFrameCurrentX = 6.f;
		break;
	case 6:
		m_tFrame.iFrameCurrentX = 2.f;
		break;
	case 2:
		m_tFrame.iFrameCurrentX = 5.f;
		break;
	case 5:
		m_tFrame.iFrameCurrentX = 3.f;
		m_iTurnNum++;
		break;
	}
}

HRESULT CPlayer::Move(_float fTimeDelta)
{
	if (STATE_SHOT == m_eState)
		return S_OK;
	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);

	CTransform::TRANSFORMDESC	tTransformDesc = m_pTransformCom->Get_TransformDesc();
	tTransformDesc.fSpeedPerSec = PLAYER_MOVESPEED;
	// 플레이어 움직임
	if (pGameInstance->Get_DIKeyState(DIK_A) & 0x80)
	{
		m_eState = STATE_WALK;
		m_eDir = DIR_LEFT;
		m_pTransformCom->Go_Left(fTimeDelta);
	}

	if (pGameInstance->Get_DIKeyState(DIK_D) & 0x80)
	{
		m_eState = STATE_WALK;

		m_eDir = DIR_RIGHT;
		m_pTransformCom->Go_Right(fTimeDelta);
	}

	if (pGameInstance->Get_DIKeyState(DIK_W) & 0x80)
	{
		m_eState = STATE_WALK;

		if (pGameInstance->Get_DIKeyState(DIK_A) & 0x80)
		{
			tTransformDesc.fSpeedPerSec = PLAYER_MOVESPEED / 1.414f;
			m_eDir = DIR_UL;
		}
		else if (pGameInstance->Get_DIKeyState(DIK_D) & 0x80)
		{
			tTransformDesc.fSpeedPerSec = PLAYER_MOVESPEED / 1.414f;
			m_eDir = DIR_UR;
		}
		else
		{
			m_eDir = DIR_UP;
		}
		m_pTransformCom->Go_Straight(fTimeDelta);
	}

	if (pGameInstance->Get_DIKeyState(DIK_S) & 0x80)
	{
		m_eState = STATE_WALK;

		if (pGameInstance->Get_DIKeyState(DIK_A) & 0x80)
		{
			tTransformDesc.fSpeedPerSec = PLAYER_MOVESPEED / 1.414f;
			m_eDir = DIR_DL;
		}
		else if (pGameInstance->Get_DIKeyState(DIK_D) & 0x80)
		{
			tTransformDesc.fSpeedPerSec = PLAYER_MOVESPEED / 1.414f;
			m_eDir = DIR_DR;
		}
		else
		{
			m_eDir = DIR_DOWN;
		}

		m_pTransformCom->Go_BackWard(fTimeDelta);
	}
	m_pTransformCom->Set_TransformDesc(tTransformDesc);


	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CPlayer::Rolling(_float fTimeDelta)
{
	if (0 > --m_iRollDelay)
		m_iRollDelay = 0;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CTransform::TRANSFORMDESC	tTransformDesc = m_pTransformCom->Get_TransformDesc();
	tTransformDesc.fSpeedPerSec = PLAYER_MOVESPEED;
	//구르기
	if (m_tFrame.iFrameStartX < 6.f && (pGameInstance->Get_DIKeyState(DIK_LSHIFT) & 0x80))
	{
		if (!m_iRollDelay)
		{
			m_iRollDelay = 40;
			m_bRolling_Dust = true; // Dust 추가
			m_eState = STATE_ROLLING;
			m_tFrame.iFrameStartX = 6.f;
			m_tFrame.iFrameCurrentX = m_tFrame.iFrameStartX;
			m_tFrame.iFrameEndX = m_tFrame.iFrameStartX + 5.f;
			if (FAILED(pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Layer_CoolTime"), TEXT("Prototype_GameObject_UI_SkillCoolingTimer")))) {
				MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Field::Ready_Layer_UI");
				return E_FAIL;
			}
		}



	}
	if (m_tFrame.iFrameStartX == 6.f)
	{
		//tTransformDesc.fSpeedPerSec *= 3.f;
		m_fSpriteSpeed = 2.f;
		//m_pTransformCom->Set_TransformDesc(tTransformDesc);

		switch (m_eDir) 	// 대각선 정규화
		{
		case DIR_RIGHT:
			m_pTransformCom->Go_Right(fTimeDelta * 3.f);
			break;
		case DIR_UP:
			m_pTransformCom->Go_Straight(fTimeDelta * 3.f);
			break;
		case DIR_LEFT:
			m_pTransformCom->Go_Left(fTimeDelta * 3.f);
			break;
		case DIR_DOWN:
			m_pTransformCom->Go_BackWard(fTimeDelta * 3.f);
			break;
		case DIR_DR:
			m_pTransformCom->Go_Right(fTimeDelta * 3.f);
			m_pTransformCom->Go_BackWard(fTimeDelta * 3.f);
			break;
		case DIR_DL:
			m_pTransformCom->Go_Left(fTimeDelta * 3.f);
			m_pTransformCom->Go_BackWard(fTimeDelta * 3.f);
			break;
		case DIR_UL:
			m_pTransformCom->Go_Straight(fTimeDelta * 3.f);
			m_pTransformCom->Go_Left(fTimeDelta * 3.f);
			break;
		case DIR_UR:
			m_pTransformCom->Go_Right(fTimeDelta * 3.f);
			m_pTransformCom->Go_Straight(fTimeDelta * 3.f);
			break;
		}
	}
	if (STATE_ROLLING == m_eState && (_uint)m_tFrame.iFrameCurrentX == 11.f && 8.f > m_tFrame.iFrameStartY)
	{
		pGameInstance->StopSound(CSoundMgr::PLAYER);// 사운드	
		tTransformDesc.fSpeedPerSec /= 4.f;
		m_tFrame.iFrameStartX -= 6.f;
		m_tFrame.iFrameEndX -= 6.f;
		m_eState = STATE_IDLE;

	}
	m_pTransformCom->Set_TransformDesc(tTransformDesc);

	if (true == m_bRolling_Dust)
	{
		m_fDustSec += 1.f * fTimeDelta;
		pGameInstance->LetsPlaySound(L"Player_Roll.ogg", CSoundMgr::PLAYER, 1.f); // 사운드
		pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Layer_Player_Dust"), TEXT("Prototype_GameObject_PlayerDust"), this);
	}

	if (m_fDustSec >= 0.3f)
	{
		m_fDustSec = 0.f;
		m_bRolling_Dust = false;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CPlayer::Shot(CGameInstance* pGameInstance, _float fTimeDelta)
{
	_long		MouseClick = 0;

	if (MouseClick = pGameInstance->Get_DIMouseButtonState(CInput_Device::MBS_LBUTTON))
	{
		if (false == m_isArrow)
		{
			if (FAILED(pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Layer_Arrow"), TEXT("Prototype_GameObject_Arrow"), this)))
				return -1;
			m_isArrow = true;
			m_eDir = DIR_UP;
			m_tFrame.iFrameStartX = 8.f;
			m_tFrame.iFrameCurrentX = m_tFrame.iFrameStartX;
			m_tFrame.iFrameEndX = 13.f;
		}
		if (true == m_bHaveArrow && true == m_isArrow)
		{
			pGameInstance->LetsPlaySound(L"arrow_charge.ogg", CSoundMgr::PLAYER_EFFECT, 1.0f); // 사운드
			m_iShotPower += 1.f;
			if (m_iShotPower > 60.f)
			{
				// 벽에 박히는 화살
				static_cast<CArrow*>(pGameInstance->Get_Front(LEVEL_STATIC, TEXT("Layer_Arrow")))->Set_FullCharging(true);
				m_iShotPower = 60.f;
				pGameInstance->StopSound(CSoundMgr::PLAYER_EFFECT);// 사운드
			}
			m_eState = STATE_SHOT;
			m_tFrame.iFrameStartY = 9.f;
		}
	}

	if ((true == m_isArrow) && true == m_bHaveArrow && pGameInstance->Get_Mouse_Up(CInput_Device::MBS_LBUTTON))
	{
		m_bMoveEndArrow = false;
		m_bHaveArrow = false;
		m_eState = STATE_IDLE;
		m_tFrame.iFrameStartX = 0.f;
		m_tFrame.iFrameCurrentX = m_tFrame.iFrameStartX;
		m_tFrame.iFrameEndX = 5.f;
		m_tFrame.iFrameStartY = 1.f;
		pGameInstance->LetsPlaySound(L"arrow_shoot.ogg", CSoundMgr::PLAYER_SHOT, 0.5f); // 사운드
	}

	if ((true == m_isArrow) && false == m_bHaveArrow)
	{
		CTransform* pArrowTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Arrow"), TEXT("Com_Transform"));
		pArrowTransform->Go_Straight(fTimeDelta * m_iShotPower);
		//m_iShotPower -= 0.01f;
		if (m_iShotPower < 0.f)
			m_iShotPower = 0.f;
	}
	else if (false == m_isArrow && true == m_bHaveArrow)
		m_iShotPower = 0.f;
	return S_OK;
}

HRESULT CPlayer::AimMove(_float fTimeDelta)
{
	_long		MouseMove = 0;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CTransform* pCameraTrans = (CTransform*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Camera"), TEXT("Com_Transform"));

	if (STATE_PULL != m_eState)
	{


		if (MouseMove = pGameInstance->Get_DIMouseMoveState(CInput_Device::MMS_Y))
		{
			//
			m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), fTimeDelta * MouseMove * 0.1f);
			//m_pTransformCom->Revolution(pCameraTrans->Get_State(CTransform::STATE_RIGHT), m_pTransformCom->Get_State(CTransform::STATE_POSITION), fTimeDelta * -MouseMove * 0.1f);
			m_iLookHeight -= MouseMove* 0.01f;

			if (m_iLookHeight > 2)
				m_iLookHeight = 2;


			if (m_iLookHeight < -1)
				m_iLookHeight = -1;

			if (true == m_bHaveArrow && true == m_isArrow)
			{
				_float3 vCameraLook = pCameraTrans->Get_State(CTransform::STATE_LOOK);
				_float3 vCameraPos = pCameraTrans->Get_State(CTransform::STATE_POSITION);
				vCameraPos += (vCameraLook * 15);
				CTransform* pArrowTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Arrow"), TEXT("Com_Transform"));

				pArrowTransform->LookAt(vCameraPos);
			}
		}

		if (MouseMove = pGameInstance->Get_DIMouseMoveState(CInput_Device::MMS_X))
		{
			m_pTransformCom->Turn(_float3(0.f, 1.f, 0.f), fTimeDelta * MouseMove * 0.1f);

			if (true == m_bHaveArrow && true == m_isArrow)
			{
				CTransform* pArrowTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Arrow"), TEXT("Com_Transform"));
				pArrowTransform->Revolution(_float3(0.f, 1.f, 0.f), m_pTransformCom->Get_State(CTransform::STATE_POSITION), fTimeDelta * MouseMove * 0.1f);
			}
		}

		static_cast<CCamera_Player*>(pGameInstance->Find_Layer(LEVEL_STATIC, TEXT("Layer_Camera"))->Get_Object().front())->Set_IsArrow(m_bHaveArrow);

		_float3 vCameraLook = pCameraTrans->Get_State(CTransform::STATE_LOOK);
		_float3 vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
		D3DXVec3Normalize(&vLook, &vLook);

		_float3 vPlayerPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);


		_float3 CameraPos = vPlayerPos - (vLook * 2);
		CameraPos += _float3(0.f, 1.2f - m_iLookHeight, 0.f);
		if (true == m_bSoulStart)
		{
			//CameraPos = vPlayerPos - (vLook * 4);
			CameraPos.y = vPlayerPos.y;
			pCameraTrans->Set_State(CTransform::STATE_POSITION, CameraPos);
			pCameraTrans->LookAt(vPlayerPos);
			g_bCameraDirecting = true;
			static_cast<CCamera_Player*>(pGameInstance->Get_Front(LEVEL_STATIC, TEXT("Layer_Camera")))->Set_Directing(true, false, false);
		}
		else
		{
			pCameraTrans->Set_State(CTransform::STATE_POSITION, CameraPos);
			pCameraTrans->LookAt(vPlayerPos + _float3(0.f, 0.7f, 0.f));
		}
	}
	else if (STATE_PULL == m_eState && false == m_bHaveArrow)
	{
		CTransform* pArrowTrans = static_cast<CTransform*>(pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Arrow"), TEXT("Com_Transform")));
		_float3 vLook = pArrowTrans->Get_State(CTransform::STATE_LOOK);
		D3DXVec3Normalize(&vLook, &vLook);

		_float3 vArrowPos = pArrowTrans->Get_State(CTransform::STATE_POSITION);

		_float3 CameraPos = vArrowPos - (vLook * 2);


		m_pTransformCom->MatchIt_XZ(CameraPos);
		m_eDir = DIR_DOWN;
		m_tFrame.iFrameStartY = 3.f;
		CameraPos += _float3(0.f, 1.f, 0.f);


		pCameraTrans->Set_State(CTransform::STATE_POSITION, CameraPos);

		pCameraTrans->LookAt(vArrowPos + _float3(0.f, 0.2f, 0.f));
	}
	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CPlayer::PullArrow(CGameInstance* pGameInstance, _float fTimeDelta)
{
	if (pGameInstance->Get_DIKeyState(DIK_E) & 0x80)
	{
		if (true == m_isArrow && false == m_bHaveArrow)
		{
			CTransform* pArrowTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Arrow"), TEXT("Com_Transform"));
			if (false == m_bMoveEndArrow)
				return S_OK;


			_float3 vArrowPos = pArrowTransform->Get_State(CTransform::STATE_POSITION);
			pArrowTransform->LookAt(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
			pArrowTransform->Go_Straight(fTimeDelta * m_iShotPower);
			m_iShotPower += 0.5f;
			if (m_iShotPower > 30.f)
				m_iShotPower = 30.f;
			if (STATE_PULL != m_eState)
			{
				pGameInstance->StopSound(CSoundMgr::PLAYER_SHOT);
				pGameInstance->LetsPlaySound(L"arrow_retrieval.ogg", CSoundMgr::PLAYER_SHOT, 1.f);
				m_tFrame.iFrameStartX = 13.f;
				m_tFrame.iFrameCurrentX = 13.f;
				m_tFrame.iFrameEndX = 14.f;
				m_tFrame.iFrameStartY = 1.f;
			}
			m_eState = STATE_PULL;
			m_bPushE = true;
			m_eDir = DIR_DOWN;
		}
	}
	else if (true == m_bPushE)
	{
		pGameInstance->StopSound(CSoundMgr::PLAYER_SHOT);
		m_pTransformCom->LookAtXZ(static_cast<CTransform*>(pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Camera"), TEXT("Com_Transform")))->Get_State(CTransform::STATE_POSITION));
		m_eDir = DIR_UP;
		m_eState = STATE_IDLE;
		m_bPushE = false;
		m_tFrame.iFrameStartX = 0.f;
		m_tFrame.iFrameCurrentX = 0.f;
		m_tFrame.iFrameEndX = 5.f;
		m_tFrame.iFrameStartY = 1.f;
	}


	return S_OK;
}

HRESULT CPlayer::Die()
{
	if (STATE_DIE == m_eState)
	{
		m_tFrame.iFrameStartX = 12.f;
		m_tFrame.iFrameCurrentX = 12.f;
		m_tFrame.iFrameEndX = 12.f;
	}
	return S_OK;
}

CPlayer * CPlayer::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPlayer*	pInstance = new CPlayer(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create CPlayer in CPlayer::Create");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CPlayer::Clone(void * pArg)
{
	CPlayer*	pInstance = new CPlayer(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create CPlayer in CPlayer::Clone");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPlayer::Free()
{
	__super::Free();
	//#ifdef _DEBUG
	Safe_Release(m_pVIBufferCollCom);

	//#endif // _DEBUG
	Safe_Release(m_pTestCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
