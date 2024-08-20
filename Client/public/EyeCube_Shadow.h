#pragma once
#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "EyeCube.h"
BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect_Z;
class CVIBuffer_Cube;
END

BEGIN(Client)

class CEyeCube_Shadow final : public CGameObject
{
private:
	explicit CEyeCube_Shadow(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CEyeCube_Shadow(const CEyeCube_Shadow& rhs);
	virtual ~CEyeCube_Shadow() = default;
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

	CEyeCube*			m_pEyeCube = nullptr;

private:
	_float3 m_fFistPos;

	_bool btest = false;
	_float m_fFrame = 0.f;
	_float m_fPetternTimer = 0;

	_bool  m_bJump = false;
	_float m_fJumpPower = 0.f;
	_float m_fJumpy;
	_float m_fTime = 0;

	_float m_fScale = 4.f;



	_bool TestStop = false;
private:
	HRESULT SetUp_Components();
	HRESULT SetUp_OnTerrain(); // 플레이어를 터레인위에 올라가게 하는 함수
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();


public:
	static CEyeCube_Shadow* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END