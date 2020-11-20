//=============================================================================
//
// 敵の処理 [enemy.h]
// Author:Taiki Hayasaka
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_ENEMY (256)	//敵の最大数

//=============================================================================
// 敵の状態
//=============================================================================
typedef enum
{
	ENEMYSTATE_NORMAL = 0,	//通常状態
	ENEMYSTATE_DAMAGE,		//ダメージ状態
	ENEMYSTATE_MAX			
}ENEMYSTATE;

//=============================================================================
// 敵の構造体
//=============================================================================
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 move;		//移動量
	int nType;				//種類
	bool bUse;				//使用しているかどうか
	ENEMYSTATE state;		//状態
	int nCounterState;		//状態カウンター
	int nLife;				//体力
}ENEMY;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
HRESULT InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
void SetEnemy(D3DXVECTOR3 pos, int nType, D3DXVECTOR3 move);
ENEMY *GetEnemy(void);
void SetVertexEnemy(int nIdx);
bool HitEnemy(int Idx, int nDamage);

#endif