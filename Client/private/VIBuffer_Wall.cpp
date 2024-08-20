#include "stdafx.h"
#include "..\public\VIBuffer_Wall.h"

CVIBuffer_Wall::CVIBuffer_Wall(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer(pGraphic_Device)
{
}

CVIBuffer_Wall::CVIBuffer_Wall(const CVIBuffer_Wall & rhs)
	: CVIBuffer(rhs)
	, m_iFirstLine(rhs.m_iFirstLine)
	, m_iSecondLine(rhs.m_iSecondLine)
{
}

HRESULT CVIBuffer_Wall::NativeConstruct_Prototype(_uint iFirstLine, _uint iSecondLine)
{
	m_iStride = sizeof(VTXTEX);
	m_iNumVertices = iFirstLine * iSecondLine;
	m_iFirstLine = iFirstLine;
	m_iSecondLine = iSecondLine;
	m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX1;
	m_ePrimitiveType = D3DPT_TRIANGLELIST;
	m_iNumPrimitive = (iFirstLine - 1) * (iSecondLine - 1) * 2;

	if (FAILED(__super::Create_VertexBuffer()))
	{
		MSGBOX("__super::Create_VertexBuffer returned E_FAIL in CVIBuffer_Wall::NativeConstruct_Prototype");
		return E_FAIL;
	}

	m_pVertices = new VTXTEX[m_iNumVertices];
	ZeroMemory(m_pVertices, sizeof(VTXTEX) * m_iNumVertices);



	VTXTEX*			pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	for (_uint i = 0; i < m_iSecondLine; ++i)
	{
		for (_uint j = 0; j < m_iFirstLine; ++j)
		{
			_uint iIndex = i * m_iFirstLine + j;


			pVertices[iIndex].vPosition = _float3((_float)j, (_float)i, 0.0f);
			pVertices[iIndex].vTexUV = _float2((_float)j / (m_iFirstLine - 1), (_float)i / (m_iSecondLine - 1));


			((VTXTEX*)m_pVertices)[iIndex] = pVertices[iIndex];

		}
	}

	m_pVB->Unlock();

	m_iIndicesSize = sizeof(FACEINDICES32);
	m_eIndexFormat = D3DFMT_INDEX32;
	if (FAILED(__super::Create_IndexBuffer()))
	{
		MSGBOX("__super::Create_IndexBuffer returned E_FAIL in CVIBuffer_Wall::NativeConstruct_Prototype");
		return E_FAIL;
	}

	FACEINDICES32*		pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	_uint		iNumPrimitive = 0;

	for (_uint i = 0; i < m_iSecondLine - 1; ++i)
	{
		for (_uint j = 0; j < m_iFirstLine - 1; ++j)
		{
			_uint iIndex = i * m_iFirstLine + j;

			_uint iIndices[4] = {
				iIndex + m_iFirstLine,
				iIndex + m_iFirstLine + 1,
				iIndex + 1,
				iIndex
			};

			pIndices[iNumPrimitive]._0 = iIndices[0];
			pIndices[iNumPrimitive]._1 = iIndices[1];
			pIndices[iNumPrimitive]._2 = iIndices[2];
			++iNumPrimitive;

			pIndices[iNumPrimitive]._0 = iIndices[0];
			pIndices[iNumPrimitive]._1 = iIndices[2];
			pIndices[iNumPrimitive]._2 = iIndices[3];
			++iNumPrimitive;
		}
	}

	m_pIB->Unlock();

	return S_OK;
}

HRESULT CVIBuffer_Wall::NativeConstruct(void * pArg)
{
	return S_OK;
}

CVIBuffer_Wall * CVIBuffer_Wall::Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iFirstLine, _uint iSecondLine)
{
	CVIBuffer_Wall*	pInstance = new CVIBuffer_Wall(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype(iFirstLine, iSecondLine)))
	{
		MSGBOX("Failed to Create CVIBuffer_Wall in CVIBuffer_Wall::Create");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CVIBuffer_Wall::Clone(void * pArg)
{
	CVIBuffer_Wall*	pInstance = new CVIBuffer_Wall(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create CVIBuffer_Wall in CVIBuffer_Wall::Clone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Wall::Free()
{
	__super::Free();

}
