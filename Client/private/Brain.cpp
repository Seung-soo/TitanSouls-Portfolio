#include "stdafx.h"
#include "..\Public\Brain.h"
#include "GameInstance.h"
#include "Camera_Player.h"
#include "DarkScreen.h"

CBrain::CBrain(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CBrain::CBrain(const CBrain & rhs)
	: CGameObject(rhs)
{
}

HRESULT CBrain::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		MSGBOX("__super::NativeConstruct_Prototype returned E_FAIL in CBrain::NativeConstruct_Prototype");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CBrain::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		MSGBOX("__super::NativeConstruct returned E_FAIL in CBrain::NativeConstruct");
		return E_FAIL;
	}

	// 컴포넌트 세팅
	if (FAILED(SetUp_Components()))
	{
		MSGBOX("SetUp_Components returned E_FAIL in CBrain::NativeConstruct");
		return E_FAIL;
	}

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	m_pCamera = static_cast<CCamera_Player*>(pGameInstance->Get_Front(LEVEL_STATIC, TEXT("Layer_Camera")));
	RELEASE_INSTANCE(CGameInstance);

	m_IceDead = (ICEDEAD*)pArg;

	m_pTransformCom->Scaled(_float3(m_fScale, m_fScale, m_fScale));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(20.f, 0.0f, 20.f));

	m_tFrame.iFrameStartY = 0.f;
	m_tFrame.iFrameCurrentX = 0.f;
	m_tFrame.iFrameStartX = 0.f;
	m_tFrame.iFrameEndX = 0.f;
	m_tFrame.iFrameHeight = 64.f;
	m_tFrame.iFrameWidth = 64.f;
	m_tFrame.iTexHeight = 64.f;
	m_tFrame.iTexWidth = 128.f;
	//m_fSpriteSpeed = 1;

	m_fSpriteSpeed = 2;


	return S_OK;
}

_int CBrain::Tick(_float fTimeDelta)
{

	if (true == m_IceDead->bBrainDead)
	{

		m_tFrame.iFrameCurrentX = 0.f;
		m_tFrame.iFrameStartX = 0.f;
		m_tFrame.iFrameEndX = 0.f;
		Go_FrameEnd(m_fSpriteSpeed, fTimeDelta);
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
	{
		MSGBOX("0 > __super::Tick in CBrain::Tick");
		return -1;
	}

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (Shake) {
		static_cast<CCamera_Player*>(pGameInstance->Get_Front(LEVEL_STATIC, TEXT("Layer_Camera")))->Set_Shake(false, true, false);
		ShakeTime++;
	}
	if (ShakeTime > 20) {
		Shake = false;
		ShakeTime = 0;
	}

	_float m_fJumpMinus = m_fJumpPower - 3.8f * m_fTime;
	if (m_fJumpMinus < 0) {
		m_tFrame.iFrameCurrentX = 0.f;
		m_tFrame.iFrameStartX = 0.f;
		m_tFrame.iFrameEndX = 0.f;
	}
	//m_fFrame += 90.0f * fTimeDelta; // 1초에 띄울 프레임의 갯수만큼 상수를 바꾼다

	//if (m_fFrame >= 90.0f) // 만약 프레임을 다 돌았다면 처음으로 되돌린다
	//	m_fFrame = 0.f;
	//m_fFrame += 90.0f * fTimeDelta; // 1초에 띄울 프레임의 갯수만큼 상수를 바꾼다

	//if (m_fFrame >= 90.0f) // 만약 프레임을 다 돌았다면 처음으로 되돌린다
	//	m_fFrame = 0.f;

	if (m_bJump == true) {
		//m_fminusScaleTime++;
		if (m_fJumpMinus < 0) {
			m_fminusScale -= 0.025f;
		}
		else {
			m_fminusScale += 0.025f;
		}
		if (m_fScale - m_fminusScale < 4.f)
			m_pTransformCom->Scaled(_float3(m_fScale - m_fminusScale, m_fScale, m_fScale));
	}

	if (m_IceDead->bBrainDead == false) {
		CTransform* m_pPlayerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"));
		if (m_IceDead->bIceDead == false) {
			CTransform* m_pCubeTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_ICEBRAIN, TEXT("Layer_Ice"), TEXT("Com_Transform"));
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pCubeTransform->Get_State(CTransform::STATE_POSITION));
		}
		_float3 vpos = m_pPlayerTransform->Get_State(CTransform::STATE_POSITION);
		/*	if (m_b2Phase == false)
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pCubeTransform->Get_State(CTransform::STATE_POSITION));*/
		if (pGameInstance->Get_DIKeyState(DIK_2)) {
			m_IceDead->bBrainDead = true;
			//m_b2Phase = true;
		}

		if (m_IceDead->bIceDead == true && m_b2Phase == false) {
			m_b2Phase = true;
			pGameInstance->Add_GameObject(LEVEL_ICEBRAIN, TEXT("Layer_IceBrain_Brain_Shadow"), TEXT("Prototype_GameObject_Brain_Shadow"), this);

		}
		if (m_b2Phase == true) {
			if (m_bJump == false) {
				m_fJumpy = m_pTransformCom->Get_State(CTransform::STATE_POSITION).y;
				m_bJump = true;
			}

			_float3 MyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			vpos.y = MyPos.y;
			if (m_bJump == true) {
				m_pTransformCom->LookAt(vpos);
				MyPos.y = m_fJumpy + (m_fJumpPower * m_fTime - 2.8f * m_fTime * m_fTime * 0.5f);
				m_fTime += 0.025f;
				m_pTransformCom->Set_State(CTransform::STATE_POSITION, MyPos);
				_float3 vdir;
				vdir = MyPos - vpos;
				if (D3DXVec3Length(&vdir) >= 0.8f)
					m_pTransformCom->Go_Straight(fTimeDelta + m_fTime * 0.005f);
			}
		}
		//////////////////////////////////////
		Go_Frame(m_fSpriteSpeed, fTimeDelta);




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
		if (m_bWaterCreat == true) {
			if (FAILED(pGameInstance->Add_GameObject(LEVEL_ICEBRAIN, TEXT("Layer_Breain_Water_Mini"), TEXT("Prototype_GameObject_Brain_water_mini"), this)))
			{
				MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CSlime_Boss::SetUp_OnTerrain");
				return E_FAIL;
			}
			pGameInstance->LetsPlaySound(L"IceBrain_Land.ogg", CSoundMgr::MONSTER1, 0.5f);
			m_iWaterCout++;
		}
		if (m_iWaterCout > 8) {
			pGameInstance->StopSound(CSoundMgr::MONSTER1);
			m_iWaterCout = 0;
			m_bWaterCreat = false;
		}

	}


	RELEASE_INSTANCE(CGameInstance);

	return _int();
}

_int CBrain::LateTick(_float fTimeDelta)
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
		MSGBOX("SetUp_OnTerrain returned E_FAIL in CBrain::LateTick");
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

	//_float4x4		ViewMatrix; //뷰행렬을 저장할 변수
	//							//카메라 뷰스페이스의 행렬을 구해서 저장한다
	//m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	////월드변환행렬로 만든다
	//D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);
	////D3DXMatrixInverse
	////라업룩에 적용해준다
	//m_pTransformCom->Set_State(CTransform::STATE_RIGHT, (*(_float3*)&ViewMatrix.m[0][0]) *4.f/* m_pTransformCom->Get_Scale().x*/);
	//m_pTransformCom->Set_State(CTransform::STATE_UP, (_float3(0.f,1.f,0.f) * 4.f));
	//m_pTransformCom->Set_State(CTransform::STATE_LOOK, (*(_float3*)&ViewMatrix.m[2][0]) * 4.f/*m_pTransformCom->Get_Scale().z*/);


	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);
	Collision_Rect();
	return _int();
}

HRESULT CBrain::Render()
{
	if (nullptr == m_pVIBufferCom)
	{
		MSGBOX("m_pVIBufferCom is nullptr in CBrain::Render")
			return E_FAIL;
	}

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
	{
		MSGBOX("m_pTransformCom->Bind_OnGraphicDevice returned E_FAIL in CBrain::Render");
		return E_FAIL;
	}

	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice()))
	{
		MSGBOX("m_pTextureCom->Bind_OnGraphicDevice in CBrain::Render");
		return E_FAIL;
	}
	if (FAILED(Go_Sprites()))
	{
		MSGBOX("Go_Sprites returned E_FAIL in CYeti::Tick");
		return E_FAIL;
	}

	if (FAILED(SetUp_RenderState()))
	{
		MSGBOX("SetUp_RenderState returned E_FAIL in CBrain::Render");
		return E_FAIL;
	}

	m_pVIBufferCom->Render();

	if (FAILED(Release_RenderState()))
	{
		MSGBOX("Release_RenderState returned E_FAIL in CBrain::Render");
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

HRESULT CBrain::SetUp_Components()
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CBrain::SetUp_Components(Transform)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CBrain::SetUp_Components(Renderer)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CBrain::SetUp_Components(VIBuffer_Rect)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_ICEBRAIN, TEXT("Prototype_Component_Texture_Brain"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CBrain::SetUp_Components(Texture)");
		return E_FAIL;
	}


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

HRESULT CBrain::SetUp_OnTerrain()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	CVIBuffer_Terrain* m_pVIBuffer_Terrain = (CVIBuffer_Terrain*)pGameInstance->Get_Component(g_eCurrentLevel, TEXT("Layer_Terrain"), TEXT("Com_VIBuffer"));
	if (m_pVIBuffer_Terrain == nullptr)
	{
		MSGBOX("pVIBuffer_Terrain is nullptr in CBrain::SetUp_OnTerrain");
		return E_FAIL;
	}
	//플레이어의 위치를 얻어와서
	_float3 vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	vPosition.y = m_pVIBuffer_Terrain->Compute_Y(vPosition) + 0.7f;
	if (m_bJump == true) {

		if (m_pTransformCom->Get_State(CTransform::STATE_POSITION).y < vPosition.y) {
			m_bJump = false;
			m_fTime = 0;
			m_bWaterCreat = true;
			Shake = true;
			m_tFrame.iFrameCurrentX = 1.f;
			m_tFrame.iFrameStartX = 1.f;
			m_tFrame.iFrameEndX = 1.f;

		}
	}
	if (m_bJump == false && m_b2Phase == true) {
		m_fminusScaleTime = 0;
		m_fminusScale = 0;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
	}
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CBrain::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
	{
		MSGBOX("m_pGraphic_Device is nullptr in CBrain::SetUp_RenderState");
		return E_FAIL;
	}

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
	//m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// 이 조건을 통과한 애들만 화면에 그리겠다
	// 이코드의 경우 현재 그릴려는 알파값(REF - 0)보다 크다면(GREATER) 그린다

	// 단순히 배경을 지우는데는 알파테스팅이 아주 유용하지만 한계가 있다
	// 반투명 처리가 불가능하기 때문

	//또한 테스팅과 블렌딩은 혼합하지 않는다 -> 가능은한데 의미가 퇴색됨
	return S_OK;
}

HRESULT CBrain::Release_RenderState()
{
	if (nullptr == m_pGraphic_Device)
	{
		MSGBOX("m_pGraphic_Device is nullptr in CBrain::Release_RenderState");
		return E_FAIL;
	}
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	//m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

HRESULT CBrain::Go_Sprites()
{
	if (FAILED(m_pVIBufferCom->Go_Sprites(m_tFrame)))
	{
		MSGBOX("m_pVIBufferCom->Go_Sprites returned E_FAIL in CYeti::Go_Sprites");
		return E_FAIL;
	}
	return S_OK;
}

void CBrain::Collision_Rect()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	for (_uint i = 0; i < 4; i++)
	{
		CCollider_Rect* pColWall = (CCollider_Rect*)pGameInstance->Get_Component(g_eCurrentLevel, TEXT("Layer_Wall"), TEXT("Com_Collider"), i);
		_float3 vPos = pGameInstance->Collision_SphereToRect(pColWall, m_pColliderCom);
		if (vPos != _float3(0.f, 0.f, 0.f))
		{
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
			_float3 vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
			_float3 vWallLook = *(_float3*)pColWall->Get_RectDesc().ObjectMatrix.m[CTransform::STATE_LOOK];
			D3DXVec3Normalize(&vLook, &vLook);
			D3DXVec3Normalize(&vWallLook, &-vWallLook);
			vLook += 1.5f * vWallLook;
			D3DXVec3Normalize(&vLook, &vLook);
			//vLook *= 4.f;

			_float3 vRight = *D3DXVec3Cross(&vRight, &_float3(0.f, 1.f, 0.f), &vLook);
			vRight = *D3DXVec3Normalize(&vRight, &vRight); //* 4.f;

			_float3 vUp = *D3DXVec3Cross(&vUp, &vLook, &vRight);
			vUp = *D3DXVec3Normalize(&vUp, &vUp); //* 4.f;

			m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight * 4.f);
			m_pTransformCom->Set_State(CTransform::STATE_UP, vUp  * 4.f);
			m_pTransformCom->Set_State(CTransform::STATE_LOOK, vLook * 4.f);
		}
	}

	RELEASE_INSTANCE(CGameInstance);
}

CBrain * CBrain::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBrain*	pInstance = new CBrain(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create Brain in CBrain::Create");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CBrain::Clone(void* pArg)
{
	/* 새로운객체를 복제하여 생성한다. */
	CBrain*	pInstance = new CBrain(*this);


	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create Brain in CBrain::Clone");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBrain::Free()
{
	__super::Free();
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pVIBufferCollCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}