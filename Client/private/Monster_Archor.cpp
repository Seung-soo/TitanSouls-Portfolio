#include "stdafx.h"
#include "..\Public\Monster_Archor.h"
#include "GameInstance.h"

CMonster_Archor::CMonster_Archor(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CMonster_Archor::CMonster_Archor(const CMonster_Archor & rhs)
	: CGameObject(rhs)
{
}

HRESULT CMonster_Archor::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		MSGBOX("__super::NativeConstruct_Prototype returned E_FAIL in CMonster_Archor::NativeConstruct_Prototype");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CMonster_Archor::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		MSGBOX("__super::NativeConstruct returned E_FAIL in CMonster_Archor::NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(SetUp_Components()))
	{
		MSGBOX("SetUp_Components returned E_FAIL in CMonster_Archor::NativeConstruct");
		return E_FAIL;
	}
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);


	_float3* a = (_float3*)pArg;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, *a);

	m_pTransformCom->Scaled(_float3(2, 2, 2));

	m_tFrame.iFrameStartY = 0.f;
	m_tFrame.iFrameCurrentX = 0.f;
	m_tFrame.iFrameStartX = 0.f;
	m_tFrame.iFrameEndX = 0.f;
	m_tFrame.iFrameHeight = 230.f;
	m_tFrame.iFrameWidth = 170.f;
	m_tFrame.iTexHeight = 1150.f;
	m_tFrame.iTexWidth = 680.f;

	m_estate = STATE_IDLE;


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}



_int CMonster_Archor::Tick(_float fTimeDelta)
{

	if (0 > __super::Tick(fTimeDelta))
		return -1;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	CTransform* m_pPlayerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"));

	_float3 MyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float3 vpos = m_pPlayerTransform->Get_State(CTransform::STATE_POSITION);
	//if(m_iSmallRank > 1)
	_float3 vdir = MyPos - vpos;
	_float lenght = D3DXVec3Length(&vdir);

	switch (m_estate)
	{
	case STATE_IDLE:
		//m_bSetState = true;
		m_fSpriteSpeed = 1;
		m_tFrame.iFrameStartY = 0.f;
		m_tFrame.iFrameStartX = 0.f;
		m_tFrame.iFrameEndX = 0.f;
		//	Go_Frame(m_fSpriteSpeed, fTimeDelta);
		break;
	case STATE_WALK:
		//m_bSetState = true;

		m_fSpriteSpeed = 1;
		m_tFrame.iFrameStartY = 1.f;
		m_tFrame.iFrameStartX = 0.f;
		//m_tFrame.iFrameCurrentX = m_tFrame.iFrameStartX;
		m_tFrame.iFrameEndX = 4.f;
		//Go_Frame(m_fSpriteSpeed, fTimeDelta);
		break;
	case STATE_ATTACK:
		//m_bSetState = true;
		m_fSpriteSpeed = 1.f;
		m_tFrame.iFrameStartY = 2.f;
		m_tFrame.iFrameStartX = 0.f;
		m_tFrame.iFrameEndX = 4.f;
		break;
	case STATE_DEAD:
		m_fSpriteSpeed = 1.f;
		m_tFrame.iFrameStartY = 4.f;
		m_tFrame.iFrameStartX = 0.f;
		m_tFrame.iFrameEndX = 3.f;
		break;
	}

	if (m_estate == STATE_DEAD) {
		if (m_tFrame.iFrameCurrentX >= m_tFrame.iFrameEndX) {
			m_bDead = true;
		}
	}

	if (m_bDead)
	{
		++g_iKill;
		RELEASE_INSTANCE(CGameInstance);
		return -1;
	}

	if (lenght < 10.f) {
		m_bAttack = true;
	}
	else if (m_iAttackTimer > 500 && lenght > 10.f && m_bAttack == true && m_estate != STATE_DEAD) {
		m_bAttack = false;
		m_iAttackTimer = 0;
		m_esavestate = m_estate;
		m_estate = STATE_IDLE;
		if (m_esavestate != m_estate) {
			m_tFrame.iFrameCurrentX = m_tFrame.iFrameStartX;
		}
	}


	if (m_bAttack == true) {
		m_iAttackTimer++;
		m_iBulletTimer++;
		m_pTransformCom->LookAtXZ(vpos);
		if (lenght < 10.f && m_bAttacking == false) {
			m_pTransformCom->Go_BackWard(fTimeDelta);
			m_esavestate = m_estate;
			m_estate = STATE_WALK;
			if (m_esavestate != m_estate) {
				m_tFrame.iFrameCurrentX = m_tFrame.iFrameStartX;
			}
		}
		if (m_iBulletTimer > 60) {
			m_esavestate = m_estate;
			m_estate = STATE_ATTACK;
			if (m_esavestate != m_estate) {
				m_tFrame.iFrameCurrentX = m_tFrame.iFrameStartX;
			}
			//pGameInstance->Add_GameObject(LEVEL_YETI, TEXT("Layer_Monster_Bullet"), TEXT("Prototype_GameObject_Monster_Archor_Buller"));
			m_bAttacking = true;
			m_iBulletTimer = 0;
		}



	}
	if (m_estate == STATE_ATTACK && m_bShotBullet == false) {
		if (m_tFrame.iFrameCurrentX >= m_tFrame.iFrameEndX) {
			pGameInstance->Add_GameObject(g_eCurrentLevel, TEXT("Layer_Monster_Bullet"), TEXT("Prototype_GameObject_Monster_Archor_Buller"), this);
			m_bAttacking = false;
			m_bShotBullet = true;
		}
	}
	if (m_bShotBullet == true) {
		m_iShotWait++;
		if (m_iShotWait > 60) {
			m_bShotBullet = false;
			m_iShotWait = 0;
		}
	}
	m_fSpriteSpeed = 1;
	Collision_Rect();


	if (m_estate != STATE_DEAD)
		Go_Frame(m_fSpriteSpeed, fTimeDelta);
	else
		Go_FrameEnd(m_fSpriteSpeed, fTimeDelta);



	RELEASE_INSTANCE(CGameInstance);

	return _int();

}

_int CMonster_Archor::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
	{
		MSGBOX("0 > __super::LateTick in CMonster_Archor::LateTick");
		return -1;
	}

	if (nullptr == m_pRendererCom)
	{
		MSGBOX("m_pRendererCom is nullptr in CMonster_Archor::LateTick");
		return -1;
	}

	if (FAILED(SetUp_OnTerrain()))
	{
		MSGBOX("SetUp_OnTerrain returned E_FAIL in CMonster_Archor::LateTick");
		return -1;
	}
	m_pTransformCom->Scaled(_float3(2, 2, 2));

	Go_Frame(m_fSpriteSpeed, fTimeDelta);


	CCollider_Sphere::SPHEREDESC SphereDesc;
	SphereDesc.ObjectMatrix = m_pTransformCom->Get_WorldMatrix();
	SphereDesc.fRadius = 0.5f;
	SphereDesc.fDist_From_Obj = _float3(0.f, 0.f, 0.f);
	m_pColliderCom->Set_SphereDesc(SphereDesc);
	m_pColliderCom->Tick();


	_float4x4		ViewMatrix; //뷰행렬을 저장할 변수
								//카메라 뷰스페이스의 행렬을 구해서 저장한다
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	//월드변환행렬로 만든다
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);
	//D3DXMatrixInverse
	//라업룩에 적용해준다
	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, (*(_float3*)&ViewMatrix.m[0][0]) * m_pTransformCom->Get_Scale().x);
	m_pTransformCom->Set_State(CTransform::STATE_UP, (*(_float3*)&ViewMatrix.m[1][0]) * m_pTransformCom->Get_Scale().y);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, (*(_float3*)&ViewMatrix.m[2][0]) * m_pTransformCom->Get_Scale().z);





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

HRESULT CMonster_Archor::Render()
{
	if (nullptr == m_pVIBufferCom)
	{
		MSGBOX("m_pVIBufferCom is nullptr in CMonster_Archor::Render")
			return E_FAIL;
	}

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
	{
		MSGBOX("m_pTransformCom->Bind_OnGraphicDevice returned E_FAIL in CMonster_Archor::Render");
		return E_FAIL;
	}

	if (FAILED(Go_Sprites()))
	{
		MSGBOX("Go_Sprites returned E_FAIL in CYeti::Tick");
		return E_FAIL;
	}
	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice()))
	{
		MSGBOX("m_pTextureCom->Bind_OnGraphicDevice in CMonster_Archor::Render");
		return E_FAIL;
	}

	////////////////////
	if (FAILED(SetUp_RenderState()))
	{
		MSGBOX("SetUp_RenderState returned E_FAIL in CMonster_Archor::Render");
		return E_FAIL;
	}

	m_pVIBufferCom->Render(); // 이렇게 순서대로 랜더의 설정을 할 경우
							  // 특정한 부분에만 블렌딩을 적용하거나 할 수 있다
	if (FAILED(Release_RenderState()))
	{
		MSGBOX("Release_RenderState returned E_FAIL in CMonster_Archor::Render");
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

HRESULT CMonster_Archor::SetUp_Components()
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CMonster_Archor::SetUp_Components(Transform)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CMonster_Archor::SetUp_Components(Renderer)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CMonster_Archor::SetUp_Components(VIBuffer_Rect)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(g_eCurrentLevel, TEXT("Prototype_Component_Texture_Monster_WIZARD"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CMonster_Archor::SetUp_Components(Texture)");
		return E_FAIL;
	}

	CCollider_Sphere::SPHEREDESC SphereDesc;
	SphereDesc.ObjectMatrix = m_pTransformCom->Get_WorldMatrix();
	SphereDesc.fRadius = 0.5f;
	SphereDesc.fDist_From_Obj = _float3(0.f, 0.f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"), TEXT("Com_Collider"), (CComponent**)&m_pColliderCom, &SphereDesc)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CMonster_Archor::SetUp_Components(Collider_Sphere)");
		return E_FAIL;
	}

	//#ifdef _DEBUG
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"), TEXT("Com_VIBuffer_Coll"), (CComponent**)&m_pVIBufferCollCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CMonster_Archor::SetUp_Components(VIBuffer_Cube)");
		return E_FAIL;
	}
	//#endif // DEBUG

	return S_OK;
}

HRESULT CMonster_Archor::SetUp_OnTerrain()
{
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
		vPosition.y = pVIBuffer_Terrain->Compute_Y_Big(vLocalPos) +  m_fScale * 0.5f;
	else
		vPosition.y = pVIBuffer_Terrain->Compute_Y(vLocalPos) + m_fScale * 0.5f;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CMonster_Archor::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
	{
		MSGBOX("m_pGraphic_Device is nullptr in CMonster_Archor::SetUp_RenderState");
		return E_FAIL;
	}

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);// 알파테스트를 하겠다
																   //해당조건을 통해서 비교를 한 다음
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 150);
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

HRESULT CMonster_Archor::Release_RenderState()
{
	if (nullptr == m_pGraphic_Device)
	{
		MSGBOX("m_pGraphic_Device is nullptr in CMonster_Archor::Release_RenderState");
		return E_FAIL;
	}
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

	return S_OK;
}

HRESULT CMonster_Archor::Go_Sprites()
{
	if (FAILED(m_pVIBufferCom->Go_Sprites(m_tFrame)))
	{
		MSGBOX("m_pVIBufferCom->Go_Sprites returned E_FAIL in CYeti::Go_Sprites");
		return E_FAIL;
	}
	return S_OK;
}


void CMonster_Archor::Collision_Rect()
{
	if (LEVEL_LOADING == g_eCurrentLevel)
		return;
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	for (_uint i = 0; i < pGameInstance->Get_Size(g_eCurrentLevel, TEXT("Layer_Wall")); i++)
	{
		_float3 vPos = pGameInstance->Collision_SphereToRect((CCollider_Rect*)pGameInstance->Get_Component(g_eCurrentLevel, TEXT("Layer_Wall"), TEXT("Com_Collider"), i), m_pColliderCom);
		if (vPos != _float3(0.f, 0.f, 0.f))
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	}

	if (m_iCollDelay >= 0)
		--m_iCollDelay;

	if (m_iCollDelay < 0)
		if ((nullptr != pGameInstance->Find_Layer(LEVEL_STATIC, TEXT("Layer_Arrow")) && 0 < pGameInstance->Get_Size(LEVEL_STATIC, TEXT("Layer_Arrow"))) && true == pGameInstance->Collision_Sphere((CCollider_Sphere*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Arrow"), TEXT("Com_Collider")), m_pColliderCom)) {

			pGameInstance->StopSound(CSoundMgr::MONSTER2); // 사운드
			pGameInstance->LetsPlaySound(L"Monster_Hit.ogg", CSoundMgr::MONSTER2, 1.0f); // 사운드
			pGameInstance->StopSound(CSoundMgr::MONSTER3); // 사운드
			pGameInstance->LetsPlaySound(L"Monster_Dead.ogg", CSoundMgr::MONSTER3, 1.0f); // 사운드

			m_iCollDelay = 60;
			m_esavestate = m_estate;
			m_estate = STATE_DEAD;
			m_tFrame.iFrameCurrentX = 0.f;
			RELEASE_INSTANCE(CGameInstance);
			return;
		}


	RELEASE_INSTANCE(CGameInstance);
}


CMonster_Archor * CMonster_Archor::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMonster_Archor*	pInstance = new CMonster_Archor(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create Slime in CMonster_Archor::Create");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CMonster_Archor::Clone(void* pArg)
{
	/* 새로운객체를 복제하여 생성한다. */
	CMonster_Archor*	pInstance = new CMonster_Archor(*this);


	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create Slime in CMonster_Archor::Clone");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMonster_Archor::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCollCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}