#pragma once

#include "Client_Defines.h"
#include "Effect.h"

BEGIN(Engine)
class CRenderer;
class CTransform;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)
class CArrow;
class CArrowLight final : public CEffect
{
private:
	explicit CArrowLight(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CArrowLight(const CArrowLight& rhs);
	virtual ~CArrowLight() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float fTimeDelta) override;
	virtual _int	LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
private:
	CTexture*			m_pTextureCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CVIBuffer_Rect*		m_pVIBufferCom = nullptr;
private:
	CArrow*				m_pArrow = nullptr;
private:
	HRESULT			SetUp_Components();
	HRESULT			SetUp_RenderState();
	HRESULT			Release_RenderState();
	HRESULT			Go_Sprites();
	void			Go_FrameLight(_float fSpeed, _float fTimeDelta);
public:
	static CArrowLight*			Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg) override;
	virtual void			Free() override;
};

END