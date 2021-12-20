//=============================================================================
//
// ���U���g���� [result.cpp]
// Author : taiki hayasaka
//
//=============================================================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "main.h"
#include "scene2D.h"

//=============================================================================
// �N���X
//=============================================================================
class CResult : public CScene
{
public:
	//�e�N�X�`���̎��
	typedef enum
	{
		TEXTURETYPE_GAMECLEAR = 0,		// �Q�[���N���A
		TEXTURETYPE_GAMEOVER,			// �Q�[���I�[�o�[
		TEXTURETYPE_MAX,
	}TEXTURETYPE;

	CResult();
	~CResult();

	static CResult *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);

	// �e�N�X�`���֌W�̊֐�
	static HRESULT Load(void);
	static void Unload(void);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	static LPDIRECT3DTEXTURE9 m_apTexture[TEXTURETYPE_MAX];			// �e�N�X�`���̃|�C���^
	CScene2D *m_pScene2D;											// �V�[��2D�̃|�C���^
};

#endif
