#include "stdafx.h"
#include "..\Public\Colosuss_Fist_Dust.h"
#include "GameInstance.h"

CColosuss_Fist_Dust::CColosuss_Fist_Dust(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CColosuss_Fist_Dust::CColosuss_Fist_Dust(const CColosuss_Fist_Dust& rhs)
	: CGameObject(rhs)
{
}

HRESULT CColosuss_Fist_Dust::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CColosuss_Fist_Dust::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	/* 현재 객체에게 추가되어야할 컴포넌트들을 복제(or 참조)하여 멤버변수에 보관한다.  */
	if (FAILED(SetUp_Components()))
		return E_FAIL;
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CColossus_Fist* TempYeti = (CColossus_Fist*)pArg;

	pos = TempYeti->Get_TransForm()->Get_State(CTransform::STATE_POSITION);
	pos.y = 0.5f;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, pos);
	//_float3 YetiPos = m_pYetiTransform->Get_State(CTransform::STATE_POSITION);
	//m_pTransformCom->Set_State(CTransform::STATE_LOOK, TempYeti->Get_TransForm()->Get_State(CTransform::STATE_LOOK));
	//m_tFrame =  TempYeti->Get_Frame();
	//m_tFrame.iFrameCurrent = 6;
	_uint a = TempYeti->Get_Dust_Num();

	switch (a / 2)
	{
	case 0:
		pos.z += (rand() % 30) * 0.1f;
		m_pTransformCom->LookAtXZ(pos);
		break;
	case 1:
		pos.z += (rand() % 30) * 0.1f;
		pos.x += (rand() % 30) * 0.1f;
		m_pTransformCom->LookAtXZ(pos);
		break;
	case 2:
		//pos.z += 1.f;
		pos.x += (rand() % 30) * 0.1f;
		m_pTransformCom->LookAtXZ(pos);
		break;
	case 3:
		pos.z -= (rand() % 30) * 0.1f;
		pos.x += (rand() % 30) * 0.1f;
		m_pTransformCom->LookAtXZ(pos);
		break;
	case 4:
		pos.z -= (rand() % 30) * 0.1f;
		//pos.x += 1.f;
		m_pTransformCom->LookAtXZ(pos);
		break;
	case 5:
		pos.z -= (rand() % 30) * 0.1f;
		pos.x -= (rand() % 30) * 0.1f;
		m_pTransformCom->LookAtXZ(pos);
		break;
	case 6:
		pos.x -= (rand() % 30) * 0.1f;
		m_pTransformCom->LookAtXZ(pos);
		break;
	case 7:
		pos.z += (rand() % 30) * 0.1f;
		pos.x -= (rand() % 30) * 0.1f;
		m_pTransformCom->LookAtXZ(pos);
		break;
	case 8:
		pos.z += (rand() % 30) * 0.1f;
		pos.x -= (rand() % 30) * 0.1f;
		m_pTransformCom->LookAtXZ(pos);
		break;
	default:
		break;
	}


	m_tFrame.iFrameStartY = 0.f;
	m_tFrame.iFrameCurrentX = 0.f;
	m_tFrame.iFrameStartX = 0.f;
	m_tFrame.iFrameEndX = 7.f;
	m_tFrame.iFrameHeight = 16.f;
	m_tFrame.iFrameWidth = 16.f;
	m_tFrame.iTexHeight = 16;
	m_tFrame.iTexWidth = 144.f;
	//pGameInstance->Add_GameObject(LEVEL_COLOSUSS, TEXT("Layer_Colossus_Fist"), TEXT("Prototype_GameObject_ColossusFist_Left_Shadow"), this);
	m_fSpriteSpeed = 2;


	m_fScale = (rand() % 20) * 0.1f;


	m_pTransformCom->Scaled(_float3(m_fScale, m_fScale, m_fScale));

	//m_bJump = true;


	_float3 b = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	if (b.y > 0) {
		int c = 1;
	}
	//YetiPos.y += 2.f;
	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, YetiPos);
	//m_pTransformCom->LookAt(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION));

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

_int CColosuss_Fist_Dust::Tick(_float fTimeDelta)
{
	if (0 > __super::Tick(fTimeDelta))
		return -1;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	m_iAlpha -= 3;
	/*_float3 MyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float3 vpos = m_pPlayerTransform->Get_State(CTransform::STATE_POSITION);*/

	//m_pTransformCom->Go_Straight(fTimeDelta * 7.f);
	if (m_iAlpha < 0 || m_fScale < 0) {

		RELEASE_INSTANCE(CGameInstance);
		return -1;
	}




	//m_pTransformCom->Set_State(CTransform::STATE_LOOK,_float3(0,0,0.7f));


	_float3 dir = pos - m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	m_fGoSpeed -= 0.3f;
	if (m_fGoSpeed > 0) {
		m_pTransformCom->Go_Straight(fTimeDelta * m_fGoSpeed);
		//_float3		vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		////_float3		vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);

		//vPosition += *D3DXVec3Normalize(&dir, &dir) * 5.f * fTimeDelta * m_fGoSpeed;

		//m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
	}
	m_pTransformCom->Go_Up(fTimeDelta* 0.1f);
	m_fScale -= 0.025f;
	m_pTransformCom->Scaled(_float3(m_fScale, m_fScale, m_fScale));


	Go_Frame(m_fSpriteSpeed, fTimeDelta);

	RELEASE_INSTANCE(CGameInstance);

	return _int();
}

_int CColosuss_Fist_Dust::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
	{
		MSGBOX("0 > __super::LateTick in CColosuss_Fist_Dust::LateTick");
		return -1;
	}

	if (nullptr == m_pRendererCom)
	{
		MSGBOX("m_pRendererCom is nullptr in CColosuss_Fist_Dust::LateTick");
		return -1;
	}


	_float4x4		ViewMatrix; //뷰행렬을 저장할 변수

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	//월드변환행렬로 만든다
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);
	//D3DXMatrixInverse
	//라업룩에 적용해준다
	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, (*(_float3*)&ViewMatrix.m[0][0]) * m_pTransformCom->Get_Scale().x);
	//m_pTransformCom->Set_State(CTransform::STATE_UP, (*(_float3*)&ViewMatrix.m[1][0]) * m_pTransformCom->Get_Scale().y);
	//m_pTransformCom->Set_State(CTransform::STATE_LOOK, (*(_float3*)&ViewMatrix.m[2][0]) * m_pTransformCom->Get_Scale().z);

	//m_pTransformCom->Scaled(_float3(m_fScale, m_fScale, m_fScale));

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
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHA, this);

	//m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHA, this);


	return _int();
}

HRESULT CColosuss_Fist_Dust::Render()
{
	if (nullptr == m_pVIBufferCom)
	{
		MSGBOX("m_pVIBufferCom is nullptr in CColosuss_Fist_Dust::Render")
			return E_FAIL;
	}

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
	{
		MSGBOX("m_pTransformCom->Bind_OnGraphicDevice returned E_FAIL in CColosuss_Fist_Dust::Render");
		return E_FAIL;
	}

	if (FAILED(Go_Sprites()))
	{
		MSGBOX("Go_Sprites returned E_FAIL in CYeti::Tick");
		return E_FAIL;
	}

	//m_pVIBufferCom->Base_Setting();
	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice()))
	{
		MSGBOX("m_pTextureCom->Bind_OnGraphicDevice in CColosuss_Fist_Dust::Render");
		return E_FAIL;
	}


	if (FAILED(SetUp_RenderState()))
	{
		MSGBOX("SetUp_RenderState returned E_FAIL in CColosuss_Fist_Dust::Render");
		return E_FAIL;
	}

	m_pVIBufferCom->Render();

	if (FAILED(Release_RenderState()))
	{
		MSGBOX("Release_RenderState returned E_FAIL in CColosuss_Fist_Dust::Render");
		return E_FAIL;
	}
	////////////////////
	return S_OK;
}

HRESULT CColosuss_Fist_Dust::Go_Sprites()
{
	if (FAILED(m_pVIBufferCom->Go_Sprites(m_tFrame)))
	{
		MSGBOX("m_pVIBufferCom->Go_Sprites returned E_FAIL in CYeti::Go_Sprites");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CColosuss_Fist_Dust::SetUp_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CColosuss_Fist_Dust::SetUp_Components(Transform)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CColosuss_Fist_Dust::SetUp_Components(Renderer)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CColosuss_Fist_Dust::SetUp_Components(VIBuffer_Rect)");
		return E_FAIL;
	}

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_COLOSUSS, TEXT("Prototype_Component_Texture_Colossus_Dust"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;


	return S_OK;
}

HRESULT CColosuss_Fist_Dust::SetUp_OnTerrain()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	CVIBuffer_Terrain* m_pVIBuffer_Terrain = (CVIBuffer_Terrain*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Terrain"), TEXT("Com_VIBuffer"));


	if (m_pVIBuffer_Terrain == nullptr)
		return E_FAIL;


	_float3 vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	vPosition.y = m_pVIBuffer_Terrain->Compute_Y(vPosition);
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

HRESULT CColosuss_Fist_Dust::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);//알파블렌딩을 하겠다
	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);//색을 더한다
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//그릴려고 하는 픽셀의 색상을 섞을때 소스 칼라의 알파를 섞겠다
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);
	m_pGraphic_Device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));



	//m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, TRUE);

	return S_OK;
}

HRESULT CColosuss_Fist_Dust::Release_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);

	//m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, FALSE);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pVIBufferCom->Base_Setting();

	return S_OK;
}

CColosuss_Fist_Dust* CColosuss_Fist_Dust::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CColosuss_Fist_Dust*	pInstance = new CColosuss_Fist_Dust(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CColosuss_Fist_Dust");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CColosuss_Fist_Dust::Clone(void* pArg)
{
	/* 새로운객체를 복제하여 생성한다. */
	CColosuss_Fist_Dust*	pInstance = new CColosuss_Fist_Dust(*this);


	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CColosuss_Fist_Dust");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CColosuss_Fist_Dust::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}