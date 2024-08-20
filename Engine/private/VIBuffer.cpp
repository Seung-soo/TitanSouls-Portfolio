#include "..\public\VIBuffer.h"

CVIBuffer::CVIBuffer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{
}

CVIBuffer::CVIBuffer(const CVIBuffer & rhs)
	: CComponent(rhs)
	, m_pVB(rhs.m_pVB)
	, m_pIB(rhs.m_pIB)
	, m_iStride(rhs.m_iStride)
	, m_iNumVertices(rhs.m_iNumVertices)
	, m_dwFVF(rhs.m_dwFVF)
	, m_ePrimitiveType(rhs.m_ePrimitiveType)
	, m_iNumPrimitive(rhs.m_iNumPrimitive)
	, m_iIndicesSize(rhs.m_iIndicesSize)
	, m_eIndexFormat(rhs.m_eIndexFormat)
	, m_pVertices(rhs.m_pVertices)
{
	Safe_AddRef(m_pIB);
	Safe_AddRef(m_pVB);
}

HRESULT CVIBuffer::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		MSGBOX("__super::NativeConstruct_Prototype returned E_FAIL in CVIBuffer::NativeConstruct_Prototype");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CVIBuffer::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		MSGBOX("__super::NativeConstruct returned E_FAIL in CVIBuffer::NativeConstruct_Prototype");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CVIBuffer::Render()
{
	if (nullptr == m_pVB)
	{
		MSGBOX("m_pVB is nullptr in CVIBuffer::Render");
		return E_FAIL;
	}

	m_pGraphic_Device->SetStreamSource(0, m_pVB, 0, m_iStride);
	m_pGraphic_Device->SetIndices(m_pIB);
	m_pGraphic_Device->SetFVF(m_dwFVF);

	m_pGraphic_Device->DrawIndexedPrimitive(m_ePrimitiveType, 0, 0, m_iNumVertices, 0, m_iNumPrimitive);

	return S_OK;
}

HRESULT CVIBuffer::Create_VertexBuffer()
{
	if (nullptr == m_pGraphic_Device)
	{
		MSGBOX("m_pGraphic_Device is nullptr in CVIBuffer::Create_VertexBuffer");
		return E_FAIL;
	}

	if (FAILED(m_pGraphic_Device->CreateVertexBuffer(m_iStride * m_iNumVertices, 0, m_dwFVF, D3DPOOL_MANAGED, &m_pVB, nullptr)))
	{
		MSGBOX("m_pGraphic_Device->CreateVertexBuffer returned E_FAIL in CVIBuffer::Create_VertexBuffer");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CVIBuffer::Create_IndexBuffer()
{
	if (nullptr == m_pGraphic_Device)
	{
		MSGBOX("m_pGraphic_Device is nullptr in CVIBuffer::Create_IndexBuffer");
		return E_FAIL;
	}

	if (FAILED(m_pGraphic_Device->CreateIndexBuffer(m_iIndicesSize * m_iNumPrimitive, 0, m_eIndexFormat, D3DPOOL_MANAGED, &m_pIB, nullptr)))
	{
		MSGBOX("m_pGraphic_Device->CreateIndexBuffer returned E_FAIL in CVIBuffer::Create_IndexBuffer");
		return E_FAIL;
	}

	return S_OK;
}

void CVIBuffer::Free()
{
	__super::Free();

	if (false == m_isCloned)
	{
		Safe_Delete_Array(m_pVertices);
		Safe_Delete_Array(m_pIndices);
	}
	Safe_Release(m_pIB);
	Safe_Release(m_pVB);
}
