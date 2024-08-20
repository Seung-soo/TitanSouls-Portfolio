#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Arrow.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CArrow_Rect final : public CGameObject
{
public:
	explicit CArrow_Rect(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CArrow_Rect(const CArrow_Rect& rhs);
	virtual ~CArrow_Rect() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	CTransform* Get_TransForm() {
		return m_pTransformCom;
	}

private:
	/* 텍스쳐 */
	CTexture*			m_pTextureCom = nullptr;

	/* 모델 */
	CVIBuffer_Rect*		m_pVIBufferCom = nullptr;

	/* 상태 (위치, 크기, 회전) */
	CTransform*			m_pTransformCom = nullptr;

	/* 그려진다. */
	CRenderer*			m_pRendererCom = nullptr;

private:
	HRESULT SetUp_Components();
	//HRESULT	Go_Sprites();
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

private:
	_int m_iAlpha = 255;
	_float m_fScale = 1.f;


public:
	static CArrow_Rect* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END