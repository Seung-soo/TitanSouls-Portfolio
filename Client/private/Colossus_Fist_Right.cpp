#include "stdafx.h"
#include "..\Public\Colossus_Fist_Right.h"
#include "GameInstance.h"
#include "Colossus.h"

CColossus_Fist_Right::CColossus_Fist_Right(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CColossus_Fist_Right::CColossus_Fist_Right(const CColossus_Fist_Right & rhs)
	: CGameObject(rhs)
{
}

HRESULT CColossus_Fist_Right::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CColossus_Fist_Right::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	/* ���� ��ü���� �߰��Ǿ���� ������Ʈ���� ����(or ����)�Ͽ� ��������� �����Ѵ�.  */
	if (FAILED(SetUp_Components()))
		return E_FAIL;
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	m_pTransformCom->Scaled(m_fScale);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(20.f, 2.f, 29.f));

	m_pColInfo = (COLOSSUSINFO*)pArg;
	m_bAttack = m_pColInfo->bRightFistAttack;

	m_tFrame.iFrameStartY = 0.f;
	m_tFrame.iFrameCurrentX = 0.f;
	m_tFrame.iFrameStartX = 0.f;
	m_tFrame.iFrameEndX = 0.f;
	m_tFrame.iFrameHeight = 64.f;
	m_tFrame.iFrameWidth = 64.f;
	m_tFrame.iTexHeight = 196;
	m_tFrame.iTexWidth = 256.f;

	pGameInstance->Add_GameObject(LEVEL_COLOSUSS, TEXT("Layer_Colossus_Fist"), TEXT("Prototype_GameObject_ColossusFist_Right_Shadow"), this);

	m_fSpriteSpeed = 1;
	RELEASE_INSTANCE(CGameInstance);


	return S_OK;
}

_int CColossus_Fist_Right::Tick(_float fTimeDelta)
{

	if (0 > __super::Tick(fTimeDelta))
		return -1;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (static_cast<CColossus*>(pGameInstance->Get_Front(LEVEL_COLOSUSS, TEXT("Layer_Colossus")))->Get_Clear())
	{
		RELEASE_INSTANCE(CGameInstance);
		return 0;
	}

	CTransform* m_pPlayerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"));
	CTransform* m_pColossusTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_COLOSUSS, TEXT("Layer_Colossus"), TEXT("Com_Transform"));
	_float3 vpos = m_pPlayerTransform->Get_State(CTransform::STATE_POSITION);
	_float3 vScale = m_pTransformCom->Get_Scale();

	_float3 MyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	//_float3 vScale = m_pTransformCom->Get_Scale();
	vpos.y = MyPos.y;
	m_bStandUp = m_pColInfo->bStandUp;
	m_bAttack = m_pColInfo->bRightFistAttack;

	_float3 a = m_pTransformCom->Get_Scale();

	if (Shake) {
		if (WallCrash == true && ShakeTime < 20)
		{
			static_cast<CCamera_Player*>(pGameInstance->Get_Front(LEVEL_STATIC, TEXT("Layer_Camera")))->Set_PowerEx(4.f);
			static_cast<CCamera_Player*>(pGameInstance->Get_Front(LEVEL_STATIC, TEXT("Layer_Camera")))->Set_ShakeEX(false, true, false);
		}
		else {
			static_cast<CCamera_Player*>(pGameInstance->Get_Front(LEVEL_STATIC, TEXT("Layer_Camera")))->Set_Shake(false, true, false);
			pGameInstance->LetsPlaySound(L"Colossus_Hit.ogg", CSoundMgr::MONSTER1, 1.0f);
		}
		ShakeTime++;
	}
	if (WallCrash == true) {
		if (ShakeTime > 100) {
			WallCrash = false;
			//m_bAttack = true;
			m_pColInfo->bRightWallCrash = false;
			no_Terrain = true;
			m_pColInfo->FistCount = 0;
			m_fFistTimer = 31;
		}
	}
	else if (ShakeTime > 20) {
		Shake = false;
		ShakeTime = 0;
		pGameInstance->StopSound(CSoundMgr::MONSTER1);
	}

	if (WallCrash == true && ShakeTime > 1) {
		if ((ShakeTime % 10) == 0) {
			pGameInstance->Add_GameObject(LEVEL_COLOSUSS, TEXT("Layer_Colossus_Jongyu"), TEXT("Prototype_GameObject_Jongyu"), this);
			pGameInstance->LetsPlaySound(L"Colossus_Hit.ogg", CSoundMgr::MONSTER1, 1.0f);
			pGameInstance->LetsPlaySound(L"Colossus_earthquake.ogg", CSoundMgr::MONSTER5, 1.0f);
		}
	}



	m_fJumpMinus = m_fJumpPower - 3.8f * m_fTime;
	if (m_fJumpMinus < 0) {
		pGameInstance->Add_GameObject(LEVEL_COLOSUSS, TEXT("Layer_Colosuss_Right_afterimage"), TEXT("Prototype_GameObject_ColossusFist_Right_afterimage"), this);
		no_Terrain = false;
	}


	if (m_bStandUp == true) {
		if (m_pColInfo->bRightWallCrash == true) {
			_float3 ColPos = m_pColossusTransform->Get_State(CTransform::STATE_POSITION);
			if (WallCrash == false) {
				ColPos.x += 4.f;
				ColPos.z -= 2.f;
				//ColPos.y -= 0.4f;
				m_pTransformCom->LookAt(ColPos);
				m_fSpeed = 2.f;
			}
			_float3 vdir;
			if (WallCrash == false)
				vdir = MyPos - ColPos;
			if (D3DXVec3Length(&vdir) >= 0.2f && WallCrash == false) {
				m_pTransformCom->Go_Straight(fTimeDelta * 4.f);
				//m_fSpeed = 0;
			}
			else if (WallCrash == false)
			{
				WallCrash = true;

			}

			if (WallCrash == true) {
				if (Shake == false)
					m_fSpeed += 0.3f;
				ColPos.x = 30.f;
				ColPos.z -= 2.f;
				m_pTransformCom->LookAt(ColPos);
				if (Shake == false) {
					m_pTransformCom->Go_Straight(fTimeDelta * m_fSpeed);
					pGameInstance->Add_GameObject(LEVEL_COLOSUSS, TEXT("Layer_Colosuss_Right_afterimage"), TEXT("Prototype_GameObject_ColossusFist_Right_afterimage"), this);
				}
			}

		}


		else {
			if (m_bAttack == true) {


				_float3 ColPos = m_pColossusTransform->Get_State(CTransform::STATE_POSITION);
				ColPos.z -= 1.f;
				ColPos.y -= 0.4f;
				m_pTransformCom->LookAt(ColPos);
				_float3 vdir = MyPos - ColPos;



				if (D3DXVec3Length(&vdir) > 5.f) {
					m_tFrame.iFrameStartY = 0.f;
					m_tFrame.iFrameStartX = 0.f;
					m_tFrame.iFrameCurrentX = m_tFrame.iFrameStartX;
					m_tFrame.iFrameEndX = m_tFrame.iFrameStartX;
				}
				else if (D3DXVec3Length(&vdir) > 2.f) {
					m_tFrame.iFrameStartY = 2.f;
					m_tFrame.iFrameStartX = 0.f;
					m_tFrame.iFrameCurrentX = m_tFrame.iFrameStartX;
					m_tFrame.iFrameEndX = m_tFrame.iFrameStartX;
				}

				if (MyPos.z != vpos.z || MyPos.x != vpos.x) {
					//m_pTransformCom->LookAt(vpos);
					if (m_bJump == false && m_fFistTimer > 30) {
						m_fJumpy = m_pTransformCom->Get_State(CTransform::STATE_POSITION).y;
						m_bJump = true;
					}

					_float3 MyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
					vpos.y = MyPos.y;
					if (m_bJump == true && m_fFistTimer > 30) {
						m_bAttacking = true;
						m_pColInfo->bRightAttacking = m_bAttacking;
						m_pTransformCom->LookAt(vpos);
						MyPos.y = m_fJumpy + (m_fJumpPower * m_fTime - 3.8f * m_fTime * m_fTime * 0.5f);
						m_fTime += 0.05f;
						m_pTransformCom->Set_State(CTransform::STATE_POSITION, MyPos);
						_float3 vdir = MyPos - vpos;
						m_fSpeed = D3DXVec3Length(&vdir);
						if (m_fSpeed > 10) {
							m_fSpeed = 10;
						}
						if (D3DXVec3Length(&vdir) > 0.5f)
							m_pTransformCom->Go_Straight(fTimeDelta * m_fTime * (m_fSpeed * 0.2f));
					}
					/*_float3 vdir = MyPos - vpos;
					if (D3DXVec3Length(&vdir) > 0.1f)
					m_pTransformCom->Go_Straight(fTimeDelta * 1.f);
					if (m_fUpcount < 30) {
					m_pTransformCom->Go_Up(fTimeDelta * 1.5f);
					m_fUpcount++;
					}
					m_fFollowTimer++;
					UpFist = true;*/
				}
				/*if (
				m_fFollowTimer >= 100) {
				m_pTransformCom->Go_Down(fTimeDelta * 2.f);
				}*/
			}
			else if (m_bAttack == false) {
				m_fFistTimer = 31;
				m_bJump = false;
				m_fTime = 0;
				UpFist = false;
				m_fFollowTimer = 0;
				m_fUpcount = 0;
				_float3 ColPos = m_pColossusTransform->Get_State(CTransform::STATE_POSITION);
				ColPos.z -= 1.f;
				ColPos.y -= 0.4f;
				//m_pTransformCom->LookAt(ColPos);
				_float3 vdir = MyPos - ColPos;
				if (D3DXVec3Length(&vdir) < 2.f) {
					m_tFrame.iFrameStartY = 2.f;
					m_tFrame.iFrameStartX = 1.f;
					m_tFrame.iFrameCurrentX = m_tFrame.iFrameStartX;
					m_tFrame.iFrameEndX = m_tFrame.iFrameStartX;
				}
				else if (D3DXVec3Length(&vdir) < 5.f) {
					m_tFrame.iFrameStartY = 2.f;
					m_tFrame.iFrameStartX = 0.f;
					m_tFrame.iFrameCurrentX = m_tFrame.iFrameStartX;
					m_tFrame.iFrameEndX = m_tFrame.iFrameStartX;
				}


				if (D3DXVec3Length(&vdir) >= 0.4f) {
					m_pTransformCom->LookAt(ColPos);
					m_pTransformCom->Go_Straight(fTimeDelta * 2.f);
				}


				if (vScale.x == 0) {
					m_pTransformCom->Scaled(m_fScale);
				}

			}
		}
	}


	Go_FrameEnd(m_fSpriteSpeed, fTimeDelta);


	RELEASE_INSTANCE(CGameInstance);

	return _int();
}

_int CColossus_Fist_Right::LateTick(_float fTimeDelta)
{
	if (true == g_bColossusClear)
		return -1;
	if (0 > __super::LateTick(fTimeDelta))
		return -1;

	if (nullptr == m_pRendererCom)
		return -1;

	if (m_bAttack == true && m_pColInfo->bRightWallCrash == false && no_Terrain == false) {
		if (FAILED(SetUp_OnTerrain()))
			return -1;
	}


	CCollider_Sphere::SPHEREDESC SphereDesc;
	SphereDesc.ObjectMatrix = m_pTransformCom->Get_WorldMatrix();
	SphereDesc.fRadius = 0.25f;
	SphereDesc.fDist_From_Obj = _float3(0.f, 0.f, 0.f);
	m_pColliderCom->Set_SphereDesc(SphereDesc);
	m_pColliderCom->Tick();

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CTransform* pCameraTrans = (CTransform*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Camera"), TEXT("Com_Transform"));
	_float3 vCameraPos = pCameraTrans->Get_State(CTransform::STATE_POSITION);
	m_pTransformCom->MatchIt_XZ(vCameraPos);
	//m_pTransformCom->Scaled(m_fScale);
	RELEASE_INSTANCE(CGameInstance);

	//_float4x4		ViewMatrix; //������� ������ ����
	//							//ī�޶� �佺���̽��� ����� ���ؼ� �����Ѵ�

	//m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	////���庯ȯ��ķ� �����
	//D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);
	////D3DXMatrixInverse
	////����迡 �������ش�
	//m_pTransformCom->Set_State(CTransform::STATE_RIGHT, (*(_float3*)&ViewMatrix.m[0][0]) * 8.f);
	//m_pTransformCom->Set_State(CTransform::STATE_UP, (*(_float3*)&ViewMatrix.m[1][0]) * 8.f);
	//m_pTransformCom->Set_State(CTransform::STATE_LOOK, (*(_float3*)&ViewMatrix.m[2][0]) *8.f);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);
	Collision_Rect();
	return _int();
}

HRESULT CColossus_Fist_Right::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
		return E_FAIL;
	if (FAILED(Go_Sprites()))
	{
		MSGBOX("Go_Sprites returned E_FAIL in CColossus_Fist_Right::Tick");
		return E_FAIL;
	}
	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice()))
		return E_FAIL;

	////////////////////
	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	m_pVIBufferCom->Render(); // �̷��� ������� ������ ������ �� ���
							  // Ư���� �κп��� ������ �����ϰų� �� �� �ִ�
	if (FAILED(Release_RenderState()))
		return E_FAIL;
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

HRESULT CColossus_Fist_Right::SetUp_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_COLOSUSS, TEXT("Prototype_Component_Texture_Colossus_Fist_Right"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	CCollider_Sphere::SPHEREDESC SphereDesc;
	SphereDesc.ObjectMatrix = m_pTransformCom->Get_WorldMatrix();
	SphereDesc.fRadius = 0.25f;
	SphereDesc.fDist_From_Obj = _float3(0.f, 0.f, 0.f);

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

HRESULT CColossus_Fist_Right::SetUp_OnTerrain()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	CVIBuffer_Terrain* m_pVIBuffer_Terrain = (CVIBuffer_Terrain*)pGameInstance->Get_Component(g_eCurrentLevel, TEXT("Layer_Terrain"), TEXT("Com_VIBuffer"));
	if (m_pVIBuffer_Terrain == nullptr)
		return E_FAIL;
	//�÷��̾��� ��ġ�� ���ͼ�
	_float3 vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	vPosition.y = m_pVIBuffer_Terrain->Compute_Y(vPosition) + (m_fScale.x / 4);
	if (vPosition.y > m_pTransformCom->Get_State(CTransform::STATE_POSITION).y) {
		if (m_bJump == true) {
			m_fFistTimer = 0;
			for (m_iDustNum; m_iDustNum < 16; m_iDustNum++) {
				pGameInstance->Add_GameObject(LEVEL_COLOSUSS, TEXT("Layer_Colosuss_Fist_Dust"), TEXT("Prototype_GameObject_Colossus_Dust"), this);
			}
			if (m_iDustNum >= 16)
				m_iDustNum = 0;

			Shake = true;
			m_pColInfo->FistCount++;
		}
		UpFist = false;
		m_fFollowTimer = 0;
		m_fUpcount = 0;
		m_bJump = false;
		m_fTime = 0;
		m_bAttacking = false;
		m_pColInfo->bRightAttacking = false;
	}
	if (m_bJump == false)
		//(UpFist == false) 
	{
		m_fFistTimer++;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
	}
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CColossus_Fist_Right::SetUp_RenderState()
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
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);// �����׽�Ʈ�� �ϰڴ�
																   //�ش������� ���ؼ� �񱳸� �� ����
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 150);
	//�׸����� �ߴ� �̹����� ���İ��� ���� ������ ���̶�
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER/*ũ�ٸ�*/);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);
	// �� ������ ����� �ֵ鸸 ȭ�鿡 �׸��ڴ�
	// ���ڵ��� ��� ���� �׸����� ���İ�(REF - 0)���� ũ�ٸ�(GREATER) �׸���

	// �ܼ��� ����� ����µ��� �����׽����� ���� ���������� �Ѱ谡 �ִ�
	// ������ ó���� �Ұ����ϱ� ����

	//���� �׽��ð� ������ ȥ������ �ʴ´� -> �������ѵ� �ǹ̰� �����
	return S_OK;
}

HRESULT CColossus_Fist_Right::Release_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

	return S_OK;
}
HRESULT CColossus_Fist_Right::Go_Sprites()
{
	if (FAILED(m_pVIBufferCom->Go_Sprites(m_tFrame)))
	{
		MSGBOX("m_pVIBufferCom->Go_Sprites returned E_FAIL in CYeti::Go_Sprites");
		return E_FAIL;
	}
	return S_OK;
}


void CColossus_Fist_Right::Collision_Rect()
{
	if (LEVEL_LOADING == g_eCurrentLevel)
		return;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	for (_uint i = 0; i < 4; i++)
	{
		_float3 vPos = pGameInstance->Collision_SphereToRect((CCollider_Rect*)pGameInstance->Get_Component(g_eCurrentLevel, TEXT("Layer_Wall"), TEXT("Com_Collider"), i), m_pColliderCom);
		if (vPos != _float3(0.f, 0.f, 0.f)) {
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
			if (WallCrash == true) {
				//m_pColInfo->bLeftWallCrash = false;
				//WallCrash = false;
				Shake = true;
				static_cast<CColossus*>(pGameInstance->Get_Front(LEVEL_COLOSUSS, TEXT("Layer_Colossus")))->Set_Cry();
			}
		}
	}

	RELEASE_INSTANCE(CGameInstance);
}

CColossus_Fist_Right * CColossus_Fist_Right::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CColossus_Fist_Right*	pInstance = new CColossus_Fist_Right(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CColossus_Fist_Right");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CColossus_Fist_Right::Clone(void* pArg)
{
	/* ���οü�� �����Ͽ� �����Ѵ�. */
	CColossus_Fist_Right*	pInstance = new CColossus_Fist_Right(*this);


	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CColossus_Fist_Right");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CColossus_Fist_Right::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);
	Safe_Release(m_pVIBufferCollCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}