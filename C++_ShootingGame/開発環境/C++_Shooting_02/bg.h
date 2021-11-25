//=============================================================================
//
// 背景処理 [bg.h]
// Author : Taiki Hayasaka
//
//=============================================================================
#ifndef _BG_H_
#define _BG_H_

#include "scene.h"
#include "main.h"

//=============================================================================
// 前方宣言
//=============================================================================
class CScene2D;

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_BG (3)				// 背景の最大数
#define GIRL_X (1280)			// 画像の幅
#define GIRL_Y (720)			// 画像の高さ

#define SCROL_SPPED01 (0.001f)	// スクロールのスピード1
#define SCROL_SPPED02 (0.005f)	// スクロールのスピード2
#define SCROL_SPPED03 (0.010f)	// スクロールのスピード3

//=============================================================================
// クラス
//=============================================================================
class CBg : public CScene
{
public:

	CBg(PRIORITY nPriority = PRIORITY_BGBEHING);
	~CBg();

	static HRESULT Load(void);
	static void Unload(void);
	static CBg *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_BG];	// テクスチャのポインタ
	CScene2D *m_apScene2D[MAX_BG];					// 2Dポリゴンのポインタ
	float m_fSpeed[MAX_BG];							// スクロールのスピード
	float m_fPosTexU[MAX_BG];						// テクスチャのU座標
	float m_fPosTexV[MAX_BG];						// テクスチャのV座標
};

#endif