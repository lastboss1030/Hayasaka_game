//=============================================================================
//
// プレイヤー処理[player.h]
// Author : Taiki Hayasaka
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "scene2D.h"
#include "main.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_PLAYER_X			(150.0f/2)		//プレイヤーの幅  (当たり判定)
#define MAX_PLAYER_Y			(150.0f/2)		//プレイヤーの高さ(当たり判定)

//=============================================================================
// クラス
//=============================================================================
class CPlayer : public CScene2D
{
public:
	CPlayer(PRIORITY nPriority = PRIORITY_PLAYER);
	~CPlayer();

	typedef enum
	{
		PLAYERSTATE_NORMAL = 0,	// 通常
		PLAYERSTATE_APPEAR,		// 出現
		PLAYERSTATE_DEATH,		// 死亡
		PLAYERSTATE_WAIT,		// 出現待ち
		PLAYERSTATE_MAX
	}PLAYERSTATE;

	// テクスチャ関係
	static HRESULT Load(void);
	static void Unload(void);

	static CPlayer *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	D3DXVECTOR3 GetMove(void);
	void SetPos(D3DXVECTOR3 pos);
	void SetMove(D3DXVECTOR3 move);
	bool HitPlayer(int nDamage);
	static PLAYERSTATE GetPlayerState(void);
	static D3DXVECTOR3 GetPlayerPos(void);

private:
	D3DXVECTOR3 m_size;							// 大きさ
	D3DXVECTOR3 m_move;							// 移動量
	D3DXCOLOR m_col;							// 色
		
	static LPDIRECT3DTEXTURE9 m_pTexture;		// 共有テクスチャのポインタ
	float m_fPosTexU;							// テクスチャのU座標
	float m_fPosTexV;							// テクスチャのV座標

	static PLAYERSTATE m_state;
	int m_nStateCnt;							// 状態カウント
	int m_nPlayerCnt;							   
	int m_nPlayerInvincibleCnt;					   
	int g_nTimePlayer = 0;						// 無敵時間記憶
	int m_nDispCnt;								// 表示カウント
	bool m_bDisp;								// 表示
												   
	int m_shootspeed;							// 発射間隔
};

#endif