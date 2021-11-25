//=============================================================================
//
//	�L�[�{�[�h���� [Input_keyboard.cpp]
//	Author: Taiki hayasaka
//
//=============================================================================
#include "Input_keyboard.h"		// �L�[�{�[�h
#include "Input.h"				// ����

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CInputKeyboard::CInputKeyboard()
{
	// ������
	for (int nData = 0; nData < MAX_KEY; nData++)
	{
		m_aKeyState[nData] = 0;				// �O��̓��͏��
		m_akeyStateTrigger[nData] = 0;		// �L�[�g���K�[���
		m_akeyStateRelease[nData] = 0;		// �L�[�����[�X���
	}
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CInputKeyboard::~CInputKeyboard()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	// ���͂̏���������
	CInput::Init(hInstance, hWnd);

	// ���̓f�o�C�X
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}

	// �f�[�^�t�H�[�}�b�g�̐ݒ�
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// �������[�h�ݒ�
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//�A�N�Z�X���̎擾
	m_pDevice->Acquire();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CInputKeyboard::Uninit(void)
{
	CInput::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CInputKeyboard::Update(void)
{
	// �ϐ��錾
	BYTE aKeyState[256];		// ���݂̓���

	// �f�o�C�X����f�[�^���擾
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (int nKey = 0; nKey < MAX_KEY; nKey++)
		{
			// �g���K�[
			m_akeyStateTrigger[nKey] = (~m_aKeyState[nKey])&(aKeyState[nKey]);

			// �����[�X
			m_akeyStateRelease[nKey] = (m_aKeyState[nKey])&(~aKeyState[nKey]);

			// �L�[�������Ă��邩�̏��ۑ�(�v���X)
			m_aKeyState[nKey] = aKeyState[nKey];
		}
	}
	else
	{
		// �A�N�Z�X���̎擾
		m_pDevice->Acquire();
	}
}

//=============================================================================
// �v���X����
//=============================================================================
bool CInputKeyboard::GetPress(int nKey)
{
	return(m_aKeyState[nKey] & 0x80) ? true : false;
}

//=============================================================================
// �g���K�[����
//=============================================================================
bool CInputKeyboard::GetTrigger(int nKey)
{
	return(m_akeyStateTrigger[nKey] & 0x80) ? true : false;
}

//=============================================================================
// �����[�X����
//=============================================================================
bool CInputKeyboard::GetRelease(int nKey)
{
	return(m_akeyStateRelease[nKey] & 0x80) ? true : false;
}