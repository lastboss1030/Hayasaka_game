//=============================================================================
//
// 弾の処理 [bullet.h]
// Author : Taiki Hayasaka
//
//=============================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

#include "scene2D.h"
#include "main.h"

//=============================================================================
// クラス
//=============================================================================
class CBullet : public CScene2D
{
public:

	CBullet(PRIORITY nPriority = PRIORITY_BULLET);
	~CBullet();

	// 生成
	static CBullet *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 size);

	// テクスチャ
	static HRESULT Load(void);
	static void Unload(void);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void LoadParticle(void);

private:
	int m_life;								// ライフ
	D3DXVECTOR3 m_move;						// 移動量
	D3DXVECTOR3 m_size;						// 大きさ
	static LPDIRECT3DTEXTURE9 m_pTexture;	// テクスチャ

	int m_nCreateP;			// パーティクル生成数
	int m_nSpeedP;			// パーティクル速度
	float m_fRadiusP;			// パーティクル半径
	int m_nLifeP;			// パーティクル寿命

	float m_fInertiaP;		// パーティクル慣性
	int m_nRangeP;			// パーティクル範囲
	float m_fAngleP;		// パーティクル角度

	D3DXCOLOR m_ColP;		// パーティクルカラー
			
};

#endif

