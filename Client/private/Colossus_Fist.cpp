#include "stdafx.h"
#include "..\Public\Colossus_Fist.h"
#include "GameInstance.h"
#include "Colossus.h"

CColossus_Fist::CColossus_Fist(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CColossus_Fist::CColossus_Fist(const CColossus_Fist & rhs)
	: CGameObject(rhs)
{
}

HRESULT CColossus_Fist::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CColossus_Fist::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	/* 현재 객체에게 추가되어야할 컴포넌트들을 복제(or 참조)하여 멤버변수에 보관한다.  */
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(10.f, 2.f, 29.f));
	m_pTransformCom->Scaled(m_fScale);

	m_pColInfo = (COLOSSUSINFO*)pArg;
	m_bAttack = m_pColInfo->bLeftFistAttack;

	m_tFrame.iFrameStartY = 0.f;
	m_tFrame.iFrameCurrentX = 0.f;
	m_tFrame.iFrameStartX = 0.f;
	m_tFrame.iFrameEndX = 0.f;
	m_tFrame.iFrameHeight = 64.f;
	m_tFrame.iFrameWidth = 64.f;
	m_tFrame.iTexHeight = 196;
	m_tFrame.iTexWidth = 256.f;
	pGameInstance->Add_GameObject(LEVEL_COLOSUSS, TEXT("Layer_Colossus_Fist"), TEXT("Prototype_GameObject_ColossusFist_Left_Shadow"), this);
	m_fSpriteSpeed = 1;
	WallCrash = false;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

_int CColossus_Fist::Tick(_float fTimeDelta)
{

	if (0 > __super::Tick(fTimeDelta))
		return -1;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	if (static_cast<CColossus*>(pGameInstance->Get_Front(LEVEL_COLOSUSS, TEXT("Layer_Colossus")))->Get_Clear())
	{
		RELEASE_INSTANCE(CGameInstance);

		return 0;
	}
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
			m_pColInfo->bLeftWallCrash = false;
			no_Terrain = true;
			m_pColInfo->FistCount = 0;
			m_fFistTimer = 31;
			ShakeTime = 0;
		}
	}
	else if (ShakeTime > 20) {
		Shake = false;
		ShakeTime = 0;
		pGameInstance->StopSound(CSoundMgr::MONSTER1);
	}

	//static_cast<CCamera_Player*>(pGameInstance->Get_Front(LEVEL_STATIC, TEXT("Layer_Camera")))->Camera_Shake_Horizon(fTimeDelta);

	CTransform* m_pPlayerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"));
	CTransform* m_pColossusTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_COLOSUSS, TEXT("Layer_Colossus"), TEXT("Com_Transform"));
	_float3 vpos = m_pPlayerTransform->Get_State(CTransform::STATE_POSITION);

	_float3 MyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float3 vScale = m_pTransformCom->Get_Scale();

	vpos.y = MyPos.y;
	m_bStandUp = m_pColInfo->bStandUp;
	m_bAttack = m_pColInfo->bLeftFistAttack;
	m_fJumpMinus = m_fJumpPower - 3.8f * m_fTime;
	if (m_fJumpMinus < 0) {
		pGameInstance->Add_GameObject(LEVEL_COLOSUSS, TEXT("Layer_Colosuss_Left_afterimage"), TEXT("Prototype_GameObject_ColossusFist_Left_afterimage"), this);
		no_Terrain = false;
	}
	/*else
	m_bFistFall = false;

	if (m_fJumpMinus > -1 && m_fJumpMinus < 1) {
	m_tFrame.iFrameStartY = 1.f;
	m_tFrame.iFrameStartX = 1.f;
	m_tFrame.iFrameCurrentX = m_tFrame.iFrameStartX;
	m_tFrame.iFrameEndX = m_tFrame.iFrameStartX;
	}
	else {
	m_tFrame.iFrameStartY = 0.f;
	m_tFrame.iFrameStartX = 0.f;
	m_tFrame.iFrameCurrentX = m_tFrame.iFrameStartX;
	m_tFrame.iFrameEndX = m_tFrame.iFrameStartX;
	}*/

	if (WallCrash == true && ShakeTime > 1) {
		if ((ShakeTime % 10) == 0)
		{
			pGameInstance->Add_GameObject(LEVEL_COLOSUSS, TEXT("Layer_Colossus_Jongyu"), TEXT("Prototype_GameObject_Jongyu"), this);
			pGameInstance->LetsPlaySound(L"Colossus_Hit.ogg", CSoundMgr::MONSTER1, 1.0f);
			pGameInstance->LetsPlaySound(L"Colossus_earthquake.ogg", CSoundMgr::MONSTER5, 1.0f);
		}
	}


	if (m_bStandUp == true) {
		if (m_pColInfo->bLeftWallCrash == true) {
			_float3 ColPos = m_pColossusTransform->Get_State(CTransform::STATE_POSITION);
			if (WallCrash == false) {
				ColPos.x -= 4.f;
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
					m_fSpeed += 1.f;
				ColPos.x = 2.f;
				ColPos.z -= 2.f;
				m_pTransformCom->LookAt(ColPos);
				if (Shake == false) {
					m_pTransformCom->Go_Straight(fTimeDelta * m_fSpeed);
					pGameInstance->Add_GameObject(LEVEL_COLOSUSS, TEXT("Layer_Colosuss_Left_afterimage"), TEXT("Prototype_GameObject_ColossusFist_Left_afterimage"), this);
				}
			}

		}


		else {
			if (m_bAttack == true) {
				//no_Terrain = false;
				_float3 ColPos = m_pColossusTransform->Get_State(CTransform::STATE_POSITION);
				ColPos.z -= 1.f;
				ColPos.y -= 0.4f;
				//m_pTransformCom->LookAt(ColPos);
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
						m_pColInfo->bLeftAttacking = m_bAttacking;
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
			else {
				//m_pColInfo->bLeftAttacking = false;
				//m_pTransformCom->Scaled(m_Fsca)
				m_fFistTimer = 31;
				m_bJump = false;
				m_fTime = 0;
				UpFist = false;
				m_fFollowTimer = 0;
				m_fUpcount = 0;
				_float3 ColPos = m_pColossusTransform->Get_State(CTransform::STATE_POSITION);
				ColPos.z -= 1.5f;
				ColPos.y -= 0.4f;
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
					int a = 0;
				}

			}
		}
	}
	//m_fFrame += 90.0f * fTimeDelta; // 1초에 띄울 프레임의 갯수만큼 상수를 바꾼다

	//if (m_fFrame >= 90.0f) // 만약 프레임을 다 돌았다면 처음으로 되돌린다
	//	m_fFrame = 0.f;

	//if (pGameInstance->Get_DIKeyState(DIK_UP) & 0x80)
	//{
	//	m_pTransformCom->Go_Straight(fTimeDelta);
	//}

	//if (pGameInstance->Get_DIKeyState(DIK_DOWN) & 0x80)
	//{
	//	m_pTransformCom->Go_BackWard(fTimeDelta);
	//}

	//if (pGameInstance->Get_DIKeyState(DIK_LEFT) & 0x80)
	//{
	//	m_pTransformCom->Turn(_float3(0.f, 1.f, 0.f), -fTimeDelta);
	//}

	//if (pGameInstance->Get_DIKeyState(DIK_RIGHT) & 0x80)
	//{
	//	m_pTransformCom->Turn(_float3(0.f, 1.f, 0.f), fTimeDelta);
	//}

	Go_FrameEnd(m_fSpriteSpeed, fTimeDelta);


	RELEASE_INSTANCE(CGameInstance);

	return _int();
}

_int CColossus_Fist::LateTick(_float fTimeDelta)
{
	if (true == g_bColossusClear)
		return -1;
	if (0 > __super::LateTick(fTimeDelta))
		return -1;

	if (nullptr == m_pRendererCom)
		return -1;
	if (m_bAttack == true && m_pColInfo->bLeftWallCrash == false && no_Terrain == false) {
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

	RELEASE_INSTANCE(CGameInstance);


	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	Collision_Rect();
	return _int();
}

HRESULT CColossus_Fist::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
		return E_FAIL;
	if (FAILED(Go_Sprites()))
	{
		MSGBOX("Go_Sprites returned E_FAIL in CColossus_Fist::Tick");
		return E_FAIL;
	}
	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice()))
		return E_FAIL;


	////////////////////
	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	m_pVIBufferCom->Render(); // 이렇게 순서대로 랜더의 설정을 할 경우
							  // 특정한 부분에만 블렌딩을 적용하거나 할 수 있다
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

HRESULT CColossus_Fist::SetUp_Components()
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
	/*if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Player"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
	return E_FAIL;*/
	if (FAILED(__super::Add_Component(LEVEL_COLOSUSS, TEXT("Prototype_Component_Texture_Colossus_Fist"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
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

HRESULT CColossus_Fist::SetUp_OnTerrain()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	CVIBuffer_Terrain* m_pVIBuffer_Terrain = (CVIBuffer_Terrain*)pGameInstance->Get_Component(g_eCurrentLevel, TEXT("Layer_Terrain"), TEXT("Com_VIBuffer"));
	if (m_pVIBuffer_Terrain == nullptr)
		return E_FAIL;
	//플레이어의 위치를 얻어와서
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
		m_pColInfo->bLeftAttacking = false;
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

HRESULT CColossus_Fist::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);//알파블렌딩을 하겠다
	//m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);//색을 더한다
	//m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	////그릴려고 하는 픽셀의 색상을 섞을때 소스 칼라의 알파를 섞겠다
	//m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	/*
	_float4		vSourColor, vDestColor;
	(vSourColor.rgb) * vSourColor.a + (vDestColor.rgb) * (1.f - vSourColor.a);
	소스컬러.RGB * 소스컬러.알파 + 데스트컬라.RGB * 1.f - 소스컬러.알파
	알파는 RGB의 0~255를 0에서 1사이로 압축한것
	플레이어를 그리려하는데 이미 그위치에 그려져 있던 색이 데스트칼라
	*/

	//다른 방법 - 알파 테스트
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);// 알파테스트를 하겠다
																   //해당조건을 통해서 비교를 한 다음
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 150);
	//그릴려고 했던 이미지의 알파값을 내가 지정한 값이랑
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER/*크다면*/);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);
	// 이 조건을 통과한 애들만 화면에 그리겠다
	// 이코드의 경우 현재 그릴려는 알파값(REF - 0)보다 크다면(GREATER) 그린다

	// 단순히 배경을 지우는데는 알파테스팅이 아주 유용하지만 한계가 있다
	// 반투명 처리가 불가능하기 때문

	//또한 테스팅과 블렌딩은 혼합하지 않는다 -> 가능은한데 의미가 퇴색됨
	return S_OK;
}

HRESULT CColossus_Fist::Release_RenderState()
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

void CColossus_Fist::Collision_Rect()
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

HRESULT CColossus_Fist::Go_Sprites()
{
	if (FAILED(m_pVIBufferCom->Go_Sprites(m_tFrame)))
	{
		MSGBOX("m_pVIBufferCom->Go_Sprites returned E_FAIL in CYeti::Go_Sprites");
		return E_FAIL;
	}
	return S_OK;
}


CColossus_Fist * CColossus_Fist::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CColossus_Fist*	pInstance = new CColossus_Fist(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CClossus_Fist");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CColossus_Fist::Clone(void* pArg)
{
	/* 새로운객체를 복제하여 생성한다. */
	CColossus_Fist*	pInstance = new CColossus_Fist(*this);


	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CClossus_Fist");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CColossus_Fist::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);
	Safe_Release(m_pVIBufferCollCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}