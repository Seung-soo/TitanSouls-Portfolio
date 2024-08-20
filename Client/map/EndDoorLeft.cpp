#include "stdafx.h"
#include "EndDoorLeft.h"
#include "GameInstance.h"


CEndDoorLeft::CEndDoorLeft(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CEndDoorLeft::CEndDoorLeft(const CEndDoorLeft & rhs)
	: CGameObject(rhs)
{
}

HRESULT CEndDoorLeft::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		MSGBOX("__super::NativeConstruct_Prototype returned E_FAIL in CEndDoorLeft::NativeConstruct_Prototype");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CEndDoorLeft::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		MSGBOX("__super::NativeConstruct returned E_FAIL in CEndDoorLeft::NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(SetUp_Components()))
	{
		MSGBOX("SetUp_Components returned E_FAIL in CEndDoorLeft::NativeConstruct");
		return E_FAIL;
	}

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);



	m_pTransformCom->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian((*(POSANGLE*)pArg).fAngle));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, (*(POSANGLE*)pArg).vPos);
	m_pTransformCom->Scaled(_float3(16, 32, 0));

	m_tFrame.iTexWidth = 640.f;	// 쓰이는 이미지 전체 가로 사이즈(예를들어 512)
	m_tFrame.iTexHeight = 256.f;	// 쓰이는 이미지 전체 세로 사이즈(예를들어 512)
	m_tFrame.iFrameWidth = 128.f;// 이미지 안에서 출력할 가로 사이즈(예를들어 16)
	m_tFrame.iFrameHeight = 256.f;// 이미지 안에서 출력할 세로 사이즈(예를들어 16)
	m_tFrame.iFrameStartX = 0;	// 몇열부터 시작할 것인지
	m_tFrame.iFrameCurrentX = 0;	// 현재 열
	m_tFrame.iFrameStartY = 0;	//몇 행부터 시작할 것인지
	m_tFrame.iFrameEndX = 4;		// 마지막 열(마지막열이)

	m_fSpriteSpeed = 0.3f;

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

_int CEndDoorLeft::Tick(_float fTimeDelta)
{
	if (0 > __super::Tick(fTimeDelta))
	{
		MSGBOX("0 > __super::Tick in CColossus01_Dark01::Tick");
		return -1;
	}

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (GetKeyState('P') & 0x8000)
	{
		m_bDoorSprite = true;
	}

	if (true == m_bDoorSprite)
	{
		Go_Frame(m_fSpriteSpeed, fTimeDelta);
		m_fFrame += 1.0f * fTimeDelta;

		if (1.f <= m_fFrame)
		{
			m_iCnt += 1;
			m_fFrame = 0.f;
		}

		if (m_iCnt == 3)
		{
			m_bDoorSprite = false;
		}
	}

	//

	if (GetKeyState('L') & 0x8000)
	{
		m_bDoorOpen = true;
	}

	if (true == m_bDoorOpen)
	{
		fDoorOpenFrame += 1.f * fTimeDelta;
		m_pTransformCom->Revolution(_float3(0.f, 1.f, 0.f), _float3(27.f, 0.f, 0.01f), fTimeDelta * 0.05f);

		if (10.f <= fDoorOpenFrame)
		{
			m_bDoorOpen = false;
			fDoorOpenFrame = 0.f;
		}
	}


	RELEASE_INSTANCE(CGameInstance);

	return _int();
}

_int CEndDoorLeft::LateTick(_float fTimeDelta)
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

HRESULT CEndDoorLeft::Render()
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

	if (FAILED(Go_Sprites()))
	{
		MSGBOX("Go_Sprites returned E_FAIL in CYeti::Tick");
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

HRESULT CEndDoorLeft::Go_Sprites()
{
	if (FAILED(m_pVIBufferCom->Go_Sprites(m_tFrame)))
	{
		MSGBOX("m_pVIBufferCom->Go_Sprites returned E_FAIL in CYeti::Go_Sprites");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CEndDoorLeft::SetUp_Components()
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 2.0f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CEndDoorLeft::SetUp_Components(Transform)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CEndDoorLeft::SetUp_Components(Renderer)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CEndDoorLeft::SetUp_Components(VIBuffer_Rect)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(g_eCurrentLevel, TEXT("Prototype_Component_Texture_EndDoorLeft"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CEndDoorLeft::SetUp_Components(Texture)");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CEndDoorLeft::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
	{
		MSGBOX("m_pGraphic_Device is nullptr in CEndDoorLeft::Release_RenderState");
		return E_FAIL;
	}
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 50);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);

	return S_OK;
}

HRESULT CEndDoorLeft::Release_RenderState()
{
	if (nullptr == m_pGraphic_Device)
	{
		MSGBOX("m_pGraphic_Device is nullptr in CEndDoorLeft::Release_RenderState");
		return E_FAIL;
	}
	m_pVIBufferCom->Base_Setting();
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	return S_OK;
}

CEndDoorLeft * CEndDoorLeft::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CEndDoorLeft*	pInstance = new CEndDoorLeft(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create CColossus01_Dark01 in CEndDoorLeft::Create");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CEndDoorLeft::Clone(void * pArg)
{
	CEndDoorLeft*	pInstance = new CEndDoorLeft(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create CColossus01_Dark01 in CEndDoorLeft::Clone");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEndDoorLeft::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
