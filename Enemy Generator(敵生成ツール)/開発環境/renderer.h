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
	void DrawFPS(void);
	void DrawValue(void);


	LPDIRECT3D9 m_pD3D = NULL;
	LPDIRECT3DDEVICE9 m_pD3DDevice = NULL;

	LPD3DXFONT m_pFont = NULL;
};

#endif