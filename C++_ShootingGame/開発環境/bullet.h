//=============================================================================
//
// �e�̏��� [bullet.h]
// Author : Taiki Hayasaka
//
//=============================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

#include "scene2D.h"
#include "main.h"

//=============================================================================
// �N���X
//=============================================================================
class CBullet : public CScene2D
{
public:
	// �e�̎��
	typedef enum
	{
		BULLETTYPE_NONE = 0,
		BULLETTYPE_PLAYER,		// �v���C���[
		BULLETTYPE_ENEMY,		// �G
		BULLETTYPE_BOSSPARTS,	// �{�X
		BULLETTYPE_ENEMYHOMING,	// �z�[�~���O
		BULLETYPE_MAX
	}BULLETTYPE;

	// �U�����@�̎��
	typedef enum
	{
		ATTACKTYPE_NONE = 0,
		ATTACKTYPE_NORMAL,	// �m�[�}��
		ATTACKTYPE_HOMING,	// �z�[�~���O
	}ATTACKTYPE;

	CBullet(PRIORITY nPriority = PRIORITY_BULLET);
	~CBullet();

	// ����
	static CBullet *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 size, BULLETTYPE type, ATTACKTYPE attacktype, int nDamage);

	// �e�N�X�`��
	static HRESULT Load(void);
	static void Unload(void);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 size, BULLETTYPE type, int nDamage);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void HitParticle(int nNum);	// �����������̃p�[�e�B�N��

	// �e�̃^�C�v
	BULLETTYPE GetBulletType(void) {return m_bulletType; };
	void SetBulletType(BULLETTYPE BulletType) {m_bulletType = BulletType; };

	// �U���^�C�v
	ATTACKTYPE GetAttackType(void) { return m_AttackType; };
	void SetAttackType(ATTACKTYPE AttackType) {m_AttackType = AttackType; }

protected:
	void HomingBullet(void);				// �z�[�~���O����

private:
	int m_life;								// ���C�t
	D3DXVECTOR3 m_move;						// �ړ���
	D3DXVECTOR3 m_size;						// �傫��
	static LPDIRECT3DTEXTURE9 m_pTexture;	// �e�N�X�`��

	BULLETTYPE m_bulletType;				// �e�̎��
	ATTACKTYPE m_AttackType;				// �U���̎��
	int m_nDamage;							// �_���[�W��

	int m_nCntHoming;						// �z�[�~���O�J�E���g
};

#endif
