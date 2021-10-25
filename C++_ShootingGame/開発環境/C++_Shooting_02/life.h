//=============================================================================
//
// ���C�t����[life.h]
// Author : taiki hayasaka
//
//=============================================================================
#ifndef _LIFE_H_
#define _LIFE_H_

#include "main.h"			// ���C��
#include "scene2D.h"		// �V�[��2D
#include "polygon.h"		// �|���S��

//=============================================================================
// �}�N����`
//=============================================================================
#define START_LIFE		(2)										// �����̃��C�t
#define MAX_LIFE		(5)										// �ő包��

#define LIFE_HALF		(MAX_LIFE/2)							// �ő吔�����̌�
#define SCORE_MATH		((nScore % nMath / (nMath / 10)))		// �X�R�A�̌v�Z

//=============================================================================
// �N���X
//=============================================================================
class CLife :public CScene
{
public:
	CLife(PRIORITY nPriority = PRIORITY_LIFE);
	~CLife();

	// �e�N�X�`���֌W�̊֐�
	static HRESULT Load(void);
	static void Unload(void);

	static CLife *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetLife(int nLife);
	void AddLife(int nValue);
	void SubtractLife(int nValue);
	int GetLife(void);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;		// �e�N�X�`���̃|�C���^
	CPolygon *m_apPolygon[MAX_LIFE];			// ���C�t��
	int m_nLife;								// ���C�t
	D3DXVECTOR3 m_pos;							// �ʒu
};

#endif

