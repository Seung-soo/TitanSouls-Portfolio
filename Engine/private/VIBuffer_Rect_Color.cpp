#include "..\public\VIBuffer_Rect_Color.h"

CVIBuffer_Rect_Color::CVIBuffer_Rect_Color(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer(pGraphic_Device)
{
}

CVIBuffer_Rect_Color::CVIBuffer_Rect_Color(const CVIBuffer_Rect_Color & rhs)
	: CVIBuffer(rhs)
{
}

HRESULT CVIBuffer_Rect_Color::NativeConstruct_Prototype()
{
	m_iStride = sizeof(VTXTEX_COLOR);
	m_iNumVertices = 4;
	m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_DIFFUSE;
	m_ePrimitiveType = D3DPT_TRIANGLELIST;
	m_iNumPrimitive = 2;

	if (FAILED(__super::Create_VertexBuffer()))
	{
		MSGBOX("__super::Create_VertexBuffer returned E_FAIL in CVIBuffer_Rect::NativeConstruct_Prototype");
		return E_FAIL;
	}

	VTXTEX_COLOR*		pVertices = nullptr;

	m_pVertices = new VTXTEX_COLOR[m_iNumVertices];
	ZeroMemory(m_pVertices, sizeof(VTXTEX_COLOR) * m_iNumVertices);

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0].vPosition = _float3(-0.15f, 4.5f, 0.001f);
	pVertices[0].dwColor = D3DCOLOR_ARGB(150, 200, 200, 200);
	pVertices[0].vTexUV = _float2(0.f, 0.f);

	((VTXTEX_COLOR*)m_pVertices)[0] = pVertices[0];
	
	pVertices[1].vPosition = _float3(0.15f, 4.5f, 0.001f);
	pVertices[1].dwColor = D3DCOLOR_ARGB(150, 200, 200, 200);
	pVertices[1].vTexUV = _float2(1.f, 0.f);

	((VTXTEX_COLOR*)m_pVertices)[1] = pVertices[1];

	pVertices[2].vPosition = _float3(0.02f, 0.f, 0.001f);
	pVertices[2].dwColor = D3DCOLOR_ARGB(255, 255, 255, 255);
	pVertices[2].vTexUV = _float2(1.f, 1.f);

	((VTXTEX_COLOR*)m_pVertices)[2] = pVertices[2];

	pVertices[3].vPosition = _float3(-0.02f, 0.f, 0.001f);
	pVertices[3].dwColor = D3DCOLOR_ARGB(255, 255, 255, 255);
	pVertices[3].vTexUV = _float2(0.f, 1.f);

	((VTXTEX_COLOR*)m_pVertices)[3] = pVertices[3];

	m_pVB->Unlock();

	m_iIndicesSize = sizeof(FACEINDICES16);
	m_eIndexFormat = D3DFMT_INDEX16;

	if (FAILED(__super::Create_IndexBuffer()))
	{
		MSGBOX("__super::Create_IndexBuffer returned E_FAIL in CVIBuffer_Rect::NativeConstruct_Prototype");
		return E_FAIL;
	}

	FACEINDICES16*		pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	pIndices[0]._0 = 0;
	pIndices[0]._1 = 1;
	pIndices[0]._2 = 2;

	pIndices[1]._0 = 0;
	pIndices[1]._1 = 2;
	pIndices[1]._2 = 3;

	m_pIB->Unlock();

	return S_OK;
}

HRESULT CVIBuffer_Rect_Color::NativeConstruct(void * pArg)
{
	return S_OK;
}

CVIBuffer_Rect_Color * CVIBuffer_Rect_Color::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CVIBuffer_Rect_Color*	pInstance = new CVIBuffer_Rect_Color(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create Rect in CVIBuffer_Rect_Color::Create");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CVIBuffer_Rect_Color::Clone(void * pArg)
{
	CVIBuffer_Rect_Color*	pInstance = new CVIBuffer_Rect_Color(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create Clone in CVIBuffer_Rect_Color::Clone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Rect_Color::Free()
{
	__super::Free();
}
