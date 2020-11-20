//=============================================================================
//
// プレイヤーの処理 [player.h]
// Author:Taiki Hayasaka
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"
#include "input.h"
#include "bullet.h"

//=============================================================================
// プレイヤーの状態
//=============================================================================
typedef enum
{
	PLAYERSTATE_APPEAR = 0,	//出現
	PLAYERSTATE_NORMAL,		//通常
	PLAYERSTATE_DAMAGE,		//ダメージ
	PLAYERSTATE_WAIT,		//出現待ち
	PLAYERSTATE_DEATH,		//死亡
	PLAYERSTATE_MAX
}PLAYERSTATE;

//=============================================================================
// プレイヤー構造体
//=============================================================================
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	PLAYERSTATE state;		//状態
	int nLife;				//体力
	int nCounterState;		//カウンター
	int nCounterDisp;		//点滅カウンター
	int nRemaining;			//残機
	bool bDisp;				//点滅
}	PLAYER;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
PLAYER *GetPlayer(void);
bool HitPlayer(int nDamage);

#endif
