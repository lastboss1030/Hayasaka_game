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

	// �e�̎��
	typedef enum
	{
		PARTICLETYPE_NONE = 0,
		PARTICLETYPE_PLAYER,		// �v���C���[
		PARTICLETYPE_ENEMY,			// �G
		PARTICLETYPE_BOSSPARTS,		// �{�X
		PARTICLETYPE_MAX
	}PARTICLETYPE;


	CParticle(PRIORITY nPriority = PRIORITY_PARTICLE);
	~CParticle();

	// ����
	static CParticle *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 size, D3DXCOLOR col, float fRadius, float fCntAlpha);

	// �e�N�X�`��
	static HRESULT Load(void);
	static void Unload(void);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 size, D3DXCOLOR col, float fRadius, float fCntAlpha);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// �p�[�e�B�N���̃^�C�v
	PARTICLETYPE GetParticleType(void) { return m_ParticleType; };
	void SetBulletType(PARTICLETYPE ParticleType) { m_ParticleType = ParticleType; };

private:
	int m_life;								// ���C�t
	D3DXVECTOR3 m_move;						// �ړ���
	D3DXVECTOR3 m_size;						// �傫��
	D3DXCOLOR m_col;						// �F
	float m_fRadius;						// ���a
	float m_fCntAlpha;						// �����x
	static LPDIRECT3DTEXTURE9 m_pTexture;	// �e�N�X�`��

	PARTICLETYPE m_ParticleType;			// �e�̎��
	int nCntminSize;						// ����������Ԋu
	float m_fInertia;						// ����
};

#endif
