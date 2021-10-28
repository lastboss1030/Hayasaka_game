//=============================================================================
//
// �}�l�[�W���[���� [manager.h]
// Author : Taiki Hayasaka
//
//=============================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "manager.h"
#include "main.h"

//=============================================================================
// �O���錾
//=============================================================================
class CRenderer;			// �����_���N���X
class CInputKeyboard;		// �L�[�{�[�h�N���X
class CSound;				// �T�E���h�N���X
class CGame;				// �Q�[���V�[��
class CTitle;				// �^�C�g���V�[��
class CResult;				// ���U���g�V�[��
class CFade;				// �t�F�[�h

//=============================================================================
// �N���X
//=============================================================================
class CManager
{
public:
	// ���[�h
	typedef enum
	{
		MODE_TITLE = 0,
		MODE_GAME,
		MODE_RESULT,
		MODE_MAX,
	}MODE;

	CManager();
	~CManager();

	HRESULT	Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// �f�o�C�X�̎擾
	static CRenderer *GetRenderer(void) { return m_pRenderer; }						// �����_���[
	static CInputKeyboard *GetInputKeyboard(void) { return m_pInputKeyboard; }		// �L�[�{�[�h
	static CSound *GetSound(void) { return m_pSound; }								// �T�E���h
	static CFade *GetFade(void) { return m_pFade; }									// �t�F�[�h
	static CGame *GetGame(void) { return m_pGame; }									// �Q�[��

	// ���[�h
	static void CManager::SetMode(MODE mode);						// ���[�h�̐ݒ�@mode:���[�h�̎��
	static MODE GetMode(void) { return m_mode; }					// �f�o�C�X�̎擾(���[�h)

	// �|�[�Y
	static void SetPause(void);										// �|�[�Y�̎擾
	static void SetPause(bool pause) { m_bPause = pause; }			// �|�[�Y�̎擾
	static bool GetPause(void) { return m_bPause; }					// �|�[�Y�̏��

private:
	// ���[�h
	static CTitle *m_pTitle;										// �^�C�g��
	static CGame *m_pGame;											// �Q�[��
	static CResult *m_pResult;										// ���U���g
	static MODE m_mode;												// ���[�h

	static CRenderer *m_pRenderer;									// �����_�����O�̃|�C���^
	static CInputKeyboard *m_pInputKeyboard;						// �L�[�{�[�h�̃|�C���^
	static CSound *m_pSound;										// �T�E���h�̃|�C���^
	static CFade *m_pFade;											// �t�F�[�h�̃|�C���^

	static bool m_bPause;											// �|�[�Y�����ǂ���
};

#endif