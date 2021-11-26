//=============================================================================
//
// �A�j���[�V�������� [animation.h]
// Author : taiki hayasaka
//
//=============================================================================
#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include "main.h"
#include "scene2D.h"
#include "player.h"

//=============================================================================
//�}�N����`
//=============================================================================
#define MAX_ANIMATION (18)

//=============================================================================
// ���j�N���X�̒�`
//=============================================================================

//�p����`��N���X
class CAnimation : public CScene2D
{
public:
	CAnimation(PRIORITY nPriority = PRIORITY_ANIMATION);		// �R���X�g���N�^
	virtual ~CAnimation();										// �f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nAnime, int nType);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CAnimation *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nAnime, int nType);
	static HRESULT Load(void);
	static void Unload(void);

	static void UninitType(int nUninitType);
	static void UninitTypeClear(void);

protected:
	static LPDIRECT3DTEXTURE9 m_pTexture[MAX_ANIMATION];	// �e�N�X�`���̃|�C���^
	static int m_nUninitType;								// �r���j���Ɏg�p
	D3DXVECTOR3 m_pos;										// �ړ���
	static CPlayer *m_pPlayer;								// �v���C���[�ւ̃|�C���^

	int m_nType;											// �^�C�v�@�e�N�X�`������
	int m_nTypeBuulet;										// ����ړ����Ɏg�p
	int m_nCounterAnime;									// �A�j���[�V�����J�E���^�[
	int m_nPatternAnime;									// �A�j���[�V�����p�^�[��
	int m_fCriss;											// ���[�v����

	float m_fAnimeX;										// �摜����X
	float m_fAnimeY;										// �摜����Y
	int m_nX;												// �摜����
	int m_nY;												// �摜����

	int m_nAnime;											// �A�j���[�V�����X�s�[�h
	float m_fColorFa = 1.0f;								// �A���t�@�l
};

#endif