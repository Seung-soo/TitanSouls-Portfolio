#include "..\public\Picking.h"

IMPLEMENT_SINGLETON(CPicking)

CPicking::CPicking()
{
}

HRESULT CPicking::NativeConstruct(LPDIRECT3DDEVICE9 pGraphic_Device, HWND hWnd)
{
	m_hWnd = hWnd;
	m_pGraphic_Device = pGraphic_Device;
	Safe_AddRef(m_pGraphic_Device);
	return S_OK;
}

HRESULT CPicking::Transform_ToWorldSpace()
{
	POINT		ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(m_hWnd, &ptMouse);

	D3DVIEWPORT9		ViewPortDesc;
	m_pGraphic_Device->GetViewport(&ViewPortDesc);

	_float4		vMousePos;
	vMousePos.x = ptMouse.x / (ViewPortDesc.Width * 0.5f) - 1;
	vMousePos.y = ptMouse.y / (ViewPortDesc.Height * 0.5f) + 1;
	vMousePos.z = 0.f;
	vMousePos.w = 1.f;

	_float4x4	ProjMatrixInverse;
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &ProjMatrixInverse);
	D3DXMatrixInverse(&ProjMatrixInverse, nullptr, &ProjMatrixInverse);
	D3DXVec4Transform(&vMousePos, &vMousePos, &ProjMatrixInverse);
	memcpy(&m_vRayDir, &(vMousePos - _float4(0.f, 0.f, 0.f, 1.f)), sizeof(_float3));
	m_vRayPos = _float3(0.f, 0.f, 0.f);

	_float4x4	ViewMatrixInverse;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrixInverse);
	D3DXMatrixInverse(&ViewMatrixInverse, nullptr, &ViewMatrixInverse);
	D3DXVec3TransformNormal(&m_vRayDir, &m_vRayDir, &ViewMatrixInverse);
	D3DXVec3TransformCoord(&m_vRayPos, &m_vRayPos, &ViewMatrixInverse);
	return S_OK;
}

HRESULT CPicking::Transform_ToLocalSpace(_float4x4 WorldMatrixInverse)
{
	D3DXVec3TransformNormal(&m_vRayDirInLocal, &m_vRayDir, &WorldMatrixInverse);
	D3DXVec3Normalize(&m_vRayDirInLocal, &m_vRayDirInLocal);
	D3DXVec3TransformNormal(&m_vRayPosInLocal, &m_vRayPos, &WorldMatrixInverse);

	return S_OK;
}

_bool CPicking::isPick(_float3 * pLocalPoints, _float3 * pOut)
{
	_float fU, fV, fDist;
	if (D3DXIntersectTri(&pLocalPoints[0], &pLocalPoints[1], &pLocalPoints[2], &m_vRayPosInLocal, &m_vRayDirInLocal, &fU, &fV, &fDist))
	{
		*pOut = m_vRayPosInLocal + m_vRayDirInLocal * fDist;
		return true;
	}
	else
		return false;
}

void CPicking::Free()
{
	Safe_Release(m_pGraphic_Device);
}
