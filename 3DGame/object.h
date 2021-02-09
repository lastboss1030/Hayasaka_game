//=============================================================================
//
// オブジェクトの処理 [object.h]
// Author:Taiki Hayasaka
//
//=============================================================================
#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "main.h"

//=============================================================================
// モデルの構造体
//=============================================================================
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXMATRIX mtxWorld;
	D3DXVECTOR3 move;
	D3DXVECTOR3 rotDest;
	D3DXVECTOR3 vtxMinObject, vtxMaxObject;	// モデルの各座標の最大値・最小値
	int nIdx;
} Object;

//==============================================================================
// プロトタイプ宣言
//==============================================================================
HRESULT InitObject(void);	
void UninitObject(void);	
void UpdateObject(void);	
void DrawObject(void);		

Object *GetObject(void);

#endif
