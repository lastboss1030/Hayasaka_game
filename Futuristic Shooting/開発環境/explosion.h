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
// プロトタイプ宣言
//=============================================================================
HRESULT InitExplosion(void);
void UninitExplosion(void);
void UpdateExplosion(void);
void DrawExplosion(void);
void SetExplosion(D3DXVECTOR3 pos, D3DXCOLOR col);

//=============================================================================
// 爆発の構造体
//=============================================================================
typedef struct
{
	D3DXVECTOR3 pos;	//爆発の位置
	D3DXCOLOR col;		//爆発の色
	int nCounterAnim;	//アニメーションカウンター
	int nPattenAnim;	//アニメーションパターン
	bool bUse;			//使用しているかどうか
}EXPLOSION;

#endif