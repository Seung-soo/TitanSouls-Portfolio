#pragma once

#include "Base.h"
#include "Timer_Manager.h"
#include "Graphic_Device.h"
#include "Level_Manager.h"
#include "Object_Manager.h"
#include "Component_Manager.h"
#include "Input_Device.h"
#include "Picking.h"
#include "SoundMgr.h"

BEGIN(Engine)

class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance)
public:
	CGameInstance();
	virtual ~CGameInstance() = default;
public:
	HRESULT			Initialize_Engine(HINSTANCE hInstance, const CGraphic_Device::GRAPHICDESC& GraphicDesc, _uint iNumLevels, LPDIRECT3DDEVICE9* ppOut);
	_int			Tick_Engine(_float fTimeDelta);
	HRESULT			Clear_LevelResource(_uint iLevelIndex);
public:		// 그래픽 디바이스 기능
	void			Render_Begin();
	void			Render_End();
public:		// 타이머 매니저 기능
	_float			Get_TimeDelta(const _tchar* pTimerTag); // 델타타임 반환
	HRESULT			Add_Timer(const _tchar* pTimerTag);		// 새로운 타이머 추가
public:		// 레벨 매니저 기능
	HRESULT			OpenLevel(_uint iNextLevelIndex, CLevel* pNextLevel);
	HRESULT			Render_Level();
public:		// 오브젝트 매니저 기능
	CLayer*			Find_Layer(_uint iLevelIndex, const _tchar * pLayerTag);
	CComponent*		Get_Component(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pComponentTag, _uint iIndex = 0);
	HRESULT			Add_Prototype(const _tchar* pPrototypeTag, CGameObject* pPrototype);
	HRESULT			Add_GameObject(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pPrototypeTag, void* pArg = nullptr);
	HRESULT			Release_LayerObject(_uint iLevelIndex, const _tchar* pLayerTag);
	size_t			Get_Size(_uint iLevelIndex, const _tchar* pLayerTag);
	CGameObject*	Get_Front(_uint iLevelIndex, const _tchar* pLayerTag);
public:		// 컴포넌트 매니저 기능
	HRESULT			Add_Prototype(_uint iLevelIndex, const _tchar* pPrototypeTag, CComponent* pPrototype);
	CComponent*		Clone_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, void* pArg = nullptr);
	_bool			Collision_Sphere(CCollider_Sphere* pDst, CCollider_Sphere* pSrc);
	_float3			Collision_SphereGuard(CCollider_Sphere* pDst, CCollider_Sphere* pSrc);
	_float3			Collision_SphereBounce(CCollider_Sphere* pDst, CCollider_Sphere* pSrc, _float3 vLook);
	_float3			Collision_SphereToRect(CCollider_Rect* pDst, CCollider_Sphere* pSrc);
	_float3			Collision_SphereToRectZ(CCollider_Rect_Z* pDst, CCollider_Sphere* pSrc);
	_float3			Collision_SphereToRect_Camera(CCollider_Rect* pDst, CCollider_Sphere* pSrc);
	_float3			Collision_SphereToRectArrow(CCollider_Rect* pDst, CCollider_Sphere* pSrc);
	_float3			Collision_SphereToRectArrowFull(CCollider_Rect* pDst, CCollider_Sphere* pSrc);
public:		// 인풋 디바이스 기능
	_byte Get_DIKeyState(_ubyte eKeyID);
	_long Get_DIMouseMoveState(CInput_Device::MOUSEMOVESTATE eMouseMoveState);
	_byte Get_DIMouseButtonState(CInput_Device::MOUSEBUTTONSTATE eMouseButtonState);
	_bool Get_Mouse_Up(CInput_Device::MOUSEBUTTONSTATE eMouseButtonState);
public:		// 사운드 매니저 기능
	int  VolumeUp(CSoundMgr::CHANNELID eID, _float _vol);
	int  VolumeDown(CSoundMgr::CHANNELID eID, _float _vol);
	int  BGMVolumeUp(_float _vol);
	int  BGMVolumeDown(_float _vol);
	int  Pause(CSoundMgr::CHANNELID eID);
	void LetsPlaySound(TCHAR* pSoundKey, CSoundMgr::CHANNELID eID, _float _vol);
	void PlayBGM(TCHAR* pSoundKey, _float _vol);
	void StopSound(CSoundMgr::CHANNELID eID);
	void StopAll();
private:
	CGraphic_Device*		m_pGraphic_Device = nullptr;
	CTimer_Manager*			m_pTimer_Manager = nullptr;
	CLevel_Manager*			m_pLevel_Manager = nullptr;
	CObject_Manager*		m_pObject_Manager = nullptr;
	CComponent_Manager*		m_pComponent_Manager = nullptr;
	CInput_Device*			m_pInput_Device = nullptr;
	CPicking*				m_pPicking = nullptr;
	CSoundMgr*				m_pSound_Manager = nullptr;
public:
	static void		Release_Engine();						// 매니저 싱글톤 정리
	virtual void	Free() override;
};

END