//=============================================================================
//
// �^�C�g������ [title.h]
// Author : Taiki Hayasaka
//
//=============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

#include "main.h"
#include "scene2D.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_TITLE (2)	// �^�C�g����ʃe�N�X�`����

//=============================================================================
// �N���X
//=============================================================================
class CTitle : public CScene
{
public:
	CTitle();
	~CTitle();

	// ����
	static CTitle *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);

	// �e�N�X�`���֌W
	static HRESULT Load(void);
	static void Unload(void);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;
	CScene2D *m_pScene2D;
};

#endif
