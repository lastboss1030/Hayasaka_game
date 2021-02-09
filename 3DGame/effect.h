//=============================================================================
//
// エフェクトの処理 [effect.cpp]
// Author:Taiki Hayasaka
//
//=============================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "main.h"

//=============================================================================
// 構造体の定義
//=============================================================================
typedef struct
{
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 move;		//移動量
	D3DXCOLOR col;			//色
	float fRadius;			//半径
	float fRemove;			//移動量の減少量
	float nCntAlpha;		//α値の移動
	float fGravity;			// 重力
	int nLife;				//寿命
	bool bUse;				//使用しているかどうか
} EFFECT;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
HRESULT InitEffect(void);	// エフェクトの初期化処理
void UninitEffect(void);	// エフェクトの終了処理
void UpdateEffect(void);	// エフェクトの更新処理
void DrawEffect(void);		// エフェクトの描画処
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col,
	float fRemove, float fRadius, float fCntAlpha, float fGravity);	// エフェクトの設定

#endif
