//=============================================================================
//
// ライフ処理[life.h]
// Author : taiki hayasaka
//
//=============================================================================
#ifndef _LIFE_H_
#define _LIFE_H_

#include "main.h"			// メイン
#include "scene2D.h"		// シーン2D
#include "polygon.h"		// ポリゴン

//=============================================================================
// マクロ定義
//=============================================================================
#define START_LIFE		(2)										// 初期のライフ
#define MAX_LIFE		(5)										// 最大桁数

#define LIFE_HALF		(MAX_LIFE/2)							// 最大数半分の桁
#define SCORE_MATH		((nScore % nMath / (nMath / 10)))		// スコアの計算

//=============================================================================
// クラス
//=============================================================================
class CLife :public CScene
{
public:
	CLife(PRIORITY nPriority = PRIORITY_LIFE);
	~CLife();

	// テクスチャ関係の関数
	static HRESULT Load(void);
	static void Unload(void);

	static CLife *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetLife(int nLife);
	void AddLife(int nValue);
	void SubtractLife(int nValue);
	int GetLife(void);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;		// テクスチャのポインタ
	CPolygon *m_apPolygon[MAX_LIFE];			// ライフ数
	int m_nLife;								// ライフ
	D3DXVECTOR3 m_pos;							// 位置
};

#endif

