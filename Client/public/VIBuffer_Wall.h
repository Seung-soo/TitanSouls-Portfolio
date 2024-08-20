#pragma once

#include "Client_Defines.h"
#include "VIBuffer.h"

BEGIN(Client)

class CVIBuffer_Wall final : public CVIBuffer
{
protected:
	explicit CVIBuffer_Wall(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CVIBuffer_Wall(const CVIBuffer_Wall& rhs);
	virtual ~CVIBuffer_Wall() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(_uint iFirstLine, _uint iSecondLine);
	virtual HRESULT NativeConstruct(void* pArg) override;

private:
	_uint			m_iFirstLine = 0;
	_uint			m_iSecondLine = 0;
public:
	static CVIBuffer_Wall* Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iFirstLine, _uint iSecondLine);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END