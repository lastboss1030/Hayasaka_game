//=============================================================================
//
// レンダリング処理 [renderer.h]
// Author : Taiki Hayasaka
//
//=============================================================================
#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "main.h"

//=============================================================================
// クラス
//=============================================================================
class CRenderer
{
public:
	CRenderer();
	~CRenderer();

	HRESULT Init(HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	LPDIRECT3DDEVICE9 GetDevice(void) { return m_pD3DDevice; }

private:
#ifdef _DEBUG
	void DrawFPS(void);
#endif

	LPDIRECT3D9 m_pD3D = NULL;
	LPDIRECT3DDEVICE9 m_pD3DDevice = NULL;

#ifdef _DEBUG
	LPD3DXFONT m_pFont = NULL;
#endif
};

#endif