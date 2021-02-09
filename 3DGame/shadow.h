//=============================================================================
//
// 影の処理 [shadow.h]
// Author:Taiki Hayasaka
//
//=============================================================================
#ifndef _SHADOW_H_
#define _SHADOW_H_

#include "main.h"

//=============================================================================
// 構造体の定義
//=============================================================================
typedef struct
{
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 rot;		//向き
	float fWidth;			//幅
	float fDepth;			//奥行
	bool bUse;				//使用しているかどうか
}Shadow;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
HRESULT InitShadow(void);
void UninitShadow(void);
void UpdateShadow(void);
void DrawShadow(void);

int SetShadow(D3DXVECTOR3 pos, float fWidth, float fDepth);
void SetPositionShadow(int nIdx, D3DXVECTOR3 pos);
void DeleteShadow(int nIdx);

#endif
