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
#define ENEMY_LIFE		(2)						// �G�̗̑�
#define MAX_ENEMY		(43)					// �G�̍ő吔
#define MAX_TEX_ENEMY	(3)						// �e�N�X�`��
#define ENEMY_TEXT_NAME ("data/TEXT/enemy.txt")	// �G�z�u�̃e�L�X�g

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

	// �G�̍\����
	typedef struct
	{
		int nTime;			// �o������
		D3DXVECTOR3 pos;	// ���W
		D3DXVECTOR3 size;	// �T�C�Y
		D3DXVECTOR3 move;	// �ړ���
		int nLife;			// ���C�t
		int nType;			// �^�C�v
	}ENEMY;

	CEnemy(PRIORITY nPriority = PRIORITY_ENEMY);
	~CEnemy();

	// �e�N�X�`���֌W�̊֐�
	static HRESULT Load(void);
	static void Unload(void);

	// ����
	static CEnemy *Create(int nIndex);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move, int nLife, int nType);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void MoveEnemy(void);
	bool HitEnemy(int nDamage);

	// �G�̏��
	ENEMYSTATE GetState(void) { return m_State; }							// ��Ԏ擾
	void SetState(ENEMYSTATE ENEMY_MAX) { m_State = ENEMY_MAX; }			// ��Ԑݒ�
	int GetEnemyCnt(void) { return m_enmeyCnt; }							// �G���擾
	
	// �^�C�v
	ENEMYTYPE GetEnemyType(void) { return m_Enemytype; };					// �^�C�v�擾
	void SetEnemyType(ENEMYTYPE EnemyType) { m_Enemytype = EnemyType; };	// �^�C�v�ݒ�

	static void EnemyGenerator(void);										// �G�������u
	static void EnemyGeneratorReset(void);									// �G�������u���Z�b�g
	static int GetCounterGame(void);										// �J�E���^�[�擾

	// �G�̒ǔ�
	void HomigEnemy(void);

	// �O���t�@�C���ǂݍ���
	void LoadData(void);

private:
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_TEX_ENEMY];	// �e�N�X�`���̃|�C���^
	D3DXVECTOR3 m_size;										// �T�C�Y
	D3DXVECTOR3 m_move;										// �ړ���
	D3DXCOLOR m_Color;										// �J���[
	ENEMYTYPE m_Enemytype;									// �G�̎��
	int m_nType;											// �^�C�v
	int m_nLife;											// ���C�t

	ENEMYSTATE m_State;										// ���
	int m_nHitCnt;											// �������Ă���̃J�E���g
	static int m_enmeyCnt;									// �G���J�E���g
	int m_nShotCnt;											// �e���˃J�E���g
	int m_nMoveCnt;											// �ړ��J�E���g
	int m_nHomingCnt;										// �z�[�~���O�J�E���g

	static ENEMY m_aEnemy[MAX_ENEMY];						// �G�\����
	static int m_nIndexEnemy;								// �G�ԍ�
	static int m_nCounterGame;								// �Q�[���J�E���g
};
#endif