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
	SOUND_LABEL_BGM_TITLE = 0,		//タイトル
	SOUND_LABEL_BGM_TUTORIAL,		//チュートリアル
	SOUND_LABEL_BGM_SELECT,			//セレクト
	SOUND_LABEL_BGM_GAME,			//ゲーム
	SOUND_LABEL_BGM_RESULT,			//リザルト
	SOUND_LABEL_BGM_RANKING,		//ランキング
	SOUND_LABEL_SE_DEATH,			//死亡
	SOUND_LABEL_SE_ENTER,			//決定
	SOUND_LABEL_SE_SELECT,			//選択
	SOUND_LABEL_SE_SHOOT_GOLEM,		//ショットゴーレム
	SOUND_LABEL_SE_SHOOT_LEO,		//ショットレオ
	SOUND_LABEL_SE_SHOOT_STALKER,	//ショットストーカー
	SOUND_LABEL_SE_HIT,				//ヒット
	SOUND_LABEL_SE_ITEM,			//アイテム
	SOUND_LABEL_SE_PAUSE,			//ポーズ
	SOUND_LABEL_SE_TIMEUP,			//時間切れ
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

