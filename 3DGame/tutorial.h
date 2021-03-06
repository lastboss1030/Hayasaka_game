//=============================================================================
//
// チュートリアルの処理 [tutorial.h]
// Author:Taiki Hayasaka
//
//=============================================================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

#include "main.h"

//=============================================================================
// プロトタイプ宣言
//=============================================================================
HRESULT InitTutorial(void);
void UninitTutorial(void);
void UpdateTutorial(void);
void DrawTutorial(void);
void SetTextureTutorial(int nCntTutorial);

#endif
