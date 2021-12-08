//=============================================================================
//
// ゲーム処理 [game.h]
// Author : taiki hayasaka
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"
#include "scene.h"

//=============================================================================
// 前方宣言
//=============================================================================
class CPlayer;				// プレイヤー
class CPolygon;				// ポリゴン

//=============================================================================
// クラス
//=============================================================================
class CGame : public CScene
{
public:
	CGame();
	~CGame();

	static CGame *Create(void);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static void LoadAll(void);										// 全てのテクスチャを読み込む
	static void UnloadAll(void);									// 全てのテクスチャを破棄

	//デバイスの取得
	static CPlayer *GetPlayer(void) { return m_pPlayer; }			// プレイヤーの取得
	static CPolygon *GetPolygon(void) { return m_pPolygon; }		// ポリゴンの取得

private:
	static CPlayer *m_pPlayer;										// プレイヤーのポインタ
	static CPolygon *m_pPolygon;									// ポリゴンのポインタ
};

#endif