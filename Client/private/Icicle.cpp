#include "stdafx.h"
#include "..\Public\Icicle.h"
#include "GameInstance.h"
#include "Player.h"

CIcicle::CIcicle(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CIcicle::CIcicle(const CIcicle & rhs)
	: CGameObject(rhs)
{
}

HRESULT CIcicle::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CIcicle::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	/* 현재 객체에게 추가되어야할 컴포넌트들을 복제(or 참조)하여 멤버변수에 보관한다.  */
	if (FAILED(SetUp_Components()))
		return E_FAIL;
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	CTransform* m_pPlayerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"));
	CTransform* m_pYetiTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_YETI, TEXT("Layer_Yeti"), TEXT("Com_Transform"));
	//_float3 YetiPos = m_pYetiTransform->Get_State(CTransform::STATE_POSITION);
	m_pTransformCom->Scaled(_float3(m_fScale, m_fScale, m_fScale));

	_float3 m_fRandPos = { (_float)(rand() % 32),15.f, (_float)(rand() % 32) };
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_fRandPos);
	m_fJumpy = m_pTransformCom->Get_State(CTransform::STATE_POSITION).y;
	m_bJump = true;

	pGameInstance->Add_GameObject(LEVEL_YETI, TEXT("Layer_Icicle_Shadow"), TEXT("Prototype_GameObject_Icicle_Shadow"), this);


	//YetiPos.y += 2.f;
	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, YetiPos);
	//m_pTransformCom->LookAt(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION));

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

_int CIcicle::Tick(_float fTimeDelta)
{


	if (0 > __super::Tick(fTimeDelta))
		return -1;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (m_bDead) {
		for (int i = 0; i < 8; i++) {
			pGameInstance->StopSound(CSoundMgr::MONSTER3);// 사운드	
			pGameInstance->Add_GameObject(LEVEL_YETI, TEXT("Layer_Icicle_Break_Effect"), TEXT("Prototype_GameObject_Icicle_Break_Effect"), this);
			pGameInstance->LetsPlaySound(L"Yeti_IcicleBreak.ogg", CSoundMgr::MONSTER3, 1.f); // 사운드	
		}
		RELEASE_INSTANCE(CGameInstance);
		return -1;
	}
	if (pGameInstance->Collision_Sphere(m_pColliderCom, (CCollider_Sphere*)pGameInstance->Get_Component(LEVEL_YETI, TEXT("Layer_Yeti"), TEXT("Com_Collider"))))
	{
		m_bDead = true;
	}
	else if (nullptr != pGameInstance->Find_Layer(LEVEL_YETI, TEXT("Layer_SnowBall")))
	{
		for (size_t i = 0; i < pGameInstance->Find_Layer(LEVEL_YETI, TEXT("Layer_SnowBall"))->Get_Object().size(); i++)
		{
			if (pGameInstance->Collision_Sphere(m_pColliderCom, (CCollider_Sphere*)pGameInstance->Get_Component(LEVEL_YETI, TEXT("Layer_SnowBall"), TEXT("Com_Collider"), (_uint)i)))
			{
				for (int i = 0; i < 8; i++) {
					pGameInstance->StopSound(CSoundMgr::MONSTER3);// 사운드	
					pGameInstance->Add_GameObject(LEVEL_YETI, TEXT("Layer_Icicle_Break_Effect"), TEXT("Prototype_GameObject_Icicle_Break_Effect"), this);
					pGameInstance->LetsPlaySound(L"Yeti_IcicleBreak.ogg", CSoundMgr::MONSTER3, 1.f); // 사운드
				}
				m_bDead = true;
			}
		}
	}




	/*_float3 MyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float3 vpos = m_pPlayerTransform->Get_State(CTransform::STATE_POSITION);*/

	//m_pTransformCom->Go_Straight(fTimeDelta * 7.f);

	RELEASE_INSTANCE(CGameInstance);

	return _int();
}

_int CIcicle::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
	{
		MSGBOX("0 > __super::LateTick in CIcicle::LateTick");
		return -1;
	}

	if (nullptr == m_pRendererCom)
	{
		MSGBOX("m_pRendererCom is nullptr in CIcicle::LateTick");
		return -1;
	}

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_float3 MyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	MyPos.y = m_fJumpy + (m_fJumpPower * m_fTime - 3.8f * m_fTime * m_fTime * 0.5f);
	m_fTime += 0.05f;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, MyPos);

	m_iDeleteCount++;
	if (m_iDeleteCount > 300) {

		m_bDead = true;
	}


	if (FAILED(SetUp_OnTerrain()))
	{
		MSGBOX("SetUp_OnTerrain returned E_FAIL in CIcicle::LateTick");
		return -1;
	}

	CCollider_Sphere::SPHEREDESC SphereDesc;
	SphereDesc.ObjectMatrix = m_pTransformCom->Get_WorldMatrix();
	SphereDesc.fRadius = 0.4f;
	SphereDesc.fDist_From_Obj = _float3(0.f, 0.f, 0.f);
	m_pColliderCom->Set_SphereDesc(SphereDesc);
	m_pColliderCom->Tick();

	/*if (FAILED(SetUp_OnTerrain()))
	return -1;*/



	CTransform* pCameraTrans = (CTransform*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Camera"), TEXT("Com_Transform"));
	_float3 vCameraPos = pCameraTrans->Get_State(CTransform::STATE_POSITION);
	m_pTransformCom->MatchIt_XZ(vCameraPos);

	RELEASE_INSTANCE(CGameInstance);

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
	Collision();
	return _int();
}

HRESULT CIcicle::Render()
{
	if (nullptr == m_pVIBufferCom)
	{
		MSGBOX("m_pVIBufferCom is nullptr in CIcicle::Render")
			return E_FAIL;
	}

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
	{
		MSGBOX("m_pTransformCom->Bind_OnGraphicDevice returned E_FAIL in CIcicle::Render");
		return E_FAIL;
	}
	m_pVIBufferCom->Base_Setting();
	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice()))
	{
		MSGBOX("m_pTextureCom->Bind_OnGraphicDevice in CIcicle::Render");
		return E_FAIL;
	}


	if (FAILED(SetUp_RenderState()))
	{
		MSGBOX("SetUp_RenderState returned E_FAIL in CIcicle::Render");
		return E_FAIL;
	}

	m_pVIBufferCom->Render();

	if (FAILED(Release_RenderState()))
	{
		MSGBOX("Release_RenderState returned E_FAIL in CIcicle::Render");
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

HRESULT CIcicle::SetUp_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CIcicle::SetUp_Components(Transform)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CIcicle::SetUp_Components(Renderer)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CIcicle::SetUp_Components(VIBuffer_Rect)");
		return E_FAIL;
	}

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_YETI, TEXT("Prototype_Component_Texture_Icicle"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	CCollider_Sphere::SPHEREDESC SphereDesc;
	SphereDesc.ObjectMatrix = m_pTransformCom->Get_WorldMatrix();
	SphereDesc.fRadius = 0.4f;
	SphereDesc.fDist_From_Obj = _float3(0.f, 0.1f, 0.f);

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

HRESULT CIcicle::SetUp_OnTerrain()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	CVIBuffer_Terrain* m_pVIBuffer_Terrain = (CVIBuffer_Terrain*)pGameInstance->Get_Component(g_eCurrentLevel, TEXT("Layer_Terrain"), TEXT("Com_VIBuffer"));


	if (m_pVIBuffer_Terrain == nullptr)
		return E_FAIL;


	_float3 vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	vPosition.y = m_pVIBuffer_Terrain->Compute_Y(vPosition);
	if (m_bJump == true) {
		pGameInstance->StopSound(CSoundMgr::MONSTER4);// 사운드	
		if (m_pTransformCom->Get_State(CTransform::STATE_POSITION).y < vPosition.y) {
			m_bJump = false;
			m_fTime = 0;
			for (int i = 0; i < 8; i++) {
				pGameInstance->Add_GameObject(LEVEL_COLOSUSS, TEXT("Layer_Icicle_Break_Effect"), TEXT("Prototype_GameObject_Icicle_Break_Effect"), this);
			}
		}
	}
	if (m_bJump == false)
	{
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
		if (true == m_bSoundStart)
		{
			pGameInstance->LetsPlaySound(L"Yeti_IcicleLand.ogg", CSoundMgr::MONSTER4, 1.f); // 사운드
			m_bSoundStart = false;
		}
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CIcicle::SetUp_RenderState()
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
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 0);
	//그릴려고 했던 이미지의 알파값을 내가 지정한 값이랑
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER/*크다면*/);
	// 이 조건을 통과한 애들만 화면에 그리겠다
	// 이코드의 경우 현재 그릴려는 알파값(REF - 0)보다 크다면(GREATER) 그린다

	// 단순히 배경을 지우는데는 알파테스팅이 아주 유용하지만 한계가 있다
	// 반투명 처리가 불가능하기 때문

	//또한 테스팅과 블렌딩은 혼합하지 않는다 -> 가능은한데 의미가 퇴색됨
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, TRUE);

	return S_OK;
}

HRESULT CIcicle::Release_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	return S_OK;
}

void CIcicle::Collision()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	_float3 vPos;
	if (true == m_bJump)
	{
		if (pGameInstance->Collision_Sphere((CCollider_Sphere*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Collider")), m_pColliderCom))
		{
			static_cast<CPlayer*>(pGameInstance->Find_Layer(LEVEL_STATIC, TEXT("Layer_Player"))->Get_Object().front())->Set_Die(true);
		}
	}
	else if (_float3(0.f, 0.f, 0.f) != (vPos = pGameInstance->Collision_SphereGuard(m_pColliderCom, (CCollider_Sphere*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Collider")))))
	{
		CTransform* pPlayerTrans = static_cast<CTransform*>(pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform")));
		pPlayerTrans->Set_State(CTransform::STATE_POSITION, vPos);
	}

	RELEASE_INSTANCE(CGameInstance);
}

CIcicle * CIcicle::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CIcicle*	pInstance = new CIcicle(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CIcicle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CIcicle::Clone(void* pArg)
{
	/* 새로운객체를 복제하여 생성한다. */
	CIcicle*	pInstance = new CIcicle(*this);


	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CIcicle");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CIcicle::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);
	Safe_Release(m_pVIBufferCollCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}