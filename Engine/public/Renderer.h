#pragma once

#include "Component.h"


BEGIN(Engine)
class CGameObject;

class ENGINE_DLL CRenderer final : public CComponent
{
public:
	enum RENDERGROUP { RENDER_PRIORITY, RENDER_NONALPHA, RENDER_ALPHA, RENDER_UI, RENDER_END };
private:
	explicit CRenderer(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CRenderer() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
public:
	HRESULT			Add_RenderGroup(RENDERGROUP eRenderGroup, CGameObject* pRenderObject);
	HRESULT			Render();
private:
	list<CGameObject*>			m_RenderObjects[RENDER_END];
	typedef list <CGameObject>	RENDEROBJECTS;
private:
	HRESULT Render_Priority();
	HRESULT Render_NonAlpha();
	HRESULT Render_Alpha();
	HRESULT Render_UI();
public:
	static CRenderer*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent*	Clone(void* pArg) override;
	virtual void		Free() override;
};

END