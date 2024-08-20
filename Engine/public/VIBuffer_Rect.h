#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Rect final : public CVIBuffer
{
private:
	explicit CVIBuffer_Rect(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CVIBuffer_Rect(const CVIBuffer_Rect& rhs);
	virtual ~CVIBuffer_Rect() = default;
public:
	virtual HRESULT				NativeConstruct_Prototype() override;
	virtual HRESULT				NativeConstruct(void* pArg) override;
public:
	HRESULT						Go_Sprites(FRAME& tFrame);
	void						Go_Sprites_Reverse_Y(FRAME& tFrame);
	HRESULT						Base_Setting();
public:
	static CVIBuffer_Rect*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent*			Clone(void* pArg);
	virtual void				Free() override;
};

END