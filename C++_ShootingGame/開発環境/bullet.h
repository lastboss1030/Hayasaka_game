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
	// 弾の種類
	typedef enum
	{
		BULLETTYPE_NONE = 0,
		BULLETTYPE_PLAYER,		// プレイヤー
		BULLETTYPE_ENEMY,		// 敵
		BULLETTYPE_BOSSPARTS,	// ボス
		BULLETTYPE_ENEMYHOMING,	// ホーミング
		BULLETYPE_MAX
	}BULLETTYPE;

	// 攻撃方法の種類
	typedef enum
	{
		ATTACKTYPE_NONE = 0,
		ATTACKTYPE_NORMAL,	// ノーマル
		ATTACKTYPE_HOMING,	// ホーミング
	}ATTACKTYPE;

	CBullet(PRIORITY nPriority = PRIORITY_BULLET);
	~CBullet();

	// 生成
	static CBullet *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 size, BULLETTYPE type, ATTACKTYPE attacktype, int nDamage);

	// テクスチャ
	static HRESULT Load(void);
	static void Unload(void);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 size, BULLETTYPE type, int nDamage);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void HitParticle(int nNum);	// 当たった時のパーティクル

	// 弾のタイプ
	BULLETTYPE GetBulletType(void) {return m_bulletType; };
	void SetBulletType(BULLETTYPE BulletType) {m_bulletType = BulletType; };

	// 攻撃タイプ
	ATTACKTYPE GetAttackType(void) { return m_AttackType; };
	void SetAttackType(ATTACKTYPE AttackType) {m_AttackType = AttackType; }

protected:
	void HomingBullet(void);				// ホーミング処理

private:
	int m_life;								// ライフ
	D3DXVECTOR3 m_move;						// 移動量
	D3DXVECTOR3 m_size;						// 大きさ
	static LPDIRECT3DTEXTURE9 m_pTexture;	// テクスチャ

	BULLETTYPE m_bulletType;				// 弾の種類
	ATTACKTYPE m_AttackType;				// 攻撃の種類
	int m_nDamage;							// ダメージ量

	int m_nCntHoming;						// ホーミングカウント
};

#endif
