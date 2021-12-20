//=============================================================================
//
// ��������[number.h]
// Author : Taiki Hayasaka
//
//=============================================================================
#ifndef _NUMBER_H_
#define _NUMBER_H_

#include "main.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define NUMBER_HEIGHT		(70.0f)		// ����
#define NUMBER_WIDTH		(35.0f)		// ��

//=============================================================================
// �N���X
//=============================================================================
class CNumber
{
public:
	CNumber();
	~CNumber();

	// �e�N�X�`���֌W�̊֐�
	static HRESULT Load(void);
	static void Unload(void);

	static CNumber *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetNumber(int nNumber);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
};

#endif