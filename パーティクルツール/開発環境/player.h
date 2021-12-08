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
#define MAX_PLAYER_X			(50.0f)		//�v���C���[�̕�  
#define MAX_PLAYER_Y			(50.0f)		//�v���C���[�̍���  

//=============================================================================
// �N���X
//=============================================================================
class CPlayer : public CScene2D
{
public:
	CPlayer(PRIORITY nPriority = PRIORITY_PLAYER);
	~CPlayer();

	// �e�N�X�`���֌W
	static HRESULT Load(void);
	static void Unload(void);

	static CPlayer *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void PlayerLimit(void);
	void ChangeParticle(void);

	D3DXVECTOR3 GetMove(void);
	void SetPos(D3DXVECTOR3 pos);
	void SetMove(D3DXVECTOR3 move);
	static D3DXVECTOR3 GetPlayerPos(void);
	static D3DXCOLOR GetCol(void);

	// �p�[�e�B�N�����擾
	static int GetCreateNum(void);
	static int GetSpeed(void);
	static float GetRadius(void);
	static int GetLife(void);
	static float GetInertia(void);

private:
	D3DXVECTOR3 m_size;							// �傫��
	D3DXVECTOR3 m_move;							// �ړ���
	static D3DXCOLOR m_col;						// �F
		
	static LPDIRECT3DTEXTURE9 m_pTexture;		// ���L�e�N�X�`���̃|�C���^
	float m_fPosTexU;							// �e�N�X�`����U���W
	float m_fPosTexV;							// �e�N�X�`����V���W

	// �p�[�e�B�N���p
	static int m_nCreateNum;					// ������
	static int m_nSpeed;						// ���x
	static float m_fRadius;						// ���a
	static int m_nLife;							// ����
	static int m_nRange;						// �͈�
	static float m_fInertia;					// ����
	static int m_nAngle;						// �p�x
};

#endif