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
// 構造体の定義
//=============================================================================
typedef struct
{
	LPD3DXMESH pMeshEnemy;			//メッシュ(頂点情報)へのポインタ
	LPD3DXBUFFER pBuffMatEnemy;		//マテリアル(材質情報)へのポインタ
	DWORD nNumMatEnemy = 0;			//マテリアルの数
	D3DXMATRIX mtxWorld;			//ワールドマトリックス
	D3DXVECTOR3 pos;				//中心座標
	D3DXVECTOR3 rot;				//回転角
	D3DXVECTOR3 rotDest;			//目的の回転角
	D3DXVECTOR3 move;				//移動量
	D3DXVECTOR3 moverot;			//回転量
	D3DXVECTOR3 vtxMinEnemy;		//座標の最小値
	D3DXVECTOR3 vtxMaxEnemy;		//座標の最大値

	int nLife;						//体力
	int nIdx;						//何番目の影か
	bool bUse;						//使用しているかどうか
}Enemy;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
HRESULT InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);

void MoveEnemy(int nCntEnemy);		//敵の移動
void SetEnemy(D3DXVECTOR3 pos);		//アイテムの設置
void VecEnemy(int nCntEnemy);	//アイテムの頂点座標
Enemy *GetEnemy(void);

bool HitEnemy(int nDamage);

#endif