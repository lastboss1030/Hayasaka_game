//=============================================================================
//
// �G���� [boss_parts.h]
// Author : Taiki Hayasaka
//
//=============================================================================
#ifndef _BOSS_PARTS_H_
#define _BOSS_PARTS_H_

#include "scene2D.h"
#include "main.h"

//=============================================================================
// �O���錾
//=============================================================================
class CEnemy;				// �G

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_PARTS (8)	// �p�[�c�ő吔

#define PARTS_X			(382.0f*0.3f)				// �p�[�c�̕�
#define PARTS_Y			(369.0f*0.3f)				// �p�[�c�̍���
#define MOVECNT2		(400.0f)
#define WAIT_Y			(600.0f)					// �ҋ@

//=============================================================================
// �G�N���X(�h���N���X)
//=============================================================================
class CParts : public CScene2D
{
public:
	// �G�̎��
	typedef enum
	{
		PARTSTYPE_NONE = 0,
		PARTSTYPE_FRONT,		// ��O
		PARTSTYPE_SHOOT1,		// �e��1
		PARTSTYPE_SHOOT2,		// �e��2
		PARTSTYPE_SHOOT3,		// �e��3
		PARTSTYPE_SHOOT4,		// �e��4
		PARTSTYPE_LEFT,			// ����
		PARTSTYPE_RIGHT,		// �E��
		PARTSTYPE_MIDLE,		// ����
		PARTSTYPE_MAX,
	}PARTSTYPE;

	// �G�̏��
	typedef enum
	{
		PARTS_NORMAL = 0,	// �ʏ�
		PARTS_DAMAGE,		// �_���[�W
		PARTS_BREAK,		// �j��
		PARTS_BREAKFALL,	// ������
		PARTS_MAX,
	}PARTSSTATE;

	CParts(PRIORITY nPriority = PRIORITY_BOSS);
	~CParts();

	// �e�N�X�`���֌W�̊֐�
	static HRESULT Load(void);
	static void Unload(void);

	// ����
	static CParts *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move, PARTSTYPE type, int nTexNum, int nLife);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move, PARTSTYPE type, int nLife);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// �{�X�p�[�c�̏��
	PARTSSTATE GetState(void) { return m_state; }
	void SetState(PARTSSTATE PARTS_MAX) { m_state = PARTS_MAX; }
	int GetPartsState(void) { return m_state; }
	void SetPartsState(PARTSSTATE PartsState);
	bool HitBossParts(int nDamage);

	// �p�[�c�^�C�v
	PARTSTYPE GetPartsType(void) { return m_Partstype; };
	void SetPartsType(PARTSTYPE PartsType) { m_Partstype = PartsType; };

	static CEnemy *GetEnemy(void) { return m_pEnemy; }			// �G�̎擾
	void Collision(void);										// �����蔻��

private:
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_PARTS];			// �e�N�X�`���̃|�C���^
	D3DXVECTOR3 m_size;											// �T�C�Y
	D3DXVECTOR3 m_move;											// �ړ���
	D3DXCOLOR m_Color;											// �J���[
	PARTSTYPE m_Partstype;										// �p�[�c�̎��
	D3DXVECTOR3 m_EnemyV;											// �v���C���[�Ɍ���

	PARTSSTATE m_state;											// ���
	int m_nLife;												// ���C�t
	int m_partsCnt;												// �G���J�E���g
	static CEnemy *m_pEnemy;									// �G�̃|�C���^
	bool m_bShoot;												// ����

	int m_nHitCnt;												// �������Ă���̃J�E���g
	int m_nCntShoot;											// �e���˃J�E���g  (�e��4�ӏ�)
	int m_nBreakCnt;											// �j���J�E���g
	int m_nExplosionCnt;										// �����J�E���g
	float m_nSwingWidth;											// �U�ꕝ

	int m_nMoveCnt;												// �ړ��J�E���g
	float m_nMoveCnt2;											// �ړ��J�E���g2
	float m_nMoveCnt3;											// �ړ��J�E���g3

	int m_nLaserCnt;											// ���[�U�[�J�E���g  (���̔��˂܂ł̃^�C�~���O)
	int m_nLaserCnt2;											// ���[�U�[�J�E���g2 (�e�̊Ԋu)
	int m_nLaserCnt3;											// ���[�U�[�J�E���g3 (�����Ă鎞��)

	int m_nCntShotHoming;										// �z�[�~���O�e�J�E���g  (���ˊԊu)
	int m_nCntShotHoming2;										// �z�[�~���O�e�J�E���g2 (�Ǐ]�܂ł̎���)

	int m_nWingBreakCnt;										// �H����ꂽ���̃J�E���g

	int m_nSoundCnt;											// �T�E���h�J�E���g  (�x����)
	int m_nSoundCnt2;											// �T�E���h�J�E���g2 (�p�[�c�j��)
	int m_nSoundCnt3;											// �T�E���h�J�E���g3 (���[�U�[��)
};
#endif
