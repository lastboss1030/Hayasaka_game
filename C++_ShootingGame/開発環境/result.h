//=============================================================================
//
// リザルト処理 [result.cpp]
// Author : taiki hayasaka
//
//=============================================================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "main.h"
#include "scene2D.h"

//=============================================================================
// クラス
//=============================================================================
class CResult : public CScene
{
public:
	//テクスチャの種類
	typedef enum
	{
		TEXTURETYPE_GAMECLEAR = 0,		// ゲームクリア
		TEXTURETYPE_GAMEOVER,			// ゲームオーバー
		TEXTURETYPE_MAX,
	}TEXTURETYPE;

	CResult();
	~CResult();

	static CResult *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);

	// テクスチャ関係の関数
	static HRESULT Load(void);
	static void Unload(void);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	static LPDIRECT3DTEXTURE9 m_apTexture[TEXTURETYPE_MAX];			// テクスチャのポインタ
	CScene2D *m_pScene2D;											// シーン2Dのポインタ
};

#endif
