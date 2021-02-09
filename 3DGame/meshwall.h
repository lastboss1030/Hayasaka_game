//=============================================================================
//
// 壁の処理 [meshwall.cpp]
// Author:Taiki Hayasaka
//
//=============================================================================
#pragma once
#ifndef _MESHWALL_H_
#define _MESHWALL_H_
#include "main.h"

//=============================================================================
// 壁の構造体
//=============================================================================
typedef struct
{
	D3DXMATRIX mtxWorld;	// ワールドマトリックス
	D3DXVECTOR3 pos;		// 初期位置
	D3DXVECTOR3	posMove;	// 初期位置からの移動した位置
	D3DXVECTOR3 rot;		// 向き
	float fWidth;			// 幅
	float fHeight;			// 高さ
	int nCntX;				// 一辺当たりの頂点数(横)
	int nCntY;				// 一辺当たりの頂点数(縦)
	bool bUse;				// 使用状態
} Meshwall;

//==============================================================================
// プロトタイプ宣言
//==============================================================================
HRESULT InitMeshwall(void);		// メッシュ壁の初期化処理
void UninitMeshwall(void);		// メッシュ壁の終了処理
void UpdateMeshwall(void);		// メッシュ壁の更新処理
void DrawMeshwall(void);		// メッシュ壁の描画処理
void SetMeshwall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight);			// メッシュ壁の描画処理

#endif

