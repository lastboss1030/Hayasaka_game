//=============================================================================
//
// ブーストアイテムの処理 [item_boost.h]
// Author:Taiki Hayasaka
//
//=============================================================================
#ifndef _ITEM_BOOST_H_
#define _ITEM_BOOST_H_

#include "main.h"

//=============================================================================
// 構造体の定義
//=============================================================================
typedef struct
{
	LPD3DXMESH pMeshItemBoost;			//メッシュ(頂点情報)へのポインタ
	LPD3DXBUFFER pBuffMatItemBoost;		//マテリアル(材質情報)へのポインタ
	DWORD nNumMatItemBoost = 0;			//マテリアルの数
	D3DXMATRIX mtxWorld;				//ワールドマトリックス
	D3DXVECTOR3 pos;					//中心座標
	D3DXVECTOR3 rot;					//回転角
	D3DXVECTOR3 rotDest;				//目的の回転角
	D3DXVECTOR3 move;					//移動量
	D3DXVECTOR3 moverot;				//回転量
	D3DXVECTOR3 vtxMinItemBoost;		//座標の最小値
	D3DXVECTOR3 vtxMaxItemBoost;		//座標の最大値
	int nIdx;							//何番目の影か
	bool bUse;							//使用しているかどうか
}ItemBoost;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
HRESULT InitItemBoost(void);
void UninitItemBoost(void);	
void UpdateItemBoost(void);	
void DrawItemBoost(void);	

void SetItemBoost(D3DXVECTOR3 pos);		//アイテムの設置
void VecItemBoost(int nCntItemBoost);	//アイテムの頂点座標

#endif
