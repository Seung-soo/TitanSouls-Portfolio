#include "..\public\Component_Manager.h"

IMPLEMENT_SINGLETON(CComponent_Manager)

CComponent_Manager::CComponent_Manager()
{
}

HRESULT CComponent_Manager::Reserve_Container(_uint iNumLevels)
{
	m_iNumLevels = iNumLevels;

	m_pPrototypes = new PROTOTYPES[iNumLevels];

	return S_OK;
}

HRESULT CComponent_Manager::Add_Prototype(_uint iLevelIndex, const _tchar * pPrototypeTag, CComponent * pPrototype)
{
	if (iLevelIndex >= m_iNumLevels)
	{
		MSGBOX("iLevelIndex >= m_iNumLevels in CComponent_Manager::Add_Prototype");
		return E_FAIL;
	}

	if (nullptr != Find_Component(iLevelIndex, pPrototypeTag))
	{
		MSGBOX("Find_Component is not nullptr in CComponent_Manager::Add_Prototype");
		return E_FAIL;
	}

	m_pPrototypes[iLevelIndex].emplace(pPrototypeTag, pPrototype);

	return S_OK;
}

CComponent * CComponent_Manager::Clone_Component(_uint iLevelIndex, const _tchar * pPrototypeTag, void * pArg)
{
	if (iLevelIndex >= m_iNumLevels)
	{
		MSGBOX("iLevelIndex >= m_iNumLevels in CComponent_Manager::Add_Prototype");
		return nullptr;
	}

	if (nullptr == m_pPrototypes)
	{
		MSGBOX("m_pPrototypes is nullptr in CComponent_Manager::Clone_Component");
		return nullptr;
	}

	CComponent*		pPrototype = Find_Component(iLevelIndex, pPrototypeTag);
	if (nullptr == pPrototype)
	{
		MSGBOX("pPrototype is nullptr in CComponent_Manager::Clone_Component");
		return nullptr;
	}

	CComponent*		pComponent = pPrototype->Clone(pArg);
	if (nullptr == pComponent)
	{
		MSGBOX("pComponent is nullptr in CComponent_Manager::Clone_Component");
		return nullptr;
	}

	return pComponent;
}

HRESULT CComponent_Manager::Clear_LevelComponent(_uint iLevelIndex)
{
	if (iLevelIndex >= m_iNumLevels)
	{
		MSGBOX("iLevelIndex >= m_iNumLevels in CComponent_Manager::Add_Prototype");
		return E_FAIL;
	}

	for (auto& Pair : m_pPrototypes[iLevelIndex])
		Safe_Release(Pair.second);

	m_pPrototypes[iLevelIndex].clear();

	return S_OK;
}

CComponent * CComponent_Manager::Find_Component(_uint iLevelIndex, const _tchar * pPrototypeTag)
{
	auto iter = find_if(m_pPrototypes[iLevelIndex].begin(), m_pPrototypes[iLevelIndex].end(), CTagFinder(pPrototypeTag));

	if (iter == m_pPrototypes[iLevelIndex].end())
		return nullptr;

	return iter->second;
}

_bool CComponent_Manager::Collision_Sphere(CCollider_Sphere * pDst, CCollider_Sphere * pSrc)
{
	_float3 vDir = pDst->Get_Position() - pSrc->Get_Position();
	_float fDist = D3DXVec3Length(&vDir);
	if (fDist < 0)
		fDist *= -1.f;

	_float fScaleDst = D3DXVec3Length((_float3*)pDst->Get_SphereDesc().ObjectMatrix.m[0]);
	_float fScaleSrc = D3DXVec3Length((_float3*)pSrc->Get_SphereDesc().ObjectMatrix.m[0]);

	if (fDist <= pDst->Get_SphereDesc().fRadius * fScaleDst + pSrc->Get_SphereDesc().fRadius * fScaleSrc)
		return true;

	return false;
}

_float3 CComponent_Manager::Collision_SphereGuard(CCollider_Sphere * pDst, CCollider_Sphere * pSrc)
{
	_float3 vDir = pDst->Get_Position() - pSrc->Get_Position();
	_float fDist = D3DXVec3Length(&vDir);
	if (fDist < 0)
		fDist *= -1.f;

	_float fScaleDst = D3DXVec3Length((_float3*)pDst->Get_SphereDesc().ObjectMatrix.m[0]);
	_float fScaleSrc = D3DXVec3Length((_float3*)pSrc->Get_SphereDesc().ObjectMatrix.m[0]);

	if (fDist < pDst->Get_SphereDesc().fRadius * fScaleDst + pSrc->Get_SphereDesc().fRadius * fScaleSrc)
	{
		D3DXVec3Normalize(&vDir, &vDir);
		vDir = -vDir * (pDst->Get_SphereDesc().fRadius * fScaleDst + pSrc->Get_SphereDesc().fRadius * fScaleSrc);

		return pDst->Get_Position() + vDir;
	}
	return _float3(0.f, 0.f, 0.f);
}

_float3 CComponent_Manager::Collision_SphereBounce(CCollider_Sphere * pDst, CCollider_Sphere * pSrc, _float3 vLook)
{
	_float3 vDir = pDst->Get_Position() - pSrc->Get_Position();
	_float fDist = D3DXVec3Length(&vDir);
	if (fDist < 0)
		fDist *= -1.f;

	_float fScaleDst = D3DXVec3Length((_float3*)pDst->Get_SphereDesc().ObjectMatrix.m[0]);
	_float fScaleSrc = D3DXVec3Length((_float3*)pSrc->Get_SphereDesc().ObjectMatrix.m[0]);

	if (fDist <= pDst->Get_SphereDesc().fRadius * fScaleDst + pSrc->Get_SphereDesc().fRadius * fScaleSrc)
	{
		vDir *= -1;
		vLook *= -1;
		D3DXVec3Normalize(&vDir, &vDir);
		D3DXVec3Normalize(&vLook, &vLook);

		_float fAngle = acos(D3DXVec3Dot(&vDir, &vLook));
		_float3 vDstRight;
		D3DXVec3Cross(&vDstRight, &_float3(0.f, 1.f, 0.f), &vDir);
		_float3 vDstUp;
		D3DXVec3Cross(&vDstUp, &vLook, &vDir);

		_float4x4	RotationMatrix;
		D3DXMatrixRotationAxis(&RotationMatrix, &vDstUp, fAngle);


		D3DXVec3TransformNormal(&vLook, &vDir, &RotationMatrix);

		return vLook;
	}

	return _float3(0.f, 0.f, 0.f);
}

_float3 CComponent_Manager::Collision_SphereToRect(CCollider_Rect * pDst, CCollider_Sphere * pSrc)
{
	_float4x4 inverseTemp = pDst->Get_RectDesc().ObjectMatrix;
	D3DXMatrixInverse(&inverseTemp, nullptr, &inverseTemp);
	CCollider_Rect::RECTDESC DstRectDesc = pDst->Get_RectDesc();
	CCollider_Sphere::SPHEREDESC SrcSphereDesc = pSrc->Get_SphereDesc();

	_float3 fSpherePos = pSrc->Get_Position();

	D3DXVec3TransformCoord(&fSpherePos, &fSpherePos, &inverseTemp);
	_float fScaleSrc = D3DXVec3Length((_float3*)pSrc->Get_SphereDesc().ObjectMatrix.m[2]);

	if (DstRectDesc.fX > fSpherePos.x && 0 < fSpherePos.x)
	{
		if (DstRectDesc.fY > fSpherePos.y && 0 < fSpherePos.y)
			if (fSpherePos.z > -SrcSphereDesc.fRadius * fScaleSrc && fSpherePos.z < SrcSphereDesc.fRadius * fScaleSrc + 0.5f)
			{
				//_float fScale = D3DXVec3Length((_float3*)SrcSphereDesc.ObjectMatrix.m[0]);
				//if (fSpherePos.z >= 0)
				//	fSpherePos.z = SrcSphereDesc.fRadius;
				//else
				fSpherePos.z = -SrcSphereDesc.fRadius * fScaleSrc;
				D3DXMatrixInverse(&inverseTemp, nullptr, &inverseTemp);
				if (fabsf(fSpherePos.x) < SrcSphereDesc.fRadius * fScaleSrc)
				{
					if (fSpherePos.x < 0)
						fSpherePos.x = -SrcSphereDesc.fRadius * fScaleSrc;
					else
						fSpherePos.x = SrcSphereDesc.fRadius * fScaleSrc;
				}
				else if (fSpherePos.x >= DstRectDesc.fX - SrcSphereDesc.fRadius * fScaleSrc && fSpherePos.x < DstRectDesc.fX + SrcSphereDesc.fRadius * fScaleSrc)
				{
					if (fSpherePos.x <= DstRectDesc.fX)
						fSpherePos.x = DstRectDesc.fX - SrcSphereDesc.fRadius * fScaleSrc;
					else
						fSpherePos.x = DstRectDesc.fX + SrcSphereDesc.fRadius * fScaleSrc;
				}


				D3DXVec3TransformCoord(&fSpherePos, &fSpherePos, &inverseTemp);
				return fSpherePos;
			}
	}
		


	return _float3(0.f, 0.f, 0.f);
}

_float3 CComponent_Manager::Collision_SphereToRectArrowFull(CCollider_Rect * pDst, CCollider_Sphere * pSrc)
{
	_float4x4 inverseTemp = pDst->Get_RectDesc().ObjectMatrix;
	D3DXMatrixInverse(&inverseTemp, nullptr, &inverseTemp);
	CCollider_Rect::RECTDESC DstRectDesc = pDst->Get_RectDesc();
	CCollider_Sphere::SPHEREDESC SrcSphereDesc = pSrc->Get_SphereDesc();

	_float3 fSpherePos = pSrc->Get_Position();

	D3DXVec3TransformCoord(&fSpherePos, &fSpherePos, &inverseTemp);
	_float fScaleSrc = D3DXVec3Length((_float3*)pSrc->Get_SphereDesc().ObjectMatrix.m[2]);

	if (DstRectDesc.fX > fSpherePos.x && 0 < fSpherePos.x)
	{
		if (DstRectDesc.fY > fSpherePos.y && 0 < fSpherePos.y)
			if (fSpherePos.z > -SrcSphereDesc.fRadius * fScaleSrc && fSpherePos.z < SrcSphereDesc.fRadius * fScaleSrc + 4.f)
			{
				fSpherePos.z = -SrcSphereDesc.fRadius * fScaleSrc;
				D3DXMatrixInverse(&inverseTemp, nullptr, &inverseTemp);
				if (fabsf(fSpherePos.x) < SrcSphereDesc.fRadius * fScaleSrc)
				{
					if (fSpherePos.x < 0)
						fSpherePos.x = -SrcSphereDesc.fRadius * fScaleSrc;
					else
						fSpherePos.x = SrcSphereDesc.fRadius * fScaleSrc;
				}
				else if (fSpherePos.x >= DstRectDesc.fX - SrcSphereDesc.fRadius * fScaleSrc && fSpherePos.x < DstRectDesc.fX + SrcSphereDesc.fRadius * fScaleSrc)
				{
					if (fSpherePos.x <= DstRectDesc.fX)
						fSpherePos.x = DstRectDesc.fX - SrcSphereDesc.fRadius * fScaleSrc;
					else
						fSpherePos.x = DstRectDesc.fX + SrcSphereDesc.fRadius * fScaleSrc;
				}


				D3DXVec3TransformCoord(&fSpherePos, &fSpherePos, &inverseTemp);
				return fSpherePos;
			}
	}



	return _float3(0.f, 0.f, 0.f);
}

_float3 CComponent_Manager::Collision_SphereToRectArrow(CCollider_Rect * pDst, CCollider_Sphere * pSrc)
{
	_float4x4 inverseTemp = pDst->Get_RectDesc().ObjectMatrix;
	D3DXMatrixInverse(&inverseTemp, nullptr, &inverseTemp);
	CCollider_Rect::RECTDESC DstRectDesc = pDst->Get_RectDesc();
	CCollider_Sphere::SPHEREDESC SrcSphereDesc = pSrc->Get_SphereDesc();

	_float3 fSpherePos = pSrc->Get_Position();
	_float3 vSrcPos = *(_float3*)SrcSphereDesc.ObjectMatrix.m[3];

	D3DXVec3TransformCoord(&fSpherePos, &fSpherePos, &inverseTemp);
	D3DXVec3TransformCoord(&vSrcPos, &vSrcPos, &inverseTemp);
	_float fScaleSrc = D3DXVec3Length((_float3*)pSrc->Get_SphereDesc().ObjectMatrix.m[0]);

	if (DstRectDesc.fX > fSpherePos.x && 0.f < fSpherePos.x)
	{
		if (DstRectDesc.fY > fSpherePos.y && 0.f < fSpherePos.y)
			if (fSpherePos.z > -0.75f && fSpherePos.z < 0.75f + 1.f)
			{
				vSrcPos.z = -0.75f;
				D3DXMatrixInverse(&inverseTemp, nullptr, &inverseTemp);
				if (fSpherePos.x < 0.75f)
				{
						vSrcPos.x = 0.75f;
				}
				else if (fSpherePos.x > DstRectDesc.fX - 0.75f)
				{
						vSrcPos.x = DstRectDesc.fX - 0.75f;
				}

				D3DXVec3TransformCoord(&vSrcPos, &vSrcPos, &inverseTemp);
				return vSrcPos;
			}
	}



	return _float3(0.f, 0.f, 0.f);
}

_float3 CComponent_Manager::Collision_SphereToRect_Camera(CCollider_Rect * pDst, CCollider_Sphere * pSrc)
{
	_float4x4 inverseTemp = pDst->Get_RectDesc().ObjectMatrix;
	D3DXMatrixInverse(&inverseTemp, nullptr, &inverseTemp);
	CCollider_Rect::RECTDESC DstRectDesc = pDst->Get_RectDesc();
	CCollider_Sphere::SPHEREDESC SrcSphereDesc = pSrc->Get_SphereDesc();

	_float3 fSpherePos = pSrc->Get_Position();

	D3DXVec3TransformCoord(&fSpherePos, &fSpherePos, &inverseTemp);
	if (DstRectDesc.fX > fSpherePos.x && -2.f < fSpherePos.x)
	{
		if (DstRectDesc.fY  > fSpherePos.y && 0.f < fSpherePos.y)
			if (fSpherePos.z > -SrcSphereDesc.fRadius && fSpherePos.z < SrcSphereDesc.fRadius + 2.f)
			{
				fSpherePos.z = -SrcSphereDesc.fRadius;
				D3DXMatrixInverse(&inverseTemp, nullptr, &inverseTemp);

				if (fSpherePos.x < SrcSphereDesc.fRadius)
					fSpherePos.x = SrcSphereDesc.fRadius;
				if (fSpherePos.x > DstRectDesc.fX - SrcSphereDesc.fRadius)
					fSpherePos.x = DstRectDesc.fX - SrcSphereDesc.fRadius;

				if (fSpherePos.y < SrcSphereDesc.fRadius)
					fSpherePos.y = SrcSphereDesc.fRadius;

				D3DXVec3TransformCoord(&fSpherePos, &fSpherePos, &inverseTemp);
				return fSpherePos;
			}
	}

	return _float3(0.f, 0.f, 0.f);
}

_float3 CComponent_Manager::Collision_SphereToRectZ(CCollider_Rect_Z * pDst, CCollider_Sphere * pSrc)
{
	_float4x4 inverseTemp = pDst->Get_RectDesc().ObjectMatrix;
	D3DXMatrixInverse(&inverseTemp, nullptr, &inverseTemp);
	CCollider_Rect_Z::RECTDESC DstRectDesc = pDst->Get_RectDesc();
	CCollider_Sphere::SPHEREDESC SrcSphereDesc = pSrc->Get_SphereDesc();

	_float3 fSpherePos = pSrc->Get_Position();

	D3DXVec3TransformCoord(&fSpherePos, &fSpherePos, &inverseTemp);
	if (DstRectDesc.fX > fSpherePos.x && 0 < fSpherePos.x)
	{
		if (DstRectDesc.fZ > fSpherePos.z && 0 < fSpherePos.z)
			if (fabsf(fSpherePos.y) < SrcSphereDesc.fRadius)
			{
				if (fSpherePos.y >= 0)
					fSpherePos.y = SrcSphereDesc.fRadius;
				else
					fSpherePos.y = -SrcSphereDesc.fRadius;
				D3DXMatrixInverse(&inverseTemp, nullptr, &inverseTemp);
				if (fabsf(fSpherePos.x) < SrcSphereDesc.fRadius)
				{
					if (fSpherePos.x < 0)
						fSpherePos.x = -SrcSphereDesc.fRadius;
					else
						fSpherePos.x = SrcSphereDesc.fRadius;
				}
				else if (fSpherePos.x >= DstRectDesc.fX - SrcSphereDesc.fRadius && fSpherePos.x < DstRectDesc.fX + SrcSphereDesc.fRadius)
				{
					if (fSpherePos.x <= DstRectDesc.fX)
						fSpherePos.x = DstRectDesc.fX - SrcSphereDesc.fRadius;
					else
						fSpherePos.x = DstRectDesc.fX + SrcSphereDesc.fRadius;
				}


				D3DXVec3TransformCoord(&fSpherePos, &fSpherePos, &inverseTemp);
				return fSpherePos;
			}
	}



	return _float3(0.f, 0.f, 0.f);
}

void CComponent_Manager::Free()
{
	for (_uint i = 0; i < m_iNumLevels; i++)
	{
		for (auto& Pair : m_pPrototypes[i])
			Safe_Release(Pair.second);
		m_pPrototypes[i].clear();
	}
	Safe_Delete_Array(m_pPrototypes);
}
