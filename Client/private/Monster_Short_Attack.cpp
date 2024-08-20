#include "stdafx.h"
#include "..\Public\Monster_Short_Attack.h"
#include "GameInstance.h"
#include "Player.h"

CMonster_Short_Attack::CMonster_Short_Attack(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CMonster_Short_Attack::CMonster_Short_Attack(const CMonster_Short_Attack & rhs)
	: CGameObject(rhs)
{
}

HRESULT CMonster_Short_Attack::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster_Short_Attack::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	/* 현재 객체에게 추가되어야할 컴포넌트들을 복제(or 참조)하여 멤버변수에 보관한다.  */
	if (FAILED(SetUp_Components()))
		return E_FAIL;
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	CTransform* m_pPlayerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"));
	//CTransform* m_pMonsterTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_YETI, TEXT("Layer_Short_Monster"), TEXT("Com_Transform"));
	m_pmonster = (CMonster_Short_Range*)pArg;
	CTransform* m_pMonsterTransform = m_pmonster->Get_TransForm();
	_float3 MonPos = m_pMonsterTransform->Get_State(CTransform::STATE_POSITION);
	_float3 a = m_pMonsterTransform->Get_State(CTransform::STATE_LOOK);
	//m_pTransformCom->Set_State(CTransform::STATE_LOOK, a);
	MonPos += m_pMonsterTransform->Get_State(CTransform::STATE_LOOK) / 2;

	m_pTransformCom->Scaled(_float3(m_fScale, m_fScale, m_fScale));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, MonPos);
	//m_pTransformCom->LookAt(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION));

	//pGameInstance->Add_GameObject(LEVEL_YETI, TEXT("Layer_SnowBall_Shadow"), TEXT("Prototype_GameObject_SnowBall_Shadow"), this);
	m_tFrame.iFrameStartY = 0.f;
	m_tFrame.iFrameCurrentX = 0.f;
	m_tFrame.iFrameStartX = 0.f;
	m_tFrame.iFrameEndX = 3.f;
	m_tFrame.iFrameHeight = 200.f;
	m_tFrame.iFrameWidth = 200.f;
	m_tFrame.iTexHeight = 200.f;
	m_tFrame.iTexWidth = 800.f;
	m_fSpriteSpeed = 1;

	//m_pTransformCom->Rotation(_float3(0,0,1), D3DXToRadian(90.f));

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

_int CMonster_Short_Attack::Tick(_float fTimeDelta)
{

	if (0 > __super::Tick(fTimeDelta))
		return -1;
	if (true == m_bObjectDead)
		return -1;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


	//m_fFrame += 90.0f * fTimeDelta; // 1초에 띄울 프레임의 갯수만큼 상수를 바꾼다

	//if (m_fFrame >= 90.0f) // 만약 프레임을 다 돌았다면 처음으로 되돌린다
	//	m_fFrame = 0.f;


	/*_float3 MyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float3 vpos = m_pPlayerTransform->Get_State(CTransform::STATE_POSITION);*/

	//m_pTransformCom->Go_Straight(fTimeDelta * 2.f);
	m_iAttackTimer++;
	if (m_iAttackTimer > 30) {
		m_bObjectDead = true;
	}
	if (m_tFrame.iFrameCurrentX >= 1.f && 	m_tFrame.iFrameCurrentX <= 3.f)
		Collision();

	Go_Frame(m_fSpriteSpeed, fTimeDelta);

	RELEASE_INSTANCE(CGameInstance);
	/*if (0 > Collision())
	m_bObjectDead = true;
	*/
	return _int();
}

_int CMonster_Short_Attack::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
	{
		MSGBOX("0 > __super::LateTick in CMonster_Short_Attack::LateTick");
		return -1;
	}

	if (nullptr == m_pRendererCom)
	{
		MSGBOX("m_pRendererCom is nullptr in CMonster_Short_Attack::LateTick");
		return -1;
	}

	/*if (FAILED(SetUp_OnTerrain()))
	{
	MSGBOX("SetUp_OnTerrain returned E_FAIL in CMonster_Short_Attack::LateTick");
	return -1;
	}
	*/
	/*if (FAILED(SetUp_OnTerrain()))
	return -1;*/

	CCollider_Sphere::SPHEREDESC SphereDesc;
	SphereDesc.ObjectMatrix = m_pTransformCom->Get_WorldMatrix();
	SphereDesc.fRadius = 0.3f;
	SphereDesc.fDist_From_Obj = _float3(0.f, 0.f, 0.f);
	m_pColliderCom->Set_SphereDesc(SphereDesc);
	m_pColliderCom->Tick();

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CTransform* pCameraTrans = (CTransform*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Camera"), TEXT("Com_Transform"));
	_float3 vCameraPos = pCameraTrans->Get_State(CTransform::STATE_POSITION);
	m_pTransformCom->MatchIt_XZ(vCameraPos);

	RELEASE_INSTANCE(CGameInstance);
	m_pTransformCom->Scaled(_float3(m_fScale, m_fScale, m_fScale));

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

	return _int();
}

HRESULT CMonster_Short_Attack::Render()
{
	if (nullptr == m_pVIBufferCom)
	{
		MSGBOX("m_pVIBufferCom is nullptr in CMonster_Short_Attack::Render")
			return E_FAIL;
	}

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
	{
		MSGBOX("m_pTransformCom->Bind_OnGraphicDevice returned E_FAIL in CMonster_Short_Attack::Render");
		return E_FAIL;
	}
	if (FAILED(Go_Sprites()))
	{
		MSGBOX("Go_Sprites returned E_FAIL in CYeti::Tick");
		return E_FAIL;
	}
	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice()))
	{
		MSGBOX("m_pTextureCom->Bind_OnGraphicDevice in CMonster_Short_Attack::Render");
		return E_FAIL;
	}


	if (FAILED(SetUp_RenderState()))
	{
		MSGBOX("SetUp_RenderState returned E_FAIL in CMonster_Short_Attack::Render");
		return E_FAIL;
	}

	m_pVIBufferCom->Render();

	if (FAILED(Release_RenderState()))
	{
		MSGBOX("Release_RenderState returned E_FAIL in CMonster_Short_Attack::Render");
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

HRESULT CMonster_Short_Attack::SetUp_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CMonster_Short_Attack::SetUp_Components(Transform)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CMonster_Short_Attack::SetUp_Components(Renderer)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect_Z"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CMonster_Short_Attack::SetUp_Components(VIBuffer_Rect)");
		return E_FAIL;
	}

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(g_eCurrentLevel, TEXT("Prototype_Component_Texture_Monster_SWORD_Bullet"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	CCollider_Sphere::SPHEREDESC SphereDesc;
	SphereDesc.ObjectMatrix = m_pTransformCom->Get_WorldMatrix();
	SphereDesc.fRadius = 0.3f;
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

HRESULT CMonster_Short_Attack::SetUp_OnTerrain()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	CVIBuffer_Terrain* m_pVIBuffer_Terrain = (CVIBuffer_Terrain*)pGameInstance->Get_Component(g_eCurrentLevel, TEXT("Layer_Terrain"), TEXT("Com_VIBuffer"));


	if (m_pVIBuffer_Terrain == nullptr)
		return E_FAIL;
	//플레이어의 위치를 얻어와서
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

HRESULT CMonster_Short_Attack::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);//알파블렌딩을 하겠다
	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);//색을 더한다
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//그릴려고 하는 픽셀의 색상을 섞을때 소스 칼라의 알파를 섞겠다
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	/*
	_float4		vSourColor, vDestColor;
	(vSourColor.rgb) * vSourColor.a + (vDestColor.rgb) * (1.f - vSourColor.a);
	소스컬러.RGB * 소스컬러.알파 + 데스트컬라.RGB * 1.f - 소스컬러.알파
	알파는 RGB의 0~255를 0에서 1사이로 압축한것
	플레이어를 그리려하는데 이미 그위치에 그려져 있던 색이 데스트칼라
	*/

	//다른 방법 - 알파 테스트
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);// 알파테스트를 하겠다
	//															   //해당조건을 통해서 비교를 한 다음
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 0);
	////그릴려고 했던 이미지의 알파값을 내가 지정한 값이랑
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER/*크다면*/);
	// 이 조건을 통과한 애들만 화면에 그리겠다
	// 이코드의 경우 현재 그릴려는 알파값(REF - 0)보다 크다면(GREATER) 그린다

	// 단순히 배경을 지우는데는 알파테스팅이 아주 유용하지만 한계가 있다
	// 반투명 처리가 불가능하기 때문

	//또한 테스팅과 블렌딩은 혼합하지 않는다 -> 가능은한데 의미가 퇴색됨
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, TRUE);

	return S_OK;
}

HRESULT CMonster_Short_Attack::Release_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);


	return S_OK;
}

HRESULT CMonster_Short_Attack::Go_Sprites()
{
	if (FAILED(m_pVIBufferCom->Go_Sprites(m_tFrame)))
	{
		MSGBOX("m_pVIBufferCom->Go_Sprites returned E_FAIL in CYeti::Go_Sprites");
		return E_FAIL;
	}
	return S_OK;
}

_int CMonster_Short_Attack::Collision()
{
	if (LEVEL_LOADING == g_eCurrentLevel)
		return 0;

	if (m_pTransformCom->Get_State(CTransform::STATE_POSITION).y < 0.5f)
		m_bObjectDead = true;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	for (_uint i = 0; i < 4; i++)
	{
		_float3 vPos = pGameInstance->Collision_SphereToRect((CCollider_Rect*)pGameInstance->Get_Component(g_eCurrentLevel, TEXT("Layer_Wall"), TEXT("Com_Collider"), i), m_pColliderCom);
		if (vPos != _float3(0.f, 0.f, 0.f))
		{
			RELEASE_INSTANCE(CGameInstance);
			return -1;
		}
	}

	if (true == pGameInstance->Collision_Sphere((CCollider_Sphere*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Collider")), m_pColliderCom))
	{
		CPlayer* pPlayer = (CPlayer*)pGameInstance->Get_Front(LEVEL_STATIC, TEXT("Layer_Player"));
		//pPlayer->Set_State(CPlayer::STATE_KNOCKBACK);
		//pPlayer->Set_KnockBackDir(m_pTransformCom->Get_State(CTransform::STATE_LOOK));
		pPlayer->Set_Die(true);
		RELEASE_INSTANCE(CGameInstance);
		return -1;
	}


	RELEASE_INSTANCE(CGameInstance);


	return 0;
}

CMonster_Short_Attack * CMonster_Short_Attack::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMonster_Short_Attack*	pInstance = new CMonster_Short_Attack(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CMonster_Short_Attack");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CMonster_Short_Attack::Clone(void* pArg)
{
	/* 새로운객체를 복제하여 생성한다. */
	CMonster_Short_Attack*	pInstance = new CMonster_Short_Attack(*this);


	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CMonster_Short_Attack");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMonster_Short_Attack::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);
	Safe_Release(m_pVIBufferCollCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}