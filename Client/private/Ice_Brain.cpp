#include "stdafx.h"
#include "..\Public\Ice_Brain.h"
#include "GameInstance.h"
#include "Fire_Button.h"
#include "Camera_Player.h"

CIce_Brain::CIce_Brain(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CIce_Brain::CIce_Brain(const CIce_Brain & rhs)
	: CGameObject(rhs)
{
}

HRESULT CIce_Brain::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		MSGBOX("__super::NativeConstruct_Prototype returned E_FAIL in CIce_Brain::NativeConstruct_Prototype");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CIce_Brain::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		MSGBOX("__super::NativeConstruct returned E_FAIL in CIce_Brain::NativeConstruct");
		return E_FAIL;
	}

	/* 현재 객체에게 추가되어야할 컴포넌트들을 복제(or 참조)하여 멤버변수에 보관한다.  */
	if (FAILED(SetUp_Components()))
	{
		MSGBOX("SetUp_Components returned E_FAIL in CIce_Brain::NativeConstruct");
		return E_FAIL;
	}
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	m_IceDead = (ICEDEAD*)pArg;
	m_pTransformCom->Scaled(_float3(4.f, 4.f, 4.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(16.f, 0.0f, 20.f));
	pGameInstance->Add_GameObject(LEVEL_ICEBRAIN, TEXT("Layer_Ice_Shadow"), TEXT("Prototype_GameObject_Ice_Shadow"), this);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

_int CIce_Brain::Tick(_float fTimeDelta)
{
	if (false == m_bStart)
		return 0;

	if (m_IceDead->bIceDead == true) {
		//RELEASE_INSTANCE(CGameInstance);

		return -1;
	}

	if (0 > __super::Tick(fTimeDelta))
	{
		MSGBOX("0 > __super::Tick in CIce_Brain::Tick");
		return -1;
	}

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	m_fFrame += 90.0f * fTimeDelta; // 1초에 띄울 프레임의 갯수만큼 상수를 바꾼다

	if (m_fFrame >= 90.0f) // 만약 프레임을 다 돌았다면 처음으로 되돌린다
		m_fFrame = 0.f;
	if (m_bJumpPattern == false && m_bSilding == false)
		m_fSearchTimer++;

	if (pGameInstance->Get_DIKeyState(DIK_1)) {
		//m_IceDead->bIceDead = true;
		m_b2Phase = true;
	}

	if (Shake) {
		static_cast<CCamera_Player*>(pGameInstance->Get_Front(LEVEL_STATIC, TEXT("Layer_Camera")))->Set_Shake(false, true, false);
		ShakeTime++;
	}
	if (ShakeTime > 20) {
		Shake = false;
		ShakeTime = 0;
	}

	if (m_b2Phase == false) {
		if (m_fSearchTimer > 100) {
			CTransform* m_pPlayerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"));
			_float3 vpos = m_pPlayerTransform->Get_State(CTransform::STATE_POSITION);
			m_pTransformCom->LookAtXZ(vpos);
			m_fSearchTimer = 0;
			//m_bSilding = true;
			m_bSearch = true;
			m_iSearchCount++;
		}
		if (m_bSearch == true && m_iSearchCount > 3) {
			pGameInstance->StopSound(CSoundMgr::MONSTER3);// 사운드
			m_bJumpPattern = true;
			pGameInstance->LetsPlaySound(L"IceBrain_Jump.ogg", CSoundMgr::MONSTER3, 0.5f); // 사운드
			m_fJumpy = m_pTransformCom->Get_State(CTransform::STATE_POSITION).y;
			m_iSearchCount = 0;
		}
		else if (m_bSearch == true && m_iSearchCount <= 3 && m_bSilding == false) {
			m_bSilding = true;
		}
		_float3 MyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

		if (m_bJumpPattern == true) {
			MyPos.y = m_fJumpy + (m_fJumpPower * m_fTime - 2.8f * m_fTime * m_fTime * 0.5f);
			m_fTime += 0.025f;
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, MyPos);
			m_pTransformCom->Go_Straight(fTimeDelta);
		}
		if (m_bJumpSliding == true) {
			m_bSearch = false;
			m_pTransformCom->Go_Straight(fTimeDelta * m_fJumpSlidingSpeed);
			pGameInstance->LetsPlaySound(L"IceBrain_Impact.ogg", CSoundMgr::MONSTER1, 0.5f); // 사운드
			m_fPetternTimer++;

			if (m_fJumpSlidingSpeed > 0)
				m_fJumpSlidingSpeed -= 0.01f;
			m_fSearchTimer = 0;
			if (m_fPetternTimer > 150) {
				m_bJumpSliding = false;
				m_bSearch = false;

				m_fJumpSlidingSpeed = 1.f;
				m_fPetternTimer = 0;
				m_fSearchTimer = 0;
			}

		}

		if (m_bSilding == true && m_bJumpPattern == false && m_bJumpSliding == false) {
			m_pTransformCom->Go_Straight(fTimeDelta * m_fSlidingSpeed);
			m_fPetternTimer++;
			m_bSilding = false;
			if (m_fSlidingSpeed > 0)
				pGameInstance->LetsPlaySound(L"IceBrain_Move.ogg", CSoundMgr::MONSTER1, 0.5f); // 사운드
				m_fSlidingSpeed -= 0.01f;
			m_fSearchTimer = 0;
			if (m_fPetternTimer > 250) {
				pGameInstance->StopSound(CSoundMgr::MONSTER1);// 사운드
				m_bSilding = false;
				m_bSearch = false;

				m_fSlidingSpeed = 3.f;
				m_fPetternTimer = 0;
			}
		}
	}
	if (m_b2Phase == true) {
		m_bHit = true;
		pGameInstance->StopSound(CSoundMgr::MONSTER1);// 사운드
		if (m_fScale > 1.f)
			Hit();
		if (m_fScale <= 1.f)
		{
			m_IceDead->bIceDead = true;
			m_bDead = true;

		}
		pGameInstance->LetsPlaySound(L"IceBrain_Melt.ogg", CSoundMgr::MONSTER5, 0.5f); // 사운드
	}
	m_pTransformCom->Scaled(_float3(m_fScale, m_fScale, m_fScale));

	RELEASE_INSTANCE(CGameInstance);


	return _int();

}


_int CIce_Brain::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
	{
		MSGBOX("0 > __super::LateTick in CIce_Brain::LateTick");
		return -1;
	}

	if (nullptr == m_pRendererCom)
	{
		MSGBOX("m_pRendererCom is nullptr in CIce_Brain::LateTick");
		return -1;
	}

	if (FAILED(SetUp_OnTerrain()))
	{
		MSGBOX("SetUp_OnTerrain returned E_FAIL in CIce_Brain::LateTick");
		return -1;
	}

	CCollider_Sphere::SPHEREDESC SphereDesc;
	SphereDesc.ObjectMatrix = m_pTransformCom->Get_WorldMatrix();
	SphereDesc.fRadius = 0.5f;
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
	m_fAlphaZ = vPos.z - 2.f;
	///////////////////////////////////

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHA, this);
	Collision_Rect();
	return _int();
}

HRESULT CIce_Brain::Render()
{
	if (nullptr == m_pVIBufferCom)
	{
		MSGBOX("m_pVIBufferCom is nullptr in CIce_Brain::Render")
			return E_FAIL;
	}

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
	{
		MSGBOX("m_pTransformCom->Bind_OnGraphicDevice returned E_FAIL in CIce_Brain::Render");
		return E_FAIL;
	}

	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice()))
	{
		MSGBOX("m_pTextureCom->Bind_OnGraphicDevice in CIce_Brain::Render");
		return E_FAIL;
	}


	if (FAILED(SetUp_RenderState()))
	{
		MSGBOX("SetUp_RenderState returned E_FAIL in CIce_Brain::Render");
		return E_FAIL;
	}

	m_pVIBufferCom->Render();

	if (FAILED(Release_RenderState()))
	{
		MSGBOX("Release_RenderState returned E_FAIL in CIce_Brain::Render");
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

HRESULT CIce_Brain::Hit()
{
	m_fIceSmallCount = 0.03f;
	m_fScale -= m_fIceSmallCount;

	return S_OK;
}

HRESULT CIce_Brain::SetUp_Components()
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CIce_Brain::SetUp_Components(Transform)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CIce_Brain::SetUp_Components(Renderer)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CIce_Brain::SetUp_Components(VIBuffer_Cube)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_ICEBRAIN, TEXT("Prototype_Component_Texture_Ice"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CIce_Brain::SetUp_Components(Texture)");
		return E_FAIL;
	}


	CCollider_Sphere::SPHEREDESC SphereDesc;
	SphereDesc.ObjectMatrix = m_pTransformCom->Get_WorldMatrix();
	SphereDesc.fRadius = 0.5f;
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

HRESULT CIce_Brain::SetUp_OnTerrain()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	CVIBuffer_Terrain* m_pVIBuffer_Terrain = (CVIBuffer_Terrain*)pGameInstance->Get_Component(g_eCurrentLevel, TEXT("Layer_Terrain"), TEXT("Com_VIBuffer"));
	if (m_pVIBuffer_Terrain == nullptr)
	{
		MSGBOX("pVIBuffer_Terrain is nullptr in CIce_Brain::SetUp_OnTerrain");
		return E_FAIL;
	}
	//플레이어의 위치를 얻어와서
	_float3 vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	vPosition.y = m_pVIBuffer_Terrain->Compute_Y(vPosition) + 2.001f;
	if (m_bJumpPattern == true) {
		if (m_pTransformCom->Get_State(CTransform::STATE_POSITION).y < vPosition.y) {
			Shake = true;
			m_bJumpPattern = false;
			m_bJumpSliding = true;
			m_fTime = 0;
			m_fPetternTimer = 70;
		}
	}

	if (m_bJump == true) {
		if (m_pTransformCom->Get_State(CTransform::STATE_POSITION).y < vPosition.y) {
			m_bJump = false;
			m_fTime = 0;
		}
	}
	if (m_bJump == false && m_bJumpPattern == false)
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CIce_Brain::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
	{
		MSGBOX("m_pGraphic_Device is nullptr in CIce_Brain::SetUp_RenderState");
		return E_FAIL;
	}
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	return S_OK;
}

HRESULT CIce_Brain::Release_RenderState()
{
	if (nullptr == m_pGraphic_Device)
	{
		MSGBOX("m_pGraphic_Device is nullptr in CIce_Brain::Release_RenderState");
		return E_FAIL;
	}
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);

	//m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	return S_OK;
}

void CIce_Brain::Collision_Rect()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	for (_uint i = 0; i < 4; i++)
	{
		CCollider_Rect* pColWall = (CCollider_Rect*)pGameInstance->Get_Component(g_eCurrentLevel, TEXT("Layer_Wall"), TEXT("Com_Collider"), i);
		_float3 vPos = pGameInstance->Collision_SphereToRect(pColWall, m_pColliderCom);
		if (vPos != _float3(0.f, 0.f, 0.f))
		{
			pGameInstance->StopSound(CSoundMgr::MONSTER3);// 사운드

			Shake = true;
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

			pGameInstance->LetsPlaySound(L"IceBrain_Impact.ogg", CSoundMgr::MONSTER3, 0.5f); // 사운드
		}
	}

	for (size_t i = 0; i < 4; i++)
	{
		if (true == pGameInstance->Collision_Sphere((CCollider_Sphere*)pGameInstance->Get_Component(LEVEL_ICEBRAIN, TEXT("Layer_FireButton"), TEXT("Com_Collider"), (_uint)i), m_pColliderCom))
		{
			static_cast<CFire_Button*>(pGameInstance->Get_Front(LEVEL_ICEBRAIN, TEXT("Layer_FireButton")))->Set_OnButton(true);
			pGameInstance->LetsPlaySound(L"IceBrain_ButtonPush.ogg", CSoundMgr::MONSTER2, 0.5f); // 사운드
			break;
		}
		if (i == 3)
		{
			pGameInstance->StopSound(CSoundMgr::MONSTER2);// 사운드
			static_cast<CFire_Button*>(pGameInstance->Get_Front(LEVEL_ICEBRAIN, TEXT("Layer_FireButton")))->Set_OnButton(false);
		}
	}



	if (m_bStart == false)
		if ((nullptr != pGameInstance->Find_Layer(LEVEL_STATIC, TEXT("Layer_Arrow")) && 0 < pGameInstance->Get_Size(LEVEL_STATIC, TEXT("Layer_Arrow"))) && true == pGameInstance->Collision_Sphere((CCollider_Sphere*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Arrow"), TEXT("Com_Collider")), m_pColliderCom))
		{
			pGameInstance->StopSound(CSoundMgr::MONSTER2);// 사운드
			pGameInstance->PlayBGM(L"IceBrain_map.ogg", 0.3f);// 사운드
			m_bStart = true;
			pGameInstance->LetsPlaySound(L"IceBrain_Melt.ogg", CSoundMgr::MONSTER2, 0.5f); // 사운드
		}

	RELEASE_INSTANCE(CGameInstance);
}

CIce_Brain * CIce_Brain::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CIce_Brain*	pInstance = new CIce_Brain(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create Ice_Brain in CIce_Brain::Create");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CIce_Brain::Clone(void* pArg)
{
	/* 새로운객체를 복제하여 생성한다. */
	CIce_Brain*	pInstance = new CIce_Brain(*this);


	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create Ice_Brain in CIce_Brain::Clone");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CIce_Brain::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);
	Safe_Release(m_pVIBufferCollCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}