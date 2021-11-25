//=============================================================================
//
// タイトル処理 [title.h]
// Author : Taiki Hayasaka
//
//=============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

#include "main.h"
#include "scene2D.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_TITLE (2)	// タイトル画面テクスチャ数

//=============================================================================
// クラス
//=============================================================================
class CTitle : public CScene
{
public:
	CTitle();
	~CTitle();

	// 生成
	static CTitle *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);

	// テクスチャ関係
	static HRESULT Load(void);
	static void Unload(void);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;
	CScene2D *m_pScene2D;
};

#endif
