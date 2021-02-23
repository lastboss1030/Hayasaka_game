//=============================================================================
//
// ポリゴン処理 [meshfield.h]
// Author:Taiki Hayasaka
//
//=============================================================================
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

#include "main.h"

//=============================================================================
// 地面の構造体
//=============================================================================
typedef struct
{
	D3DXMATRIX mtxWorld;	// ワールドマトリックス
	D3DXVECTOR3 pos;		// 初期位置
	D3DXVECTOR3	posMove;	// 初期位置からの移動した位置
	D3DXVECTOR3 rot;		// 向き
	float fWidth;			// 幅
	float fHeight;			// 高さ
	bool bUse;				// 使用状態
} Meshfield;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
HRESULT InitMeshfield(void);
void UninitMeshfield(void);
void UpdateMeshfield(void);
void DrawMeshfield(void);

void SetMeshfield(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight);			// メッシュ床の描画処理


#endif

