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
#define MAX_ENEMY (3)	// �G�ő吔

#define ENEMY_X			(382.0f*0.3f)				// �G�̕�
#define ENEMY_Y			(369.0f*0.3f)				// �G�̍���
#define ENEMY_WIDTH		(120.0f)
#define ENEMY_HEIGHT	(120.0f)

// Create(��)
#define ENEMY_POS		(D3DXVECTOR3(1400.0f, (float)SCREEN_HEIGHT*0.5f, 0.0f))		// �ʒu
#define ENEMY_POS1		(D3DXVECTOR3(1600.0f, 500.0f, 0.0f))						// �ʒu

#define ENEMY_SIZE		(D3DXVECTOR3(120, 120, 0.0f))								// �傫��
#define ENEMY_MOVE		(D3DXVECTOR3(5.0f,0.0f,0.0f))								// �ړ���
#define ENEMY_SPEED		(D3DXVECTOR3(5.0f,0.0f,0.0f))								// �X�s�[�h

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
		ENEMY_MOVING,		// �ړ�
		ENEMY_MAX,
	}ENEMY;

	CEnemy(PRIORITY nPriority = PRIORITY_ENEMY);
	~CEnemy();

	// �e�N�X�`���֌W�̊֐�
	static HRESULT Load(void);
	static void Unload(void);

	// ����
	static CEnemy *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move, D3DXVECTOR3 speed, ENEMYTYPE type);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move, D3DXVECTOR3 speed, ENEMYTYPE type);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void MoveEnemy(void);

	// �G�̏��
	static ENEMY GetState(void) { return m_State; }
	void SetState(ENEMY ENEMY_MAX) { m_State = ENEMY_MAX; }
	int GetEnemyCnt(void) { return m_enmeyCnt; }

	ENEMYTYPE GetEnemyType(void) { return m_Enemytype; };
	void SetEnemyType(ENEMYTYPE EnemyType) { m_Enemytype = EnemyType; };

private:
	static LPDIRECT3DTEXTURE9 m_apTexture;				// �e�N�X�`���̃|�C���^
	D3DXVECTOR3 m_size;									// �T�C�Y
	D3DXVECTOR3 m_move;									// �ړ���
	D3DXCOLOR m_Color;									// �J���[
	ENEMYTYPE m_Enemytype;								// �G�̎��

	static ENEMY m_State;								// ���
	static int m_enmeyCnt;								// �G���J�E���g

	int m_nShotCnt;										// �e���˃J�E���g
	int m_nCounter;										// �J�E���^�[
	int m_nCntColor;									// �F�̐؂�ւ��J�E���^�[
};
#endif