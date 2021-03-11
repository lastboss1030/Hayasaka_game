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
// 弾の種類
//=============================================================================
typedef enum
{
	BULLETTYPE_PLAYER = 0,	//プレイヤーの弾
	BULLETTYPE_ENEMY,		//敵の弾
	BULLETTYPE_EFFECT,
	BULLETTYPE_MAX
}BULLETTYPE;

//=============================================================================
// 弾の構造体
//=============================================================================
typedef struct
{
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	D3DXVECTOR3 pos;		//出現位置
	D3DXVECTOR3 move;		//移動量
	BULLETTYPE type;		//種類
	float fWidth;			//幅
	float fHeight;			//高さ
	bool bUse;				//使用しているかどうか
}BULLET;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
HRESULT InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void SetBullet(D3DXVECTOR3 pos,		//出現位置
				D3DXVECTOR3 move,	//移動量
				float fWidth,		//幅
				float fHeight,		//高さ
				BULLETTYPE type);	//種類
void FollowBullet(void);

BULLET *GetBullet(void);

#endif
