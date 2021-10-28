//=============================================================================
//
// 敵処理 [boss_parts.h]
// Author : Taiki Hayasaka
//
//=============================================================================
#ifndef _BOSS_PARTS_H_
#define _BOSS_PARTS_H_

#include "scene2D.h"
#include "main.h"

//=============================================================================
// 前方宣言
//=============================================================================
class CEnemy;				// 敵

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_PARTS (8)	// パーツ最大数

#define PARTS_X			(382.0f*0.3f)				// パーツの幅
#define PARTS_Y			(369.0f*0.3f)				// パーツの高さ
#define MOVECNT2		(400.0f)
#define WAIT_Y			(600.0f)					// 待機

//=============================================================================
// 敵クラス(派生クラス)
//=============================================================================
class CParts : public CScene2D
{
public:
	// 敵の種類
	typedef enum
	{
		PARTSTYPE_NONE = 0,
		PARTSTYPE_FRONT,		// 手前
		PARTSTYPE_SHOOT1,		// 銃口1
		PARTSTYPE_SHOOT2,		// 銃口2
		PARTSTYPE_SHOOT3,		// 銃口3
		PARTSTYPE_SHOOT4,		// 銃口4
		PARTSTYPE_LEFT,			// 左翼
		PARTSTYPE_RIGHT,		// 右翼
		PARTSTYPE_MIDLE,		// 中央
		PARTSTYPE_MAX,
	}PARTSTYPE;

	// 敵の状態
	typedef enum
	{
		PARTS_NORMAL = 0,	// 通常
		PARTS_DAMAGE,		// ダメージ
		PARTS_BREAK,		// 破損
		PARTS_BREAKFALL,	// 落ちる
		PARTS_MAX,
	}PARTSSTATE;

	CParts(PRIORITY nPriority = PRIORITY_BOSS);
	~CParts();

	// テクスチャ関係の関数
	static HRESULT Load(void);
	static void Unload(void);

	// 生成
	static CParts *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move, PARTSTYPE type, int nTexNum, int nLife);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move, PARTSTYPE type, int nLife);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// ボスパーツの状態
	PARTSSTATE GetState(void) { return m_state; }
	void SetState(PARTSSTATE PARTS_MAX) { m_state = PARTS_MAX; }
	int GetPartsState(void) { return m_state; }
	void SetPartsState(PARTSSTATE PartsState);
	bool HitBossParts(int nDamage);

	// パーツタイプ
	PARTSTYPE GetPartsType(void) { return m_Partstype; };
	void SetPartsType(PARTSTYPE PartsType) { m_Partstype = PartsType; };

	static CEnemy *GetEnemy(void) { return m_pEnemy; }			// 敵の取得
	void Collision(void);										// 当たり判定

private:
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_PARTS];			// テクスチャのポインタ
	D3DXVECTOR3 m_size;											// サイズ
	D3DXVECTOR3 m_move;											// 移動量
	D3DXCOLOR m_Color;											// カラー
	PARTSTYPE m_Partstype;										// パーツの種類
	D3DXVECTOR3 m_EnemyV;											// プレイヤーに向く

	PARTSSTATE m_state;											// 状態
	int m_nLife;												// ライフ
	int m_partsCnt;												// 敵数カウント
	static CEnemy *m_pEnemy;									// 敵のポインタ
	bool m_bShoot;												// 発射

	int m_nHitCnt;												// 当たってからのカウント
	int m_nCntShoot;											// 弾発射カウント  (銃口4箇所)
	int m_nBreakCnt;											// 破損カウント
	int m_nExplosionCnt;										// 爆発カウント
	float m_nSwingWidth;											// 振れ幅

	int m_nMoveCnt;												// 移動カウント
	float m_nMoveCnt2;											// 移動カウント2
	float m_nMoveCnt3;											// 移動カウント3

	int m_nLaserCnt;											// レーザーカウント  (次の発射までのタイミング)
	int m_nLaserCnt2;											// レーザーカウント2 (弾の間隔)
	int m_nLaserCnt3;											// レーザーカウント3 (撃ってる時間)

	int m_nCntShotHoming;										// ホーミング弾カウント  (発射間隔)
	int m_nCntShotHoming2;										// ホーミング弾カウント2 (追従までの時間)

	int m_nWingBreakCnt;										// 羽が壊れた時のカウント

	int m_nSoundCnt;											// サウンドカウント  (警告音)
	int m_nSoundCnt2;											// サウンドカウント2 (パーツ破損)
	int m_nSoundCnt3;											// サウンドカウント3 (レーザー音)
};
#endif
