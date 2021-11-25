//=============================================================================
//
//	キーボード処理 [Input_keyboard.h]
//	Author: Taiki Hayasaka
//
//=============================================================================
#ifndef _INPUT_KEYBOARD_H_
#define _INPUT_KEYBOARD_H_

#include "main.h"
#include "Input.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_KEY		(256)

//=============================================================================
// キーボードクラス(派生クラス)
//=============================================================================
class CInputKeyboard : public CInput
{
public:
	CInputKeyboard();									// コンストラク
	~CInputKeyboard();									// デストラクタ

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);		// 初期化処理
	void Uninit(void);									// 終了処理
	void Update(void);									// 更新処理
	bool GetPress(int nKey);							// プレス(押し続ける)
	bool GetTrigger(int nKey);							// トリガー(押した瞬間)
	bool GetRelease(int nKey);							// リリース

private:
	BYTE m_aKeyState[MAX_KEY];							// 過去の入力情報
	BYTE m_akeyStateTrigger[MAX_KEY];					// キートリガー情報
	BYTE m_akeyStateRelease[MAX_KEY];					// キーリリース情報
};

#endif
