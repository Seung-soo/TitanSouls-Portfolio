#include "..\public\Transform.h"

CTransform::CTransform(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{
}

CTransform::CTransform(const CTransform & rhs)
	: CComponent(rhs)
	, m_WorldMatrix(rhs.m_WorldMatrix)
{
}

_float3 CTransform::Get_Scale()
{
	return _float3(D3DXVec3Length(&Get_State(CTransform::STATE_RIGHT)), D3DXVec3Length(&Get_State(CTransform::STATE_UP)), D3DXVec3Length(&Get_State(CTransform::STATE_LOOK)));
}

_float4x4 CTransform::Get_WorldMatrixInverse()
{
	_float4x4		WorldMatrixInverse;

	D3DXMatrixInverse(&WorldMatrixInverse, nullptr, &m_WorldMatrix);

	return WorldMatrixInverse;
}

void CTransform::Set_State(STATE eState, const _float3 & vState)
{
	memcpy(&m_WorldMatrix.m[eState][0], &vState, sizeof(_float3));
}

void CTransform::Set_TransformDesc(const TRANSFORMDESC & TransformDesc)
{
	m_TransformDesc = TransformDesc;
}

HRESULT CTransform::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		MSGBOX("__super::NativeConstruct_Prototype returned E_FAIL in CTransform::NativeConstruct_Prototype");
		return E_FAIL;
	}

	m_WorldMatrix = *D3DXMatrixIdentity(&m_WorldMatrix);

	return S_OK;
}

HRESULT CTransform::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		MSGBOX("__super::NativeConstruct returned E_FAIL in CTransform::NativeConstruct");
		return E_FAIL;
	}

	if (nullptr != pArg)
		memcpy(&m_TransformDesc, pArg, sizeof(TRANSFORMDESC));

	return S_OK;
}

void CTransform::Go_Straight(_float fTimeDelta)
{
	_float3		vPosition = Get_State(CTransform::STATE_POSITION);
	_float3		vLook = Get_State(CTransform::STATE_LOOK);

	vPosition += *D3DXVec3Normalize(&vLook, &vLook) * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	Set_State(CTransform::STATE_POSITION, vPosition);
}

void CTransform::Go_BackWard(_float fTimeDelta)
{
	_float3		vPosition = Get_State(CTransform::STATE_POSITION);
	_float3		vLook = Get_State(CTransform::STATE_LOOK);

	vPosition -= *D3DXVec3Normalize(&vLook, &vLook) * m_TransformDesc.fSpeedPerSec * fTimeDelta;
	Set_State(CTransform::STATE_POSITION, vPosition);
}

void CTransform::Go_Down(_float fTimeDelta)
{
	_float3		vPosition = Get_State(CTransform::STATE_POSITION);
	_float3		vUp = Get_State(CTransform::STATE_UP);

	vPosition -= *D3DXVec3Normalize(&vUp, &vUp) * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	Set_State(CTransform::STATE_POSITION, vPosition);
}

void CTransform::Go_Up(_float fTimeDelta)
{
	_float3		vPosition = Get_State(CTransform::STATE_POSITION);
	_float3		vUp = Get_State(CTransform::STATE_UP);

	vPosition += *D3DXVec3Normalize(&vUp, &vUp) * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	Set_State(CTransform::STATE_POSITION, vPosition);
}

void CTransform::Go_Left(_float fTimeDelta)
{
	_float3		vPosition = Get_State(CTransform::STATE_POSITION);
	_float3		vRight = Get_State(CTransform::STATE_RIGHT);

	vPosition -= *D3DXVec3Normalize(&vRight, &vRight) * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	Set_State(CTransform::STATE_POSITION, vPosition);
}

void CTransform::Go_Right(_float fTimeDelta)
{
	_float3		vPosition = Get_State(CTransform::STATE_POSITION);
	_float3		vRight = Get_State(CTransform::STATE_RIGHT);

	vPosition += *D3DXVec3Normalize(&vRight, &vRight) * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	Set_State(CTransform::STATE_POSITION, vPosition);
}

void CTransform::Go_Vector(_float3 vDir, _float fTimeDelta)
{
	_float3		vPosition = Get_State(CTransform::STATE_POSITION);

	vPosition += vDir * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	Set_State(CTransform::STATE_POSITION, vPosition);
}

void CTransform::LookAt(_float3 vTargetPos)
{
	_float3	vPosition = Get_State(CTransform::STATE_POSITION);
	_float3	vScale = Get_Scale();

	_float3 vRight, vUp, vLook;

	vLook = vTargetPos - vPosition;
	vLook = *D3DXVec3Normalize(&vLook, &vLook);

	vRight = *D3DXVec3Cross(&vRight, &_float3(0.f, 1.f, 0.f), &vLook);
	vRight = *D3DXVec3Normalize(&vRight, &vRight);

	vUp = *D3DXVec3Cross(&vUp, &vLook, &vRight);
	vUp = *D3DXVec3Normalize(&vUp, &vUp);

	Set_State(CTransform::STATE_RIGHT, vRight * vScale.x);
	Set_State(CTransform::STATE_UP, vUp * vScale.y);
	Set_State(CTransform::STATE_LOOK, vLook * vScale.z);
}

void CTransform::LookAtUp(_float3 vTargetPos)
{
	_float3	vPosition = Get_State(CTransform::STATE_POSITION);
	_float3	vScale = Get_Scale();

	_float3 vRight, vUp, vLook;

	vUp = vTargetPos - vPosition;
	vUp = *D3DXVec3Normalize(&vUp, &vUp) * vScale.y;

	vRight = *D3DXVec3Cross(&vRight, &vUp, &_float3(0.f, -1.f, 0.f));
	vRight = *D3DXVec3Normalize(&vRight, &vRight) * vScale.x;

	vLook = *D3DXVec3Cross(&vLook, &vRight, &vUp);
	vLook = *D3DXVec3Normalize(&vLook, &vLook) * vScale.z;

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);
}

void CTransform::LookAtXZ(_float3 vTargetPos)
{
	_float3	vPosition = Get_State(CTransform::STATE_POSITION);
	_float3	vScale = Get_Scale();

	_float3 vRight, vUp, vLook;

	vLook = vTargetPos - vPosition;
	vLook.y = 0.f;
	vLook = *D3DXVec3Normalize(&vLook, &vLook) * vScale.z;

	vRight = *D3DXVec3Cross(&vRight, &_float3(0.f, 1.f, 0.f), &vLook);
	vRight = *D3DXVec3Normalize(&vRight, &vRight) * vScale.x;

	vUp = *D3DXVec3Cross(&vUp, &vLook, &vRight);
	vUp = *D3DXVec3Normalize(&vUp, &vUp) * vScale.y;

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);
}

void CTransform::MatchIt_XZ(_float3 vTargetPos)
{
	_float3	vPosition = Get_State(CTransform::STATE_POSITION);
	_float3	vScale = Get_Scale();

	_float3 vRight, vUp, vLook;

	vLook = vPosition -vTargetPos;
	vLook.y = 0.f;
	vLook = *D3DXVec3Normalize(&vLook, &vLook);

	vRight = *D3DXVec3Cross(&vRight, &_float3(0.f, 1.f, 0.f), &vLook);
	vRight = *D3DXVec3Normalize(&vRight, &vRight);

	vUp = *D3DXVec3Cross(&vUp, &vLook, &vRight);
	vUp = *D3DXVec3Normalize(&vUp, &vUp);

	Set_State(CTransform::STATE_RIGHT, vRight * vScale.x);
	Set_State(CTransform::STATE_UP, vUp * vScale.y);
	Set_State(CTransform::STATE_LOOK, vLook * vScale.z);
}

void CTransform::Turn(_float3 vAxis, _float fTimeDelta)
{
	_float3		vRight = Get_State(CTransform::STATE_RIGHT);
	_float3		vUp = Get_State(CTransform::STATE_UP);
	_float3		vLook = Get_State(CTransform::STATE_LOOK);

	_float4x4	RotationMatrix;
	D3DXMatrixRotationAxis(&RotationMatrix, &vAxis, m_TransformDesc.fRotationPerSec * fTimeDelta);

	D3DXVec3TransformNormal(&vRight, &vRight, &RotationMatrix);
	D3DXVec3TransformNormal(&vUp, &vUp, &RotationMatrix);
	D3DXVec3TransformNormal(&vLook, &vLook, &RotationMatrix);

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);
}

void CTransform::LookTurn(_float3 vAxis, _float3 vTarget, _float fTimeDelta)
{
	_float3		vRight = Get_State(CTransform::STATE_RIGHT);
	_float3		vUp = Get_State(CTransform::STATE_UP);
	_float3		vLook = Get_State(CTransform::STATE_LOOK);
	_float3		vPos = Get_State(STATE_POSITION);

	_float3 vDir = vTarget - vPos;
	vDir.y = 0.f;
	D3DXVec3Normalize(&vDir, &vDir);
	_float fAngle = D3DXToDegree(acos(D3DXVec3Dot(&vDir, &vLook)));
	_float d = -(vRight.x * vPos.x + vRight.y * vPos.y + vRight.z * vPos.z);
	_float LR = vRight.x * vTarget.x + vRight.y * vTarget.y + vRight.z * vTarget.z + d;
	if (0 > LR)
		fAngle = -fAngle;
	fAngle = D3DXToRadian(fAngle);
	_float4x4	RotationMatrix;
	D3DXMatrixRotationAxis(&RotationMatrix, &vAxis, fAngle * fTimeDelta);

	D3DXVec3TransformNormal(&vRight, &vRight, &RotationMatrix);
	D3DXVec3TransformNormal(&vUp, &vUp, &RotationMatrix);
	D3DXVec3TransformNormal(&vLook, &vLook, &RotationMatrix);

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);
}

void CTransform::Rotation(_float3 vAxis, _float fRadian)
{
	_float3		vRight = _float3(1.f, 0.f, 0.f) * Get_Scale().x;
	_float3		vUp = _float3(0.f, 1.f, 0.f) * Get_Scale().y;
	_float3		vLook = _float3(0.f, 0.f, 1.f) * Get_Scale().z;

	_float4x4	RotationMatrix;
	D3DXMatrixRotationAxis(&RotationMatrix, &vAxis, fRadian);

	D3DXVec3TransformNormal(&vRight, &vRight, &RotationMatrix);
	D3DXVec3TransformNormal(&vUp, &vUp, &RotationMatrix);
	D3DXVec3TransformNormal(&vLook, &vLook, &RotationMatrix);

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);
}

void CTransform::Revolution(_float3 vAxis, _float3 vTarget, _float fTimeDelta)
{
	_float3		vRight = Get_State(CTransform::STATE_RIGHT);
	_float3		vUp = Get_State(CTransform::STATE_UP);
	_float3		vLook = Get_State(CTransform::STATE_LOOK);
	_float3		vPos = Get_State(CTransform::STATE_POSITION);
	_float4x4	RotationMatrix;
	D3DXMatrixRotationAxis(&RotationMatrix, &vAxis, m_TransformDesc.fRotationPerSec * fTimeDelta);

	vPos -= vTarget;
	D3DXVec3TransformNormal(&vRight, &vRight, &RotationMatrix);
	D3DXVec3TransformNormal(&vUp, &vUp, &RotationMatrix);
	D3DXVec3TransformNormal(&vLook, &vLook, &RotationMatrix);
	D3DXVec3TransformNormal(&vPos, &vPos, &RotationMatrix);

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);

	vPos += vTarget;
	Set_State(CTransform::STATE_POSITION, vPos);
}

void CTransform::Scaled(_float3 vScale)
{
	_float3		vRight = Get_State(CTransform::STATE_RIGHT);
	_float3		vUp = Get_State(CTransform::STATE_UP);
	_float3		vLook = Get_State(CTransform::STATE_LOOK);

	vRight = *D3DXVec3Normalize(&vRight, &vRight) * vScale.x;
	vUp = *D3DXVec3Normalize(&vUp, &vUp) * vScale.y;
	vLook = *D3DXVec3Normalize(&vLook, &vLook) * vScale.z;

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);
}

HRESULT CTransform::Bind_OnGraphicDevice()
{
	if (nullptr == m_pGraphic_Device)
	{
		MSGBOX("m_pGraphic_Device is nullptr in CTransform::Bind_OnGraphicDevice");
		return E_FAIL;
	}

	return m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_WorldMatrix);
}

CTransform * CTransform::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTransform*	pInstance = new CTransform(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create CTransform in CTransform::Create");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CTransform::Clone(void * pArg)
{
	CTransform*	pInstance = new CTransform(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CTransform in CTransform::Clone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTransform::Free()
{
	__super::Free();
}
