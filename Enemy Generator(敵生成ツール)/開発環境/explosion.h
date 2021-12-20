//=============================================================================
//
// �������� [explosion.h]
// Author : Taiki Hayasaka
//
//=============================================================================
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

#include "scene2D.h"
#include "main.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_ANIMATION_COUNTER	(3)			// ���j�̃X�s�[�h
#define MAX_ANIMATION_PATTERN	(8)			// �����̃y�[�W��
#define MAX_EXPLOSION_SIZE_X	(100)		// �����̕�
#define MAX_EXPLOSION_SIZE_Y	(100)		// �����̍���

//=============================================================================
// �N���X
//=============================================================================
class CExplosion : public CScene2D
{
public:
	CExplosion(PRIORITY nPriority = PRIORITY_EXPLOSION);
	~CExplosion();

	// �e�N�X�`���֌W�̊֐�
	static HRESULT Load(void);
	static void Unload(void);
	
	// ����
	static CExplosion *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	D3DXVECTOR3 m_size;						// �傫��
	D3DXVECTOR3 m_move;						// �ړ���
	D3DXCOLOR m_colorExplosion;				// �����F
	static LPDIRECT3DTEXTURE9 m_pTexture;	// �e�N�X�`��

	int m_nCountAnim;						// �J�E���^�[
	int m_nPatternAnim;						// �p�^�[��No
};

#endif