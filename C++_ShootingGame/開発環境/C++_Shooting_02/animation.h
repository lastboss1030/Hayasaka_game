//=============================================================================
//
// アニメーション処理 [animation.h]
// Author : taiki hayasaka
//
//=============================================================================
#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include "main.h"
#include "scene2D.h"
#include "player.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define MAX_ANIMATION (18)

//=============================================================================
// 爆破クラスの定義
//=============================================================================

//継承先描画クラス
class CAnimation : public CScene2D
{
public:
	CAnimation(PRIORITY nPriority = PRIORITY_ANIMATION);		// コンストラクタ
	virtual ~CAnimation();										// デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nAnime, int nType);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CAnimation *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nAnime, int nType);
	static HRESULT Load(void);
	static void Unload(void);

	static void UninitType(int nUninitType);
	static void UninitTypeClear(void);

protected:
	static LPDIRECT3DTEXTURE9 m_pTexture[MAX_ANIMATION];	// テクスチャのポインタ
	static int m_nUninitType;								// 途中破棄に使用
	D3DXVECTOR3 m_pos;										// 移動量
	static CPlayer *m_pPlayer;								// プレイヤーへのポインタ

	int m_nType;											// タイプ　テクスチャ識別
	int m_nTypeBuulet;										// 特殊移動時に使用
	int m_nCounterAnime;									// アニメーションカウンター
	int m_nPatternAnime;									// アニメーションパターン
	int m_fCriss;											// ループ判定

	float m_fAnimeX;										// 画像分割X
	float m_fAnimeY;										// 画像分割Y
	int m_nX;												// 画像分割
	int m_nY;												// 画像分割

	int m_nAnime;											// アニメーションスピード
	float m_fColorFa = 1.0f;								// アルファ値
};

#endif