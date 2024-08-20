#include "stdafx.h"
#include "..\Public\Slime_Boss.h"
#include "GameInstance.h"

CSlime_Boss::CSlime_Boss(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CSlime_Boss::CSlime_Boss(const CSlime_Boss & rhs)
	: CGameObject(rhs)
{
}

HRESULT CSlime_Boss::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		MSGBOX("__super::NativeConstruct_Prototype returned E_FAIL in CSlime_Boss::NativeConstruct_Prototype");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CSlime_Boss::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		MSGBOX("__super::NativeConstruct returned E_FAIL in CSlime_Boss::NativeConstruct");
		return E_FAIL;
	}

	// ������Ʈ ����
	if (FAILED(SetUp_Components()))
	{
		MSGBOX("SetUp_Components returned E_FAIL in CSlime_Boss::NativeConstruct");
		return E_FAIL;
	}
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	m_SlimeDead = (SLIMEDEAD*)pArg;

	m_pTransformCom->Scaled(_float3(m_fScale, m_fScale, m_fScale));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(20.f, 4.0f, 20.f));
	pGameInstance->Add_GameObject(LEVEL_SLIME, TEXT("Layer_Slime_Boss_Shadow"), TEXT("Prototype_GameObject_Slime_Boss_Shadow"), this);

	m_SlimeInfo.tSlimeDead = m_SlimeDead;
	//m_pBossDead = (_bool*)pArg;
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}



_int CSlime_Boss::Tick(_float fTimeDelta)
{
	if (0 > __super::Tick(fTimeDelta))
	{
		MSGBOX("0 > __super::Tick in CSlime_Boss::Tick");
		return -1;
	}
	if (m_bObjectDead) {
		return -1;
	}

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	//m_fFrame += 90.0f * fTimeDelta; // 1�ʿ� ��� �������� ������ŭ ����� �ٲ۴�

	//if (m_fFrame >= 90.0f) // ���� �������� �� ���Ҵٸ� ó������ �ǵ�����
	//	m_fFrame = 0.f;
	//m_fFrame += 90.0f * fTimeDelta; // 1�ʿ� ��� �������� ������ŭ ����� �ٲ۴�

	//if (m_fFrame >= 90.0f) // ���� �������� �� ���Ҵٸ� ó������ �ǵ�����
	//	m_fFrame = 0.f;

	if (m_bHitReady == false) {
		m_iHitTimer++;
	}

	if (m_iHitTimer > 20) {
		m_bHitReady = true;
		m_iHitTimer = 0;
	}

	/*_float3 temp = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	if (m_bSplitPattern == true && temp.y < m_fScale * 0.5f) {
	temp.y = 0 + m_fScale + 0.1f;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, temp);

	}*/

	if (m_bSplitPattern == true) {
		m_fSplitPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		m_fCrushTime = 0;
		if (m_bJump == false) {
			temppos = m_fSplitPos;
			//temppos
			m_fSplittagetPos = m_fSplitPos;
			m_iSplitCount++;
			if (m_iSplitCount < 2) {
				m_fSplittagetPos.x -= 3.f;
			}
			else
			{
				m_fSplittagetPos.x += 3.f;
			}
			m_pTransformCom->LookAtXZ(m_fSplittagetPos);
			m_fJumpy = m_pTransformCom->Get_State(CTransform::STATE_POSITION).y;
			m_bJump = true;
		}
		if (m_bJump == true) {

			m_fSplitPos.y = m_fJumpy + (1.f * m_fTime - 5.8f * m_fTime * m_fTime * 0.5f);
			m_fTime += 0.025f;
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_fSplitPos);
			_float3 vdir = m_fSplittagetPos - m_fSplitPos;
			//if (D3DXVec3Length(&vdir) >= 0.5f)
			m_pTransformCom->Go_Straight(fTimeDelta * (m_iSmallRank));
		}

	}

	else {
		if (m_iSmallRank <= 3 && pGameInstance->Get_DIKeyState(DIK_4)) {
			m_bObjectDead = true;
			m_SlimeDead->bBossSlimeDead = true;
		}
		CTransform* m_pPlayerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"));
		_float3 vpos = m_pPlayerTransform->Get_State(CTransform::STATE_POSITION);
		if (m_fCrushTime == 0) {
			if (m_bJump == false) {
				m_fJumpy = m_pTransformCom->Get_State(CTransform::STATE_POSITION).y;
				m_bJump = true;
			}

			_float3 MyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			vpos.y = MyPos.y;
			if (m_bJump == true) {
				m_pTransformCom->LookAt(vpos);
				MyPos.y = m_fJumpy + ((6.f - (m_iSmallRank)) * m_fTime - 2.8f * m_fTime * m_fTime * 0.5f);
				m_fTime += 0.025f;
				m_pTransformCom->Set_State(CTransform::STATE_POSITION, MyPos);
				m_pTransformCom->Go_Straight(fTimeDelta);
			}
		}
	}
	//////////////////////////////////////


	if (m_bWaterCreat == true) {
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_SLIME, TEXT("Layer_Slime_Water_Mini"), TEXT("Prototype_GameObject_Slime_water_mini"), this)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CSlime_Boss::SetUp_OnTerrain");
			return E_FAIL;
		}
		pGameInstance->LetsPlaySound(L"SlimeBig.ogg", CSoundMgr::MONSTER1, 0.5f); // ����
		m_iWaterCout++;
	}
	if (m_iWaterCout > 4) {
		pGameInstance->StopSound(CSoundMgr::MONSTER1); // ����
		m_iWaterCout = 0;
		m_bWaterCreat = false;
	}


	Collision_Rect();


	RELEASE_INSTANCE(CGameInstance);

	return _int();
}

_int CSlime_Boss::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
	{
		MSGBOX("0 > __super::LateTick in CSlime_Boss::LateTick");
		return -1;
	}

	if (nullptr == m_pRendererCom)
	{
		MSGBOX("m_pRendererCom is nullptr in CSlime_Boss::LateTick");
		return -1;
	}

	if (FAILED(SetUp_OnTerrain()))
	{
		MSGBOX("SetUp_OnTerrain returned E_FAIL in CSlime_Boss::LateTick");
		return -1;
	}

	CCollider_Sphere::SPHEREDESC SphereDesc;
	SphereDesc.ObjectMatrix = m_pTransformCom->Get_WorldMatrix();
	SphereDesc.fRadius = 0.4f;
	SphereDesc.fDist_From_Obj = _float3(0.f, 0.f, 0.f);
	m_pColliderCom->Set_SphereDesc(SphereDesc);
	m_pColliderCom->Tick();


	_float4x4		ViewMatrix; //������� ������ ����
								//ī�޶� �佺���̽��� ����� ���ؼ� �����Ѵ�
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	//���庯ȯ��ķ� �����
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);
	//D3DXMatrixInverse
	//����迡 �������ش�
	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, (*(_float3*)&ViewMatrix.m[0][0]) * m_pTransformCom->Get_Scale().x);
	//m_pTransformCom->Set_State(CTransform::STATE_UP, (*(_float3*)&ViewMatrix.m[1][0]) * m_pTransformCom->Get_Scale().y);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, (*(_float3*)&ViewMatrix.m[2][0]) * m_pTransformCom->Get_Scale().z);




	//////////���ļ���Z�� �ֱ�///////////
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
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHA, this);

	return _int();
}

HRESULT CSlime_Boss::Render()
{
	if (nullptr == m_pVIBufferCom)
	{
		MSGBOX("m_pVIBufferCom is nullptr in CSlime_Boss::Render")
			return E_FAIL;
	}

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
	{
		MSGBOX("m_pTransformCom->Bind_OnGraphicDevice returned E_FAIL in CSlime_Boss::Render");
		return E_FAIL;
	}
	m_pVIBufferCom->Base_Setting();
	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice()))
	{
		MSGBOX("m_pTextureCom->Bind_OnGraphicDevice in CSlime_Boss::Render");
		return E_FAIL;
	}


	if (FAILED(SetUp_RenderState()))
	{
		MSGBOX("SetUp_RenderState returned E_FAIL in CSlime_Boss::Render");
		return E_FAIL;
	}

	m_pVIBufferCom->Render();

	if (FAILED(Release_RenderState()))
	{
		MSGBOX("Release_RenderState returned E_FAIL in CSlime_Boss::Render");
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

HRESULT CSlime_Boss::SetUp_Components()
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CSlime_Boss::SetUp_Components(Transform)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CSlime_Boss::SetUp_Components(Renderer)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CSlime_Boss::SetUp_Components(VIBuffer_Rect)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_SLIME, TEXT("Prototype_Component_Texture_Slime"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CSlime_Boss::SetUp_Components(Texture)");
		return E_FAIL;
	}

	CCollider_Sphere::SPHEREDESC SphereDesc;
	SphereDesc.ObjectMatrix = m_pTransformCom->Get_WorldMatrix();
	SphereDesc.fRadius = 0.3f;
	SphereDesc.fDist_From_Obj = _float3(0.f, 0.f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"), TEXT("Com_Collider"), (CComponent**)&m_pColliderCom, &SphereDesc)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CSlime_Boss::SetUp_Components(Collider_Sphere)");
		return E_FAIL;
	}

	//#ifdef _DEBUG
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"), TEXT("Com_VIBuffer_Coll"), (CComponent**)&m_pVIBufferCollCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CSlime_Boss::SetUp_Components(VIBuffer_Cube)");
		return E_FAIL;
	}
	//#endif // DEBUG

	return S_OK;
}

HRESULT CSlime_Boss::SetUp_OnTerrain()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	CVIBuffer_Terrain* m_pVIBuffer_Terrain = (CVIBuffer_Terrain*)pGameInstance->Get_Component(g_eCurrentLevel, TEXT("Layer_Terrain"), TEXT("Com_VIBuffer"));
	if (m_pVIBuffer_Terrain == nullptr)
	{
		MSGBOX("pVIBuffer_Terrain is nullptr in CSlime_Boss::SetUp_OnTerrain");
		return E_FAIL;
	}
	//�÷��̾��� ��ġ�� ���ͼ�
	_float3 vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	//vPosition.y = m_pVIBuffer_Terrain->Compute_Y(vPosition) + m_fScale / 2 ;
	if (m_bJump == true) {
		vPosition.y = m_pVIBuffer_Terrain->Compute_Y(vPosition) + m_fScale / 4;

	}

	if (m_bJump == true) {
		if (m_pTransformCom->Get_State(CTransform::STATE_POSITION).y < vPosition.y) {
			if (m_bSplitPattern == true) {
				m_bSplitPattern = false;
			}
			if (m_fCrushTime < 1) {
				savePos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
				m_bWaterCreat = true;
					
				
			}
			m_fCrushTime++;
			if (m_fCrushTime <= 25)
				m_fCrushScale -= 0.025f * m_iSmallRank;
			else if (m_fCrushTime <= 50)
				m_fCrushScale += 0.025f * m_iSmallRank;
			else if (m_fCrushTime <= 75)
				m_fCrushScale -= 0.025f * m_iSmallRank;
			else if (m_fCrushTime <= 100)
				m_fCrushScale += 0.025f * m_iSmallRank;
			/*	else if (m_fCrushTime <= 100)
			m_fCrushScale -= 0.025f * m_iSmallRank;*/
			_float3 pos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

			m_pTransformCom->Scaled(_float3(m_fScale, m_fScale + m_fCrushScale, m_fScale));
			pos.y = savePos.y + m_fCrushScale / 4;
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, pos);
			m_bCruch = true;
			if (m_fCrushTime > 100) {
				//pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Boss"), TEXT("Prototype_GameObject_Slime"),&m_bDead);
				if (m_iSmallRank > 2) {
					//Hit();
				}
				m_fCrushScale = 0;
				m_bJump = false;
				m_pTransformCom->Scaled(_float3(m_fScale, m_fScale, m_fScale));
				//vPosition.y = m_pVIBuffer_Terrain->Compute_Y(vPosition) + m_fScale / 2;
				//m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
				m_fCrushTime = 0;
				m_bCruch = false;
			}
			m_fTime = 0;
		}
	}
	if (m_bJump == false) {
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
	}
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CSlime_Boss::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
	{
		MSGBOX("m_pGraphic_Device is nullptr in CSlime_Boss::SetUp_RenderState");
		return E_FAIL;
	}

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);//���ĺ����� �ϰڴ�
	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);//���� ���Ѵ�
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	////�׸����� �ϴ� �ȼ��� ������ ������ �ҽ� Į���� ���ĸ� ���ڴ�
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	/*
	_float4		vSourColor, vDestColor;
	(vSourColor.rgb) * vSourColor.a + (vDestColor.rgb) * (1.f - vSourColor.a);
	�ҽ��÷�.RGB * �ҽ��÷�.���� + ����Ʈ�ö�.RGB * 1.f - �ҽ��÷�.����
	���Ĵ� RGB�� 0~255�� 0���� 1���̷� �����Ѱ�
	�÷��̾ �׸����ϴµ� �̹� ����ġ�� �׷��� �ִ� ���� ����ƮĮ��
	*/
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//�ٸ� ��� - ���� �׽�Ʈ
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);// �����׽�Ʈ�� �ϰڴ�
	//															   �ش������� ���ؼ� �񱳸� �� ����
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 0);
	//�׸����� �ߴ� �̹����� ���İ��� ���� ������ ���̶�
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER/*ũ�ٸ�*/);
	//m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

	// �� ������ ����� �ֵ鸸 ȭ�鿡 �׸��ڴ�
	// ���ڵ��� ��� ���� �׸����� ���İ�(REF - 0)���� ũ�ٸ�(GREATER) �׸���

	// �ܼ��� ����� ����µ��� �����׽����� ���� ���������� �Ѱ谡 �ִ�
	// ������ ó���� �Ұ����ϱ� ����

	//���� �׽��ð� ������ ȥ������ �ʴ´� -> �������ѵ� �ǹ̰� �����
	return S_OK;
}

HRESULT CSlime_Boss::Release_RenderState()
{
	if (nullptr == m_pGraphic_Device)
	{
		MSGBOX("m_pGraphic_Device is nullptr in CSlime_Boss::Release_RenderState");
		return E_FAIL;
	}
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	//m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

HRESULT CSlime_Boss::Hit()
{
	/*CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	m_fScale = m_fScale / 2;
	m_SlimeInfo.fScale = m_fScale;
	m_SlimeInfo.fvPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	m_SlimeInfo.iSmallRank = m_iSmallRank - 1;
	m_iSmallRank--;
	m_bSplitPattern = true;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_SLIME, TEXT("Layer_Slime"), TEXT("Prototype_GameObject_Slime"), &m_SlimeInfo)))
	{
	MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CSlime_Boss::Hit");
	return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);*/

	m_bWaterCreat = true;
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	m_fScale = m_fScale / 1.5f;
	//m_pTransformCom->Scaled(_float3(m_fScale, m_fScale, m_fScale));
	m_SlimeInfo.fvPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	if (m_SlimeInfo.fvPos.y < 1.f || m_SlimeInfo.fvPos.y - m_fScale  < 0.f) {
		//if(m_bCruch == false)
		m_SlimeInfo.fvPos.y = 0 + m_fScale * 0.5f;
		/*	else
		m_SlimeInfo.fvPos.y = 0 + m_fScale * 0.5f;*/

	}
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_SlimeInfo.fvPos);
	//m_SlimeInfo_Mini.fvPos.y += 10.f;
	m_SlimeInfo.fScale = m_fScale;
	m_SlimeInfo.iSmallRank = m_iSmallRank - 1;
	m_iSmallRank--;
	pGameInstance->Add_GameObject(LEVEL_SLIME, TEXT("Layer_Slime"), TEXT("Prototype_GameObject_Slime"), &m_SlimeInfo);

	m_pTransformCom->Scaled(_float3(m_fScale, m_fScale, m_fScale));

	m_bHitReady = false;
	//m_bJump = false;
	m_bSplitPattern = true;
	if (m_bJump == true) {
		m_fJumpy = m_pTransformCom->Get_State(CTransform::STATE_POSITION).y;
		m_fTime = 0;
	}

	//m_SlimeInfo_Mini.fvPos.y += m_fScale * 2.f;
	/*_float3 a = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	a.y -= scal*/
	//m_pTransformCom->Set_State
	//m_pTransformCom->Scaled(_float3(m_fScale, m_fScale, m_fScale));

	//m_bSplitPattern = true;

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

void CSlime_Boss::Collision_Rect()
{
	if (LEVEL_LOADING == g_eCurrentLevel)
		return;
	if (m_bHitReady) {
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
		for (_uint i = 0; i < 4; i++)
		{
			_float3 vPos = pGameInstance->Collision_SphereToRect((CCollider_Rect*)pGameInstance->Get_Component(g_eCurrentLevel, TEXT("Layer_Wall"), TEXT("Com_Collider"), i), m_pColliderCom);
			if (vPos != _float3(0.f, 0.f, 0.f))
				m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
		}
		--m_iCollDelay;

		if (m_iCollDelay < 0)
			if ((nullptr != pGameInstance->Find_Layer(LEVEL_STATIC, TEXT("Layer_Arrow")) && 0 < pGameInstance->Get_Size(LEVEL_STATIC, TEXT("Layer_Arrow"))) && true == pGameInstance->Collision_Sphere((CCollider_Sphere*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Arrow"), TEXT("Com_Collider")), m_pColliderCom))
			{
				pGameInstance->LetsPlaySound(L"SlimeSlice.ogg", CSoundMgr::EFFECT, 1.0f); // ����
				if (m_iSmallRank > 1)
					Hit();
				else
				{
					m_bObjectDead = true;
					m_SlimeDead->bBossSlimeDead = true;
				}
				m_iCollDelay = 60;
				RELEASE_INSTANCE(CGameInstance);
				return;
			}

		RELEASE_INSTANCE(CGameInstance);
	}
}

CSlime_Boss * CSlime_Boss::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSlime_Boss*	pInstance = new CSlime_Boss(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create Slime_Boss in CSlime_Boss::Create");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CSlime_Boss::Clone(void* pArg)
{
	/* ���οü�� �����Ͽ� �����Ѵ�. */
	CSlime_Boss*	pInstance = new CSlime_Boss(*this);


	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create Slime_Boss in CSlime_Boss::Clone");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSlime_Boss::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCollCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}