//=============================================================================
//
// 数字処理[number.h]
// Author : Taiki Hayasaka
//
//=============================================================================
#ifndef _NUMBER_H_
#define _NUMBER_H_

#include "main.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define NUMBER_HEIGHT		(70.0f)		// 高さ
#define NUMBER_WIDTH		(35.0f)		// 幅

//=============================================================================
// クラス
//=============================================================================
class CNumber
{
public:
	CNumber();
	~CNumber();

	// テクスチャ関係の関数
	static HRESULT Load(void);
	static void Unload(void);

	static CNumber *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetNumber(int nNumber);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
};

#endif