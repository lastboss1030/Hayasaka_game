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

	// 弾の種類
	typedef enum
	{
		PARTICLETYPE_NONE = 0,
		PARTICLETYPE_PLAYER,		// プレイヤー
		PARTICLETYPE_ENEMY,			// 敵
		PARTICLETYPE_BOSSPARTS,		// ボス
		PARTICLETYPE_MAX
	}PARTICLETYPE;


	CParticle(PRIORITY nPriority = PRIORITY_PARTICLE);
	~CParticle();

	// 生成
	static CParticle *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 size, D3DXCOLOR col, float fRadius, float fCntAlpha);

	// テクスチャ
	static HRESULT Load(void);
	static void Unload(void);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 size, D3DXCOLOR col, float fRadius, float fCntAlpha);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// パーティクルのタイプ
	PARTICLETYPE GetParticleType(void) { return m_ParticleType; };
	void SetBulletType(PARTICLETYPE ParticleType) { m_ParticleType = ParticleType; };

private:
	int m_life;								// ライフ
	D3DXVECTOR3 m_move;						// 移動量
	D3DXVECTOR3 m_size;						// 大きさ
	D3DXCOLOR m_col;						// 色
	float m_fRadius;						// 半径
	float m_fCntAlpha;						// 透明度
	static LPDIRECT3DTEXTURE9 m_pTexture;	// テクスチャ

	PARTICLETYPE m_ParticleType;			// 弾の種類
	int nCntminSize;						// 小さくする間隔
	float m_fInertia;						// 慣性
};

#endif
