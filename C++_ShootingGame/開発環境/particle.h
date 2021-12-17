//=============================================================================
//
// �p�[�e�B�N���̏��� [particle.h]
// Author : Taiki Hayasaka
//
//=============================================================================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "scene2D.h"
#include "main.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_PARTICLE (8)							// �p�[�e�B�N���ő吔
#define PARTICLE_FILE_NAME "data/TEXT/particle.txt"	// �e�L�X�g

//=============================================================================
// �N���X
//=============================================================================
class CParticle : public CScene2D
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


	CParticle(PRIORITY nPriority = PRIORITY_PARTICLE);
	~CParticle();

	// ����
	static CParticle *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 size, D3DXCOLOR col, int nLife, float fInertia);

	// �e�N�X�`��
	static HRESULT Load(void);
	static void Unload(void);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 size, D3DXCOLOR col, int nLife, float fInertia);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// �p�[�e�B�N�����擾
	static PARTICLE GetParticle(void);			// �{��
	static D3DXCOLOR GetCol(void);				// �F
	static int GetCreateNum(void);				// ������
	static int GetSpeed(void);					// ���x
	static float GetRadius(void);				// ���a
	static int GetLife(void);					// ����
	static float GetInertia(void);				// ����
	static int GetRange(void);					// �͈�
	static float GetAngle(void);				// �p�x
	static int GetIndexParticle(void);			// �ԍ�

	// �O���t�@�C��
	void LoadData(void);

private:
	int m_life;									// ���C�t
	D3DXVECTOR3 m_move;							// �ړ���
	D3DXVECTOR3 m_size;							// �傫��
	D3DXCOLOR m_col;							// �F
	float m_fRadius;							// ���a
	static LPDIRECT3DTEXTURE9 m_pTexture;		// �e�N�X�`��

	int nCntminSize;							// ����������Ԋu
	float m_fInertia;							// ����

	static PARTICLE m_aParticle[MAX_PARTICLE];	// �p�[�e�B�N���\����
	static int m_nIndexParticle;				// �p�[�e�B�N���ԍ�
};

#endif
