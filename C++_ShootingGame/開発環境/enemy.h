//=============================================================================
//
// �G���� [enemy.h]
// Author : Taiki Hayasaka
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "scene2D.h"
#include "main.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define ENEMY_LIFE (10)		// �G�̗̑�

//=============================================================================
// �G�N���X(�h���N���X)
//=============================================================================
class CEnemy : public CScene2D
{
public:
	// �G�̎��
	typedef enum
	{
		ENEMYTYPE_NONE = 0,
		ENEMYTYPE_STOP,		// ��~�^�C�v
		ENEMYTYPE_MOVE,		// �ړ��P
		ENEMYTYPE_MOVE2,	// �ړ��Q
		ENEMYTYPE_MAX,
	}ENEMYTYPE;

	// �G�̏��
	typedef enum
	{
		ENEMY_NONE = 0,
		ENEMY_NORMAL,		// �ʏ�
		ENEMY_DAMAGE,		// �_���[�W
		ENEMY_MAX,
	}ENEMYSTATE;

	CEnemy(PRIORITY nPriority = PRIORITY_ENEMY);
	~CEnemy();

	// �e�N�X�`���֌W�̊֐�
	static HRESULT Load(void);
	static void Unload(void);

	// ����
	static CEnemy *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move, D3DXVECTOR3 speed, ENEMYTYPE type, int nLife);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move, D3DXVECTOR3 speed, ENEMYTYPE type, int nLife);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void MoveEnemy(void);
	bool HitEnemy(int nDamage);

	// �G�̏��
	ENEMYSTATE GetState(void) { return m_State; }
	void SetState(ENEMYSTATE ENEMY_MAX) { m_State = ENEMY_MAX; }
	int GetEnemyCnt(void) { return m_enmeyCnt; }

	ENEMYTYPE GetEnemyType(void) { return m_Enemytype; };
	void SetEnemyType(ENEMYTYPE EnemyType) { m_Enemytype = EnemyType; };

private:
	static LPDIRECT3DTEXTURE9 m_apTexture;				// �e�N�X�`���̃|�C���^
	D3DXVECTOR3 m_size;									// �T�C�Y
	D3DXVECTOR3 m_move;									// �ړ���
	D3DXCOLOR m_Color;									// �J���[
	ENEMYTYPE m_Enemytype;								// �G�̎��
	int m_nLife;										// ���C�t

	ENEMYSTATE m_State;									// ���
	int m_nHitCnt;										// �������Ă���̃J�E���g
	static int m_enmeyCnt;								// �G���J�E���g

	int m_nShotCnt;										// �e���˃J�E���g
	int m_nCounter;										// �J�E���^�[
	int m_nCntColor;									// �F�̐؂�ւ��J�E���^�[
};
#endif