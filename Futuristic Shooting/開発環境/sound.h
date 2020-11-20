//=============================================================================
//
// サウンド処理 [sound.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// サウンドファイル
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_BGM000 = 0,		// BGM0
	SOUND_LABEL_BGM001,			// BGM1
	SOUND_LABEL_BGM002,			// BGM2
	SOUND_LABEL_BGM003,			// BGM3
	SOUND_LABEL_BGM_TITLE,		// BGMタイトル
	SOUND_LABEL_BGM_END,		// BGMリザルト
	SOUND_LABEL_BGM_BOSS,		// BGMボス
	SOUND_LABEL_TUTORIAL,		// チュートリアル
	SOUND_LABEL_SE_SHOT,		// 弾発射音
	SOUND_LABEL_SE_LASER,		// レーザー音
	SOUND_LABEL_SE_LASER2,		// レーザー音
	SOUND_LABEL_SE_HIT,			// ヒット音
	SOUND_LABEL_SE_EXPLOSION,	// 爆発音
	SOUND_LABEL_SE_EXPLOSION2,	// 爆発音2
	SOUND_LABEL_SE_EXPLOSION3,	// 爆発音ボス
	SOUND_LABEL_SE_PAUSE,		// ポーズ
	SOUND_LABEL_SE_START,		// 開始
	SOUND_LABEL_SE_DECISION,	// 決定
	SOUND_LABEL_SE_SELECT,		// セレクト
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
