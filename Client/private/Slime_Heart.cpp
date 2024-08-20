#include "stdafx.h"
#include "..\Public\Slime_Heart.h"
#include "GameInstance.h"
#include "Camera_Player.h"
#include "DarkScreen.h"

CSlime_Heart::CSlime_Heart(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CSlime_Heart::CSlime_Heart(const CSlime_Heart & rhs)
	: CGameObject(rhs)
{
}

HRESULT CSlime_Heart::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		MSGBOX("__super::NativeConstruct_Prototype returned E_FAIL in CSlime_Heart::NativeConstruct_Prototype");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CSlime_Heart::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		MSGBOX("__super::NativeConstruct returned E_FAIL in CSlime_Heart::NativeConstruct");
		return E_FAIL;
	}

	// 컴포넌트 세팅
	if (FAILED(SetUp_Components()))
	{
		MSGBOX("SetUp_Components returned E_FAIL in CSlime_Heart::NativeConstruct");
		return E_FAIL;
	}
	m_SlimeDead = (SLIMEDEAD*)pArg;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	m_pCamera = static_cast<CCamera_Player*>(pGameInstance->Get_Front(LEVEL_STATIC, TEXT("Layer_Camera")));

	m_pTransformCom->Scaled(_float3(m_fScale, m_fScale, m_fScale));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(20.f, 0.0f, 20.01f));
	//m_pBossDead = (_bool*)pArg;
	m_tFrame.iFrameStartY = 0.f;
	m_tFrame.iFrameCurrentX = 0.f;
	m_tFrame.iFrameStartX = 0.f;
	m_tFrame.iFrameEndX = 4.f;
	m_tFrame.iFrameHeight = 32.f;
	m_tFrame.iFrameWidth = 32.f;
	m_tFrame.iTexHeight = 32.f;
	m_tFrame.iTexWidth = 160.f;
	//m_fSpriteSpeed = 1;

	m_fSpriteSpeed = 2;

	pGameInstance->Add_GameObject(LEVEL_SLIME, TEXT("Layer_Slime_Heart_Shadow"), TEXT("Prototype_GameObject_Slime_Heart_Shadow"), this);


	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}


_int CSlime_Heart::Tick(_float fTimeDelta)
{

	if (0 > __super::Tick(fTimeDelta))
	{
		MSGBOX("0 > __super::Tick in CSlime_Heart::Tick");
		return -1;
	}

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	//m_fFrame += 90.0f * fTimeDelta; // 1초에 띄울 프레임의 갯수만큼 상수를 바꾼다

	//if (m_fFrame >= 90.0f) // 만약 프레임을 다 돌았다면 처음으로 되돌린다
	//	m_fFrame = 0.f;
	//m_fFrame += 90.0f * fTimeDelta; // 1초에 띄울 프레임의 갯수만큼 상수를 바꾼다

	//if (m_fFrame >= 90.0f) // 만약 프레임을 다 돌았다면 처음으로 되돌린다
	//	m_fFrame = 0.f;
	//if()
	if (m_bDead) {
		// e를 누르면 플레이어가 쳐다보게 만들고 (룩턴)
		// 카메라 떨리게 하고 
		// 2초간 누르면 영혼 방출

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

		m_bObjectDead = true;
		m_SlimeDead->bHeartDead = true;
		RELEASE_INSTANCE(CGameInstance);
		return 0;

	}

	if (m_SlimeDead->bBossSlimeDead == false) {
		CTransform* m_pSlimeTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_SLIME, TEXT("Layer_Slime_Boss"), TEXT("Com_Transform"));
		_float3 a = m_pSlimeTransform->Get_State(CTransform::STATE_LOOK);
		_float3 Slimepos = m_pSlimeTransform->Get_State(CTransform::STATE_POSITION);
		CTransform* m_pPlayerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"));
		//	_float3 mypos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_float3 vpos = m_pPlayerTransform->Get_State(CTransform::STATE_POSITION);
		if (Slimepos.z < vpos.z) {
			Slimepos.z -= a.z * 0.01f;
		}
		else if (Slimepos.z > vpos.z) {
			Slimepos.z += a.z * 0.01f;
		}
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, Slimepos);
	}
	CTransform* m_pPlayerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"));
	_float3 vpos = m_pPlayerTransform->Get_State(CTransform::STATE_POSITION);
	if (m_SlimeDead->bBossSlimeDead == true) {
		if (m_bfirstJump == false) {
			m_bJump = true;
			m_bfirstJump = true;
			m_fJumpy = m_pTransformCom->Get_State(CTransform::STATE_POSITION).y;

		}
		if (m_bJump == false) {
			Go_FrameEnd(m_fSpriteSpeed, fTimeDelta);
			m_fJumpy = m_pTransformCom->Get_State(CTransform::STATE_POSITION).y;
			if (m_tFrame.iFrameCurrentX == m_tFrame.iFrameEndX) {
				m_bJump = true;
				m_tFrame.iFrameCurrentX = 0;
				int b = rand() % 2;

				switch (rand() % 4)
				{
				case 0:
					m_pTransformCom->LookAtXZ(_float3(vpos.x - b, 0, vpos.z + b));
					break;
				case 1:
					m_pTransformCom->LookAtXZ(_float3(vpos.x + b, 0, vpos.z + b));
					break;
				case 2:
					m_pTransformCom->LookAtXZ(_float3(vpos.x - b, 0, vpos.z - b));
					break;
				case 3:
					m_pTransformCom->LookAtXZ(_float3(vpos.x + b, 0, vpos.z - b));
					break;
				}
				m_pTransformCom->LookAtXZ(vpos);

			}
		}

		_float3 MyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		//vpos.y = MyPos.y;
		if (m_bJump == true) {
			MyPos.y = m_fJumpy + (2.f * m_fTime - 2.8f * m_fTime * m_fTime * 0.5f);
			m_fTime += 0.025f;
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, MyPos);
			m_pTransformCom->Go_BackWard(fTimeDelta * 0.2f);

		}
		Collision();

		if (m_bWaterCreat == true) {
			if (FAILED(pGameInstance->Add_GameObject(LEVEL_SLIME, TEXT("Layer_Heart_Water_Mini"), TEXT("Prototype_GameObject_Heart_water_mini"), this)))
			{
				MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CSlime_Boss::SetUp_OnTerrain");
				return E_FAIL;
			}
			pGameInstance->LetsPlaySound(L"SlimeHeart.ogg", CSoundMgr::MONSTER3, 0.5f); // 사운드
			m_iWaterCout++;
		}
		if (m_iWaterCout > 8) {
			pGameInstance->StopSound(CSoundMgr::MONSTER3); // 사운드
			m_iWaterCout = 0;
			m_bWaterCreat = false;
		}

	}




	RELEASE_INSTANCE(CGameInstance);

	return _int();
}

_int CSlime_Heart::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
	{
		MSGBOX("0 > __super::LateTick in CSlime_Heart::LateTick");
		return -1;
	}

	if (nullptr == m_pRendererCom)
	{
		MSGBOX("m_pRendererCom is nullptr in CSlime_Heart::LateTick");
		return -1;
	}

	if (FAILED(SetUp_OnTerrain()))
	{
		MSGBOX("SetUp_OnTerrain returned E_FAIL in CSlime_Heart::LateTick");
		return -1;
	}
	CCollider_Sphere::SPHEREDESC SphereDesc;
	SphereDesc.ObjectMatrix = m_pTransformCom->Get_WorldMatrix();
	SphereDesc.fRadius = 0.5f;
	SphereDesc.fDist_From_Obj = _float3(0.f, 0.f, 0.f);
	m_pColliderCom->Set_SphereDesc(SphereDesc);
	m_pColliderCom->Tick();






	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);





	CTransform* pCameraTrans = (CTransform*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Camera"), TEXT("Com_Transform"));
	_float3 vCameraPos = pCameraTrans->Get_State(CTransform::STATE_POSITION);
	m_pTransformCom->MatchIt_XZ(vCameraPos);




	RELEASE_INSTANCE(CGameInstance);


	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

HRESULT CSlime_Heart::Render()
{
	if (nullptr == m_pVIBufferCom)
	{
		MSGBOX("m_pVIBufferCom is nullptr in CSlime_Heart::Render")
			return E_FAIL;
	}

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
	{
		MSGBOX("m_pTransformCom->Bind_OnGraphicDevice returned E_FAIL in CSlime_Heart::Render");
		return E_FAIL;
	}
	if (FAILED(Go_Sprites()))
	{
		MSGBOX("Go_Sprites returned E_FAIL in CYeti::Tick");
		return E_FAIL;
	}
	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice()))
	{
		MSGBOX("m_pTextureCom->Bind_OnGraphicDevice in CSlime_Heart::Render");
		return E_FAIL;
	}


	if (FAILED(SetUp_RenderState()))
	{
		MSGBOX("SetUp_RenderState returned E_FAIL in CSlime_Heart::Render");
		return E_FAIL;
	}

	m_pVIBufferCom->Render();

	if (FAILED(Release_RenderState()))
	{
		MSGBOX("Release_RenderState returned E_FAIL in CSlime_Heart::Render");
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

void CSlime_Heart::Collision()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	for (_uint i = 0; i < pGameInstance->Get_Size(g_eCurrentLevel, TEXT("Layer_Wall")); i++)
	{
		_float3 vPos = pGameInstance->Collision_SphereToRect((CCollider_Rect*)pGameInstance->Get_Component(g_eCurrentLevel, TEXT("Layer_Wall"), TEXT("Com_Collider"), i), m_pColliderCom);
		if (vPos != _float3(0.f, 0.f, 0.f))
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	}

	if (0 == pGameInstance->Get_Size(g_eCurrentLevel, TEXT("Layer_Slime_Boss")))
	{
		if (m_iCollDelay >= 0)
			--m_iCollDelay;

		if (m_iCollDelay < 0)
			if ((nullptr != pGameInstance->Find_Layer(LEVEL_STATIC, TEXT("Layer_Arrow")) && 0 < pGameInstance->Get_Size(LEVEL_STATIC, TEXT("Layer_Arrow"))) && true == pGameInstance->Collision_Sphere((CCollider_Sphere*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Arrow"), TEXT("Com_Collider")), m_pColliderCom))
			{
				pGameInstance->LetsPlaySound(L"SlimeSlice.ogg", CSoundMgr::EFFECT, 1.0f); // 사운드
				if (false == m_bDead)
				{
					pGameInstance->Add_GameObject(g_eCurrentLevel, TEXT("Layer_DarkScreen"), TEXT("Prototype_GameObject_Effect_DarkScreen"));
					pGameInstance->Add_GameObject(g_eCurrentLevel, TEXT("Layer_WhiteScreen"), TEXT("Prototype_GameObject_Effect_WhiteScreen"));
					m_bDead = true;
					RELEASE_INSTANCE(CGameInstance);
					return;
				}
			}
	}
	

	RELEASE_INSTANCE(CGameInstance);
}

HRESULT CSlime_Heart::SetUp_Components()
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CSlime_Heart::SetUp_Components(Transform)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CSlime_Heart::SetUp_Components(Renderer)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CSlime_Heart::SetUp_Components(VIBuffer_Rect)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_SLIME, TEXT("Prototype_Component_Texture_Slime_Heart"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CSlime_Heart::SetUp_Components(Texture)");
		return E_FAIL;
	}

	CCollider_Sphere::SPHEREDESC SphereDesc;
	SphereDesc.ObjectMatrix = m_pTransformCom->Get_WorldMatrix();
	SphereDesc.fRadius = 0.5f;
	SphereDesc.fDist_From_Obj = _float3(0.f, 0.f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"), TEXT("Com_Collider"), (CComponent**)&m_pColliderCom, &SphereDesc)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CSlime::SetUp_Components(Collider_Sphere)");
		return E_FAIL;
	}

	//#ifdef _DEBUG
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"), TEXT("Com_VIBuffer_Coll"), (CComponent**)&m_pVIBufferCollCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CSlime::SetUp_Components(VIBuffer_Cube)");
		return E_FAIL;
	}
	//#endif // DEBUG

	return S_OK;
}

HRESULT CSlime_Heart::SetUp_OnTerrain()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	CVIBuffer_Terrain* m_pVIBuffer_Terrain = (CVIBuffer_Terrain*)pGameInstance->Get_Component(g_eCurrentLevel, TEXT("Layer_Terrain"), TEXT("Com_VIBuffer"));
	if (m_pVIBuffer_Terrain == nullptr)
	{
		MSGBOX("pVIBuffer_Terrain is nullptr in CSlime_Heart::SetUp_OnTerrain");
		return E_FAIL;
	}
	//플레이어의 위치를 얻어와서
	_float3 vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	vPosition.y = m_pVIBuffer_Terrain->Compute_Y(vPosition) + m_fScale / 2;
	if (m_SlimeDead->bBossSlimeDead == true) {
		if (m_bJump == true) {
			if (m_pTransformCom->Get_State(CTransform::STATE_POSITION).y < vPosition.y) {
				m_bWaterCreat = true;
				m_bJump = false;
				m_fTime = 0;
			}
		}
		if (m_bJump == false)
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
	}
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CSlime_Heart::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
	{
		MSGBOX("m_pGraphic_Device is nullptr in CSlime_Heart::SetUp_RenderState");
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
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 100);
	//그릴려고 했던 이미지의 알파값을 내가 지정한 값이랑
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER/*크다면*/);
	//m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

	// 이 조건을 통과한 애들만 화면에 그리겠다
	// 이코드의 경우 현재 그릴려는 알파값(REF - 0)보다 크다면(GREATER) 그린다

	// 단순히 배경을 지우는데는 알파테스팅이 아주 유용하지만 한계가 있다
	// 반투명 처리가 불가능하기 때문
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//또한 테스팅과 블렌딩은 혼합하지 않는다 -> 가능은한데 의미가 퇴색됨
	return S_OK;
}

HRESULT CSlime_Heart::Go_Sprites()
{
	if (FAILED(m_pVIBufferCom->Go_Sprites(m_tFrame)))
	{
		MSGBOX("m_pVIBufferCom->Go_Sprites returned E_FAIL in CYeti::Go_Sprites");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CSlime_Heart::Release_RenderState()
{
	if (nullptr == m_pGraphic_Device)
	{
		MSGBOX("m_pGraphic_Device is nullptr in CSlime_Heart::Release_RenderState");
		return E_FAIL;
	}
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	//m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

CSlime_Heart * CSlime_Heart::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSlime_Heart*	pInstance = new CSlime_Heart(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create Slime_Heart in CSlime_Heart::Create");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CSlime_Heart::Clone(void* pArg)
{
	/* 새로운객체를 복제하여 생성한다. */
	CSlime_Heart*	pInstance = new CSlime_Heart(*this);


	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create Slime_Heart in CSlime_Heart::Clone");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSlime_Heart::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCollCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}