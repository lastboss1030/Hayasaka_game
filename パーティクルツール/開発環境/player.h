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
#define MAX_PARTICLE			(8)			// パーティクル最大数

//=============================================================================
// クラス
//=============================================================================
class CPlayer : public CScene2D
{
public:

	typedef struct
	{
		int nCreateNum;					// 生成数
		int nSpeed;						// 速度
		float fRadius;					// 半径
		int nLife;						// 寿命
		int nRange;						// 範囲
		float fInertia;					// 慣性
		float fAngle;					// 角度
		D3DXCOLOR col;					// 色
	}PARTICLE;

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
	static PARTICLE GetParticle(void);

	// パーティクル情報取得
	static int GetCreateNum(void);
	static int GetSpeed(void);
	static float GetRadius(void);
	static int GetLife(void);
	static float GetInertia(void);
	static int GetRange(void);
	static float GetAngle(void);
	static int GetIndexParticle(void);

	// 外部ファイル
	void SaveData(void);	
	void LoadData(void);	

private:
	D3DXVECTOR3 m_size;							// 大きさ
	D3DXVECTOR3 m_move;							// 移動量
		
	static LPDIRECT3DTEXTURE9 m_pTexture;		// 共有テクスチャのポインタ
	float m_fPosTexU;							// テクスチャのU座標
	float m_fPosTexV;							// テクスチャのV座標

	static PARTICLE m_aParticle[MAX_PARTICLE];					// パーティクル構造体

	static int m_nIndexParticle;
};

#endif