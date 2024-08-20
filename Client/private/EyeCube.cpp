#include "stdafx.h"
#include "..\Public\EyeCube.h"
#include "GameInstance.h"
#include "Camera_Player.h"
#include "DarkScreen.h"
CEyeCube::CEyeCube(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CEyeCube::CEyeCube(const CEyeCube & rhs)
	: CGameObject(rhs),
	m_fRollingSpeed(3.f)
{
}

HRESULT CEyeCube::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEyeCube::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	/* ���� ��ü���� �߰��Ǿ���� ������Ʈ���� ����(or ����)�Ͽ� ��������� �����Ѵ�.  */
	if (FAILED(SetUp_Components()))
		return E_FAIL;
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	m_pCamera = static_cast<CCamera_Player*>(pGameInstance->Get_Front(LEVEL_STATIC, TEXT("Layer_Camera")));

	m_bDead = (_bool*)pArg;
	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(16.f, 0.f, 16.f));
	m_pTransformCom->Scaled(_float3(4.f, 4.f, 4.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(16.f, 2.0f, 20.f));
	pGameInstance->Add_GameObject(LEVEL_EYECUBE, TEXT("Layer_EyeCube_Shadow"), TEXT("Prototype_GameObject_EyeCube_Shadow"), this);


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

_int CEyeCube::Tick(_float fTimeDelta)
{

	if (m_bStart == false)
		return 0;

	if (true == m_bClear)
	{
		m_fFrame = 0;
		CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
		if (pGameInstance->Get_DIKeyState(DIK_Q) & 0x80)
		{
			--m_iSoulOutTime;
			if (m_iSoulOutTime == 0)
			{
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
		return -1;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	CTransform* m_pPlayerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"));
	if (m_iRollingCount > 2 || m_bRayzerPattern == true) {
		m_fFrame += 4.0f * fTimeDelta * 5.f; // 1�ʿ� ��� �������� ������ŭ ����� �ٲ۴�
		if (m_fFrame >= 4.0f) // ���� �������� �� ���Ҵٸ� ó������ �ǵ�����
			m_fFrame = 2.f;
	}
	else if (m_bRayzerPattern == false) {
		// ���� �������� �� ���Ҵٸ� ó������ �ǵ�����
		m_fFrame = 0.f;
	}
	_float m_fJumpMinus = m_fJumpPower - 3.8f * m_fTime;
	if (m_fJumpMinus < 0) {
		m_bRayzerPattern = false;
		m_iRollingTimer = 0;
	}



	_float3 MyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float3 vpos = m_pPlayerTransform->Get_State(CTransform::STATE_POSITION);



	//D3DXVec3Length(&(_float3(0, 0, MyPos.z) - _float3(0, 0, vpos.z))) > D3DXVec3Length(&(_float3(MyPos.x, 0, 0) - _float3(vpos.x, 0, 0))); // x��� z������ ���̸� ���Ѵ�
	if (m_bGoZUP == false &&
		m_bGoZDown == false &&
		m_bGoXLeft == false &&
		m_bGoXRight == false &&
		m_iRollingTimer > 10 && m_bRayzerPattern == false) {
		// �÷��̾�� �ڽ��� ��ġ�� �񱳿� z���� x�������� �Ÿ��� ���ؼ� ������ �켱�ؼ� �����̵��� ���� ������
		if (MyPos.z < vpos.z && D3DXVec3Length(&(_float3(0, 0, MyPos.z) - _float3(0, 0, vpos.z))) > D3DXVec3Length(&(_float3(MyPos.x, 0, 0) - _float3(vpos.x, 0, 0))))
		{
			pGameInstance->StopSound(CSoundMgr::MONSTER1);
			m_bGoZUP = true;
			pGameInstance->LetsPlaySound(L"EyeCube_Move.ogg", CSoundMgr::MONSTER1, 0.5f); // ����
		}
		else if (MyPos.z > vpos.z && D3DXVec3Length(&(_float3(0, 0, MyPos.z) - _float3(0, 0, vpos.z))) > D3DXVec3Length(&(_float3(MyPos.x, 0, 0) - _float3(vpos.x, 0, 0))))
		{
			pGameInstance->StopSound(CSoundMgr::MONSTER1);// ����
			m_bGoZDown = true;
			pGameInstance->LetsPlaySound(L"EyeCube_Move.ogg", CSoundMgr::MONSTER1, 0.5f); // ����
		}
		else if (MyPos.x > vpos.x && D3DXVec3Length(&(_float3(0, 0, MyPos.z) - _float3(0, 0, vpos.z))) < D3DXVec3Length(&(_float3(MyPos.x, 0, 0) - _float3(vpos.x, 0, 0))))
		{
			pGameInstance->StopSound(CSoundMgr::MONSTER1);// ����
			m_bGoXLeft = true;
			pGameInstance->LetsPlaySound(L"EyeCube_Move.ogg", CSoundMgr::MONSTER1, 0.5f); // ����
		}
		else if (MyPos.x < vpos.x && D3DXVec3Length(&(_float3(0, 0, MyPos.z) - _float3(0, 0, vpos.z))) < D3DXVec3Length(&(_float3(MyPos.x, 0, 0) - _float3(vpos.x, 0, 0))))
		{
			pGameInstance->StopSound(CSoundMgr::MONSTER1);// ����
			m_bGoXRight = true;
			pGameInstance->LetsPlaySound(L"EyeCube_Move.ogg", CSoundMgr::MONSTER1, 0.5f); // ����
		}
		m_iRollingTimer = 0;
	}
	if (m_bGoZUP == true) {
		if (btest == false) { // ť�� �������� �״�� y���� 0�ΰ��� ��ġ������ ������ ���̸� ���ϰ� ���� ������
			_float3 TempY0 = { MyPos.x,0.f,MyPos.z };
			m_fDir = MyPos - TempY0;
			m_fDir_Length = D3DXVec3Length(&m_fDir);
			RevelPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			// �� ����
			RevelPos.z += (m_pTransformCom->Get_Scale().x / 2);
			RevelPos.y -= (m_pTransformCom->Get_Scale().x / 2);
			btest = true;
			m_iRollingCount++;
		}
		m_pTransformCom->Revolution(_float3(1.f, 0.f, 0.f), RevelPos, fTimeDelta * m_fRollingSpeed); // ȸ��
		MyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION); // ȸ���ϸ鼭 ���� ���� ������� ������ ���̸� ��� ����
		_float3 TempY0 = { MyPos.x,0.f,MyPos.z };
		m_fDir_Check = MyPos - TempY0;
		m_fDir_Length_Check = D3DXVec3Length(&m_fDir_Check);
	}
	else if (m_bGoZDown == true) {
		if (btest == false) {
			_float3 TempY0 = { MyPos.x,0.f,MyPos.z };
			m_fDir = MyPos - TempY0;
			m_fDir_Length = D3DXVec3Length(&m_fDir);
			RevelPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			RevelPos.z -= (m_pTransformCom->Get_Scale().x / 2);
			RevelPos.y -= (m_pTransformCom->Get_Scale().x / 2);
			btest = true;
			m_iRollingCount++;

		}
		m_pTransformCom->Revolution(_float3(1.f, 0.f, 0.f), RevelPos, -fTimeDelta * m_fRollingSpeed);
		MyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_float3 TempY0 = { MyPos.x,0.f,MyPos.z };
		m_fDir_Check = MyPos - TempY0;
		m_fDir_Length_Check = D3DXVec3Length(&m_fDir_Check);
	}
	else if (m_bGoXLeft == true) {
		if (btest == false) {
			_float3 TempY0 = { MyPos.x,0.f,MyPos.z };
			m_fDir = MyPos - TempY0;
			m_fDir_Length = D3DXVec3Length(&m_fDir);
			RevelPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			RevelPos.x -= (m_pTransformCom->Get_Scale().x / 2);
			RevelPos.y -= (m_pTransformCom->Get_Scale().x / 2);
			btest = true;
			m_iRollingCount++;

		}
		m_pTransformCom->Revolution(_float3(0.f, 0.f, 1.f), RevelPos, fTimeDelta * m_fRollingSpeed);
		MyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_float3 TempY0 = { MyPos.x,0.f,MyPos.z };
		m_fDir_Check = MyPos - TempY0;
		m_fDir_Length_Check = D3DXVec3Length(&m_fDir_Check);
	}
	else if (m_bGoXRight == true) {
		if (btest == false) {
			_float3 TempY0 = { MyPos.x,0.f,MyPos.z };
			m_fDir = MyPos - TempY0;
			m_fDir_Length = D3DXVec3Length(&m_fDir);
			RevelPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			RevelPos.x += (m_pTransformCom->Get_Scale().x / 2);
			RevelPos.y -= (m_pTransformCom->Get_Scale().x / 2);
			btest = true;
			m_iRollingCount++;

		}
		m_pTransformCom->Revolution(_float3(0.f, 0.f, 1.f), RevelPos, -fTimeDelta * m_fRollingSpeed);
		MyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_float3 TempY0 = { MyPos.x,0.f,MyPos.z };
		m_fDir_Check = MyPos - TempY0;
		m_fDir_Length_Check = D3DXVec3Length(&m_fDir_Check);
	}




	if (m_fDir_Length_Check <= m_fDir_Length && m_bRayzerPattern == false) { // ť�갡 �ٴڿ� ����� ��
		btest = false; // �ϴ� false
		m_bGoZUP = false;
		m_bGoZDown = false;
		m_bGoXLeft = false;
		m_bGoXRight = false;
		m_iRollingTimer++;
		if (m_iRollingCount >= 3)
		{
			pGameInstance->LetsPlaySound(L"LaserCharge.ogg", CSoundMgr::MONSTER3, 0.5f); // ����
		}
		if (m_iRollingCount >= 5)
		{
			pGameInstance->LetsPlaySound(L"Laser.ogg", CSoundMgr::MONSTER2, 0.5f); // ����
			m_bRayzerPattern = true;
		}
		if (m_iRollingCount > 0)
			m_pCamera->Set_Shake(false, true, false);
		//TestStop = true; //�׽�Ʈ��
	}

	if (m_bRayzerPattern == true) {
		m_pCamera->Set_Shake(true, false, false);
		m_iRollingCount = 0;
		m_fPatternTime++;
		//m_bRayzerPattern = false;
		m_iRollingTimer = 0;
		fSoundTimer += 1.f * fTimeDelta; // ����
	}

	if (fSoundTimer >= 2.f) // ����
	{
		pGameInstance->StopSound(CSoundMgr::MONSTER3);
		fSoundTimer = 0.f;
	}

	if (m_fPatternTime == 15 && m_fPatternTime < 16) {
		m_fRayerUsePos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_float3 m_fjumppos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		pGameInstance->Add_GameObject(LEVEL_EYECUBE, TEXT("Layer_EyeCube_Rayzer"), TEXT("Prototype_GameObject_EyeCube_Rayzer"));
		if ((m_fjumppos += (m_pTransformCom->Get_State(CTransform::STATE_LOOK) * 1)).y < 0.f) {
			m_fJumpy = m_pTransformCom->Get_State(CTransform::STATE_POSITION).y;
			//m_pTransformCom->Go_Up()

			_float3 a = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			_float i = a.x;
			if ((_int)i % 8 <= 5 && (_int)i % 8 >= 3) { // x�� Ȧ��
				if ((_int)a.z % 8 <= 6 && (_int)a.z % 8 >= 2) { // y�� Ȧ��
					m_bWhiteTileRayzer = true;
				}
				else if ((_int)a.z % 8 >= 0 && (_int)a.z % 8 <= 9) { // y�� ¦��
					m_bBlackTileRayzer = true;
				}

			}
			else if (((_int)i % 8 >= 0 && (_int)i % 8 <= 1) || (_int)i % 8 <= 8 && (_int)i % 8 >= 7) { //x�� ¦��
				if ((_int)a.z % 8 <= 5 && (_int)a.z % 8 >= 3) { // y�� Ȧ��
					m_bBlackTileRayzer = true;
				}
				else if ((_int)a.z % 8 >= 0 && (_int)a.z % 8 <= 8) { // y�� ¦��
					m_bWhiteTileRayzer = true;
				}

			}


			if (m_bWhiteTileRayzer == true) {
				for (int i = 0; i < 7; i++) {
					for (int j = 0; j < 7; j++) {
						if (i % 2 == 0) {//y�� ¦�����
							if (j % 2 == 0) {
								m_tRayPos.Xpos = j * 4.f + 4.f;
								m_tRayPos.Ypos = i * 4.f + 4.f;
								pGameInstance->Add_GameObject(LEVEL_EYECUBE, TEXT("Layer_EyeCube_Rayzer_Vertical"), TEXT("Prototype_GameObject_EyeCube_Rayzer_Vertical"), &m_tRayPos);
							}

						}
						else if (i % 2 == 1) { //y�� Ȧ�����
							if (j % 2 == 1) {
								m_tRayPos.Xpos = 4.f * (j + 1);
								m_tRayPos.Ypos = i * 4.f + 4.f;
								pGameInstance->Add_GameObject(LEVEL_EYECUBE, TEXT("Layer_EyeCube_Rayzer_Vertical"), TEXT("Prototype_GameObject_EyeCube_Rayzer_Vertical"), &m_tRayPos);
							}

						}
					}
				}
			}
			else if (m_bBlackTileRayzer == true) {
				for (int i = 0; i < 7; i++) {
					for (int j = 0; j < 7; j++) {
						if (i % 2 == 0) {//y�� ¦�����
							if (j % 2 == 1) {
								m_tRayPos.Xpos = 4.f * (j + 1);
								m_tRayPos.Ypos = i * 4.f + 4.f;
								pGameInstance->Add_GameObject(LEVEL_EYECUBE, TEXT("Layer_EyeCube_Rayzer_Vertical"), TEXT("Prototype_GameObject_EyeCube_Rayzer_Vertical"), &m_tRayPos);
							}

						}
						else if (i % 2 == 1) { //y�� Ȧ�����
							if (j % 2 == 0) {
								m_tRayPos.Xpos = j * 4.f + 4.f;
								m_tRayPos.Ypos = i * 4.f + 4.f;
								pGameInstance->Add_GameObject(LEVEL_EYECUBE, TEXT("Layer_EyeCube_Rayzer_Vertical"), TEXT("Prototype_GameObject_EyeCube_Rayzer_Vertical"), &m_tRayPos);
							}
						}
					}
				}
			}



			m_bJump = true;
		}
		m_bWhiteTileRayzer = false;
		m_bBlackTileRayzer = false;

		//m_fPatternTime = 0;
	}
	if (m_bJump == true) {
		_float3 MyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		MyPos.y = m_fJumpy + (m_fJumpPower * m_fTime - 3.8f * m_fTime * m_fTime * 0.5f);
		m_fTime += 0.05f;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, MyPos);
		/*MyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_float3 TempY0 = { MyPos.x,0.f,MyPos.z };
		m_fDir_Check = MyPos - TempY0;
		m_fDir_Length_Check = D3DXVec3Length(&m_fDir_Check);*/
	}
	if (m_bJump == true &&
		m_pTransformCom->Get_State(CTransform::STATE_POSITION).y - 2.f < 0.f
		&& m_fTime > 1) {
		SetUp_OnTerrain();
		m_fTime = 0;
		m_bJump = false;
	}
	if (m_fPatternTime >= 50 && m_bJump == false) {
		m_bRayzerPattern = false;
		m_fPatternTime = 0;
	}


	/*if (MyPos.z > vpos.z)
	{
	if (btest == false) {
	RevelPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	RevelPos.z -= (m_pTransformCom->Get_Scale().x / 2);
	RevelPos.y -= (m_pTransformCom->Get_Scale().x / 2);
	btest = true;
	}
	m_pTransformCom->Revolution(_float3(1.f, 0.f, 0.f), RevelPos, -fTimeDelta);
	}

	if (MyPos.x > vpos.x)
	{
	if (btest == false){
	RevelPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	RevelPos.x -= (m_pTransformCom->Get_Scale().x / 2);
	RevelPos.y -= (m_pTransformCom->Get_Scale().x / 2);
	btest = true;
	}
	m_pTransformCom->Revolution(_float3(0.f, 0.f, 1.f), RevelPos, fTimeDelta);
	}

	if (MyPos.x < vpos.x)
	{
	if (btest == false) {
	RevelPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	RevelPos.x -= (m_pTransformCom->Get_Scale().x / 2);
	RevelPos.y -= (m_pTransformCom->Get_Scale().x / 2);
	btest = true;
	}
	m_pTransformCom->Revolution(_float3(0.f, 0.f, 1.f), RevelPos, -fTimeDelta);
	}*/



	RELEASE_INSTANCE(CGameInstance);

	return _int();
}

_int CEyeCube::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
		return -1;

	if (nullptr == m_pRendererCom)
		return -1;

	/*if (FAILED(SetUp_OnTerrain()))
	return -1;*/

	CCollider_Sphere::SPHEREDESC SphereDesc;
	SphereDesc.ObjectMatrix = m_pTransformCom->Get_WorldMatrix();
	SphereDesc.fRadius = 0.55f;
	SphereDesc.fDist_From_Obj = _float3(0.f, 0.f, 0.f);
	m_pColliderCom->Set_SphereDesc(SphereDesc);
	m_pColliderCom->Tick();
	SphereDesc.fRadius = 0.35f;
	SphereDesc.fDist_From_Obj = _float3(0.f, 0.f, 0.35f);
	m_pWeaknessCom->Set_SphereDesc(SphereDesc);
	m_pWeaknessCom->Tick();

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (m_bStart == false)
		if ((nullptr != pGameInstance->Find_Layer(LEVEL_STATIC, TEXT("Layer_Arrow")) && 0 < pGameInstance->Get_Size(LEVEL_STATIC, TEXT("Layer_Arrow"))) && true == pGameInstance->Collision_Sphere((CCollider_Sphere*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Arrow"), TEXT("Com_Collider")), m_pColliderCom))
		{
			m_bStart = true;
			pGameInstance->PlayBGM(L"Eyecube_map.ogg", 0.3f);// ����
		}

	RELEASE_INSTANCE(CGameInstance);


	return _int();
}

HRESULT CEyeCube::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice((_uint)m_fFrame)))
		return E_FAIL;


	////////////////////
	/*if (FAILED(SetUp_RenderState()))
	return E_FAIL;*/

	m_pVIBufferCom->Render(); // �̷��� ������� ������ ������ �� ���
							  // Ư���� �κп��� ������ �����ϰų� �� �� �ִ�
							  //if (FAILED(Release_RenderState()))
							  //	return E_FAIL;
							  ////////////////////
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

	return S_OK;
}

HRESULT CEyeCube::SetUp_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CEyeCube::SetUp_Components(Transform)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CEyeCube::SetUp_Components(Renderer)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CEyeCube::SetUp_Components(VIBuffer_Cube)");
		return E_FAIL;
	}

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_EYECUBE, TEXT("Prototype_Component_Texture_EyeCube"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	CCollider_Sphere::SPHEREDESC SphereDesc;
	SphereDesc.ObjectMatrix = m_pTransformCom->Get_WorldMatrix();
	SphereDesc.fRadius = 0.5f;
	SphereDesc.fDist_From_Obj = _float3(0.f, 0.f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"), TEXT("Com_Collider"), (CComponent**)&m_pColliderCom, &SphereDesc)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CEyeCube::SetUp_Components(Collider_Sphere)");
		return E_FAIL;
	}

	SphereDesc.fDist_From_Obj = _float3(0.f, 0.f, 0.4f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"), TEXT("Com_Weak"), (CComponent**)&m_pWeaknessCom, &SphereDesc)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CEyeCube::SetUp_Components(Collider_Weak)");
		return E_FAIL;
	}

	//#ifdef _DEBUG
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"), TEXT("Com_VIBuffer_Coll"), (CComponent**)&m_pVIBufferCollCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CEyeCube::SetUp_Components(VIBuffer_Cube)");
		return E_FAIL;
	}
	//#endif // DEBUG


	return S_OK;
}

HRESULT CEyeCube::SetUp_OnTerrain()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	CVIBuffer_Terrain* m_pVIBuffer_Terrain = (CVIBuffer_Terrain*)pGameInstance->Get_Component(g_eCurrentLevel, TEXT("Layer_Terrain"), TEXT("Com_VIBuffer"));


	if (m_pVIBuffer_Terrain == nullptr)
		return E_FAIL;
	//�÷��̾��� ��ġ�� ���ͼ�
	_float3 vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	vPosition.y = m_pVIBuffer_Terrain->Compute_Y(vPosition) + 2.f;
	if (m_bJump == true) {
		if (m_pTransformCom->Get_State(CTransform::STATE_POSITION).y < vPosition.y) {
			m_bJump = false;
			m_fTime = 0;
		}
	}
	if (m_bJump == false)
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CEyeCube::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);//���ĺ����� �ϰڴ�
	//m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);//���� ���Ѵ�
	//m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	////�׸����� �ϴ� �ȼ��� ������ ������ �ҽ� Į���� ���ĸ� ���ڴ�
	//m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	/*
	_float4		vSourColor, vDestColor;
	(vSourColor.rgb) * vSourColor.a + (vDestColor.rgb) * (1.f - vSourColor.a);
	�ҽ��÷�.RGB * �ҽ��÷�.���� + ����Ʈ�ö�.RGB * 1.f - �ҽ��÷�.����
	���Ĵ� RGB�� 0~255�� 0���� 1���̷� �����Ѱ�
	�÷��̾ �׸����ϴµ� �̹� ����ġ�� �׷��� �ִ� ���� ����ƮĮ��
	*/

	//�ٸ� ��� - ���� �׽�Ʈ
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);// �����׽�Ʈ�� �ϰڴ�
	//															   //�ش������� ���ؼ� �񱳸� �� ����
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 0);
	////�׸����� �ߴ� �̹����� ���İ��� ���� ������ ���̶�
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER/*ũ�ٸ�*/);
	// �� ������ ����� �ֵ鸸 ȭ�鿡 �׸��ڴ�
	// ���ڵ��� ��� ���� �׸����� ���İ�(REF - 0)���� ũ�ٸ�(GREATER) �׸���

	// �ܼ��� ����� ����µ��� �����׽����� ���� ���������� �Ѱ谡 �ִ�
	// ������ ó���� �Ұ����ϱ� ����

	//���� �׽��ð� ������ ȥ������ �ʴ´� -> �������ѵ� �ǹ̰� �����
	m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	return S_OK;
}

HRESULT CEyeCube::Release_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;
	/*m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);*/
	m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	return S_OK;
}

CEyeCube * CEyeCube::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CEyeCube*	pInstance = new CEyeCube(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CEyeCube");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CEyeCube::Clone(void* pArg)
{
	/* ���οü�� �����Ͽ� �����Ѵ�. */
	CEyeCube*	pInstance = new CEyeCube(*this);


	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CEyeCube");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEyeCube::Free()
{
	__super::Free();

	Safe_Release(m_pWeaknessCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pVIBufferCollCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}