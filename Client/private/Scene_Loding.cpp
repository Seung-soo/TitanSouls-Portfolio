#include "stdafx.h"
#include "..\public\Scene_Loding.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "Player.h"

CScene_Loading::CScene_Loading(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUI(pGraphic_Device)
{
}

CScene_Loading::CScene_Loading(const CScene_Loading & rhs)
	: CUI(rhs)
{
}

HRESULT CScene_Loading::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		MSGBOX("__super::NativeConstruct_Prototype returned E_FAIL in CScene_Loading::NativeConstruct_Prototype");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CScene_Loading::NativeConstruct(void * pArg)
{

	if (FAILED(SetUp_Components()))
		return E_FAIL;
	m_tSceneChange = *(SCENECHANGE*)pArg;
	if (m_tSceneChange.bCrease == false)
		m_fFrame = 0.f;
	else
		m_fFrame = 19.f;
	//static const unsigned int g_iWinCX = 1280;
	//static const unsigned int g_iWinCY = 720;

	m_fX = g_iWinCX * 0.5f;
	m_fY = g_iWinCY * 0.5f;
	m_fSizeX = g_iWinCX;
	m_fSizeY = g_iWinCY;

	m_pTransformCom->Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_fX - g_iWinCX * 0.5f, -m_fY + g_iWinCY * 0.5f, 0.f));

	m_iDepth = 0;

	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;


	return S_OK;
}

_int CScene_Loading::Tick(_float fTimeDelta)
{
	if (0 > __super::Tick(fTimeDelta))
		return -1;

	//if (GetKeyState('Z') & 0x8000)
	//{

	//	m_bCnt = true;
	//
	//}

	if (m_tSceneChange.bCrease == false)
	{
		m_fFrame += 19.0f * fTimeDelta;

		if (m_fFrame >= 19.0f)
		{
			CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
			CTransform* pPlayerTrans = (CTransform*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"));
			g_eNextLevel = (LEVEL)m_tSceneChange.iLevelIndex;
			g_bGoLevel = true;
			switch (g_eCurrentLevel)
			{
			case LEVEL_RESPAWN1:
				pPlayerTrans->Set_State(CTransform::STATE_POSITION, _float3(9.f, 0.5f, 3.f));
				break;
			case LEVEL_RESPAWN2:
				pPlayerTrans->Set_State(CTransform::STATE_POSITION, _float3(32.f, 0.5f, 3.f));
				break;
			case LEVEL_SLIME:
				pPlayerTrans->Set_State(CTransform::STATE_POSITION, _float3(15.5f, 0.f, 46.0f));
				break;
			case LEVEL_ICEBRAIN:
				pPlayerTrans->Set_State(CTransform::STATE_POSITION, _float3(48.5f, 0.5f, 41.5f));
				break;
			case LEVEL_COLOSUSS:
				pPlayerTrans->Set_State(CTransform::STATE_POSITION, _float3(32.f, 0.5f, 32.f));
				break;
			case LEVEL_EYECUBE:
				pPlayerTrans->Set_State(CTransform::STATE_POSITION, _float3(55.5f, 0.5f, 40.f));
				break;
			case LEVEL_YETI:
				pPlayerTrans->Set_State(CTransform::STATE_POSITION, _float3(15.5f, 0.5f, 41.5f));
				break;
			}
			RELEASE_INSTANCE(CGameInstance);
			return -1;
			//m_fFrame = 0.f;
			//m_bCnt = false;
		}	
	}
	else
	{
		m_fFrame -= 19.0f * fTimeDelta;

		if (m_fFrame <= 0.f)
		{

			return -1;
			//m_fFrame = 0.f;
			//m_bCnt = false;
		}
	}

	
	return _int();
}

_int CScene_Loading::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
	{
		MSGBOX("0 > __super::LateTick in CScene_Loading::LateTick");
		return -1;
	}

	//if (nullptr == m_pRendererCom)
	//{
	//	MSGBOX("m_pRendererCom is nullptr in CScene_Loading::LateTick");
	//	return -1;
	//}

	//m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

	return _int();
}

HRESULT CScene_Loading::Render()
{	
	if (0 > __super::Render())
	{
		MSGBOX("0 > __super::Render in CScene_Loading::Render");
		return -1;
	}

	m_pVIBufferCom->Base_Setting();

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
	{
		MSGBOX("m_pTransformCom->Bind_OnGraphicDevice returned E_FAIL in CScene_Loading::Render");
		return E_FAIL;
	}

	//_float4x4		ViewMatrix;
	//D3DXMatrixIdentity(&ViewMatrix);
	//m_pGraphic_Device->SetTransform(D3DTS_VIEW, &ViewMatrix);
	//m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice((_uint)m_fFrame)))
		return E_FAIL;


	//알파 테스팅

	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 0);
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);


	m_pVIBufferCom->Render();


	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);


	return S_OK;
}

HRESULT CScene_Loading::SetUp_Components()
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 0;
	TransformDesc.fRotationPerSec = D3DXToRadian(0.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CScene_Loading::SetUp_Components(Transform)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CScene_Loading::SetUp_Components(Renderer)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CScene_Loading::SetUp_Components(VIBuffer_Rect)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_SceneLoading"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CScene_Loading::SetUp_Components(Texture)");
		return E_FAIL;
	}

	return S_OK;
}

CScene_Loading * CScene_Loading::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CScene_Loading*	pInstance = new CScene_Loading(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create Aim in CScene_Loading::Create");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CScene_Loading::Clone(void * pArg)
{
	CGameObject*	pInstance = new CScene_Loading(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create CScene_Loading in CScene_Loading::Clone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CScene_Loading::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
