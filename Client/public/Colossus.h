#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Camera_Player.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
class CCollider_Rect;
class CCollider_Sphere;
class CVIBuffer_Cube;
END

BEGIN(Client)
class CCamera_Player;
class CColossus final : public CGameObject
{
private:
	explicit CColossus(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CColossus(const CColossus& rhs);
	virtual ~CColossus() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;

	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
public:
	_bool	Get_Start() { return m_bStart; }
	void	Set_Start(_bool bStart) { m_bStart = bStart; }
	_bool	Get_Clear() { return m_bClear; }
	void	Set_Clear(_bool bClear) { m_bClear = bClear; }
	void    Set_Cry() {
		m_bCry = true;
		m_iCryTime = 0;
	}
private:
	CTexture*			m_pTextureCom = nullptr;
	CVIBuffer_Rect*		m_pVIBufferCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CCollider_Rect*		m_pColliderCom = nullptr;
	CCollider_Sphere*	m_pWeaknessCom = nullptr;
	CVIBuffer_Cube*		m_pVIBufferCollCom = nullptr;
private:
	_float m_fScale = 4.f;
	COLOSSUSINFO* m_pColInfo;
	_bool m_bUpStanding = false;

	_bool m_bAttackOn = false;
	_bool m_bCry = false;
	_uint m_iCryTime = 0;

	_bool m_bSoulOut = false;
	//점프
	_bool  m_bJump = false;
	_float m_fJumpPower = 10.f;
	_float m_fJumpy;
	_float m_fTime = 0;
	//
	_float m_fUpDownTimer = 0;
	_bool m_bUp = false;

	_bool m_bStart = false;
	_bool m_bClear = false;

	CCamera_Player*		m_pCamera = nullptr;
	_int	m_iSoulOutTime = 120;
	_bool	m_iOutSoul = false;
private:
	HRESULT SetUp_Components();
	HRESULT SetUp_OnTerrain(); // 플레이어를 터레인위에 올라가게 하는 함수
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();
	HRESULT			Go_Sprites();


public:
	static CColossus* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END