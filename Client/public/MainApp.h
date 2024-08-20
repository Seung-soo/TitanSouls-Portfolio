#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CGameInstance;
class CRenderer;
END

BEGIN(Client)

class CMainApp final : public CBase
{
private:
	CMainApp();
	virtual ~CMainApp() = default;
public:
	HRESULT				NativeConstruct();		// �ʱ�ȭ �Լ�
	_int				Tick(_float fTimeDelta);	// ����
	HRESULT				Render();				// �׸�
private:
	LPDIRECT3DDEVICE9	m_pGraphic_Device = nullptr;
	CGameInstance*		m_pGameInstance = nullptr;
	CRenderer*			m_pRenderer = nullptr;
private:
	HRESULT				OpenLevel(LEVEL eNextLevel);	// ù ���� ����
	HRESULT				DefaultSetting();
	HRESULT				Ready_Prototype_GameObject();
	HRESULT				Ready_Prototype_Component();
public:
	static CMainApp*	Create();
	virtual void		Free() override;
};

END