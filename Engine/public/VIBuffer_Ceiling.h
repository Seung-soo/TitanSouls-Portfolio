#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Ceiling final : public CVIBuffer
{
protected:
	explicit CVIBuffer_Ceiling(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CVIBuffer_Ceiling(const CVIBuffer_Ceiling& rhs);
	virtual ~CVIBuffer_Ceiling() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(_uint iNumVerticesX, _uint iNumVerticesZ);
	virtual HRESULT NativeConstruct_Prototype(const _tchar* pHeightMapFilePath);
	virtual HRESULT NativeConstruct(void* pArg) override;

private:
	_uint			m_iNumVerticesX = 0;
	_uint			m_iNumVerticesZ = 0;

public:
	static CVIBuffer_Ceiling* Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iNumVerticesX, _uint iNumVerticesZ);
	static CVIBuffer_Ceiling* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pHeightMapFilePath);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END