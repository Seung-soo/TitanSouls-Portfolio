#pragma once
#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Colossus_Fist.h"
BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect_Z;
class CVIBuffer_Cube;
END

BEGIN(Client)

class CColossusFist_Left_Shadow final : public CGameObject
{
private:
	explicit CColossusFist_Left_Shadow(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CColossusFist_Left_Shadow(const CColossusFist_Left_Shadow& rhs);
	virtual ~CColossusFist_Left_Shadow() = default;
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

	CColossus_Fist*			m_pColossus_Fist = nullptr;

private:
	_float3 m_fFistPos;

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
	HRESULT SetUp_OnTerrain(); // 플레이어를 터레인위에 올라가게 하는 함수
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();


public:
	static CColossusFist_Left_Shadow* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END