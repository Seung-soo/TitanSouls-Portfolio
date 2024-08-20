#pragma once
#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Jongyu.h"
BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect_Z;
class CVIBuffer_Cube;
END

BEGIN(Client)

class CJongyu_Shadow final : public CGameObject
{
private:
	explicit CJongyu_Shadow(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CJongyu_Shadow(const CJongyu_Shadow& rhs);
	virtual ~CJongyu_Shadow() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	/* 텍스쳐 */
	CTexture*			m_pTextureCom = nullptr;

	/* 모델 */
	CVIBuffer_Rect_Z*		m_pVIBufferCom = nullptr;

	/* 상태 (위치, 크기, 회전) */
	CTransform*			m_pTransformCom = nullptr;

	/* 그려진다. */
	CRenderer*			m_pRendererCom = nullptr;

	CJongyu*			m_pIcicle = nullptr;

private:
	_float3 m_fIciclePos;


	_bool btest = false;
	_float m_fFrame = 0.f;
	_float m_fPetternTimer = 0;

	_bool  m_bJump = false;
	_float m_fJumpPower = 0.f;
	_float m_fJumpy;
	_float m_fTime = 0;

	_float m_fScale = 6.f;



	_bool TestStop = false;
private:
	HRESULT SetUp_Components();
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();


public:
	static CJongyu_Shadow* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END