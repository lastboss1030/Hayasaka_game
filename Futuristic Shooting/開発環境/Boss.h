//=============================================================================
//
// ボスの処理 [Boss.h]
// Author:Taiki Hayasaka
//
//=============================================================================
#ifndef _BOSS_H_
#define _BOSS_H_

#include "main.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_BOSS (256)	//敵の最大数

//=============================================================================
// ボスの状態
//=============================================================================
typedef enum
{
	BOSSSTATE_NORMAL = 0,	//通常状態
	BOSSSTATE_DAMAGE,		//ダメージ状態
	BOSSSTATE_MAX
}BOSSSTATE;

//=============================================================================
// ボスの構造体
//=============================================================================
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 move;		//移動量
	bool bUse;				//使用しているかどうか
	BOSSSTATE state;		//状態
	int nCounterState;		//状態カウンター
	int nLife;				//体力
}BOSS;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
HRESULT InitBoss(void);
void UninitBoss(void);
void UpdateBoss(void);
void DrawBoss(void);
void SetBoss(D3DXVECTOR3 pos, D3DXVECTOR3 move);
BOSS *GetBoss(void);
void SetVertexBoss(int nIdx);
bool HitBoss(int nDamage);

#endif
