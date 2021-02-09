//=============================================================================
//
// リザルトの処理 [result.h]
// Author:Taiki Hayasaka
//
//=============================================================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "main.h"

//=============================================================================
// プロトタイプ宣言
//=============================================================================
HRESULT InitResult(void);
void UninitResult(void);
void UpdateResult(void);
void DrawResult(void);
void SetTextureResult(int nCntResult);

#endif
