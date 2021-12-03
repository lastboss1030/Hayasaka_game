//=============================================================================
//
// 敵処理 [enemy.h]
// Author : Taiki Hayasaka
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "scene2D.h"
#include "main.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define ENEMY_LIFE (10)		// 敵の体力

//=============================================================================
// 敵クラス(派生クラス)
//=============================================================================
class CEnemy : public CScene2D
{
public:
	// 敵の種類
	typedef enum
	{
		ENEMYTYPE_NONE = 0,
		ENEMYTYPE_STOP,		// 停止タイプ
		ENEMYTYPE_MOVE,		// 移動１
		ENEMYTYPE_MOVE2,	// 移動２
		ENEMYTYPE_MAX,
	}ENEMYTYPE;

	// 敵の状態
	typedef enum
	{
		ENEMY_NONE = 0,
		ENEMY_NORMAL,		// 通常
		ENEMY_DAMAGE,		// ダメージ
		ENEMY_MAX,
	}ENEMYSTATE;

	CEnemy(PRIORITY nPriority = PRIORITY_ENEMY);
	~CEnemy();

	// テクスチャ関係の関数
	static HRESULT Load(void);
	static void Unload(void);

	// 生成
	static CEnemy *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move, D3DXVECTOR3 speed, ENEMYTYPE type, int nLife);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move, D3DXVECTOR3 speed, ENEMYTYPE type, int nLife);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void MoveEnemy(void);
	bool HitEnemy(int nDamage);

	// 敵の状態
	ENEMYSTATE GetState(void) { return m_State; }
	void SetState(ENEMYSTATE ENEMY_MAX) { m_State = ENEMY_MAX; }
	int GetEnemyCnt(void) { return m_enmeyCnt; }

	ENEMYTYPE GetEnemyType(void) { return m_Enemytype; };
	void SetEnemyType(ENEMYTYPE EnemyType) { m_Enemytype = EnemyType; };

private:
	static LPDIRECT3DTEXTURE9 m_apTexture;				// テクスチャのポインタ
	D3DXVECTOR3 m_size;									// サイズ
	D3DXVECTOR3 m_move;									// 移動量
	D3DXCOLOR m_Color;									// カラー
	ENEMYTYPE m_Enemytype;								// 敵の種類
	int m_nLife;										// ライフ

	ENEMYSTATE m_State;									// 状態
	int m_nHitCnt;										// 当たってからのカウント
	static int m_enmeyCnt;								// 敵数カウント

	int m_nShotCnt;										// 弾発射カウント
	int m_nCounter;										// カウンター
	int m_nCntColor;									// 色の切り替えカウンター
};
#endif