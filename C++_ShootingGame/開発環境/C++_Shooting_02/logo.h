//=============================================================================
//
// ���S���� [logo.h]
// Author : taiki hayasaka
//
//=============================================================================
#ifndef _LOGO_H_
#define _LOGO_H_

#include "main.h"
#include "scene.h"
#include "scene2D.h"
#include "player.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_LOGO (33)	// �e�N�X�`�����\���Ă���|���S���̕`��

//=============================================================================
// ���S�N���X�̒�`
//=============================================================================
class CLogo : public CScene2D
{
public:
	CLogo(PRIORITY nPriority = PRIORITY_LOGO);			// �R���X�g���N�^
	~CLogo();											// �f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nType);				// ����������
	void Uninit(void);														// �I������
	void Update(void);														// �X�V����
	void Draw(void);														// �`�揈��
																					   
	static CLogo *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nType);		// ��������
	static HRESULT Load(void);												// �e�N�X�`���ǂݍ���
	static void Unload(void);												// �e�N�X�`���j��
																					   
																			// ����֐�
	static void TypeUninit(int nType);										// �r���j��
	static void TypeMove(D3DXVECTOR3 pos, int nTypeMove);					// �r���ړ�
	static void TypeChangeCollar(D3DXCOLOR col, int nTypeCollar);			// �r���F�ω�

protected:
	static LPDIRECT3DTEXTURE9 m_apTextureLogo[MAX_LOGO];					// �p�ӂ���e�N�X�`���̐�
	static CPlayer				*m_pPlayer;									// �v���C���[�ւ̃|�C���^	�z�[�~���O���Ɏg�p

	CScene2D *m_apScene2D[3];												// �V�[��2D�ւ̃|�C���^
	D3DXVECTOR3		m_pos;													// �ʒu���
	float			m_nCntAnime;											// �e�N�X�`�������炷
	int				m_nType;												// �^�C�v�Ńe�N�X�`�����Ǘ����Ă���
};

//=============================================================================
// ���S�N���X�̒�` (Pause)
//=============================================================================
class CLogoPause : public CLogo
{
public:
	CLogoPause(PRIORITY nPriority = PRIORITY_PAUSE);		// �R���X�g���N�^
	~CLogoPause();											// �f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nType);							// ����������
	void Uninit(void);																	// �I������
	void Update(void);																	// �X�V����
	void Draw(void);																	// �`�揈��
																						   
	static CLogoPause *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nType);			// ��������
	static void Unload(void);															// �e�N�X�`���ǂݍ���
	static void TypeUninit(int nType);													// �r���j��
	static void TypeMove(D3DXVECTOR3 pos, int nTypeMove);								// �r���ړ�
	static void TypeChangeCollar(D3DXCOLOR col, int nTypeCollar);						// �r���F�ω�
};

#endif
