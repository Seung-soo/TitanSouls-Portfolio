#pragma once

#include "Client_Defines.h"
#include "VIBuffer.h"

BEGIN(Client)

class CVIBuffer_Cube_Rayzer final : public CVIBuffer
{
private:
	explicit CVIBuffer_Cube_Rayzer(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CVIBuffer_Cube_Rayzer(const CVIBuffer_Cube_Rayzer& rhs);
	virtual ~CVIBuffer_Cube_Rayzer() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;

public:
	static CVIBuffer_Cube_Rayzer* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END