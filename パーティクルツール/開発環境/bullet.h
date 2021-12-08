//=============================================================================
//
// �e�̏��� [bullet.h]
// Author : Taiki Hayasaka
//
//=============================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

#include "scene2D.h"
#include "main.h"

//=============================================================================
// �N���X
//=============================================================================
class CBullet : public CScene2D
{
public:

	CBullet(PRIORITY nPriority = PRIORITY_BULLET);
	~CBullet();

	// ����
	static CBullet *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 size);

	// �e�N�X�`��
	static HRESULT Load(void);
	static void Unload(void);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	int m_life;								// ���C�t
	D3DXVECTOR3 m_move;						// �ړ���
	D3DXVECTOR3 m_size;						// �傫��
	static LPDIRECT3DTEXTURE9 m_pTexture;	// �e�N�X�`��
};

#endif

