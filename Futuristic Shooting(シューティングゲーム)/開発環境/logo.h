//=============================================================================
//
// ロゴ処理 [logo.h]
// Author : taiki hayasaka
//
//=============================================================================
#ifndef _LOGO_H_
#define _LOGO_H_

#include "main.h"
#include "scene.h"
#include "scene2D.h"
#include "player.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_LOGO (33)	// テクスチャが貼ってあるポリゴンの描画

//=============================================================================
// ロゴクラスの定義
//=============================================================================
class CLogo : public CScene2D
{
public:
	CLogo(PRIORITY nPriority = PRIORITY_LOGO);			// コンストラクタ
	~CLogo();											// デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nType);				// 初期化処理
	void Uninit(void);														// 終了処理
	void Update(void);														// 更新処理
	void Draw(void);														// 描画処理
																					   
	static CLogo *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nType);		// 生成処理
	static HRESULT Load(void);												// テクスチャ読み込み
	static void Unload(void);												// テクスチャ破棄
																					   
																			// 特殊関数
	static void TypeUninit(int nType);										// 途中破棄
	static void TypeMove(D3DXVECTOR3 pos, int nTypeMove);					// 途中移動
	static void TypeChangeCollar(D3DXCOLOR col, int nTypeCollar);			// 途中色変化

protected:
	static LPDIRECT3DTEXTURE9 m_apTextureLogo[MAX_LOGO];					// 用意するテクスチャの数
	static CPlayer				*m_pPlayer;									// プレイヤーへのポインタ	ホーミング時に使用

	CScene2D *m_apScene2D[3];												// シーン2Dへのポインタ
	D3DXVECTOR3		m_pos;													// 位置情報
	float			m_nCntAnime;											// テクスチャをずらす
	int				m_nType;												// タイプでテクスチャを管理している
};

//=============================================================================
// ロゴクラスの定義 (Pause)
//=============================================================================
class CLogoPause : public CLogo
{
public:
	CLogoPause(PRIORITY nPriority = PRIORITY_PAUSE);		// コンストラクタ
	~CLogoPause();											// デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nType);							// 初期化処理
	void Uninit(void);																	// 終了処理
	void Update(void);																	// 更新処理
	void Draw(void);																	// 描画処理
																						   
	static CLogoPause *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nType);			// 生成処理
	static void Unload(void);															// テクスチャ読み込み
	static void TypeUninit(int nType);													// 途中破棄
	static void TypeMove(D3DXVECTOR3 pos, int nTypeMove);								// 途中移動
	static void TypeChangeCollar(D3DXCOLOR col, int nTypeCollar);						// 途中色変化
};

#endif
