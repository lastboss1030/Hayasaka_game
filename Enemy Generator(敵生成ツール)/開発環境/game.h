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
class CBg;					// 背景
class CEnemy;				// 敵
class CNumber;				// ナンバー
class CScore;				// スコア
class CPolygon;				// ポリゴン
class CLife;				// ライフ
class CParts;				// ボスパーツ

//=============================================================================
// クラス
//=============================================================================
class CGame : public CScene
{
public:
	// リザルトの種類
	typedef enum
	{
		RESULTMODE_NONE = 0,
		RESULTMODE_GAMECLEAR,		// クリア
		RESULTMODE_GAMEOVER,		// ゲームオーバー
		RESULTMODE_MAX,
	}RESULTMODE;

	CGame();
	~CGame();

	static CGame *Create(void);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static void LoadAll(void);										// 全てのテクスチャを読み込む
	static void UnloadAll(void);									// 全てのテクスチャを破棄

	void BossAll(void);												// ボス管理

	//デバイスの取得
	static CPlayer *GetPlayer(void) { return m_pPlayer; }			// プレイヤーの取得
	static CBg *GetBg(void) { return m_pBg; }						// 背景の取得
	static CEnemy *GetEnemy(void) { return m_pEnemy; }				// 敵の取得
	static CNumber *GetNumber(void) { return m_pNumber; }			// ナンバーの取得
	static CScore *GetScore(void) { return m_pScore; }				// スコアの取得
	static CPolygon *GetPolygon(void) { return m_pPolygon; }		// ポリゴンの取得
	static CParts *GetParts(void) { return m_pParts; }				// パーツ

	static RESULTMODE GetResult(void) { return m_resultmode; }		// リザルトの取得
	static void SetResult(RESULTMODE resultmode);					// リザルト受け取り

private:
	static CPlayer *m_pPlayer;										// プレイヤーのポインタ
	static CBg *m_pBg;												// 背景のポインタ
	static CEnemy *m_pEnemy;										// 敵のポインタ
	static CNumber *m_pNumber;										// ナンバーのポインタ
	static CScore *m_pScore;										// スコアのポインタ
	static CPolygon *m_pPolygon;									// ポリゴンのポインタ
	static CParts *m_pParts;										// パーツ

	static RESULTMODE m_resultmode;									// リザルトモード
};

#endif