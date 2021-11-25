//=============================================================================
//
// エフェクト処理 [effect.h]
// Author : Taiki Hayasaka
//
//=============================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

//=============================================================================
// ヘッダファイルのインクルード
//=============================================================================
#include "scene2D.h"

//=============================================================================
// クラス定義
//=============================================================================
class CEffect :public CScene2D
{
public:
	CEffect(PRIORITY nPriority = PRIORITY_EFFECT);
	~CEffect();

	static HRESULT Load(void);
	static void Unload(void);
	static CEffect *Create(D3DXVECTOR3 pos, int nLife, D3DXVECTOR3 size, D3DXCOLOR col);

	HRESULT Init(D3DXVECTOR3 pos, int nLife, D3DXVECTOR3 size, D3DXCOLOR col);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	int m_nLife;
	D3DXVECTOR3 m_size;
	D3DXVECTOR3 m_move;			// 移動量
	D3DXCOLOR m_col;
	static LPDIRECT3DTEXTURE9 m_pTexture;
};

#endif