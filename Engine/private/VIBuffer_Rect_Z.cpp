#include "..\Public\VIBuffer_Rect_Z.h"

CVIBuffer_Rect_Z::CVIBuffer_Rect_Z(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer(pGraphic_Device)
{
}

CVIBuffer_Rect_Z::CVIBuffer_Rect_Z(const CVIBuffer_Rect_Z & rhs)
	: CVIBuffer(rhs)
{
}

HRESULT CVIBuffer_Rect_Z::NativeConstruct_Prototype()
{
	m_iStride = sizeof(VTXTEX);
	m_iNumVertices = 4;
	m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX1;
	m_ePrimitiveType = D3DPT_TRIANGLELIST;
	m_iNumPrimitive = 2;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	VTXTEX*			pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0].vPosition = _float3(-0.5f, 0.0f, 0.5f);
	pVertices[0].vTexUV = _float2(0.f, 0.f);

	pVertices[1].vPosition = _float3(0.5f, 0.0f, 0.5f);
	pVertices[1].vTexUV = _float2(1.f, 0.f);

	pVertices[2].vPosition = _float3(0.5f, 0.0f, -0.5f);
	pVertices[2].vTexUV = _float2(1.f, 1.f);

	pVertices[3].vPosition = _float3(-0.5f, 0.0f, -0.5f);
	pVertices[3].vTexUV = _float2(0.f, 1.f);

	m_pVB->Unlock();

	m_iIndicesSize = sizeof(FACEINDICES16);
	m_eIndexFormat = D3DFMT_INDEX16;
	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

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

HRESULT CVIBuffer_Rect_Z::NativeConstruct(void * pArg)
{
	return S_OK;
}

HRESULT CVIBuffer_Rect_Z::Go_Sprites(FRAME& tFrame)
{
	VTXTEX*		pVertices = nullptr;


	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0].vTexUV = _float2((_uint)tFrame.iFrameCurrentX / tFrame.iTexWidth * tFrame.iFrameWidth, tFrame.iFrameStartY / tFrame.iTexHeight * tFrame.iFrameHeight);
	//((VTXTEX*)m_pVertices)[0] = pVertices[0];

	pVertices[1].vTexUV = _float2(((_uint)tFrame.iFrameCurrentX + 1) / tFrame.iTexWidth * tFrame.iFrameWidth, tFrame.iFrameStartY / tFrame.iTexHeight * tFrame.iFrameHeight);
	//((VTXTEX*)m_pVertices)[1] = pVertices[1];

	pVertices[2].vTexUV = _float2(((_uint)tFrame.iFrameCurrentX + 1) / tFrame.iTexWidth * tFrame.iFrameWidth, (tFrame.iFrameStartY + 1) / tFrame.iTexHeight * tFrame.iFrameHeight);
	//((VTXTEX*)m_pVertices)[2] = pVertices[2];

	pVertices[3].vTexUV = _float2((_uint)tFrame.iFrameCurrentX / tFrame.iTexWidth * tFrame.iFrameWidth, (tFrame.iFrameStartY + 1) / tFrame.iTexHeight * tFrame.iFrameHeight);
	//((VTXTEX*)m_pVertices)[3] = pVertices[3];

	m_pVB->Unlock();



	return S_OK;
}

void CVIBuffer_Rect_Z::Go_Sprites_Reverse_Y(FRAME & tFrame)
{
	VTXTEX*		pVertices = nullptr;


	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[3].vTexUV = _float2((_uint)tFrame.iFrameCurrentX / tFrame.iTexWidth * tFrame.iFrameWidth, tFrame.iFrameStartY / tFrame.iTexHeight * tFrame.iFrameHeight);
	//((VTXTEX*)m_pVertices)[3] = pVertices[3];

	pVertices[2].vTexUV = _float2(((_uint)tFrame.iFrameCurrentX + 1) / tFrame.iTexWidth * tFrame.iFrameWidth, tFrame.iFrameStartY / tFrame.iTexHeight * tFrame.iFrameHeight);
	//((VTXTEX*)m_pVertices)[2] = pVertices[2];

	pVertices[1].vTexUV = _float2(((_uint)tFrame.iFrameCurrentX + 1) / tFrame.iTexWidth * tFrame.iFrameWidth, (tFrame.iFrameStartY + 1) / tFrame.iTexHeight * tFrame.iFrameHeight);
	//((VTXTEX*)m_pVertices)[1] = pVertices[1];

	pVertices[0].vTexUV = _float2((_uint)tFrame.iFrameCurrentX / tFrame.iTexWidth * tFrame.iFrameWidth, (tFrame.iFrameStartY + 1) / tFrame.iTexHeight * tFrame.iFrameHeight);
	//((VTXTEX*)m_pVertices)[0] = pVertices[0];

	m_pVB->Unlock();
}

HRESULT CVIBuffer_Rect_Z::Base_Setting()
{
	VTXTEX*		pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0].vPosition = _float3(-0.5f, 0.0f, 0.5f);
	pVertices[0].vTexUV = _float2(0.f, 0.f);

	pVertices[1].vPosition = _float3(0.5f, 0.0f, 0.5f);
	pVertices[1].vTexUV = _float2(1.f, 0.f);

	pVertices[2].vPosition = _float3(0.5f, 0.0f, -0.5f);
	pVertices[2].vTexUV = _float2(1.f, 1.f);

	pVertices[3].vPosition = _float3(-0.5f, 0.0f, -0.5f);
	pVertices[3].vTexUV = _float2(0.f, 1.f);

	m_pVB->Unlock();

	return S_OK;
}

CVIBuffer_Rect_Z * CVIBuffer_Rect_Z::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CVIBuffer_Rect_Z*	pInstance = new CVIBuffer_Rect_Z(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CVIBuffer_Rect_Z");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CVIBuffer_Rect_Z::Clone(void * pArg)
{
	CVIBuffer_Rect_Z*	pInstance = new CVIBuffer_Rect_Z(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CVIBuffer_Rect_Z");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Rect_Z::Free()
{
	__super::Free();

}
