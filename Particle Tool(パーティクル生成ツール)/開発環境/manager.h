//=============================================================================
//
// マネージャー処理 [manager.h]
// Author : Taiki Hayasaka
//
//=============================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "manager.h"
#include "main.h"
#include <stdio.h>

//=============================================================================
// 前方宣言
//=============================================================================
class CRenderer;			// レンダラクラス
class CInputKeyboard;		// キーボードクラス
class CGame;				// ゲームシーン
class CFade;				// フェード

//=============================================================================
// クラス
//=============================================================================
class CManager
{
public:
	// モード
	typedef enum
	{
		MODE_GAME,
		MODE_MAX,
	}MODE;

	CManager();
	~CManager();

	HRESULT	Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// デバイスの取得
	static CRenderer *GetRenderer(void) { return m_pRenderer; }						// レンダラー
	static CInputKeyboard *GetInputKeyboard(void) { return m_pInputKeyboard; }		// キーボード
	static CFade *GetFade(void) { return m_pFade; }									// フェード
	static CGame *GetGame(void) { return m_pGame; }									// ゲーム

	// モード
	static void CManager::SetMode(MODE mode);						// モードの設定　mode:モードの種類
	static MODE GetMode(void) { return m_mode; }					// デバイスの取得(モード)

private:
	// モード
	static CGame *m_pGame;											// ゲーム
	static MODE m_mode;												// モード

	static CRenderer *m_pRenderer;									// レンダリングのポインタ
	static CInputKeyboard *m_pInputKeyboard;						// キーボードのポインタ
	static CFade *m_pFade;											// フェードのポインタ
};

#endif