#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Player.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CPlayer_Dust final : public CGameObject
{
public:
	explicit CPlayer_Dust(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CPlayer_Dust(const CPlayer_Dust& rhs);
	virtual ~CPlayer_Dust() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	CTransform* Get_TransForm() 
	{
		return m_pTransformCom;
	}

private:
	/* �ؽ��� */
	CTexture*			m_pTextureCom = nullptr;

	/* �� */
	CVIBuffer_Rect*		m_pVIBufferCom = nullptr;

	/* ���� (��ġ, ũ��, ȸ��) */
	CTransform*			m_pTransformCom = nullptr;

	/* �׷�����. */
	CRenderer*			m_pRendererCom = nullptr;

private:
	HRESULT SetUp_Components();
	HRESULT			Go_Sprites();
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

private:
	_int m_iAlpha = 255;
	_float m_fScale = 0.8f;


public:
	static CPlayer_Dust* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END