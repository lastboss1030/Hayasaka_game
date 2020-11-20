//=============================================================================
//
// 弾の処理 [bullet.h]
// Author:Taiki Hayasaka
//
//=============================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

#include "main.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_BALLET (256)	//弾の最大数

//=============================================================================
// 弾の種類
//=============================================================================
typedef enum
{
	BULLETTYPE_PLAYER = 0,	//プレイヤーの弾
	BULLETTYPE_ENEMY,		//敵の弾
	BULLETTYPE_MAX
}BULLETTYPE;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
HRESULT InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void SetBullet(D3DXVECTOR3 pos,		//出現位置
				D3DXVECTOR3 move,	//移動量
				int nLife,			//弾の寿命
				BULLETTYPE type);	//弾の種類
	
void SetVertexBullet(int nIdx);

//=============================================================================
// 弾の構造体
//=============================================================================
typedef struct
{
	D3DXVECTOR3 pos;	//出現位置
	D3DXVECTOR3 move;	//移動量
	int nLife;			//弾の寿命
	bool bUse;			//使用しているかどうか
	BULLETTYPE type;	//種類
}BULLET;

#endif