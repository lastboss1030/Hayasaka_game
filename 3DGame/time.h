//=============================================================================
//
// タイムの処理 [time.h]
// Author:Taiki Hayasaka
//
//=============================================================================
#ifndef _TIME_H_
#define _TIME_H_

#include "main.h"

//-------------------------------------------------------------------
// プロトタイプ宣言
//-------------------------------------------------------------------
HRESULT InitTime(void);			//タイマーの初期化処理
void UninitTime(void);			//タイマーの終了処理
void UpdateTime(void);			//タイマーの更新処理
void DrawTime(void);			//タイマーの描画処理
int GetTime(void);				//タイマー情報の取得

#endif


