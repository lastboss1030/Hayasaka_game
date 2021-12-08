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
#define MAX_PLAYER_X			(50.0f)		//プレイヤーの幅  
#define MAX_PLAYER_Y			(50.0f)		//プレイヤーの高さ  

//=============================================================================
// クラス
//=============================================================================
class CPlayer : public CScene2D
{
public:
	CPlayer(PRIORITY nPriority = PRIORITY_PLAYER);
	~CPlayer();

	// テクスチャ関係
	static HRESULT Load(void);
	static void Unload(void);

	static CPlayer *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void PlayerLimit(void);
	void ChangeParticle(void);

	D3DXVECTOR3 GetMove(void);
	void SetPos(D3DXVECTOR3 pos);
	void SetMove(D3DXVECTOR3 move);
	static D3DXVECTOR3 GetPlayerPos(void);
	static D3DXCOLOR GetCol(void);

	// パーティクル情報取得
	static int GetCreateNum(void);
	static int GetSpeed(void);
	static float GetRadius(void);
	static int GetLife(void);
	static float GetInertia(void);

private:
	D3DXVECTOR3 m_size;							// 大きさ
	D3DXVECTOR3 m_move;							// 移動量
	static D3DXCOLOR m_col;						// 色
		
	static LPDIRECT3DTEXTURE9 m_pTexture;		// 共有テクスチャのポインタ
	float m_fPosTexU;							// テクスチャのU座標
	float m_fPosTexV;							// テクスチャのV座標

	// パーティクル用
	static int m_nCreateNum;					// 生成数
	static int m_nSpeed;						// 速度
	static float m_fRadius;						// 半径
	static int m_nLife;							// 寿命
	static int m_nRange;						// 範囲
	static float m_fInertia;					// 慣性
	static int m_nAngle;						// 角度
};

#endif