//=============================================================================
//
// スコア処理[sccore.h]
// Author : taiki hayasaka
//
//=============================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

#include "scene.h"
#include "number.h"
#include "main.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_NUMBER  (8)										// 最大数
#define NUMBER_HALF	(MAX_NUMBER/2)							// 最大数半分の桁
#define SCORE_MATH	((nScore % nMath / (nMath / 10)))		// スコアの計算

//=============================================================================
// クラス
//=============================================================================
class CScore : public CScene
{
public:
	CScore(PRIORITY nPriority = PRIORITY_SCORE);
	~CScore();

	static CScore *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static void SetScore(int nScore);
	static void AddScore(int nValue);
	int GetScore(void);

private:
	static CNumber *m_apNumber[MAX_NUMBER];
	static int m_nScore;
};

#endif