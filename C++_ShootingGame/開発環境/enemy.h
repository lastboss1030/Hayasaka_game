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
#define ENEMY_LIFE		(2)						// 敵の体力
#define MAX_ENEMY		(43)					// 敵の最大数
#define MAX_TEX_ENEMY	(3)						// テクスチャ
#define ENEMY_TEXT_NAME ("data/TEXT/enemy.txt")	// 敵配置のテキスト

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

	// 敵の構造体
	typedef struct
	{
		int nTime;			// 出現時間
		D3DXVECTOR3 pos;	// 座標
		D3DXVECTOR3 size;	// サイズ
		D3DXVECTOR3 move;	// 移動量
		int nLife;			// ライフ
		int nType;			// タイプ
	}ENEMY;

	CEnemy(PRIORITY nPriority = PRIORITY_ENEMY);
	~CEnemy();

	// テクスチャ関係の関数
	static HRESULT Load(void);
	static void Unload(void);

	// 生成
	static CEnemy *Create(int nIndex);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move, int nLife, int nType);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void MoveEnemy(void);
	bool HitEnemy(int nDamage);

	// 敵の状態
	ENEMYSTATE GetState(void) { return m_State; }							// 状態取得
	void SetState(ENEMYSTATE ENEMY_MAX) { m_State = ENEMY_MAX; }			// 状態設定
	int GetEnemyCnt(void) { return m_enmeyCnt; }							// 敵数取得
	
	// タイプ
	ENEMYTYPE GetEnemyType(void) { return m_Enemytype; };					// タイプ取得
	void SetEnemyType(ENEMYTYPE EnemyType) { m_Enemytype = EnemyType; };	// タイプ設定

	static void EnemyGenerator(void);										// 敵生成装置
	static void EnemyGeneratorReset(void);									// 敵生成装置リセット
	static int GetCounterGame(void);										// カウンター取得

	// 敵の追尾
	void HomigEnemy(void);

	// 外部ファイル読み込み
	void LoadData(void);

private:
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_TEX_ENEMY];	// テクスチャのポインタ
	D3DXVECTOR3 m_size;										// サイズ
	D3DXVECTOR3 m_move;										// 移動量
	D3DXCOLOR m_Color;										// カラー
	ENEMYTYPE m_Enemytype;									// 敵の種類
	int m_nType;											// タイプ
	int m_nLife;											// ライフ

	ENEMYSTATE m_State;										// 状態
	int m_nHitCnt;											// 当たってからのカウント
	static int m_enmeyCnt;									// 敵数カウント
	int m_nShotCnt;											// 弾発射カウント
	int m_nMoveCnt;											// 移動カウント
	int m_nHomingCnt;										// ホーミングカウント

	static ENEMY m_aEnemy[MAX_ENEMY];						// 敵構造体
	static int m_nIndexEnemy;								// 敵番号
	static int m_nCounterGame;								// ゲームカウント
};
#endif