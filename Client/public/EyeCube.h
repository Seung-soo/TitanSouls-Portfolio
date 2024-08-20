#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
class CVIBuffer_Cube;
class CCollider_Sphere;
END

BEGIN(Client)
class CCamera_Player;
class CEyeCube final : public CGameObject
{
private:
	explicit CEyeCube(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CEyeCube(const CEyeCube& rhs);
	virtual ~CEyeCube() = default;
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

	_bool Get_Jump() {
		return m_bJump;
	}
	_bool	Get_Clear() { return m_bClear; }
	void	Set_Clear(_bool bClear) { m_bClear = bClear; }
private:
	/* 텍스쳐 */
	CTexture*			m_pTextureCom = nullptr;

	/* 모델 */
	CVIBuffer_Cube*		m_pVIBufferCom = nullptr;

	/* 상태 (위치, 크기, 회전) */
	CTransform*			m_pTransformCom = nullptr;

	/* 그려진다. */
	CRenderer*			m_pRendererCom = nullptr;
	CCollider_Sphere*	m_pColliderCom = nullptr;
	CCollider_Sphere*	m_pWeaknessCom = nullptr;
	CVIBuffer_Cube*		m_pVIBufferCollCom = nullptr;
private:
	_bool btest = false;
	_float m_fFrame = 0.f;
	_float m_fPetternTimer = 0;

	_bool m_bBlackTileRayzer = false;
	_bool m_bWhiteTileRayzer = false;

	_bool  m_bJump = false;
	_float m_fJumpPower = 10.f;
	_float m_fJumpy;
	_float m_fTime = 0;

	_float3 RevelPos = { 0,0,0 };
	_bool* m_bDead = false;


	//구르기 속도
	_float m_fRollingSpeed = 2.f;
	_uint m_iRollingTimer = 0;
	//구른횟수 체크
	_uint m_iRollingCount = 0;;
	//위치 검사용
	_float3 m_fCompaerCubePos;
	_float3 m_fCompaerPlayerPos;

	//이동용
	_bool m_bGoZUP;
	_bool m_bGoZDown;
	_bool m_bGoXRight;
	_bool m_bGoXLeft;

	_bool m_bRayzerPattern = false;
	_float m_fPatternTime = 0;
	_bool m_bRayzer_Jump = false;
	_float3 m_fRayerUsePos;
	// save용
	_float3 m_fDir;
	_float m_fDir_Length;
	//항시 검사용
	_float3 m_fDir_Check;
	_float m_fDir_Length_Check;

	_bool TestStop = false;
	_bool m_bClear = false;

	_bool m_bStart = false;

	CCamera_Player*		m_pCamera = nullptr;
	_int	m_iSoulOutTime = 120;
	_bool	m_iOutSoul = false;

	RAYZERPOS m_tRayPos;
	_float	fSoundTimer = 0.f;
private:
	HRESULT SetUp_Components();
	HRESULT SetUp_OnTerrain(); // 플레이어를 터레인위에 올라가게 하는 함수
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();


public:
	static CEyeCube* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END