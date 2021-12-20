//=============================================================================
//
// 爆発処理 [explosion.h]
// Author : Taiki Hayasaka
//
//=============================================================================
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

#include "scene2D.h"
#include "main.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_ANIMATION_COUNTER	(3)			// 爆破のスピード
#define MAX_ANIMATION_PATTERN	(8)			// 爆発のページ数
#define MAX_EXPLOSION_SIZE_X	(100)		// 爆発の幅
#define MAX_EXPLOSION_SIZE_Y	(100)		// 爆発の高さ

//=============================================================================
// クラス
//=============================================================================
class CExplosion : public CScene2D
{
public:
	CExplosion(PRIORITY nPriority = PRIORITY_EXPLOSION);
	~CExplosion();

	// テクスチャ関係の関数
	static HRESULT Load(void);
	static void Unload(void);
	
	// 生成
	static CExplosion *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	D3DXVECTOR3 m_size;						// 大きさ
	D3DXVECTOR3 m_move;						// 移動量
	D3DXCOLOR m_colorExplosion;				// 爆発色
	static LPDIRECT3DTEXTURE9 m_pTexture;	// テクスチャ

	int m_nCountAnim;						// カウンター
	int m_nPatternAnim;						// パターンNo
};

#endif