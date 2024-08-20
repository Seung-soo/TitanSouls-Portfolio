#include "stdafx.h"
#include "All_Door.h"
#include "GameInstance.h"


CAll_Door::CAll_Door(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CAll_Door::CAll_Door(const CAll_Door & rhs)
	: CGameObject(rhs)
{
}

HRESULT CAll_Door::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		MSGBOX("__super::NativeConstruct_Prototype returned E_FAIL in CAll_Door::NativeConstruct_Prototype");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CAll_Door::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		MSGBOX("__super::NativeConstruct returned E_FAIL in CAll_Door::NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(SetUp_Components()))
	{
		MSGBOX("SetUp_Components returned E_FAIL in CAll_Door::NativeConstruct");
		return E_FAIL;
	}

	m_pTransformCom->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian((*(POSANGLE*)pArg).fAngle));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, (*(POSANGLE*)pArg).vPos);
	m_pTransformCom->Scaled(_float3(8, 8, 0));

	return S_OK;
}

_int CAll_Door::Tick(_float fTimeDelta)
{
	if (0 > __super::Tick(fTimeDelta))
	{
		MSGBOX("0 > __super::Tick in CColossus01_Dark01::Tick");
		return -1;
	}
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (GetKeyState('Z') & 0x8000)
	{
		m_bCnt = true;
	}


	if (m_bCnt == true)
	{
		m_iCnt += 1;
	}

	if (m_bCnt == true)
	{
		if (true == bSound)
		{
			pGameInstance->LetsPlaySound(L"Door.ogg", CSoundMgr::SYSTEM_EFFECT3, 1.f);// »ç¿îµå
			bSound = false;
		}
		m_fFrame += 1.f * fTimeDelta;

		m_pTransformCom->Go_Up(fTimeDelta * 0.9f);

		if (m_fFrame >= 3.f)
		{
			m_bCnt = false;
			m_iCnt = 0;
		}
	}

	RELEASE_INSTANCE(CGameInstance);


	return _int();
}

_int CAll_Door::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
	{
		MSGBOX("0 > __super::LateTick in CColossus01_Dark01::LateTick");
		return -1;
	}

	if (nullptr == m_pRendererCom)
	{
		MSGBOX("m_pRendererCom is nullptr in CColossus01_Dark01::LateTick");
		return -1;
	}

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

HRESULT CAll_Door::Render()
{
	if (nullptr == m_pVIBufferCom)
	{
		MSGBOX("m_pVIBufferCom is nullptr in CColossus01_Dark01::Render");
		return E_FAIL;
	}

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
	{
		MSGBOX("m_pTransformCom->Bind_OnGraphicDevice returned E_FAIL in CColossus01_Dark01::Render");
		return E_FAIL;
	}

	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice()))
		return E_FAIL;

	if (FAILED(SetUp_RenderState()))
	{
		MSGBOX("SetUp_RenderState returned E_FAIL in CColossus01_Dark01::Render");
		return E_FAIL;
	}

	m_pVIBufferCom->Render();

	if (FAILED(Release_RenderState()))
	{
		MSGBOX("Release_RenderState returned E_FAIL in CColossus01_Dark01::Render");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CAll_Door::SetUp_Components()
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 2.0f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CAll_Door::SetUp_Components(Transform)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CAll_Door::SetUp_Components(Renderer)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CAll_Door::SetUp_Components(VIBuffer_Rect)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_AllDoor"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CAll_Door::SetUp_Components(Texture)");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CAll_Door::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
	{
		MSGBOX("m_pGraphic_Device is nullptr in CAll_Door::Release_RenderState");
		return E_FAIL;
	}

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);

	return S_OK;
}

HRESULT CAll_Door::Release_RenderState()
{
	if (nullptr == m_pGraphic_Device)
	{
		MSGBOX("m_pGraphic_Device is nullptr in CAll_Door::Release_RenderState");
		return E_FAIL;
	}



	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	return S_OK;
}

CAll_Door * CAll_Door::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CAll_Door*	pInstance = new CAll_Door(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create CColossus01_Dark01 in CAll_Door::Create");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CAll_Door::Clone(void * pArg)
{
	CAll_Door*	pInstance = new CAll_Door(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create CColossus01_Dark01 in CAll_Door::Clone");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CAll_Door::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
