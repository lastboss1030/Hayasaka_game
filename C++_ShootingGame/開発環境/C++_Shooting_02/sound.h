//=============================================================================
//
// �T�E���h����
// Author : Taiki hayasaka
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//=============================================================================
// �N���X
//=============================================================================
class CSound
{
public:
	CSound();
	virtual ~CSound();

	//�T�E���h�t�@�C��
	typedef enum
	{
		// ���ʉ�
		SOUND_LABEL_SE_SHOTPLAYER,		// �e���ˉ�
		SOUND_LABEL_SE_SHOTENEMY,		// �G���ˉ�
		SOUND_LABEL_SE_SHOTBOSS,		// �{�X�e���ˉ�
		SOUND_LABEL_SE_EXPLOSION,		// ������
		SOUND_LABEL_SE_PARTSBREAK,		// �p�[�c�j��
		SOUND_LABEL_SE_ALERT,			// �A���[�g
		SOUND_LABEL_SE_HIT,				// �q�b�g
		SOUND_LABEL_SE_LASER,			// ���[�U�[��
		SOUND_LABEL_MAX,
	} SOUND_LABEL;

	HRESULT Init(HWND hWnd);
	void Uninit(void);
	HRESULT Play(SOUND_LABEL label);
	void Stop(SOUND_LABEL label);
	void Stop(void);

private:
	//�p�����[�^�\����
	typedef struct
	{
		char *pFilename;	// �t�@�C����
		int nCntLoop;		// ���[�v�J�E���g
	} SOUNDPARAM;

	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	IXAudio2 *m_pXAudio2 = NULL;								// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
	IXAudio2MasteringVoice *m_pMasteringVoice = NULL;			// �}�X�^�[�{�C�X
	IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX] = {};	// �\�[�X�{�C�X
	BYTE *m_apDataAudio[SOUND_LABEL_MAX] = {};					// �I�[�f�B�I�f�[�^
	DWORD m_aSizeAudio[SOUND_LABEL_MAX] = {};					// �I�[�f�B�I�f�[�^�T�C�Y

	// �e���f�ނ̃p�����[�^
	static SOUNDPARAM m_aParam[SOUND_LABEL_MAX];
};

#endif
