//=============================================================================
//
//	ポリゴンの処理 [polygon.h]
//	Author: Taiki hayasaka
//
//=============================================================================
#ifndef _POLYGON_H_
#define _POLYGON_H_

#include "main.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define LIFE_HEIGHT			(60.0f)			// 高さ
#define LIFE_WIDTH			(60.0f)			// 幅

//=============================================================================
// ポリゴンクラス
//=============================================================================
class CPolygon
{
public:
	CPolygon();
	~CPolygon();

	// オブジェクトの生成
	static CPolygon *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 設定
	void SetPolygon(int nNumber);

	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);

private:
	LPDIRECT3DTEXTURE9 m_pTexture;				// テクスチャのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;			// 頂点バッファへのポインタ
};

#endif
