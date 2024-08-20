#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer abstract : public CComponent
{
protected:
	explicit CVIBuffer(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CVIBuffer(const CVIBuffer& rhs);
	virtual ~CVIBuffer() = default;

public:
	virtual HRESULT				NativeConstruct_Prototype() override;
	virtual HRESULT				NativeConstruct(void* pArg) override;

public:
	virtual HRESULT				Render();

protected:
	LPDIRECT3DVERTEXBUFFER9		m_pVB = nullptr;
	_uint						m_iStride = 0;
	_uint						m_iNumVertices = 0;
	_uint						m_dwFVF = 0;
	void*						m_pVertices = nullptr;

	D3DPRIMITIVETYPE			m_ePrimitiveType;
	_uint						m_iNumPrimitive = 0;

	LPDIRECT3DINDEXBUFFER9		m_pIB = nullptr;
	_uint						m_iIndicesSize = 0;
	D3DFORMAT					m_eIndexFormat;
	void*						m_pIndices = nullptr;


protected:
	HRESULT						Create_VertexBuffer();
	HRESULT						Create_IndexBuffer();

public:
	virtual CComponent*			Clone(void* pArg) = 0;
	virtual void				Free() override;
};

END