#include "stdafx.h"
#include "..\Public\Icicle_Break_Effect.h"
#include "GameInstance.h"
#include <time.h>

CIcicle_Break_Effect::CIcicle_Break_Effect(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CIcicle_Break_Effect::CIcicle_Break_Effect(const CIcicle_Break_Effect & rhs)
	: CGameObject(rhs)
{
}

HRESULT CIcicle_Break_Effect::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		MSGBOX("__super::NativeConstruct_Prototype returned E_FAIL in CIcicle_Break_Effect::NativeConstruct_Prototype");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CIcicle_Break_Effect::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		MSGBOX("__super::NativeConstruct returned E_FAIL in CIcicle_Break_Effect::NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(SetUp_Components()))
	{
		MSGBOX("SetUp_Components returned E_FAIL in CIcicle_Break_Effect::NativeConstruct");
		return E_FAIL;
	}
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	//srand(time(NULL));

	m_pSlime = (CIcicle*)pArg;
	CTransform* a = m_pSlime->Get_TransForm();
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, a->Get_State(CTransform::STATE_POSITION));
	_float3 pos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	int b = rand() % 3;
	int c = rand() % 4;
	switch (c)
	{
	case 0:
		m_pTransformCom->LookAtXZ(_float3(pos.x - b, 0, pos.z + b));
		break;
	case 1:
		m_pTransformCom->LookAtXZ(_float3(pos.x + b, 0, pos.z + b));
		break;
	case 2:
		m_pTransformCom->LookAtXZ(_float3(pos.x - b, 0, pos.z - b));
		break;
	case 3:
		m_pTransformCom->LookAtXZ(_float3(pos.x + b, 0, pos.z - b));
		break;
	}

	m_tFrame.iFrameStartY = 0;
	m_tFrame.iFrameStartX = rand() % 3;
	m_tFrame.iFrameCurrentX = m_tFrame.iFrameStartX;
	m_tFrame.iFrameEndX = m_tFrame.iFrameStartX;
	m_tFrame.iFrameHeight = 16.f;
	m_tFrame.iFrameWidth = 16.f;
	m_tFrame.iTexHeight = 16.f;
	m_tFrame.iTexWidth = 64.f;


	m_fSpriteSpeed = 1;

	m_pTransformCom->Scaled(_float3(m_fScale, m_fScale, m_fScale));
	//pGameInstance->Add_GameObject(LEVEL_SLIME, TEXT("Layer_Slime_Shadow"), TEXT("Prototype_GameObject_Slime_Shadow"), this);
	//m_bJump = true;
	//m_pBossDead = (_bool*)pArg;
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}



_int CIcicle_Break_Effect::Tick(_float fTimeDelta)
{
	if (m_bDead)
		return -1;
	if (0 > __super::Tick(fTimeDelta))
		return -1;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	m_pTransformCom->Go_Straight(fTimeDelta * 0.2f);

	if (m_bJump == false) {
		m_fJumpy = m_pTransformCom->Get_State(CTransform::STATE_POSITION).y;
		m_bJump = true;
	}

	if (m_fScale < 0) {
		m_bDead = true;
	}

	_float3 MyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	//vpos.y = MyPos.y;
	if (m_bJump == true) {
		//m_pTransformCom->LookAt(vpos);
		MyPos.y = m_fJumpy + (m_fJumpPower * m_fTime - 5.8f * m_fTime * m_fTime * 0.5f);
		m_fTime += 0.025f;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, MyPos);
		m_pTransformCom->Go_Straight(fTimeDelta * 0.1f);
	}
	//m_fScale = 0.5f;
	m_fScale -= 0.01f;
	m_pTransformCom->Scaled(_float3(m_fScale, m_fScale, m_fScale));


	Go_Frame(m_fSpriteSpeed,fTimeDelta);
	RELEASE_INSTANCE(CGameInstance);

	return _int();

}

_int CIcicle_Break_Effect::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
	{
		MSGBOX("0 > __super::LateTick in CIcicle_Break_Effect::LateTick");
		return -1;
	}

	if (nullptr == m_pRendererCom)
	{
		MSGBOX("m_pRendererCom is nullptr in CIcicle_Break_Effect::LateTick");
		return -1;
	}

	
	CCollider_Sphere::SPHEREDESC SphereDesc;
	SphereDesc.ObjectMatrix = m_pTransformCom->Get_WorldMatrix();
	SphereDesc.fRadius = 0.5f;
	SphereDesc.fDist_From_Obj = _float3(0.f, 0.f, 0.f);
	//m_pColliderCom->Set_SphereDesc(SphereDesc);
	//m_pColliderCom->Tick();


	_float4x4		ViewMatrix; //뷰행렬을 저장할 변수
								//카메라 뷰스페이스의 행렬을 구해서 저장한다
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	//월드변환행렬로 만든다
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);
	//D3DXMatrixInverse
	//라업룩에 적용해준다
	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, (*(_float3*)&ViewMatrix.m[0][0]) * m_pTransformCom->Get_Scale().x);
	//m_pTransformCom->Set_State(CTransform::STATE_UP, (*(_float3*)&ViewMatrix.m[1][0]) * m_pTransformCom->Get_Scale().y);
	//m_pTransformCom->Set_State(CTransform::STATE_LOOK, (*(_float3*)&ViewMatrix.m[2][0]) * m_pTransformCom->Get_Scale().z);





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

HRESULT CIcicle_Break_Effect::Render()
{
	if (nullptr == m_pVIBufferCom)
	{
		MSGBOX("m_pVIBufferCom is nullptr in CIcicle_Break_Effect::Render")
			return E_FAIL;
	}

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
	{
		MSGBOX("m_pTransformCom->Bind_OnGraphicDevice returned E_FAIL in CIcicle_Break_Effect::Render");
		return E_FAIL;
	}
	if (FAILED(Go_Sprites()))
	{
		MSGBOX("Go_Sprites returned E_FAIL in CColossus_Fist_Right::Tick");
		return E_FAIL;
	}
	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice()))
	{
		MSGBOX("m_pTextureCom->Bind_OnGraphicDevice in CIcicle_Break_Effect::Render");
		return E_FAIL;
	}

	////////////////////
	if (FAILED(SetUp_RenderState()))
	{
		MSGBOX("SetUp_RenderState returned E_FAIL in CIcicle_Break_Effect::Render");
		return E_FAIL;
	}

	m_pVIBufferCom->Render(); // 이렇게 순서대로 랜더의 설정을 할 경우
							  // 특정한 부분에만 블렌딩을 적용하거나 할 수 있다
	if (FAILED(Release_RenderState()))
	{
		MSGBOX("Release_RenderState returned E_FAIL in CIcicle_Break_Effect::Render");
		return E_FAIL;
	}
	////////////////////

//#ifdef _DEBUG
//	//CCollider_Sphere::SPHEREDESC tSphereDesc = m_pColliderCom->Get_SphereDesc();
////	m_pVIBufferCollCom->MakeHitBox(tSphereDesc);
//	m_pGraphic_Device->SetTexture(0, FALSE);
//	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
//	m_pVIBufferCollCom->Render();
//	m_pVIBufferCollCom->BaseSetting();
//	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
//
//#endif // _DEBUG
	return S_OK;
}

HRESULT CIcicle_Break_Effect::SetUp_Components()
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CIcicle_Break_Effect::SetUp_Components(Transform)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CIcicle_Break_Effect::SetUp_Components(Renderer)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CIcicle_Break_Effect::SetUp_Components(VIBuffer_Rect)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_YETI, TEXT("Prototype_Component_Texture_Icicle_Break"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CIcicle_Break_Effect::SetUp_Components(Texture)");
		return E_FAIL;
	}

	

	//#ifdef _DEBUG
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"), TEXT("Com_VIBuffer_Coll"), (CComponent**)&m_pVIBufferCollCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CIcicle_Break_Effect::SetUp_Components(VIBuffer_Cube)");
		return E_FAIL;
	}
	//#endif // DEBUG

	return S_OK;
}

HRESULT CIcicle_Break_Effect::SetUp_OnTerrain()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	CVIBuffer_Terrain* m_pVIBuffer_Terrain = (CVIBuffer_Terrain*)pGameInstance->Get_Component(g_eCurrentLevel, TEXT("Layer_Terrain"), TEXT("Com_VIBuffer"));
	if (m_pVIBuffer_Terrain == nullptr)
	{
		MSGBOX("pVIBuffer_Terrain is nullptr in CSlime_Boss::SetUp_OnTerrain");
		return E_FAIL;
	}
	//플레이어의 위치를 얻어와서
	_float3 vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	vPosition.y = m_pVIBuffer_Terrain->Compute_Y(vPosition) + m_fScale / 2 ;
	if (m_bJump == true) {
		if (m_pTransformCom->Get_State(CTransform::STATE_POSITION).y < vPosition.y) {
			if (FAILED(pGameInstance->Add_GameObject(LEVEL_ICEBRAIN, TEXT("Layer_Brain_Water"), TEXT("Prototype_GameObject_Brain_water"), this)))
			{
				MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CSlime_Boss::SetUp_OnTerrain");
				return E_FAIL;
			}
			m_bDead = true;
		}

	}

	
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CIcicle_Break_Effect::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
	{
		MSGBOX("m_pGraphic_Device is nullptr in CIcicle_Break_Effect::SetUp_RenderState");
		return E_FAIL;
	}

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);// 알파테스트를 하겠다
																   //해당조건을 통해서 비교를 한 다음
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 100);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	return S_OK;
}

HRESULT CIcicle_Break_Effect::Release_RenderState()
{
	if (nullptr == m_pGraphic_Device)
	{
		MSGBOX("m_pGraphic_Device is nullptr in CIcicle_Break_Effect::Release_RenderState");
		return E_FAIL;
	}
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	//m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	return S_OK;
}

HRESULT CIcicle_Break_Effect::Go_Sprites()
{
	if (FAILED(m_pVIBufferCom->Go_Sprites(m_tFrame)))
	{
		MSGBOX("m_pVIBufferCom->Go_Sprites returned E_FAIL in CYeti::Go_Sprites");
		return E_FAIL;
	}
	return S_OK;
}

CIcicle_Break_Effect * CIcicle_Break_Effect::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CIcicle_Break_Effect*	pInstance = new CIcicle_Break_Effect(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create Slime in CIcicle_Break_Effect::Create");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CIcicle_Break_Effect::Clone(void* pArg)
{
	/* 새로운객체를 복제하여 생성한다. */
	CIcicle_Break_Effect*	pInstance = new CIcicle_Break_Effect(*this);


	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create Slime in CIcicle_Break_Effect::Clone");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CIcicle_Break_Effect::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCollCom);
	//Safe_Release(m_pColliderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}