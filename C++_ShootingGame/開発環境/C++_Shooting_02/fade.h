//=============================================================================
//
//	�t�F�[�h���� [fade.h]
//	Author: Taiki Hayasaka
//
//=============================================================================
#ifndef _FADE_H_
#define	_FADE_H_

#include "main.h"
#include "scene2D.h"
#include "manager.h"

//=============================================================================
// �G�̃N���X(�h���N���X)
//=============================================================================
class CFade : public CScene2D
{
public:

	typedef enum
	{
		FADE_NONE = 0,		//�������Ă��Ȃ�
		FADE_IN,			//�t�F�[�h�C��
		FADE_OUT,			//�t�F�[�h�A�E�g
		FADE_MAX,
	}FADE;

	CFade(PRIORITY nPriority = PRIORITY_FADE);
	~CFade();

	// ����
	static CFade *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, CManager::MODE modeNext);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetFade(FADE fade, CManager::MODE modeNext);
	FADE GetFade(void);

private:
	FADE m_fade;					//�t�F�[�h�̏��
	CManager::MODE m_modeNext;		//���̃��[�h(���)
	D3DXCOLOR m_colorFade;			//�t�F�[�h�F
};

#endif
