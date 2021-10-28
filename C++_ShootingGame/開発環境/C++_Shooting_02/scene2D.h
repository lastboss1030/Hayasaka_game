//=============================================================================
//
// �I�u�W�F�N�g�N���X[scene2D.h]
// Author : Taiki hayasaka
//
//=============================================================================
#ifndef _SCENE2D_H_
#define _SCENE2D_H_

#include "scene.h"
#include "main.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define SCREEN_CENTER_X	(640.0f)	// ��
#define SCREEN_CENTER_Y	(360.0f)	// ����

//=============================================================================
// �\���̒�`
//=============================================================================
typedef struct
{
	D3DXVECTOR3 pos;	//���_���W
	float rhw;			//1.0�ŌŒ�
	D3DCOLOR col;		//���_�J���[
	D3DXVECTOR2 tex;	//�e�N�X�`�����W
}VERTEX_2D;

//=============================================================================
// �N���X
//=============================================================================
class CScene2D : public CScene
{
public:
	CScene2D(PRIORITY nPriority = PRIORITY_DEFAULT);
	~CScene2D();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CScene2D *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);		// �I�u�W�F�N�g�̐���

	void SetPosition(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	D3DXVECTOR3 GetPosition(void);									// �v���C���[�̈ʒu���擾

	void SetTex(int m_nPatternAnim, int PatterMax);					// �e�N�X�`�����W�X�V(�A�j���[�V����)
	void SetTexBg(float TexU, float TexV);							// �e�N�X�`�����W�X�V(�X�N���[��)
	void SetCol(D3DXCOLOR col);										// �J���[�̍X�V
	void CScene2D::SetEffect(D3DXCOLOR col, float nData);

	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);					// �e�N�X�`�����蓖��
	bool Collision(D3DXVECTOR3 pos, D3DXVECTOR3 fsize, CScene::OBJTYPE Type, CScene **pScene);
	D3DXVECTOR3 GetMove(void) { return m_move; }					//�ړ��̎擾

private:
	LPDIRECT3DTEXTURE9		m_pTexture = NULL;						// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;						// ���_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3				m_posPolygon;							// �|���S���̈ʒu(���S���W)
	D3DXVECTOR3				m_size;									// �傫��
	D3DXVECTOR3				m_move;									// �ړ�

	int m_nPatternAnim;												// �A�j���[�V�����̃p�^�[��No
	float m_fsizeX, m_fsizeY;										// �T�C�Y

};

#endif
