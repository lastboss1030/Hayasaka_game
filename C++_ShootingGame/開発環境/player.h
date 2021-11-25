//=============================================================================
//
// �v���C���[����[player.h]
// Author : Taiki Hayasaka
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "scene2D.h"
#include "main.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_PLAYER_X			(150.0f/2)		//�v���C���[�̕�  (�����蔻��)
#define MAX_PLAYER_Y			(150.0f/2)		//�v���C���[�̍���(�����蔻��)

//=============================================================================
// �N���X
//=============================================================================
class CPlayer : public CScene2D
{
public:
	CPlayer(PRIORITY nPriority = PRIORITY_PLAYER);
	~CPlayer();

	typedef enum
	{
		PLAYERSTATE_NORMAL = 0,	// �ʏ�
		PLAYERSTATE_APPEAR,		// �o��
		PLAYERSTATE_DEATH,		// ���S
		PLAYERSTATE_WAIT,		// �o���҂�
		PLAYERSTATE_MAX
	}PLAYERSTATE;

	// �e�N�X�`���֌W
	static HRESULT Load(void);
	static void Unload(void);

	static CPlayer *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	D3DXVECTOR3 GetMove(void);
	void SetPos(D3DXVECTOR3 pos);
	void SetMove(D3DXVECTOR3 move);
	bool HitPlayer(int nDamage);
	static PLAYERSTATE GetPlayerState(void);
	static D3DXVECTOR3 GetPlayerPos(void);

private:
	D3DXVECTOR3 m_size;							// �傫��
	D3DXVECTOR3 m_move;							// �ړ���
	D3DXCOLOR m_col;							// �F
		
	static LPDIRECT3DTEXTURE9 m_pTexture;		// ���L�e�N�X�`���̃|�C���^
	float m_fPosTexU;							// �e�N�X�`����U���W
	float m_fPosTexV;							// �e�N�X�`����V���W

	static PLAYERSTATE m_state;
	int m_nStateCnt;							// ��ԃJ�E���g
	int m_nPlayerCnt;							   
	int m_nPlayerInvincibleCnt;					   
	int g_nTimePlayer = 0;						// ���G���ԋL��
	int m_nDispCnt;								// �\���J�E���g
	bool m_bDisp;								// �\��
												   
	int m_shootspeed;							// ���ˊԊu
};

#endif