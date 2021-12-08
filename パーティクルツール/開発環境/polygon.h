//=============================================================================
//
//	�|���S���̏��� [polygon.h]
//	Author: Taiki hayasaka
//
//=============================================================================
#ifndef _POLYGON_H_
#define _POLYGON_H_

#include "main.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define LIFE_HEIGHT			(60.0f)			// ����
#define LIFE_WIDTH			(60.0f)			// ��

//=============================================================================
// �|���S���N���X
//=============================================================================
class CPolygon
{
public:
	CPolygon();
	~CPolygon();

	// �I�u�W�F�N�g�̐���
	static CPolygon *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// �ݒ�
	void SetPolygon(int nNumber);

	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);

private:
	LPDIRECT3DTEXTURE9 m_pTexture;				// �e�N�X�`���̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;			// ���_�o�b�t�@�ւ̃|�C���^
};

#endif
