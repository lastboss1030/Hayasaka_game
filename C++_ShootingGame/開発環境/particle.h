//=============================================================================
//
// パーティクルの処理 [particle.h]
// Author : Taiki Hayasaka
//
//=============================================================================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "scene2D.h"
#include "main.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_PARTICLE (8)							// パーティクル最大数
#define PARTICLE_FILE_NAME "data/TEXT/particle.txt"	// テキスト

//=============================================================================
// クラス
//=============================================================================
class CParticle : public CScene2D
{
public:

	// パーティクル構造体
	typedef struct
	{
		int nCreateNum;					// 生成数
		int nSpeed;						// 速度
		float fRadius;					// 半径
		int nLife;						// 寿命
		float fInertia;					// 慣性
		int nRange;						// 範囲
		float fAngle;					// 角度
		D3DXCOLOR col;					// 色
	}PARTICLE;


	CParticle(PRIORITY nPriority = PRIORITY_PARTICLE);
	~CParticle();

	// 生成
	static CParticle *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 size, D3DXCOLOR col, int nLife, float fInertia);

	// テクスチャ
	static HRESULT Load(void);
	static void Unload(void);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 size, D3DXCOLOR col, int nLife, float fInertia);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// パーティクル情報取得
	static PARTICLE GetParticle(void);			// 本体
	static D3DXCOLOR GetCol(void);				// 色
	static int GetCreateNum(void);				// 生成数
	static int GetSpeed(void);					// 速度
	static float GetRadius(void);				// 半径
	static int GetLife(void);					// 寿命
	static float GetInertia(void);				// 慣性
	static int GetRange(void);					// 範囲
	static float GetAngle(void);				// 角度
	static int GetIndexParticle(void);			// 番号

	// 外部ファイル
	void LoadData(void);

private:
	int m_life;									// ライフ
	D3DXVECTOR3 m_move;							// 移動量
	D3DXVECTOR3 m_size;							// 大きさ
	D3DXCOLOR m_col;							// 色
	float m_fRadius;							// 半径
	static LPDIRECT3DTEXTURE9 m_pTexture;		// テクスチャ

	int nCntminSize;							// 小さくする間隔
	float m_fInertia;							// 慣性

	static PARTICLE m_aParticle[MAX_PARTICLE];	// パーティクル構造体
	static int m_nIndexParticle;				// パーティクル番号
};

#endif
