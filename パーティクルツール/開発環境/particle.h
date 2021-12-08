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
// �N���X
//=============================================================================
class CParticle : public CScene2D
{
public:
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

private:
	int m_life;								// ���C�t
	D3DXVECTOR3 m_move;						// �ړ���
	D3DXVECTOR3 m_size;						// �傫��
	static D3DXCOLOR m_col;					// �F
	float m_fRadius;						// ���a
	static LPDIRECT3DTEXTURE9 m_pTexture;	// �e�N�X�`��

	int nCntminSize;						// ����������Ԋu
	float m_fInertia;						// ����
};

#endif
