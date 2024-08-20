#include "stdafx.h"
#include "..\public\VIBuffer_Cube_Rayzer.h"

CVIBuffer_Cube_Rayzer::CVIBuffer_Cube_Rayzer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer(pGraphic_Device)
{
}

CVIBuffer_Cube_Rayzer::CVIBuffer_Cube_Rayzer(const CVIBuffer_Cube_Rayzer & rhs)
	: CVIBuffer(rhs)
{
}

HRESULT CVIBuffer_Cube_Rayzer::NativeConstruct_Prototype()
{
	m_iStride = sizeof(VTXCUBETEX);
	m_iNumVertices = 8;
	m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);
	m_ePrimitiveType = D3DPT_TRIANGLELIST;
	m_iNumPrimitive = 12;

	if (FAILED(__super::Create_VertexBuffer()))
	{
		MSGBOX("__super::Create_VertexBuffer returned E_FAIL in CVIBuffer_Cube_Rayzer::NativeConstruct_Prototype");
		return E_FAIL;
	}

	m_pVertices = new VTXCUBETEX[m_iNumVertices];
	ZeroMemory(m_pVertices, sizeof(VTXCUBETEX) * m_iNumVertices);

	VTXCUBETEX*		pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0].vPosition = _float3(-0.5f, 0.5f, -0.5f);
	pVertices[0].vTexUV = pVertices[0].vPosition;

	pVertices[1].vPosition = _float3(0.5f, 0.5f, -0.5f);
	pVertices[1].vTexUV = pVertices[1].vPosition;

	pVertices[2].vPosition = _float3(0.5f, -0.5f, -0.5f);
	pVertices[2].vTexUV = pVertices[2].vPosition;

	pVertices[3].vPosition = _float3(-0.5f, -0.5f, -0.5f);
	pVertices[3].vTexUV = pVertices[3].vPosition;

	pVertices[4].vPosition = _float3(-0.5f, 0.5f, 10.5f);
	pVertices[4].vTexUV = pVertices[4].vPosition;

	pVertices[5].vPosition = _float3(0.5f, 0.5f, 10.5f);
	pVertices[5].vTexUV = pVertices[5].vPosition;

	pVertices[6].vPosition = _float3(0.5f, -0.5f, 10.5f);
	pVertices[6].vTexUV = pVertices[6].vPosition;

	pVertices[7].vPosition = _float3(-0.5f, -0.5f, 10.5f);
	pVertices[7].vTexUV = pVertices[7].vPosition;

	m_pVB->Unlock();

	m_iIndicesSize = sizeof(FACEINDICES16);
	m_eIndexFormat = D3DFMT_INDEX16;

	if (FAILED(__super::Create_IndexBuffer()))
	{
		MSGBOX("__super::Create_IndexBuffer returned E_FAIL in CVIBuffer_Cube_Rayzer::NativeConstruct_Prototype");
		return E_FAIL;
	}

	m_pIndices = new FACEINDICES16[m_iNumPrimitive];
	ZeroMemory(m_pIndices, sizeof(FACEINDICES16) * m_iNumPrimitive);

	FACEINDICES16*		pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	/* LEFT */
	pIndices[0]._0 = 1; pIndices[0]._1 = 5; pIndices[0]._2 = 6;
	pIndices[1]._0 = 1; pIndices[1]._1 = 6; pIndices[1]._2 = 2;

	/* RIGHT */
	pIndices[2]._0 = 4; pIndices[2]._1 = 0; pIndices[2]._2 = 3;
	pIndices[3]._0 = 4; pIndices[3]._1 = 3; pIndices[3]._2 = 7;

	/* TOP */
	pIndices[4]._0 = 4; pIndices[4]._1 = 5; pIndices[4]._2 = 1;
	pIndices[5]._0 = 4; pIndices[5]._1 = 1; pIndices[5]._2 = 0;

	/* BOTTOM */
	pIndices[6]._0 = 3; pIndices[6]._1 = 2; pIndices[6]._2 = 6;
	pIndices[7]._0 = 3; pIndices[7]._1 = 6; pIndices[7]._2 = 7;

	/* Back */
	pIndices[8]._0 = 5; pIndices[8]._1 = 4; pIndices[8]._2 = 7;
	pIndices[9]._0 = 5; pIndices[9]._1 = 7; pIndices[9]._2 = 6;

	/* Front */
	pIndices[10]._0 = 0; pIndices[10]._1 = 1; pIndices[10]._2 = 2;
	pIndices[11]._0 = 0; pIndices[11]._1 = 2; pIndices[11]._2 = 3;

	m_pIB->Unlock();

	memcpy(m_pIndices, pIndices, sizeof(FACEINDICES16) * m_iNumPrimitive);

	return S_OK;
}

HRESULT CVIBuffer_Cube_Rayzer::NativeConstruct(void * pArg)
{
	return S_OK;
}

CVIBuffer_Cube_Rayzer * CVIBuffer_Cube_Rayzer::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CVIBuffer_Cube_Rayzer*	pInstance = new CVIBuffer_Cube_Rayzer(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create CVIBuffer_Cube_Rayzer in CVIBuffer_Cube_Rayzer::Create");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CVIBuffer_Cube_Rayzer::Clone(void * pArg)
{
	CVIBuffer_Cube_Rayzer*	pInstance = new CVIBuffer_Cube_Rayzer(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create CVIBuffer_Cube_Rayzer in CVIBuffer_Cube_Rayzer::Clone");
		Safe_Release(pInstance);
	}

	return pInstance; return nullptr;
}

void CVIBuffer_Cube_Rayzer::Free()
{
	__super::Free();
}
