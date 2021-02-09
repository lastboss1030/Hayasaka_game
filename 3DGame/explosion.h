//=============================================================================
//
// 爆発の処理 [explosion.h]
// Author:Taiki Hayasaka
//
//=============================================================================
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

#include "main.h"

//=============================================================================
// 構造体の定義
//=============================================================================
typedef struct
{
	D3DXMATRIX mtxWorld;	// ワールドマトリックス
	D3DXVECTOR3 pos;		// 位置
	int nCounterAnimE;		// アニメーションカウンター
	int nPatternAnimE;		// テクスチャパターンのカウント
	bool bUse;				// 使用しているかどうか
} Explosion;

//-------------------------------------------------------------------
// プロトタイプ宣言
//-------------------------------------------------------------------
HRESULT InitExplosion(void);			// 爆発の初期化処理
void UninitExplosion(void);				// 爆発の終了処理
void UpdateExplosion(void);				// 爆発の更新処理
void DrawExplosion(void);				// 爆発の描画処理	

void SetExplosion(D3DXVECTOR3 pos);		// 爆発の設定

#endif
