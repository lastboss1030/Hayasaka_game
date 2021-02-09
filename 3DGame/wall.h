//=============================================================================
//
// 壁の処理 [wall.h]
// Author:Taiki Hayasaka
//
//=============================================================================
#ifndef _WALL_H_
#define _WALL_H_

#include "main.h"

//=============================================================================
// 壁の構造体
//=============================================================================
typedef struct
{
	D3DXMATRIX mtxWorld;	// ワールドマトリックス
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 rot;		//向き
	float fWidth;			//幅
	float fHeight;			//高さ
	bool bUse;				//使用しているかどうか
}Wall;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
HRESULT InitWall(void);
void UninitWall(void);
void UpdateWall(void);
void DrawWall(void);
void SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight);
Wall *GetWall(void);

#endif

