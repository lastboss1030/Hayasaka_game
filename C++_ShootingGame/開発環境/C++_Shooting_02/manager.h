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

//=============================================================================
// 前方宣言
//=============================================================================
class CRenderer;			// レンダラクラス
class CInputKeyboard;		// キーボードクラス
class CSound;				// サウンドクラス
class CGame;				// ゲームシーン
class CTitle;				// タイトルシーン
class CResult;				// リザルトシーン
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
		MODE_TITLE = 0,
		MODE_GAME,
		MODE_RESULT,
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
	static CSound *GetSound(void) { return m_pSound; }								// サウンド
	static CFade *GetFade(void) { return m_pFade; }									// フェード
	static CGame *GetGame(void) { return m_pGame; }									// ゲーム

	// モード
	static void CManager::SetMode(MODE mode);						// モードの設定　mode:モードの種類
	static MODE GetMode(void) { return m_mode; }					// デバイスの取得(モード)

	// ポーズ
	static void SetPause(void);										// ポーズの取得
	static void SetPause(bool pause) { m_bPause = pause; }			// ポーズの取得
	static bool GetPause(void) { return m_bPause; }					// ポーズの情報

private:
	// モード
	static CTitle *m_pTitle;										// タイトル
	static CGame *m_pGame;											// ゲーム
	static CResult *m_pResult;										// リザルト
	static MODE m_mode;												// モード

	static CRenderer *m_pRenderer;									// レンダリングのポインタ
	static CInputKeyboard *m_pInputKeyboard;						// キーボードのポインタ
	static CSound *m_pSound;										// サウンドのポインタ
	static CFade *m_pFade;											// フェードのポインタ

	static bool m_bPause;											// ポーズ中かどうか
};

#endif