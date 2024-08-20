#include "stdafx.h"
#include "..\Public\Yeti.h"
#include "GameInstance.h"
#include "Camera_Player.h"
#include "DarkScreen.h"

CYeti::CYeti(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CYeti::CYeti(const CYeti & rhs)
	: CGameObject(rhs)
{
}

HRESULT CYeti::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CYeti::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	/* 현재 객체에게 추가되어야할 컴포넌트들을 복제(or 참조)하여 멤버변수에 보관한다.  */
	if (FAILED(SetUp_Components()))
		return E_FAIL;
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	m_pCamera = static_cast<CCamera_Player*>(pGameInstance->Get_Front(LEVEL_STATIC, TEXT("Layer_Camera")));
	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(16.f, 0.f, 16.f));
	m_pTransformCom->Scaled(_float3(m_fScale, m_fScale, m_fScale));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(16.f, 2.0f, 20.F));
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, _float3(0, 0, -1.f));
	m_tFrame.iFrameStartY = 7.f;
	m_tFrame.iFrameCurrentX = 0.f;
	m_tFrame.iFrameStartX = 0.f;
	m_tFrame.iFrameEndX = 2.f;
	m_tFrame.iFrameHeight = 96.f;
	m_tFrame.iFrameWidth = 96.f;
	m_tFrame.iTexHeight = 1600.f;
	m_tFrame.iTexWidth = 1024.f;

	m_eState = STATE_IDLE;

	pGameInstance->Add_GameObject(LEVEL_YETI, TEXT("Layer_Yeti_Shadow"), TEXT("Prototype_GameObject_Yeti_Shadow"), this);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

_int CYeti::Tick(_float fTimeDelta)
{


	if (0 > __super::Tick(fTimeDelta))
		return -1;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	CTransform* m_pPlayerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"));
	CTransform* m_pCameraTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Camera"), TEXT("Com_Transform"));
	_float3 PlayerPos = m_pPlayerTransform->Get_State(CTransform::STATE_POSITION);
	_float3 CameraPos = m_pCameraTransform->Get_State(CTransform::STATE_POSITION);
	//m_tFrame.
	m_fFrame += 90.0f * fTimeDelta; // 1초에 띄울 프레임의 갯수만큼 상수를 바꾼다
									// 예티 이미지 사이즈는 96이다
	if (m_fFrame >= 90.0f) // 만약 프레임을 다 돌았다면 처음으로 되돌린다
		m_fFrame = 0.f;
	//m_pTransformCom->LookTurn(m_pTransformCom->Get_State(CTransform::STATE_POSITION),m_pPlayerTransform->Get_State(CTransform::STATE_POSITION),fTimeDelta);
	//_float angle = m_pTransformCom->LookCompare(_float3(0.f,0.f,1.f));

	_float3		vRight = _float3(-1.f, 0.f, 0.f);
	_float3		vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	_float3		vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	_float3 vDir = CameraPos - vPos;
	//_float3 vDir = PlayerPos - vPos;
	vDir.y = 0.f;
	D3DXVec3Normalize(&vDir, &vDir);
	D3DXVec3Normalize(&vLook, &vLook);
	_float fAngle = D3DXToDegree(acos(D3DXVec3Dot(&vDir, &vLook)));
	_float d = -(vRight.x * vPos.x + vRight.y * vPos.y + vRight.z * vPos.z);
	//_float LR = vRight.x * PlayerPos.x + vRight.y * PlayerPos.y + vRight.z * PlayerPos.z + d;
	_float LR = vRight.x * CameraPos.x + vRight.y * CameraPos.y + vRight.z * CameraPos.z + d;
	if (0 > LR)
		fAngle = -fAngle;

	if (Shake) {
		static_cast<CCamera_Player*>(pGameInstance->Get_Front(LEVEL_STATIC, TEXT("Layer_Camera")))->Set_Shake(false, true, false);
		ShakeTime++;
	}
	if (ShakeTime > 20) {
		Shake = false;
		ShakeTime = 0;
	}

	/*if (g_bYetiClear == true) {
	m_tFrame.iFrameStartX = 6.f;
	m_tFrame.iFrameCurrentX = m_tFrame.iFrameStartX;
	m_tFrame.iFrameEndX = m_tFrame.iFrameStartX + 2.f;
	}
	*/
	//movesprite
	m_fSpriteSpeed = 1.f;
	if (m_eState == STATE_ROLLING) {
		m_fSpriteSpeed = 5.f;
	}
	if (m_bWaiting == true) {
		m_iRolingWaitTime++;
	}
	if (m_iRolingWaitTime > 150) {
		m_bWaiting = false;
		m_iRolingWaitTime = 0;
	}

	if (fAngle >= 0 && fAngle <= 22.5f) { //정면
		if (m_eState == STATE_SHOT) {
			m_tFrame.iFrameStartY = 3;
			/*m_tFrame.iFrameStartX = 3.f;
			m_tFrame.iFrameCurrentX = m_tFrame.iFrameStartX;
			m_tFrame.iFrameEndX = m_tFrame.iFrameStartX + 2.f;*/
		}
		else if (m_eState == STATE_ROLLING) {
			m_tFrame.iFrameStartY = 3;
			/*		m_tFrame.iFrameStartX = 6.f;
			m_tFrame.iFrameCurrentX = m_tFrame.iFrameStartX;
			m_tFrame.iFrameEndX = m_tFrame.iFrameStartX + 2.f;*/
		}
		else if (m_eState == STATE_IDLE)
			m_tFrame.iFrameStartY = 11;
	}
	else if (fAngle > 22.5f && fAngle <= 67.5f) { //왼 대각측면
		if (m_eState == STATE_SHOT) {
			m_tFrame.iFrameStartY = 6;
			/*	m_tFrame.iFrameStartX = 3.f;
			m_tFrame.iFrameCurrentX = m_tFrame.iFrameStartX;
			m_tFrame.iFrameEndX = m_tFrame.iFrameStartX + 2.f;*/
		}
		else if (m_eState == STATE_ROLLING) {
			m_tFrame.iFrameStartY = 6;
			/*	m_tFrame.iFrameStartX = 6.f;
			m_tFrame.iFrameCurrentX = m_tFrame.iFrameStartX;
			m_tFrame.iFrameEndX = m_tFrame.iFrameStartX + 2.f;*/
		}
		else if (m_eState == STATE_IDLE)
			m_tFrame.iFrameStartY = 14;
	}
	else if (fAngle > 67.5f && fAngle <= 112.5f) { //측면 
		if (m_eState == STATE_SHOT) {
			m_tFrame.iFrameStartY = 1;
			/*m_tFrame.iFrameStartX = 3.f;
			m_tFrame.iFrameCurrentX = m_tFrame.iFrameStartX;
			m_tFrame.iFrameEndX = m_tFrame.iFrameStartX + 2.f;*/
		}
		else if (m_eState == STATE_ROLLING) {
			m_tFrame.iFrameStartY = 1;
			/*m_tFrame.iFrameStartX = 6.f;
			m_tFrame.iFrameCurrentX = m_tFrame.iFrameStartX;
			m_tFrame.iFrameEndX = m_tFrame.iFrameStartX + 2.f;*/
		}
		else if (m_eState == STATE_IDLE)
			m_tFrame.iFrameStartY = 8;
	}
	else if (fAngle > 112.5f && fAngle <= 157.5f) { // 왼 대각 뒷측면
		if (m_eState == STATE_SHOT) {
			m_tFrame.iFrameStartY = 4;
			/*m_tFrame.iFrameStartX = 3.f;
			m_tFrame.iFrameCurrentX = m_tFrame.iFrameStartX;
			m_tFrame.iFrameEndX = m_tFrame.iFrameStartX + 2.f;*/
		}
		else if (m_eState == STATE_ROLLING) {
			m_tFrame.iFrameStartY = 4;
			/*m_tFrame.iFrameStartX = 6.f;
			m_tFrame.iFrameCurrentX = m_tFrame.iFrameStartX;
			m_tFrame.iFrameEndX = m_tFrame.iFrameStartX + 2.f;*/
		}
		else if (m_eState == STATE_IDLE)
			m_tFrame.iFrameStartY = 12;
	}
	else if (fAngle > 157.5f && fAngle <= 180.f) { //뒷면
		if (m_eState == STATE_SHOT) {
			m_tFrame.iFrameStartY = 2;
			/*	m_tFrame.iFrameStartX = 3.f;
			m_tFrame.iFrameCurrentX = m_tFrame.iFrameStartX;
			m_tFrame.iFrameEndX = m_tFrame.iFrameStartX + 2.f;*/
		}
		else if (m_eState == STATE_ROLLING) {
			m_tFrame.iFrameStartY = 2;
			/*	m_tFrame.iFrameStartX = 6.f;
			m_tFrame.iFrameCurrentX = m_tFrame.iFrameStartX;
			m_tFrame.iFrameEndX = m_tFrame.iFrameStartX + 2.f;*/
		}
		else if (m_eState == STATE_IDLE)
			m_tFrame.iFrameStartY = 10;
	}
	else if (fAngle < -157.5f && fAngle >= -180.f) {
		if (m_eState == STATE_SHOT) {
			m_tFrame.iFrameStartY = 2;
			/*m_tFrame.iFrameStartX = 3.f;
			m_tFrame.iFrameCurrentX = m_tFrame.iFrameStartX;
			m_tFrame.iFrameEndX = m_tFrame.iFrameStartX + 2.f;*/
		}
		else if (m_eState == STATE_ROLLING) {
			m_tFrame.iFrameStartY = 2;
			/*	m_tFrame.iFrameStartX = 6.f;
			m_tFrame.iFrameCurrentX = m_tFrame.iFrameStartX;
			m_tFrame.iFrameEndX = m_tFrame.iFrameStartX + 2.f;*/
		}
		else if (m_eState == STATE_IDLE)
			m_tFrame.iFrameStartY = 10;
	}
	else if (fAngle < -112.5f && fAngle >= -157.5f) {
		if (m_eState == STATE_SHOT) {
			m_tFrame.iFrameStartY = 5;
			//m_tFrame.iFrameStartX = 3.f;
			//m_tFrame.iFrameCurrentX = m_tFrame.iFrameStartX;
			//m_tFrame.iFrameEndX = m_tFrame.iFrameStartX + 2.f;
		}
		else if (m_eState == STATE_ROLLING) {
			m_tFrame.iFrameStartY = 5;
			/*m_tFrame.iFrameStartX = 6.f;
			m_tFrame.iFrameCurrentX = m_tFrame.iFrameStartX;
			m_tFrame.iFrameEndX = m_tFrame.iFrameStartX + 2.f;*/
		}
		else if (m_eState == STATE_IDLE)
			m_tFrame.iFrameStartY = 12;
	}
	else if (fAngle < -67.5f && fAngle >= -112.5f) {
		if (m_eState == STATE_SHOT) {
			m_tFrame.iFrameStartY = 0;
			/*m_tFrame.iFrameStartX = 3.f;
			m_tFrame.iFrameCurrentX = m_tFrame.iFrameStartX;
			m_tFrame.iFrameEndX = m_tFrame.iFrameStartX + 2.f;*/
		}
		else if (m_eState == STATE_ROLLING) {
			m_tFrame.iFrameStartY = 0;
			/*m_tFrame.iFrameStartX = 6.f;
			m_tFrame.iFrameCurrentX = m_tFrame.iFrameStartX;
			m_tFrame.iFrameEndX = m_tFrame.iFrameStartX + 2.f;*/
		}
		else if (m_eState == STATE_IDLE)
			m_tFrame.iFrameStartY = 9;
	}
	else if (fAngle < -22.5f && fAngle >= -67.5f) { // 오른 앞 측
		if (m_eState == STATE_SHOT) {
			m_tFrame.iFrameStartY = 7;
			/*	m_tFrame.iFrameStartX = 3.f;
			m_tFrame.iFrameCurrentX = m_tFrame.iFrameStartX;
			m_tFrame.iFrameEndX = m_tFrame.iFrameStartX + 2.f;*/
		}
		else if (m_eState == STATE_ROLLING) {
			m_tFrame.iFrameStartY = 7;
			/*	m_tFrame.iFrameStartX = 6.f;
			m_tFrame.iFrameCurrentX = m_tFrame.iFrameStartX;
			m_tFrame.iFrameEndX = m_tFrame.iFrameStartX + 2.f;*/
		}
		else if (m_eState == STATE_IDLE)
			m_tFrame.iFrameStartY = 15;
	}
	if (fAngle <= 0 && fAngle >= -22.5f) { //정면
		if (m_eState == STATE_SHOT) {
			m_tFrame.iFrameStartY = 3;
			/*m_tFrame.iFrameStartX = 3.f;
			m_tFrame.iFrameCurrentX = m_tFrame.iFrameStartX;
			m_tFrame.iFrameEndX = m_tFrame.iFrameStartX + 2.f;*/
		}
		else if (m_eState == STATE_ROLLING) {
			m_tFrame.iFrameStartY = 3;
			/*m_tFrame.iFrameStartX = 6.f;
			m_tFrame.iFrameCurrentX = m_tFrame.iFrameStartX;
			m_tFrame.iFrameEndX = m_tFrame.iFrameStartX + 2.f;*/
		}
		else if (m_eState == STATE_IDLE)
			m_tFrame.iFrameStartY = 11;
	}

	m_fAngle = fAngle;

	if (true == m_bClear)
	{
		if (false == m_iOutSoul)
		{
			//m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
			//m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TFACTOR);
			//m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TEXTURE);
			//m_pGraphic_Device->SetRenderState(D3DRS_TEXTUREFACTOR, 0xff2d2d2d);
		}
		if (pGameInstance->Get_DIKeyState(DIK_Q) & 0x80)
		{
			--m_iSoulOutTime;
			if (m_iSoulOutTime == 0)
			{
				if (m_tFrame.iFrameStartX != 4.f) {
					m_tFrame.iFrameStartX = 4.f;
					m_tFrame.iFrameCurrentX = m_tFrame.iFrameStartX;
					m_tFrame.iFrameEndX = m_tFrame.iFrameStartX + 2.f;
				}
				pGameInstance->Add_GameObject(g_eCurrentLevel, TEXT("Layer_EatSouls"), TEXT("Prototype_GameObject_Effect_EatSouls"), &m_pTransformCom->Get_State(CTransform::STATE_POSITION));
				static_cast<CTransform*>(pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform")))->LookAtXZ(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
				static_cast<CDarkScreen*>(pGameInstance->Get_Front(g_eCurrentLevel, TEXT("Layer_DarkScreen")))->Set_SoulEat(true);
				//m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
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

	if (m_bSnowBallPatter == true && m_bRollingPattern == false) {

		m_eState = STATE_SHOT;
		m_pTransformCom->LookAtXZ(PlayerPos);

		/*	m_tFrame.iFrameStartX = 3.f;
		m_tFrame.iFrameCurrentX = m_tFrame.iFrameStartX;
		m_tFrame.iFrameEndX = m_tFrame.iFrameStartX + 2.f;*/
		//m_tFrame.i
		m_fSpriteSpeed = 2.f;
		m_fSnowBallSrowTimer++;
		if (m_fSnowBallSrowTimer > 25) {
			m_tFrame.iFrameStartX = 3.f;
			m_tFrame.iFrameCurrentX = m_tFrame.iFrameStartX;
			m_tFrame.iFrameEndX = m_tFrame.iFrameStartX + 2.f;
			pGameInstance->Add_GameObject(LEVEL_YETI, TEXT("Layer_SnowBall"), TEXT("Prototype_GameObject_Yeti_SnowBall"));
			for (m_iDustNum; m_iDustNum < 8; m_iDustNum++) {
				pGameInstance->StopSound(CSoundMgr::MONSTER1); // 사운드
				pGameInstance->Add_GameObject(LEVEL_YETI, TEXT("Layer_SnowBall_Paticle"), TEXT("Prototype_GameObject_Yeti_Paticle_Big"), this);
				pGameInstance->LetsPlaySound(L"Yeti_SnowThrow.ogg", CSoundMgr::MONSTER1, 0.5f); // 사운드
			}
			if (m_iDustNum >= 8)
				m_iDustNum = 0;

			//pGameInstance->Add_GameObject(LEVEL_YETI, TEXT("Layer_SnowBall_Paticle"), TEXT("Prototype_GameObject_Yeti_Paticle_Big"), this);
			m_fSnowBallSrowTimer = 0;
			m_iSrowCount++;
		}
		if (m_iSrowCount >= 4) {
			m_bRollingPattern = true;
			m_bSnowBallPatter = false;
			m_iSrowCount = 0;
		}

	}


	if (m_bRollingPattern == true && m_bWaiting == false) {
		if (m_bRollingScan == false && m_bWaiting == false) {
			pGameInstance->StopSound(CSoundMgr::MONSTER2); // 사운드
			m_pTransformCom->LookAtXZ(PlayerPos);
			m_bRollingScan = true;
			m_eState = STATE_ROLLING;
			m_tFrame.iFrameStartX = 6.f;
			m_tFrame.iFrameCurrentX = m_tFrame.iFrameStartX;
			m_tFrame.iFrameEndX = m_tFrame.iFrameStartX + 2.f;
			pGameInstance->LetsPlaySound(L"Yeti_Roll.ogg", CSoundMgr::MONSTER2, 0.5f); // 사운드
		}
		if (m_bJump != true && m_bRollingPattern == true) {
			m_pTransformCom->Go_Straight(fTimeDelta * 5.f);
			pGameInstance->Add_GameObject(LEVEL_YETI, TEXT("Layer_Yeti_Dust"), TEXT("Prototype_GameObject_Yeti_Dust"), this);
			pGameInstance->Add_GameObject(LEVEL_YETI, TEXT("Layer_Yeti_Dust"), TEXT("Prototype_GameObject_Yeti_Dust"), this);
			pGameInstance->Add_GameObject(LEVEL_YETI, TEXT("Layer_Yeti_Dust"), TEXT("Prototype_GameObject_Yeti_Dust"), this);
			pGameInstance->Add_GameObject(LEVEL_YETI, TEXT("Layer_Yeti_Dust"), TEXT("Prototype_GameObject_Yeti_Dust"), this);
			pGameInstance->Add_GameObject(LEVEL_YETI, TEXT("Layer_Yeti_Dust"), TEXT("Prototype_GameObject_Yeti_Dust"), this);
			pGameInstance->Add_GameObject(LEVEL_YETI, TEXT("Layer_Yeti_Effect"), TEXT("Prototype_GameObject_Yeti_afterimage"), this);
		}
	}
	//if (pGameInstance->Get_DIKeyState(DIK_2) && m_bJump == false) // 충돌시 점프
	//{
	//	//m_pTransformCom->Set_State(CTransform::STATE_LOOK, m_pTransformCom->Get_State(CTransform::STATE_LOOK) * -1);
	//	//m_fJumpy = m_pTransformCom->Get_State(CTransform::STATE_POSITION).y;
	//	//for (int i = 0; i < 5; i++)
	//	//	pGameInstance->Add_GameObject(LEVEL_YETI, TEXT("Layer_Yeti"), TEXT("Prototype_GameObject_Icicle"));

	//	////m_pTransformCom->Go_Up()
	//	//m_bJump = true;
	//	if (!m_bJump)
	//	{
	//		m_pTransformCom->Set_State(CTransform::STATE_LOOK, m_pTransformCom->Get_State(CTransform::STATE_LOOK) * -1);
	//		m_fJumpy = m_pTransformCom->Get_State(CTransform::STATE_POSITION).y;
	//		for (int i = 0; i < 5; i++)
	//			pGameInstance->Add_GameObject(LEVEL_YETI, TEXT("Layer_Yeti"), TEXT("Prototype_GameObject_Icicle"));

	//		//m_pTransformCom->Go_Up()
	//		m_bJump = true;

	//	}
	//}
	//if (m_bJump == true) {
	//	_float3 MyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	//	MyPos.y = m_fJumpy + (m_fJumpPower * m_fTime - 3.8f * m_fTime * m_fTime * 0.5f);
	//	m_fTime += 0.05f;
	//	m_pTransformCom->Set_State(CTransform::STATE_POSITION, MyPos);
	//	m_pTransformCom->Go_Straight(fTimeDelta * 0.5f);
	//}


	//if (pGameInstance->Get_DIKeyState(DIK_1)) {
	//	//m_IceDead->bIceDead = true;
	//	m_bSnowBallPatter = true;
	//}

	if (false == btest && nullptr != pGameInstance->Find_Layer(LEVEL_STATIC, TEXT("Layer_Arrow")) && 0 < pGameInstance->Find_Layer(LEVEL_STATIC, TEXT("Layer_Arrow"))->Get_Object().size())
	{
		if (pGameInstance->Collision_Sphere((CCollider_Sphere*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Arrow"), TEXT("Com_Collider")), m_pColliderCom))
		{
			pGameInstance->PlayBGM(L"Yeti_map.ogg", 0.3f);// 사운드
			m_bSnowBallPatter = true;
			btest = true;
		}

		//	//m_IceDead->bIceDead = true;
	}
	_float3 MyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float3 vpos = m_pPlayerTransform->Get_State(CTransform::STATE_POSITION);





	RELEASE_INSTANCE(CGameInstance);

	return _int();
}

_int CYeti::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
	{
		MSGBOX("0 > __super::LateTick in CBrain::LateTick");
		return -1;
	}

	if (nullptr == m_pRendererCom)
	{
		MSGBOX("m_pRendererCom is nullptr in CBrain::LateTick");
		return -1;
	}

	if (FAILED(SetUp_OnTerrain()))
	{
		MSGBOX("SetUp_OnTerrain returned E_FAIL in CYeti::LateTick");
		return -1;
	}

	CCollider_Sphere::SPHEREDESC SphereDesc;
	SphereDesc.ObjectMatrix = m_pTransformCom->Get_WorldMatrix();
	SphereDesc.fRadius = 0.3f;
	SphereDesc.fDist_From_Obj = _float3(0.f, -0.1f, 0.f);
	m_pColliderCom->Set_SphereDesc(SphereDesc);
	m_pColliderCom->Tick();

	/*if (FAILED(SetUp_OnTerrain()))
	return -1;*/
	_float4x4		ViewMatrix; //뷰행렬을 저장할 변수

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	//월드변환행렬로 만든다
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);
	//D3DXMatrixInverse
	//라업룩에 적용해준다

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, (*(_float3*)&ViewMatrix.m[0][0]) * m_pTransformCom->Get_Scale().x);

	//m_pTransformCom->Set_State(CTransform::STATE_UP, (*(_float3*)&ViewMatrix.m[1][0]) * m_pTransformCom->Get_Scale().y);
	//m_pTransformCom->Set_State(CTransform::STATE_LOOK, (*(_float3*)&ViewMatrix.m[2][0]) * m_pTransformCom->Get_Scale().z);

	m_pTransformCom->Scaled(_float3(m_fScale, m_fScale, m_fScale));

	if (m_bClear == false)
		Go_Frame(m_fSpriteSpeed, fTimeDelta);
	if (m_iOutSoul == true) {
		Go_FrameEnd(1.f, fTimeDelta);
	}

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

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);
	Collision_Rect();
	if (m_bJump == true) {
		_float3 MyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		MyPos.y = m_fJumpy + (m_fJumpPower * m_fTime - 3.8f * m_fTime * m_fTime * 0.5f);
		m_fTime += 0.05f;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, MyPos);
		m_pTransformCom->Go_Straight(4.f * fTimeDelta * 0.5f);
	}

	return _int();
}

HRESULT CYeti::Render()
{
	if (nullptr == m_pVIBufferCom)
	{
		MSGBOX("m_pVIBufferCom is nullptr in CYeti::Render")
			return E_FAIL;
	}

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
	{
		MSGBOX("m_pTransformCom->Bind_OnGraphicDevice returned E_FAIL in CYeti::Render");
		return E_FAIL;
	}
	//m_pVIBufferCom->Base_Setting();

	if (FAILED(Go_Sprites()))
	{
		MSGBOX("Go_Sprites returned E_FAIL in CYeti::Tick");
		return E_FAIL;
	}

	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice()))
	{
		MSGBOX("m_pTextureCom->Bind_OnGraphicDevice in CYeti::Render");
		return E_FAIL;
	}

	if (FAILED(SetUp_RenderState()))
	{
		MSGBOX("SetUp_RenderState returned E_FAIL in CYeti::Render");
		return E_FAIL;
	}

	m_pVIBufferCom->Render();
	m_pVIBufferCom->Base_Setting();
	if (FAILED(Release_RenderState()))
	{
		MSGBOX("Release_RenderState returned E_FAIL in CYeti::Render");
		return E_FAIL;
	}
	////////////////////
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

	return S_OK;
}

HRESULT CYeti::ComParePlayerPos()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	CTransform* m_pPlayerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"));
	_float3 PlayerPos = m_pPlayerTransform->Get_State(CTransform::STATE_POSITION);
	_float3 PlayerLook = m_pPlayerTransform->Get_State(CTransform::STATE_LOOK);
	_float3 MyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float3 MyLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;

}

HRESULT CYeti::SetUp_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CYeti::SetUp_Components(Transform)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CYeti::SetUp_Components(Renderer)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CYeti::SetUp_Components(VIBuffer_Rect)");
		return E_FAIL;
	}

	/* For.Com_Texture */

	if (FAILED(__super::Add_Component(LEVEL_YETI, TEXT("Prototype_Component_Texture_Yeti"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	CCollider_Sphere::SPHEREDESC SphereDesc;
	SphereDesc.ObjectMatrix = m_pTransformCom->Get_WorldMatrix();
	SphereDesc.fRadius = 0.3f;
	SphereDesc.fDist_From_Obj = _float3(0.f, -0.1f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"), TEXT("Com_Collider"), (CComponent**)&m_pColliderCom, &SphereDesc)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CColossus_Fist_Right::SetUp_Components(Collider_Sphere)");
		return E_FAIL;
	}

	//#ifdef _DEBUG
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"), TEXT("Com_VIBuffer_Coll"), (CComponent**)&m_pVIBufferCollCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CColossus_Fist_Right::SetUp_Components(VIBuffer_Cube)");
		return E_FAIL;
	}
	//#endif // DEBUG

	return S_OK;
}

HRESULT CYeti::SetUp_OnTerrain()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	CVIBuffer_Terrain* m_pVIBuffer_Terrain = (CVIBuffer_Terrain*)pGameInstance->Get_Component(g_eCurrentLevel, TEXT("Layer_Terrain"), TEXT("Com_VIBuffer"));


	if (m_pVIBuffer_Terrain == nullptr)
		return E_FAIL;
	//플레이어의 위치를 얻어와서
	_float3 vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	vPosition.y = m_pVIBuffer_Terrain->Compute_Y(vPosition) + m_fScale / 2;
	if (m_bJump == true) {
		if (m_pTransformCom->Get_State(CTransform::STATE_POSITION).y < vPosition.y) {
			//m_pTransformCom->Set_State(CTransform::STATE_LOOK, m_pTransformCom->Get_State(CTransform::STATE_LOOK) * -1);

			for (m_iDustNum; m_iDustNum < 16; m_iDustNum++) {
				pGameInstance->Add_GameObject(LEVEL_YETI, TEXT("Layer_Yeti_Jump_Dust"), TEXT("Prototype_GameObject_Yeti_Jump_Dust"), this);
			}
			if (m_iDustNum >= 16)
				m_iDustNum = 0;


			Shake = true;
			m_bWaiting = true;
			m_bJump = false;
			m_fTime = 0;
			m_eState = STATE_IDLE;
			//m_tFrame.iFrameStartY = 7.f;
			m_tFrame.iFrameStartX = 0.f;
			m_tFrame.iFrameCurrentX = m_tFrame.iFrameStartX;
			m_tFrame.iFrameEndX = m_tFrame.iFrameStartX + 2.f;
			if (m_bRollingScan == true && m_bRollingPattern == true) {
				m_bRollingScan = false;
			}
			m_iWallBoundCount++;

			if (m_iWallBoundCount >= 3) {
				m_bRollingPattern = false;
				m_bSnowBallPatter = true;
				m_iWallBoundCount = 0;
			}
		}
	}
	if (m_bJump == false) {
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
		if (m_bRollingPattern == true)
			m_iRolingWaitTime++;
	}
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CYeti::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	//알파 테스팅
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 50);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, TRUE);

	return S_OK;
}

HRESULT CYeti::Release_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	//m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, FALSE);

	/*m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);*/

	return S_OK;
}

HRESULT CYeti::Go_Sprites()
{
	if (FAILED(m_pVIBufferCom->Go_Sprites(m_tFrame)))
	{
		MSGBOX("m_pVIBufferCom->Go_Sprites returned E_FAIL in CYeti::Go_Sprites");
		return E_FAIL;
	}
	return S_OK;
}

void CYeti::Collision_Rect()
{
	if (LEVEL_LOADING == g_eCurrentLevel)
		return;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	for (_uint i = 0; i < 4; i++)
	{
		_float3 vPos = pGameInstance->Collision_SphereToRect((CCollider_Rect*)pGameInstance->Get_Component(g_eCurrentLevel, TEXT("Layer_Wall"), TEXT("Com_Collider"), i), m_pColliderCom);
		if (vPos != _float3(0.f, 0.f, 0.f))
		{
			Shake = true;
			if (m_bJump == false) // 충돌시 점프
			{
				//m_pTransformCom->Set_State(CTransform::STATE_LOOK, m_pTransformCom->Get_State(CTransform::STATE_LOOK) * -1);
				//m_fJumpy = m_pTransformCom->Get_State(CTransform::STATE_POSITION).y;
				//for (int i = 0; i < 5; i++)
				//	pGameInstance->Add_GameObject(LEVEL_YETI, TEXT("Layer_Yeti"), TEXT("Prototype_GameObject_Icicle"));

				////m_pTransformCom->Go_Up()
				//m_bJump = true;

				m_pTransformCom->Set_State(CTransform::STATE_LOOK, m_pTransformCom->Get_State(CTransform::STATE_LOOK) * -1);
				m_fJumpy = m_pTransformCom->Get_State(CTransform::STATE_POSITION).y;
				for (int i = 0; i < 5; i++)
					pGameInstance->Add_GameObject(LEVEL_YETI, TEXT("Layer_Icicle"), TEXT("Prototype_GameObject_Icicle"));

				//m_pTransformCom->Go_Up()
				m_bJump = true;

			}
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
		}
	}

	RELEASE_INSTANCE(CGameInstance);
}

CYeti * CYeti::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CYeti*	pInstance = new CYeti(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CYeti");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CYeti::Clone(void* pArg)
{
	/* 새로운객체를 복제하여 생성한다. */
	CYeti*	pInstance = new CYeti(*this);


	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CYeti");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CYeti::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);
	Safe_Release(m_pVIBufferCollCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}