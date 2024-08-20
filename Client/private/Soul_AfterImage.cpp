#include "stdafx.h"
#include "..\public\Soul_AfterImage.h"
#include "GameInstance.h"

CSoul_AfterImage::CSoul_AfterImage(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CSoul_AfterImage::CSoul_AfterImage(const CSoul_AfterImage& rhs)
	: CGameObject(rhs)
{
}

HRESULT CSoul_AfterImage::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSoul_AfterImage::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	/* 현재 객체에게 추가되어야할 컴포넌트들을 복제(or 참조)하여 멤버변수에 보관한다.  */
	if (FAILED(SetUp_Components()))
		return E_FAIL;
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	m_pSoul = (CSoul*)pArg;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, static_cast<CTransform*>(m_pSoul->Get_Component(TEXT("Com_Transform")))->Get_State(CTransform::STATE_POSITION));


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

_int CSoul_AfterImage::Tick(_float fTimeDelta)
{
	if (0 > __super::Tick(fTimeDelta))
		return -1;

	m_iAlpha -= 5;
	m_fScale -= 0.01f;
	m_pTransformCom->Scaled(_float3(m_fScale, m_fScale, m_fScale));

	if (m_iAlpha < 0) 
		return -1;



	return _int();
}

_int CSoul_AfterImage::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
	{
		MSGBOX("0 > __super::LateTick in CSoul_AfterImage::LateTick");
		return -1;
	}

	if (nullptr == m_pRendererCom)
	{
		MSGBOX("m_pRendererCom is nullptr in CSoul_AfterImage::LateTick");
		return -1;
	}

	/*if (FAILED(SetUp_OnTerrain()))
	return -1;*/
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CTransform* pCameraTrans = (CTransform*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Camera"), TEXT("Com_Transform"));
	_float3 vCameraPos = pCameraTrans->Get_State(CTransform::STATE_POSITION);
	m_pTransformCom->MatchIt_XZ(vCameraPos);
	RELEASE_INSTANCE(CGameInstance);
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

	return _int();
}

HRESULT CSoul_AfterImage::Render()
{
	if (nullptr == m_pVIBufferCom)
	{
		MSGBOX("m_pVIBufferCom is nullptr in CSoul_AfterImage::Render")
			return E_FAIL;
	}

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
	{
		MSGBOX("m_pTransformCom->Bind_OnGraphicDevice returned E_FAIL in CSoul_AfterImage::Render");
		return E_FAIL;
	}

	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice()))
	{
		MSGBOX("m_pTextureCom->Bind_OnGraphicDevice in CColosuss_Left_Fist_afterimage::Render");
		return E_FAIL;
	}


	if (FAILED(SetUp_RenderState()))
	{
		MSGBOX("SetUp_RenderState returned E_FAIL in CColosuss_Left_Fist_afterimage::Render");
		return E_FAIL;
	}

	m_pVIBufferCom->Render();

	if (FAILED(Release_RenderState()))
	{
		MSGBOX("Release_RenderState returned E_FAIL in CColosuss_Left_Fist_afterimage::Render");
		return E_FAIL;
	}
	////////////////////
	return S_OK;
}

HRESULT CSoul_AfterImage::SetUp_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CSoul_AfterImage::SetUp_Components(Transform)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CSoul_AfterImage::SetUp_Components(Renderer)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CSoul_AfterImage::SetUp_Components(VIBuffer_Rect)");
		return E_FAIL;
	}

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Soul"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;


	return S_OK;
}

HRESULT CSoul_AfterImage::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	//m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	//m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
	//m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);
	//m_pGraphic_Device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, 255, 255, 255));

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);//알파블렌딩을 하겠다
	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);//색을 더한다
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//그릴려고 하는 픽셀의 색상을 섞을때 소스 칼라의 알파를 섞겠다
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);



	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 0);
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, TRUE);

	return S_OK;
}

HRESULT CSoul_AfterImage::Release_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	//m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);

	//m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, FALSE);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);


	return S_OK;
}

CSoul_AfterImage* CSoul_AfterImage::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSoul_AfterImage*	pInstance = new CSoul_AfterImage(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CSoul_AfterImage");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CSoul_AfterImage::Clone(void* pArg)
{
	/* 새로운객체를 복제하여 생성한다. */
	CSoul_AfterImage*	pInstance = new CSoul_AfterImage(*this);


	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CSoul_AfterImage");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSoul_AfterImage::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}