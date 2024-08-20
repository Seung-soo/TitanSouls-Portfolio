#include "stdafx.h"
#include "..\public\Arrow.h"
#include "GameInstance.h"
#include "Player.h"
#include "Brain.h"
#include "Yeti.h"
#include "Colossus.h"
#include "EyeCube.h"
#include "Fire.h"
#include "Ice_Brain.h"
#include "Brother.h"

CArrow::CArrow(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CArrow::CArrow(const CArrow & rhs)
	: CGameObject(rhs)
{
}

HRESULT CArrow::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		MSGBOX("__super::NativeConstruct_Prototype returned E_FAIL in CArrow::NativeConstruct_Prototype");
		return E_FAIL;
	}


	return S_OK;
}

HRESULT CArrow::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		MSGBOX("__super::NativeConstruct returned E_FAIL in CArrow::NativeConstruct");
		return E_FAIL;
	}

	m_pPlayer = (CPlayer*)pArg;

	m_tFrame.iFrameCurrentX = 31.f;
	m_tFrame.iFrameEndX = 31.f;
	m_tFrame.iFrameHeight = 16.f;
	m_tFrame.iFrameWidth = 16.f;
	m_tFrame.iFrameStartY = 0.f;
	m_tFrame.iTexHeight = 512.f;
	m_tFrame.iTexWidth = 512.f;

	// 컴포넌트 세팅
	if (FAILED(SetUp_Components()))
	{
		MSGBOX("SetUp_Components returned E_FAIL in CArrow::NativeConstruct");
		return E_FAIL;
	}

	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);

	if (false == g_bColossusClear)
		pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Layer_ArrowLight"), TEXT("Prototype_GameObject_ArrowLight"), this);

	CTransform* pPlayerTrans = static_cast<CTransform*>(pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform")));
	_float3 vPlayerRight = pPlayerTrans->Get_State(CTransform::STATE_RIGHT);
	D3DXVec3Normalize(&vPlayerRight, &vPlayerRight);
	_float3 vPlayerUp = pPlayerTrans->Get_State(CTransform::STATE_UP);
	D3DXVec3Normalize(&vPlayerUp, &vPlayerUp);
	_float3 vPlayerLook = pPlayerTrans->Get_State(CTransform::STATE_LOOK);
	D3DXVec3Normalize(&vPlayerLook, &vPlayerLook);

	RELEASE_INSTANCE(CGameInstance);
	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vPlayerRight);
	m_pTransformCom->Set_State(CTransform::STATE_UP, vPlayerUp);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, vPlayerLook);
	_float3 vPlayerPos = pPlayerTrans->Get_State(CTransform::STATE_POSITION);

	vPlayerPos += vPlayerLook;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPlayerPos);
	// 화살 크기 조절
	m_pTransformCom->Scaled(_float3(1.5f, 1.5f, 1.5f));

	return S_OK;
}

_int CArrow::Tick(_float fTimeDelta)
{
	if (0 > __super::Tick(fTimeDelta))
	{
		MSGBOX("0 > __super::Tick in CArrow::Tick");
		return -1;
	}
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	if (true == m_bGoAlpha)
	{
		--m_iAlpha;
		if (m_iAlpha <= -60)
		{
			CCamera_Player* pCamera = static_cast<CCamera_Player*>(pGameInstance->Get_Front(LEVEL_STATIC, TEXT("Layer_Camera")));
			m_pPlayer = static_cast<CPlayer*>(pGameInstance->Get_Front(LEVEL_STATIC, TEXT("Layer_Player")));
			pCamera->Set_EndingSoulHit(false);
			pCamera->Set_EndingExit(true);
			g_bCameraDirecting = true;
			static_cast<CTransform*>(pCamera->Get_Component(TEXT("Com_Transform")))->Set_State(CTransform::STATE_POSITION, _float3(16.f, 14.f, 18.f));
			static_cast<CTransform*>(pCamera->Get_Component(TEXT("Com_Transform")))->LookAt(_float3(16.f, 14.f, 10.f));
			m_pPlayer->Get_TransForm()->Set_State(CTransform::STATE_POSITION, _float3(16.f, 0.5f, 3.f));
			m_pPlayer->Get_TransForm()->LookAt(_float3(16.f, 0.5f, 0.f));
			RELEASE_INSTANCE(CGameInstance);
			return -1;
		}
	}


	CCollider_Sphere::SPHEREDESC SphereDesc;
	SphereDesc.ObjectMatrix = m_pTransformCom->Get_WorldMatrix();
	SphereDesc.fRadius = 0.12f;
	SphereDesc.fDist_From_Obj = _float3(0.f, 0.f, 0.25f);
	m_pColliderCom->Set_SphereDesc(SphereDesc);
	m_pColliderCom->Tick();

	if (pGameInstance->Collision_Sphere(m_pColliderCom, (CCollider_Sphere*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Collider"))))
	{

		// ArrowPull 추가
		pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Layer_Arrow_Rect"), TEXT("Prototype_GameObject_ArrowPull"), this);
		pGameInstance->StopSound(CSoundMgr::PLAYER_SHOT);
		pGameInstance->StopSound(CSoundMgr::SYSTEM_EFFECT3);
		pGameInstance->LetsPlaySound(L"Arrow_tick.ogg", CSoundMgr::SYSTEM_EFFECT3, 1.f);
		m_bObjectDead = true;
		m_bArrow_Rect = false; // ArrowRect추가
		m_pPlayer->Set_Arrow(true, false);
		RELEASE_INSTANCE(CGameInstance);
		return -1;
	}

	if (m_bFire == true && false == m_pPlayer->Get_HaveArrow())
	{
		pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Layer_Arrow_FireRect"), TEXT("Prototype_GameObject_ArrowRect"), this);
	}

	RELEASE_INSTANCE(CGameInstance);
	return _int();
}

_int CArrow::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
	{
		MSGBOX("0 > __super::LateTick in CArrow::LateTick");
		return -1;
	}

	if (nullptr == m_pRendererCom)
	{
		MSGBOX("m_pRendererCom is nullptr in CArrow::LateTick");
		return -1;
	}

	if (0 > Collision_Rect())
		return -1;
	Gravity(fTimeDelta);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

HRESULT CArrow::Render()
{
	if (nullptr == m_pVIBufferCom)
	{
		MSGBOX("m_pVIBufferCom is nullptr in CArrow::Render")
			return E_FAIL;
	}



	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
	{
		MSGBOX("m_pTransformCom->Bind_OnGraphicDevice returned E_FAIL in CPlayer::Render");
		return E_FAIL;
	}

	m_pVIBufferCom->Go_Sprites_Reverse_Y(m_tFrame);

	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice()))
	{
		MSGBOX("m_pTextureCom->Bind_OnGraphicDevice in CArrow::Render");
		return E_FAIL;
	}

	if (FAILED(SetUp_RenderState()))
	{
		MSGBOX("SetUp_RenderState returned E_FAIL in CArrow::Render");
		return E_FAIL;
	}


	m_pVIBufferCom->Render();

	if (FAILED(Release_RenderState()))
	{
		MSGBOX("Release_RenderState returned E_FAIL in CArrow::Render");
		return E_FAIL;
	}

//#ifdef _DEBUG
//	CCollider_Sphere::SPHEREDESC tSphereDesc = m_pColliderCom->Get_SphereDesc();
//	m_pVIBufferCollCom->MakeHitBox(tSphereDesc);
//	m_pGraphic_Device->SetTexture(0, FALSE);
//	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
//	//m_pVIBufferCollCom->Render();
//	m_pVIBufferCollCom->BaseSetting();
//	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
//#endif // _DEBUG


	return S_OK;
}

HRESULT CArrow::SetUp_Components()
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 1.5f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CArrow::SetUp_Components(Transform)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CArrow::SetUp_Components(Renderer)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect_Z"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CArrow::SetUp_Components(VIBuffer_RectZ)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Arrow"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CArrow::SetUp_Components(Texture)");
		return E_FAIL;
	}

	CCollider_Sphere::SPHEREDESC SphereDesc;
	SphereDesc.ObjectMatrix = m_pTransformCom->Get_WorldMatrix();
	SphereDesc.fRadius = 0.12f;
	SphereDesc.fDist_From_Obj = _float3(0.f, 0.f, 0.25f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"), TEXT("Com_Collider"), (CComponent**)&m_pColliderCom, &SphereDesc)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CArrow::SetUp_Components(Collider_Sphere)");
		return E_FAIL;
	}

//#ifdef _DEBUG
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"), TEXT("Com_VIBuffer_Coll"), (CComponent**)&m_pVIBufferCollCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CArrow::SetUp_Components(VIBuffer_Cube)");
		return E_FAIL;
	}
//#endif // DEBUG

	return S_OK;
}

HRESULT CArrow::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
	{
		MSGBOX("m_pGraphic_Device is nullptr in CPlayer::SetUp_RenderState");
	}

	if (LEVEL_COLOSUSS == g_eCurrentLevel && true == g_bColossusClear)
	{
		m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TFACTOR);
		m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TEXTURE);
	}

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 0);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);
	return S_OK;
}

HRESULT CArrow::Release_RenderState()
{
	if (g_eCurrentLevel == LEVEL_COLOSUSS && true == g_bColossusClear)
	{
		m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
		m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
	}

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

_float3 CArrow::Get_FirePos()
{
	return m_pColliderCom->Get_Position();
}

_int CArrow::Collision_Rect()
{

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	if (LEVEL_LOADING != g_eCurrentLevel)
		for (_uint i = 0; i < pGameInstance->Get_Size(g_eCurrentLevel, TEXT("Layer_Wall")); i++)
		{

			CCollider_Rect* pColWall = (CCollider_Rect*)pGameInstance->Get_Component(g_eCurrentLevel, TEXT("Layer_Wall"), TEXT("Com_Collider"), i);
			if (true == m_bFullCharging)
			{
				
				_float3 vPos = pGameInstance->Collision_SphereToRectArrowFull(pColWall, m_pColliderCom);
				if (vPos != _float3(0.f, 0.f, 0.f))
				{
					if (m_bSound)
					{
						pGameInstance->StopSound(CSoundMgr::SYSTEM_EFFECT2);
						pGameInstance->LetsPlaySound(L"Arrow_tick.ogg", CSoundMgr::SYSTEM_EFFECT2, 1.f);
						m_bSound = false;
					}

					if (pGameInstance->Get_DIKeyState(DIK_E))
					{
						m_bFullCharging = false;
						m_pPlayer->Set_MoveEndArrow(true);
						//m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos + m_pTransformCom->Get_State(CTransform::STATE_LOOK) * 10.f);
						//m_pPlayer->Set_ShotPower(30.f);
						break;
					}
					m_pPlayer->Set_ShotPower(0.f);
					m_fGravity = -0.03f;
					
					if (g_eCurrentLevel == LEVEL_ICEBRAIN)
					{
						m_bFire = false;
						m_bFireOff = true;
					}
				}
			}
			else
			{
				_float3 vPos = pGameInstance->Collision_SphereToRectArrow(pColWall, m_pColliderCom);
				if (vPos != _float3(0.f, 0.f, 0.f))
				{
					pGameInstance->StopSound(CSoundMgr::SYSTEM_EFFECT2);
					pGameInstance->LetsPlaySound(L"Arrow_tick.ogg", CSoundMgr::SYSTEM_EFFECT2, 1.f);

					pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Layer_Arrow_Rect"), TEXT("Prototype_GameObject_ArrowPull"), this);
					if (g_eCurrentLevel == LEVEL_ICEBRAIN)
					{
						m_bFire = false;
						m_bFireOff = true;
					}
					m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);

					_float3 vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
					_float3 vWallLook = *(_float3*)pColWall->Get_RectDesc().ObjectMatrix.m[CTransform::STATE_LOOK];
					D3DXVec3Normalize(&vLook, &-vLook);
					D3DXVec3Normalize(&vWallLook, &vWallLook);

					_float fScale = D3DXVec3Dot(&vLook, &vWallLook);
					vWallLook = vWallLook * fScale * 2.f;

					_float3 vReflectVector = vWallLook - vLook;
					D3DXVec3Normalize(&vReflectVector, &vReflectVector);

					_float3 vRight = *D3DXVec3Cross(&vRight, &_float3(0.f, 1.f, 0.f), &vReflectVector);
					vRight = *D3DXVec3Normalize(&vRight, &vRight);

					_float3 vUp = *D3DXVec3Cross(&vUp, &vReflectVector, &vRight);
					vUp = *D3DXVec3Normalize(&vUp, &vUp);

					m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight);
					m_pTransformCom->Set_State(CTransform::STATE_UP, vUp);
					m_pTransformCom->Set_State(CTransform::STATE_LOOK, vReflectVector);

					m_pPlayer->Set_ShotPower(0.6f);
					//_float3 vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
					//_float3 vWallLook = *(_float3*)pColWall->Get_RectDesc().ObjectMatrix.m[CTransform::STATE_LOOK];
					//D3DXVec3Normalize(&vLook, &vLook);
					//D3DXVec3Normalize(&vWallLook, &-vWallLook);
					//vLook += 1.5f * vWallLook;
					//D3DXVec3Normalize(&vLook, &vLook);
					//vLook *= 1.5f;

					//_float3 vRight = *D3DXVec3Cross(&vRight, &_float3(0.f, 1.f, 0.f), &vLook);
					//vRight = *D3DXVec3Normalize(&vRight, &vRight) * 1.5f;

					//_float3 vUp = *D3DXVec3Cross(&vUp, &vLook, &vRight);
					//vUp = *D3DXVec3Normalize(&vUp, &vUp) * 1.5f;

					//m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight);
					//m_pTransformCom->Set_State(CTransform::STATE_UP, vUp);
					//m_pTransformCom->Set_State(CTransform::STATE_LOOK, vLook);
					//m_pPlayer->Set_ShotPower(0.6f);
				}
			}
			
		}

	switch (g_eCurrentLevel)
	{
	case LEVEL_FIELD:
		break;
	case LEVEL_SLIME:
	{
		if (m_pPlayer->Get_HaveArrow() == true)
			break;
		CCollider_Rect* pColWall = (CCollider_Rect*)pGameInstance->Get_Component(g_eCurrentLevel, TEXT("Layer_Wall"), TEXT("Com_Collider"), 4);
		_float3 vPos = pGameInstance->Collision_SphereToRectArrow(pColWall, m_pColliderCom);
		if (vPos != _float3(0.f, 0.f, 0.f))
		{
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
			_float3 vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
			_float3 vWallLook = *(_float3*)pColWall->Get_RectDesc().ObjectMatrix.m[CTransform::STATE_LOOK];
			D3DXVec3Normalize(&vLook, &vLook);
			D3DXVec3Normalize(&vWallLook, &-vWallLook);
			vLook += 1.5f * vWallLook;
			D3DXVec3Normalize(&vLook, &vLook);
			vLook *= 1.5f;

			_float3 vRight = *D3DXVec3Cross(&vRight, &_float3(0.f, 1.f, 0.f), &vLook);
			vRight = *D3DXVec3Normalize(&vRight, &vRight) * 1.5f;

			_float3 vUp = *D3DXVec3Cross(&vUp, &vLook, &vRight);
			vUp = *D3DXVec3Normalize(&vUp, &vUp) * 1.5f;

			m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight);
			m_pTransformCom->Set_State(CTransform::STATE_UP, vUp);
			m_pTransformCom->Set_State(CTransform::STATE_LOOK, vLook);
			m_pPlayer->Set_ShotPower(0.6f);
		}

		if (0 == pGameInstance->Get_Size(g_eCurrentLevel, TEXT("Layer_Slime_Boss")))
			if (pGameInstance->Collision_Sphere((CCollider_Sphere*)pGameInstance->Get_Component(g_eCurrentLevel, TEXT("Layer_Slime_Heart"), TEXT("Com_Collider")), m_pColliderCom))
			{
				if (m_bArrowDrive == true)
				{
					m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTransformCom->Get_State(CTransform::STATE_POSITION) + m_pTransformCom->Get_State(CTransform::STATE_LOOK) * 0.25f);
					pGameInstance->Release_LayerObject(LEVEL_STATIC, TEXT("Layer_ArrowLight"));
					m_bArrowDrive = false;
				}
				m_pPlayer->Set_ShotPower(0.f);
				m_fGravity = -0.03f;
			}
	}	
		// 슬라임 분열시키기	// 구 충돌

		// 심장 죽이기			// 구 충돌
	

		break;
	case LEVEL_ICEBRAIN:
		// 아이스 큐브에 튕기기	// 큐브 충돌
		// 브레인 죽이기		// 구 충돌
	{
		_float3 vNewLook = pGameInstance->Collision_SphereBounce((CCollider_Sphere*)pGameInstance->Get_Component(g_eCurrentLevel, TEXT("Layer_Ice"), TEXT("Com_Collider")), m_pColliderCom, m_pTransformCom->Get_State(CTransform::STATE_LOOK)) * 1.5f;
		if (_float3(0.f, 0.f, 0.f) != vNewLook)
		{
			if (m_bFire == true)
			{
				pGameInstance->LetsPlaySound(L"ArrowIgnite.ogg", CSoundMgr::PLAYER_SHOT, 1.f);
				static_cast<CIce_Brain*>(pGameInstance->Get_Front(g_eCurrentLevel, TEXT("Layer_Ice")))->Set_Phase(true);
			}

			m_bFireOff = true;
			m_bFire = false;
			_float3 vPos = pGameInstance->Collision_SphereGuard((CCollider_Sphere*)pGameInstance->Get_Component(g_eCurrentLevel, TEXT("Layer_Ice"), TEXT("Com_Collider")), m_pColliderCom);

			if (pGameInstance->Find_Layer(LEVEL_ICEBRAIN, TEXT("Layer_Ice"))->Get_Object().size() == 2)
			{
				m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
				_float3 vRight = *D3DXVec3Cross(&vRight, &_float3(0.f, 1.f, 0.f), &vNewLook);
				vRight = *D3DXVec3Normalize(&vRight, &vRight) * 1.5f;

				_float3 vUp = *D3DXVec3Cross(&vUp, &vNewLook, &vRight);
				vUp = *D3DXVec3Normalize(&vUp, &vUp) * 1.5f;

				m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight);
				m_pTransformCom->Set_State(CTransform::STATE_UP, vUp);
				m_pTransformCom->Set_State(CTransform::STATE_LOOK, vNewLook);
				m_pPlayer->Set_ShotPower(0.6f);
			}

			if (pGameInstance->Find_Layer(LEVEL_ICEBRAIN, TEXT("Layer_Ice"))->Get_Object().size() == 1)
			{
				if (m_bArrowDrive == true)
				{
					if (false == static_cast<CBrain*>(pGameInstance->Get_Front(LEVEL_ICEBRAIN, TEXT("Layer_Ice")))->Get_Dead())
					{
						static_cast<CBrain*>(pGameInstance->Get_Front(LEVEL_ICEBRAIN, TEXT("Layer_Ice")))->Set_Dead(true);
						pGameInstance->Add_GameObject(g_eCurrentLevel, TEXT("Layer_DarkScreen"), TEXT("Prototype_GameObject_Effect_DarkScreen"));
						pGameInstance->Add_GameObject(g_eCurrentLevel, TEXT("Layer_WhiteScreen"), TEXT("Prototype_GameObject_Effect_WhiteScreen"));
					}
					m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTransformCom->Get_State(CTransform::STATE_POSITION) + m_pTransformCom->Get_State(CTransform::STATE_LOOK) * 0.25f);
					pGameInstance->Release_LayerObject(LEVEL_STATIC, TEXT("Layer_ArrowLight"));
					m_bArrowDrive = false;
				}
				m_pPlayer->Set_ShotPower(0.f);
				m_fGravity = -0.03f;
			}
		}
		--m_iArrowFireTime;
		// 불화살				// 구 충돌
		if ((nullptr != pGameInstance->Find_Layer(LEVEL_ICEBRAIN, TEXT("Layer_Fire")) && 0 < pGameInstance->Get_Size(LEVEL_ICEBRAIN, TEXT("Layer_Fire"))) && true == pGameInstance->Collision_Sphere((CCollider_Sphere*)pGameInstance->Get_Component(LEVEL_ICEBRAIN, TEXT("Layer_Fire"), TEXT("Com_Collider")), m_pColliderCom))
		{
			if (0 > m_iArrowFireTime && m_bFire == false && true == static_cast<CFire*>(pGameInstance->Get_Front(LEVEL_ICEBRAIN, TEXT("Layer_Fire")))->Get_ButtonOn())
			{
				m_bFire = true;
				pGameInstance->Add_GameObject(LEVEL_ICEBRAIN, TEXT("Layer_ArrowFire"), TEXT("Prototype_GameObject_ArrowFire"), this);
				m_iArrowFireTime = 60;
			}
		}
		if (true == m_bFire && m_pTransformCom->Get_State(CTransform::STATE_POSITION).y < 0.5f)
		{
			m_bFire = false;
			m_bFireOff = true;
		}
	}
		break;
	case LEVEL_COLOSUSS:
		if (true == g_bColossusClear)
		{
			if (pGameInstance->Get_Size(g_eCurrentLevel, TEXT("Layer_Brother")) != 0)
				if (pGameInstance->Collision_Sphere(static_cast<CCollider_Sphere*>(pGameInstance->Get_Component(g_eCurrentLevel, TEXT("Layer_Brother"), TEXT("Com_Collider"))), m_pColliderCom))
				{
					if (true == m_bColStart)
					{
						pGameInstance->LetsPlaySound(L"Kill.ogg", CSoundMgr::SYSTEM_EFFECT, 1.f);

						static_cast<CTransform*>(pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Camera"), TEXT("Com_Transform")))->Set_State(CTransform::STATE_POSITION, _float3(16.f, 1.f, 21.f));
						static_cast<CTransform*>(pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Camera"), TEXT("Com_Transform")))->LookAt(_float3(16.f, 0.f, 25.f));
						static_cast<CCamera_Player*>(pGameInstance->Get_Front(LEVEL_STATIC, TEXT("Layer_Camera")))->Set_EndingSoulHit(true);
						g_bCameraDirecting = true;
						m_bColStart = false;
					}
					static_cast<CBrother*>(pGameInstance->Get_Front(g_eCurrentLevel, TEXT("Layer_Brother")))->Set_GoAlpha(true);
					m_bGoAlpha = true;
					g_bTimeSlow = true;
				}
			break;
		}
		// 주먹에 튕기기		// 구 충돌
	{
		if (m_pPlayer->Get_HaveArrow() == true)
			break;
		_float3 vNewLook = pGameInstance->Collision_SphereBounce((CCollider_Sphere*)pGameInstance->Get_Component(g_eCurrentLevel, TEXT("Layer_Clossus_Fist_Left"), TEXT("Com_Collider")), m_pColliderCom, m_pTransformCom->Get_State(CTransform::STATE_LOOK)) * 1.5f;
		if (_float3(0.f, 0.f, 0.f) != vNewLook)
		{
			_float3 vPos = pGameInstance->Collision_SphereGuard((CCollider_Sphere*)pGameInstance->Get_Component(g_eCurrentLevel, TEXT("Layer_Clossus_Fist_Left"), TEXT("Com_Collider")), m_pColliderCom);
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
			_float3 vRight = *D3DXVec3Cross(&vRight, &_float3(0.f, 1.f, 0.f), &vNewLook);
			vRight = *D3DXVec3Normalize(&vRight, &vRight) * 1.5f;

			_float3 vUp = *D3DXVec3Cross(&vUp, &vNewLook, &vRight);
			vUp = *D3DXVec3Normalize(&vUp, &vUp) * 1.5f;

			m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight);
			m_pTransformCom->Set_State(CTransform::STATE_UP, vUp);
			m_pTransformCom->Set_State(CTransform::STATE_LOOK, vNewLook);
			m_pPlayer->Set_ShotPower(0.6f);
		}
		vNewLook = pGameInstance->Collision_SphereBounce((CCollider_Sphere*)pGameInstance->Get_Component(g_eCurrentLevel, TEXT("Layer_Clossus_Fist_Right"), TEXT("Com_Collider")), m_pColliderCom, m_pTransformCom->Get_State(CTransform::STATE_LOOK)) * 1.5f;
		if (_float3(0.f, 0.f, 0.f) != vNewLook)
		{
			_float3 vPos = pGameInstance->Collision_SphereGuard((CCollider_Sphere*)pGameInstance->Get_Component(g_eCurrentLevel, TEXT("Layer_Clossus_Fist_Right"), TEXT("Com_Collider")), m_pColliderCom);
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
			_float3 vRight = *D3DXVec3Cross(&vRight, &_float3(0.f, 1.f, 0.f), &vNewLook);
			vRight = *D3DXVec3Normalize(&vRight, &vRight) * 1.5f;

			_float3 vUp = *D3DXVec3Cross(&vUp, &vNewLook, &vRight);
			vUp = *D3DXVec3Normalize(&vUp, &vUp) * 1.5f;

			m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight);
			m_pTransformCom->Set_State(CTransform::STATE_UP, vUp);
			m_pTransformCom->Set_State(CTransform::STATE_LOOK, vNewLook);
			m_pPlayer->Set_ShotPower(0.6f);
		}

		// 콜로서스 약점 죽이기	// 구 충돌
		if (pGameInstance->Collision_Sphere((CCollider_Sphere*)pGameInstance->Get_Component(LEVEL_COLOSUSS, TEXT("Layer_Colossus"), TEXT("Com_Weak")), m_pColliderCom))
		{
			if (m_bArrowDrive == true)
			{
				if (false == static_cast<CColossus*>(pGameInstance->Get_Front(LEVEL_COLOSUSS, TEXT("Layer_Colossus")))->Get_Clear())
				{
					static_cast<CColossus*>(pGameInstance->Get_Front(LEVEL_COLOSUSS, TEXT("Layer_Colossus")))->Set_Clear(true);
					pGameInstance->Add_GameObject(g_eCurrentLevel, TEXT("Layer_DarkScreen"), TEXT("Prototype_GameObject_Effect_DarkScreen"));
					pGameInstance->Add_GameObject(g_eCurrentLevel, TEXT("Layer_WhiteScreen"), TEXT("Prototype_GameObject_Effect_WhiteScreen"));
				}

				pGameInstance->Release_LayerObject(LEVEL_STATIC, TEXT("Layer_ArrowLight"));

				m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(15.f, 4.90f, 30.4f));
				m_pTransformCom->LookAt(_float3(15.f, 4.90f, 32.f));
				m_bArrowDrive = false;
			}
			m_pPlayer->Set_ShotPower(0.f);
			m_fGravity = -0.03f;
		}
		// 콜로서스 본체 충돌
		if (_float3(0.f, 0.f, 0.f) != pGameInstance->Collision_SphereToRect((CCollider_Rect*)pGameInstance->Get_Component(LEVEL_COLOSUSS, TEXT("Layer_Colossus"), TEXT("Com_Collider")), m_pColliderCom))
		{
			CColossus* pColossus = static_cast<CColossus*>(pGameInstance->Get_Front(LEVEL_COLOSUSS, TEXT("Layer_Colossus")));
			if (false == pColossus->Get_Start())
			{
				pColossus->Set_Start(true);
				pGameInstance->PlayBGM(L"Colossus_map.ogg", 0.3f);// 사운드
			}
		}
		if (nullptr == pGameInstance->Find_Layer(LEVEL_COLOSUSS, TEXT("Layer_Colossus_Jongyu")))
			break;
		size_t iIcicleNum = pGameInstance->Find_Layer(LEVEL_COLOSUSS, TEXT("Layer_Colossus_Jongyu"))->Get_Object().size();
		for (_uint i = 0; i < iIcicleNum; i++)
		{
			_float3 vNewLook = pGameInstance->Collision_SphereBounce((CCollider_Sphere*)pGameInstance->Get_Component(g_eCurrentLevel, TEXT("Layer_Colossus_Jongyu"), TEXT("Com_Collider"), i), m_pColliderCom, m_pTransformCom->Get_State(CTransform::STATE_LOOK)) * 1.5f;

			if (_float3(0.f, 0.f, 0.f) != vNewLook)
			{
				_float3 vPos = pGameInstance->Collision_SphereGuard((CCollider_Sphere*)pGameInstance->Get_Component(g_eCurrentLevel, TEXT("Layer_Colossus_Jongyu"), TEXT("Com_Collider"), i), m_pColliderCom);
				m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
				_float3 vRight = *D3DXVec3Cross(&vRight, &_float3(0.f, 1.f, 0.f), &vNewLook);
				vRight = *D3DXVec3Normalize(&vRight, &vRight) * 1.5f;

				_float3 vUp = *D3DXVec3Cross(&vUp, &vNewLook, &vRight);
				vUp = *D3DXVec3Normalize(&vUp, &vUp) * 1.5f;

				m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight);
				m_pTransformCom->Set_State(CTransform::STATE_UP, vUp);
				m_pTransformCom->Set_State(CTransform::STATE_LOOK, vNewLook);
				m_pPlayer->Set_ShotPower(0.6f);
			}
		}
	}
		break;
	case LEVEL_EYECUBE:
		// 아이큐브에 튕기기	// 큐브 충돌
		if (m_pPlayer->Get_HaveArrow() == true)
			break;
		{

			if (pGameInstance->Collision_Sphere(m_pColliderCom, (CCollider_Sphere*)pGameInstance->Get_Component(g_eCurrentLevel, TEXT("Layer_EyeCube"), TEXT("Com_Weak"))))
			{
				if (m_bArrowDrive == true)
				{
					m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTransformCom->Get_State(CTransform::STATE_POSITION) + m_pTransformCom->Get_State(CTransform::STATE_LOOK) * 0.25f);
					pGameInstance->Release_LayerObject(LEVEL_STATIC, TEXT("Layer_ArrowLight"));
					m_bArrowDrive = false;
				}
				m_pPlayer->Set_ShotPower(0.f);
				m_fGravity = -0.03f;
			}
			else
			{
				_float3 vNewLook = pGameInstance->Collision_SphereBounce((CCollider_Sphere*)pGameInstance->Get_Component(g_eCurrentLevel, TEXT("Layer_EyeCube"), TEXT("Com_Collider")), m_pColliderCom, m_pTransformCom->Get_State(CTransform::STATE_LOOK)) * 1.5f;
				if (_float3(0.f, 0.f, 0.f) != vNewLook)
				{
					_float3 vPos = pGameInstance->Collision_SphereGuard((CCollider_Sphere*)pGameInstance->Get_Component(g_eCurrentLevel, TEXT("Layer_EyeCube"), TEXT("Com_Collider")), m_pColliderCom);
					m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
					_float3 vRight = *D3DXVec3Cross(&vRight, &_float3(0.f, 1.f, 0.f), &vNewLook);
					vRight = *D3DXVec3Normalize(&vRight, &vRight) * 1.5f;

					_float3 vUp = *D3DXVec3Cross(&vUp, &vNewLook, &vRight);
					vUp = *D3DXVec3Normalize(&vUp, &vUp) * 1.5f;

					m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight);
					m_pTransformCom->Set_State(CTransform::STATE_UP, vUp);
					m_pTransformCom->Set_State(CTransform::STATE_LOOK, vNewLook);
					m_pPlayer->Set_ShotPower(0.6f);
				}
			}
	}
		// 눈 맞추면 죽이기		// 구 충돌
	
	if (pGameInstance->Collision_Sphere((CCollider_Sphere*)pGameInstance->Get_Component(LEVEL_EYECUBE, TEXT("Layer_EyeCube"), TEXT("Com_Weak")), m_pColliderCom))
	{
		if (false == static_cast<CEyeCube*>(pGameInstance->Get_Front(LEVEL_EYECUBE, TEXT("Layer_EyeCube")))->Get_Clear())
		{
			
			static_cast<CEyeCube*>(pGameInstance->Get_Front(LEVEL_EYECUBE, TEXT("Layer_EyeCube")))->Set_Clear(true);
			pGameInstance->Add_GameObject(g_eCurrentLevel, TEXT("Layer_DarkScreen"), TEXT("Prototype_GameObject_Effect_DarkScreen"));
			pGameInstance->Add_GameObject(g_eCurrentLevel, TEXT("Layer_WhiteScreen"), TEXT("Prototype_GameObject_Effect_WhiteScreen"));
		}
	}

		break;
	case LEVEL_YETI:
		if (m_pPlayer->Get_HaveArrow() == true)
			break;
		{
			CYeti* pYeti = (CYeti*)pGameInstance->Get_Front(LEVEL_YETI, TEXT("Layer_Yeti"));
			_float fAngle = pYeti->Get_Angle();
			if ((112.5f < fAngle && 180.f >= fAngle) || (-112.5f > fAngle && -180.f <= fAngle))
			{
				_float fBoob = m_pTransformCom->Get_State(CTransform::STATE_POSITION).y;
				if (fBoob >= 0.6f && fBoob < 1.f)
				{
					if (pGameInstance->Collision_Sphere(m_pColliderCom, (CCollider_Sphere*)pGameInstance->Get_Component(g_eCurrentLevel, TEXT("Layer_Yeti"), TEXT("Com_Collider"))))
					{
						if (m_bArrowDrive == true)
						{
							m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTransformCom->Get_State(CTransform::STATE_POSITION) + m_pTransformCom->Get_State(CTransform::STATE_LOOK) * 0.6f);
							pGameInstance->Release_LayerObject(LEVEL_STATIC, TEXT("Layer_ArrowLight"));
							m_bArrowDrive = false;
						}
						m_pPlayer->Set_ShotPower(0.f);
						m_fGravity = -0.03f;
						// 예티 멈춤
						if (false == pYeti->Get_Clear())
						{

							pYeti->Set_Clear(true);
							pGameInstance->Add_GameObject(g_eCurrentLevel, TEXT("Layer_DarkScreen"), TEXT("Prototype_GameObject_Effect_DarkScreen"));
							pGameInstance->Add_GameObject(g_eCurrentLevel, TEXT("Layer_WhiteScreen"), TEXT("Prototype_GameObject_Effect_WhiteScreen"));
						}
					}

					
				}
			}
		// 예티에 튕기기		// 구충돌
			if (false == pYeti->Get_Clear())
			{
				_float3 vNewLook = pGameInstance->Collision_SphereBounce((CCollider_Sphere*)pGameInstance->Get_Component(g_eCurrentLevel, TEXT("Layer_Yeti"), TEXT("Com_Collider")), m_pColliderCom, m_pTransformCom->Get_State(CTransform::STATE_LOOK)) * 1.5f;

				if (_float3(0.f, 0.f, 0.f) != vNewLook)
				{
					CYeti* pYeti = (CYeti*)pGameInstance->Get_Front(LEVEL_YETI, TEXT("Layer_Yeti"));
					_float fAngle = pYeti->Get_Angle();
					//if ((112.5f < fAngle && 180.f >= fAngle) || (-112.5f > fAngle && -180.f <= fAngle))
					//{
					//	_float fBoob = m_pTransformCom->Get_State(CTransform::STATE_POSITION).y;
					//	if (fBoob >= 0.5f && fBoob < 1.f)
					//	{
					//		// 예티 멈춤
					//		if (false == pYeti->Get_Clear())
					//		{
					//			
					//			pYeti->Set_Clear(true);

					//			pGameInstance->Add_GameObject(g_eCurrentLevel, TEXT("Layer_WhiteScreen"), TEXT("Prototype_GameObject_Effect_WhiteScreen"));
					//		}
					//	}
					//}
					_float3 vPos = pGameInstance->Collision_SphereGuard((CCollider_Sphere*)pGameInstance->Get_Component(g_eCurrentLevel, TEXT("Layer_Yeti"), TEXT("Com_Collider")), m_pColliderCom);
					m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
					_float3 vRight = *D3DXVec3Cross(&vRight, &_float3(0.f, 1.f, 0.f), &vNewLook);
					vRight = *D3DXVec3Normalize(&vRight, &vRight) * 1.5f;

					_float3 vUp = *D3DXVec3Cross(&vUp, &vNewLook, &vRight);
					vUp = *D3DXVec3Normalize(&vUp, &vUp) * 1.5f;

					m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight);
					m_pTransformCom->Set_State(CTransform::STATE_UP, vUp);
					m_pTransformCom->Set_State(CTransform::STATE_LOOK, vNewLook);
					m_pPlayer->Set_ShotPower(0.6f);
				}
			}
		

		// 고드름에 튕기기		// 구충돌
		if (nullptr == pGameInstance->Find_Layer(LEVEL_YETI, TEXT("Layer_Icicle")))
			break;
		size_t iIcicleNum = pGameInstance->Find_Layer(LEVEL_YETI, TEXT("Layer_Icicle"))->Get_Object().size();
		for (_uint i = 0; i < iIcicleNum; i++)
		{
			_float3 vNewLook = pGameInstance->Collision_SphereBounce((CCollider_Sphere*)pGameInstance->Get_Component(g_eCurrentLevel, TEXT("Layer_Icicle"), TEXT("Com_Collider"), i), m_pColliderCom, m_pTransformCom->Get_State(CTransform::STATE_LOOK)) * 1.5f;

			if (_float3(0.f, 0.f, 0.f) != vNewLook)
			{
				_float3 vPos = pGameInstance->Collision_SphereGuard((CCollider_Sphere*)pGameInstance->Get_Component(g_eCurrentLevel, TEXT("Layer_Icicle"), TEXT("Com_Collider"), i), m_pColliderCom);
				m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
				_float3 vRight = *D3DXVec3Cross(&vRight, &_float3(0.f, 1.f, 0.f), &vNewLook);
				vRight = *D3DXVec3Normalize(&vRight, &vRight) * 1.5f;

				_float3 vUp = *D3DXVec3Cross(&vUp, &vNewLook, &vRight);
				vUp = *D3DXVec3Normalize(&vUp, &vUp) * 1.5f;

				m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight);
				m_pTransformCom->Set_State(CTransform::STATE_UP, vUp);
				m_pTransformCom->Set_State(CTransform::STATE_LOOK, vNewLook);
				m_pPlayer->Set_ShotPower(0.6f);
			}
		}
		
	}

		break;
	}

	RELEASE_INSTANCE(CGameInstance);
	return 0;
}

void CArrow::Gravity(_float fTimeDelta)
{
	_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_bool bIsArrow = m_pPlayer->Get_IsArrow();
	_bool bHaveArrow = m_pPlayer->Get_HaveArrow();
	if (true == bIsArrow && false == bHaveArrow)
	{

		m_fGravity += 0.03f;
		vPos.y -= m_fGravity * fTimeDelta;

	}
	else if (false == bIsArrow && true == bHaveArrow)
	{
		m_bStart = true;
	}

	if (vPos.y < 0.05f)
	{
		vPos.y = 0.05f;
		m_fGravity = 0.f;
		m_pTransformCom->Set_State(CTransform::STATE_UP, _float3(0.f, 1.5f, 0.f));
		_float3 vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
		vLook.y = 0.f;
		D3DXVec3Normalize(&vLook, &vLook);
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, vLook * 1.5f);
		_float3 vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
		vRight.y = 0.f;
		D3DXVec3Normalize(&vRight, &vRight);
		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight * 1.5f);

		if (true == m_bStart)
		{

			m_pPlayer->Set_ShotPower(0.1f);

			m_bStart = false;
			m_pPlayer->Set_MoveEndArrow(true);
		}
	}
	else
	{
		if (false == g_bTimeSlow)
		{
			_float3 vMyLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
			_float3 vMyRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
			_float3 vMyUp = m_pTransformCom->Get_State(CTransform::STATE_UP);
			D3DXVec3Normalize(&vMyLook, &vMyLook);
			_float3 vGravity = _float3(0.f, -m_fGravity, 0.f);
			vMyLook = vMyLook * m_pPlayer->Get_ShotPower() * 1.5f * fTimeDelta + vGravity * fTimeDelta * 0.5f;
			D3DXVec3Normalize(&vMyLook, &vMyLook);
			vMyLook *= 1.5f;

			vMyRight = *D3DXVec3Cross(&vMyRight, &_float3(0.f, 1.f, 0.f), &vMyLook);
			vMyRight = *D3DXVec3Normalize(&vMyRight, &vMyRight) * 1.5f;

			vMyUp = *D3DXVec3Cross(&vMyUp, &vMyLook, &vMyRight);
			vMyUp = *D3DXVec3Normalize(&vMyUp, &vMyUp) * 1.5f;

			if (vMyUp.y > 0)
			{
				m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vMyRight);
				m_pTransformCom->Set_State(CTransform::STATE_UP, vMyUp);
				m_pTransformCom->Set_State(CTransform::STATE_LOOK, vMyLook);
			}
		}
		
		
	}
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	
	





}

CArrow * CArrow::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CArrow*	pInstance = new CArrow(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create CArrow in CArrow::Create");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CArrow::Clone(void * pArg)
{
	CArrow*	pInstance = new CArrow(*this);


	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create CArrow in CArrow::Clone");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CArrow::Free()
{
	__super::Free();
//#ifdef _DEBUG
	Safe_Release(m_pVIBufferCollCom);
//#endif // _DEBUG
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
