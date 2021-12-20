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
// クラス
//=============================================================================
class CParticle : public CScene2D
{
public:
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

private:
	int m_life;								// ライフ
	D3DXVECTOR3 m_move;						// 移動量
	D3DXVECTOR3 m_size;						// 大きさ
	static D3DXCOLOR m_col;					// 色
	float m_fRadius;						// 半径
	static LPDIRECT3DTEXTURE9 m_pTexture;	// テクスチャ

	int nCntminSize;						// 小さくする間隔
	float m_fInertia;						// 慣性
};

#endif
