#pragma once
#include "Client_Defines.h"
#include "UI.h"
BEGIN(Engine)
class CTransform;
END

BEGIN(Client)
class CUI_NameEmpty final : public CUI
{
private:
	explicit CUI_NameEmpty(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CUI_NameEmpty(const CUI_NameEmpty& rhs);
	virtual ~CUI_NameEmpty() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float fTimeDelta) override;
	virtual _int	LateTick(_float fTimeDelta) override;
public:
	void	Ready_Level_UI(LEVEL eCurrentLevel, _tchar* pLayerTag);
	HRESULT Ready_BossName_UI(LEVEL eCurrentLevel, _tchar* pLayerTag);
private:
	CTransform*			m_pTransformCom = nullptr;
	//EFFECTINFO			m_tEffectInfo;
	_int				m_iInterval = 0;
	_int				m_iDirCnt = 0;
	LEVEL				m_eCurrentLevel;
private:
	virtual		HRESULT SetUp_Components(void);
public:
	static CUI_NameEmpty*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CUI*		Clone(void* pArg) override;
	virtual void				Free() override;
};
END