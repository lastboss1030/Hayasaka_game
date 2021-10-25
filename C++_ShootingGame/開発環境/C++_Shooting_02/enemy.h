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
#define MAX_ENEMY (3)	// 敵最大数

#define ENEMY_X			(382.0f*0.3f)				// 敵の幅
#define ENEMY_Y			(369.0f*0.3f)				// 敵の高さ
#define ENEMY_WIDTH		(120.0f)
#define ENEMY_HEIGHT	(120.0f)

// Create(仮)
#define ENEMY_POS		(D3DXVECTOR3(1400.0f, (float)SCREEN_HEIGHT*0.5f, 0.0f))		// 位置
#define ENEMY_POS1		(D3DXVECTOR3(1600.0f, 500.0f, 0.0f))						// 位置

#define ENEMY_SIZE		(D3DXVECTOR3(120, 120, 0.0f))								// 大きさ
#define ENEMY_MOVE		(D3DXVECTOR3(5.0f,0.0f,0.0f))								// 移動量
#define ENEMY_SPEED		(D3DXVECTOR3(5.0f,0.0f,0.0f))								// スピード

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
		ENEMY_MOVING,		// 移動
		ENEMY_MAX,
	}ENEMY;

	CEnemy(PRIORITY nPriority = PRIORITY_ENEMY);
	~CEnemy();

	// テクスチャ関係の関数
	static HRESULT Load(void);
	static void Unload(void);

	// 生成
	static CEnemy *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move, D3DXVECTOR3 speed, ENEMYTYPE type);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move, D3DXVECTOR3 speed, ENEMYTYPE type);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void MoveEnemy(void);

	// 敵の状態
	static ENEMY GetState(void) { return m_State; }
	void SetState(ENEMY ENEMY_MAX) { m_State = ENEMY_MAX; }
	int GetEnemyCnt(void) { return m_enmeyCnt; }

	ENEMYTYPE GetEnemyType(void) { return m_Enemytype; };
	void SetEnemyType(ENEMYTYPE EnemyType) { m_Enemytype = EnemyType; };

private:
	static LPDIRECT3DTEXTURE9 m_apTexture;				// テクスチャのポインタ
	D3DXVECTOR3 m_size;									// サイズ
	D3DXVECTOR3 m_move;									// 移動量
	D3DXCOLOR m_Color;									// カラー
	ENEMYTYPE m_Enemytype;								// 敵の種類

	static ENEMY m_State;								// 状態
	static int m_enmeyCnt;								// 敵数カウント

	int m_nShotCnt;										// 弾発射カウント
	int m_nCounter;										// カウンター
	int m_nCntColor;									// 色の切り替えカウンター
};
#endif