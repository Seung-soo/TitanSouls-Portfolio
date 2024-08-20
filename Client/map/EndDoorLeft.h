#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CEndDoorLeft final : public CGameObject
{
public:
	explicit CEndDoorLeft(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CEndDoorLeft(const CEndDoorLeft& rhs);
	virtual ~CEndDoorLeft() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float fTimeDelta) override;
	virtual _int	LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	CTransform* Get_TransForm() { return m_pTransformCom; }

public:
	void			Set_BoolCnt(_bool bCnt) { m_bCnt = bCnt; }
	void			Set_DoorSprite(_bool bDoorSprite) { m_bDoorSprite = bDoorSprite; }
	void			Set_DoorOpen(_bool bDoorOpen) { m_bDoorOpen = bDoorOpen; }
private:
	CTexture*			m_pTextureCom = nullptr;
	CVIBuffer_Rect*		m_pVIBufferCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;

	_float			m_fFrame = 0.f;
	_bool			m_bCnt = false;
	_bool			bSound = true;// »ç¿îµå
	_uint			m_iCnt = 0;

	_bool			m_bDoorOpen = false;
	_bool			m_bDoorSprite = false;
	_float			fDoorOpenFrame = 0.f;


	_float			fRadian = 0.f;
	HRESULT			Go_Sprites();

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

public:
	static	CEndDoorLeft*			Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg) override;
	virtual void			Free() override;
};

END