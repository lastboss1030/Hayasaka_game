//================================================================================
//
// �|�[�Y���� [pause.h]
// Author : taiki hayasaka
//
//================================================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

#include "scene2D.h"

//================================================================================
//�}�N����`
//================================================================================
#define MAX_PAUSE (4)	// �|�[�Y�̍ő吔

//================================================================================
//�I�u�W�F�N�g�N���X(�w�i)
//================================================================================
class CPause :public CScene
{
public:

	typedef enum
	{
		PAUSE_NONE = 0,
		PAUSE_CONTINUE,		// �R���e�B�j���[
		PAUSE_RETRY,		// ���g���C
		PAUSE_TITLE,		// �^�C�g��
		PAUSE_MAX
	}PAUSE;

	CPause(PRIORITY nPriority = PRIORITY_PAUSE);						// �R���X�g���N�^
	~CPause();															// �f�X�g���N�^

	static CPause *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);			// �v���C���[�̐���

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);					// ����������
	void Uninit(void);													// �I������
	void Update(void);													// �X�V����
	void Draw(void);													// �`�揈��

private:
	float m_fMove;														// �ړ�
	float m_fAnim;														// �A�j���[�V����
	int	  m_ColorCnt;													// �F�_�ŃJ�E���g
	int   m_nSelectCnt;													// �I���J�E���g

	int nTimeGamePause = 0;												//����
	int nTimeCounterPause = 0;											//���ԃJ�E���^�[
	int g_nPointerPauseX = 0;											//�|�C���^�[�ʒu
	bool g_bButtonDownPause = false;									//�A�Ŗh�~

	static LPDIRECT3DTEXTURE9 m_apTexture[PAUSE_MAX];					// �e�N�X�`��
	CScene2D*m_apScene2D[PAUSE_MAX];									// CScene2D�̃|�C���^
};
#endif