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
#define MAX_PARTICLE			(8)			// �p�[�e�B�N���ő吔

//=============================================================================
// �N���X
//=============================================================================
class CPlayer : public CScene2D
{
public:

	// �p�[�e�B�N���\����
	typedef struct
	{
		int nCreateNum;					// ������
		int nSpeed;						// ���x
		float fRadius;					// ���a
		int nLife;						// ����
		float fInertia;					// ����
		int nRange;						// �͈�
		float fAngle;					// �p�x
		D3DXCOLOR col;					// �F
	}PARTICLE;

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
	static PARTICLE GetParticle(void);

	// �p�[�e�B�N�����擾
	static int GetCreateNum(void);				// ������
	static int GetSpeed(void);					// ���x
	static float GetRadius(void);				// ���a
	static int GetLife(void);					// ����
	static float GetInertia(void);				// ����
	static int GetRange(void);					// �͈�
	static float GetAngle(void);				// �p�x
	static int GetIndexParticle(void);			// �ԍ�

	// �O���t�@�C��
	void SaveData(void);	
	void LoadData(void);	

private:
	D3DXVECTOR3 m_size;							// �傫��
	D3DXVECTOR3 m_move;							// �ړ���
		
	static LPDIRECT3DTEXTURE9 m_pTexture;		// ���L�e�N�X�`���̃|�C���^
	float m_fPosTexU;							// �e�N�X�`����U���W
	float m_fPosTexV;							// �e�N�X�`����V���W

	static PARTICLE m_aParticle[MAX_PARTICLE];	// �p�[�e�B�N���\����

	static int m_nIndexParticle;				// �p�[�e�B�N���ԍ�
};

#endif