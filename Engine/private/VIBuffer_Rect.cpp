#include "..\public\VIBuffer_Rect.h"

CVIBuffer_Rect::CVIBuffer_Rect(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer(pGraphic_Device)
{
}

CVIBuffer_Rect::CVIBuffer_Rect(const CVIBuffer_Rect & rhs)
	: CVIBuffer(rhs)
{
}

HRESULT CVIBuffer_Rect::NativeConstruct_Prototype()
{
	m_iStride = sizeof(VTXTEX);
	m_iNumVertices = 4;
	m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX1;
	m_ePrimitiveType = D3DPT_TRIANGLELIST;
	m_iNumPrimitive = 2;

	if (FAILED(__super::Create_VertexBuffer()))
	{
		MSGBOX("__super::Create_VertexBuffer returned E_FAIL in CVIBuffer_Rect::NativeConstruct_Prototype");
		return E_FAIL;
	}

	VTXTEX*		pVertices = nullptr;

	m_pVertices = new VTXTEX[m_iNumVertices];
	ZeroMemory(m_pVertices, sizeof(VTXTEX) * m_iNumVertices);

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0].vPosition = _float3(-0.5f, 0.5f, 0.f);
	pVertices[0].vTexUV = _float2(0.f, 0.f);
	((VTXTEX*)m_pVertices)[0] = pVertices[0];

	pVertices[1].vPosition = _float3(0.5f, 0.5f, 0.f);
	pVertices[1].vTexUV = _float2(1.f, 0.f);
	((VTXTEX*)m_pVertices)[1] = pVertices[1];

	pVertices[2].vPosition = _float3(0.5f, -0.5f, 0.f);
	pVertices[2].vTexUV = _float2(1.f, 1.f);
	((VTXTEX*)m_pVertices)[2] = pVertices[2];

	pVertices[3].vPosition = _float3(-0.5f, -0.5f, 0.f);
	pVertices[3].vTexUV = _float2(0.f, 1.f);
	((VTXTEX*)m_pVertices)[3] = pVertices[3];

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

HRESULT CVIBuffer_Rect::NativeConstruct(void * pArg)
{
	return S_OK;
}

HRESULT CVIBuffer_Rect::Go_Sprites(FRAME& tFrame)
{
	VTXTEX*		pVertices = nullptr;


	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0].vTexUV = _float2((_uint)tFrame.iFrameCurrentX / tFrame.iTexWidth * tFrame.iFrameWidth, tFrame.iFrameStartY / tFrame.iTexHeight * tFrame.iFrameHeight);
	((VTXTEX*)m_pVertices)[0] = pVertices[0];

	pVertices[1].vTexUV = _float2(((_uint)tFrame.iFrameCurrentX + 1) / tFrame.iTexWidth * tFrame.iFrameWidth, tFrame.iFrameStartY / tFrame.iTexHeight * tFrame.iFrameHeight);
	((VTXTEX*)m_pVertices)[1] = pVertices[1];

	pVertices[2].vTexUV = _float2(((_uint)tFrame.iFrameCurrentX + 1) / tFrame.iTexWidth * tFrame.iFrameWidth, (tFrame.iFrameStartY + 1) / tFrame.iTexHeight * tFrame.iFrameHeight);
	((VTXTEX*)m_pVertices)[2] = pVertices[2];

	pVertices[3].vTexUV = _float2((_uint)tFrame.iFrameCurrentX / tFrame.iTexWidth * tFrame.iFrameWidth, (tFrame.iFrameStartY + 1) / tFrame.iTexHeight * tFrame.iFrameHeight);
	((VTXTEX*)m_pVertices)[3] = pVertices[3];

	m_pVB->Unlock();

	

	return S_OK;
}

void CVIBuffer_Rect::Go_Sprites_Reverse_Y(FRAME & tFrame)
{
	VTXTEX*		pVertices = nullptr;


	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[3].vTexUV = _float2((_uint)tFrame.iFrameCurrentX / tFrame.iTexWidth * tFrame.iFrameWidth, tFrame.iFrameStartY / tFrame.iTexHeight * tFrame.iFrameHeight);
	((VTXTEX*)m_pVertices)[3] = pVertices[3];

	pVertices[2].vTexUV = _float2(((_uint)tFrame.iFrameCurrentX + 1) / tFrame.iTexWidth * tFrame.iFrameWidth, tFrame.iFrameStartY / tFrame.iTexHeight * tFrame.iFrameHeight);
	((VTXTEX*)m_pVertices)[2] = pVertices[2];

	pVertices[1].vTexUV = _float2(((_uint)tFrame.iFrameCurrentX + 1) / tFrame.iTexWidth * tFrame.iFrameWidth, (tFrame.iFrameStartY + 1) / tFrame.iTexHeight * tFrame.iFrameHeight);
	((VTXTEX*)m_pVertices)[1] = pVertices[1];

	pVertices[0].vTexUV = _float2((_uint)tFrame.iFrameCurrentX / tFrame.iTexWidth * tFrame.iFrameWidth, (tFrame.iFrameStartY + 1) / tFrame.iTexHeight * tFrame.iFrameHeight);
	((VTXTEX*)m_pVertices)[0] = pVertices[0];

	m_pVB->Unlock();
}

HRESULT CVIBuffer_Rect::Base_Setting()
{
	VTXTEX*		pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0].vPosition = _float3(-0.5f, 0.5f, 0.f);
	pVertices[0].vTexUV = _float2(0.f, 0.f);
	((VTXTEX*)m_pVertices)[0] = pVertices[0];

	pVertices[1].vPosition = _float3(0.5f, 0.5f, 0.f);
	pVertices[1].vTexUV = _float2(1.f, 0.f);
	((VTXTEX*)m_pVertices)[1] = pVertices[1];

	pVertices[2].vPosition = _float3(0.5f, -0.5f, 0.f);
	pVertices[2].vTexUV = _float2(1.f, 1.f);
	((VTXTEX*)m_pVertices)[2] = pVertices[2];

	pVertices[3].vPosition = _float3(-0.5f, -0.5f, 0.f);
	pVertices[3].vTexUV = _float2(0.f, 1.f);
	((VTXTEX*)m_pVertices)[3] = pVertices[3];

	m_pVB->Unlock();

	return S_OK;
}

CVIBuffer_Rect * CVIBuffer_Rect::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CVIBuffer_Rect*	pInstance = new CVIBuffer_Rect(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create Rect in CVIBuffer_Rect::Create");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CVIBuffer_Rect::Clone(void * pArg)
{
	CVIBuffer_Rect*	pInstance = new CVIBuffer_Rect(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create Clone in CVIBuffer_Rect::Clone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Rect::Free()
{
	__super::Free();
}
