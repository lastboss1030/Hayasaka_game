//=============================================================================
//
// �I�u�W�F�N�g���� [scene.cpp]
// Author : Taiki Hayasaka
//
//=============================================================================
#include "scene2D.h"
#include "manager.h"
#include "renderer.h"
#include "Input_Keyboard.h"

#include "scene.h"
#include "bg.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define FVF_VERTEX_2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)	// �e�N�X�`���̍��W

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CScene2D::CScene2D(PRIORITY nPriority) : CScene(nPriority)
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CScene2D::~CScene2D()
{

}

//=============================================================================
// ������
//=============================================================================
HRESULT CScene2D::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	//�ϐ��錾
	VERTEX_2D *pVtx;

	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL)))
	{
		return E_FAIL;
	}

	// ���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	m_posPolygon = pos;
	m_size = size;

	// ���_���W�̍X�V
	pVtx[0].pos = D3DXVECTOR3(m_posPolygon.x - size.x / 2.0f, m_posPolygon.y + size.y / 2.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_posPolygon.x + size.x / 2.0f, m_posPolygon.y + size.y / 2.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_posPolygon.x - size.x / 2.0f, m_posPolygon.y - size.y / 2.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_posPolygon.x + size.x / 2.0f, m_posPolygon.y - size.y / 2.0f, 0.0f);

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CScene2D::Uninit(void)
{
	// ���_�o�b�t�@�̊J��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// �I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CScene2D::Update(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void CScene2D::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=============================================================================
// ����
//=============================================================================
CScene2D *CScene2D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	CScene2D* pScene2D;
	pScene2D = new CScene2D;

	if (pScene2D != NULL)
	{
		pScene2D->Init(pos, size);
	}
	
	return pScene2D;
}

//=============================================================================
// �Z�b�g�|�W�V����
//=============================================================================
void CScene2D::SetPosition(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// ���
	m_posPolygon = pos;
	m_size = size;

	// �ϐ��錾
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(m_posPolygon.x - size.x / 2.0f, m_posPolygon.y + size.y / 2.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_posPolygon.x - size.x / 2.0f, m_posPolygon.y - size.y / 2.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_posPolygon.x + size.x / 2.0f, m_posPolygon.y + size.y / 2.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_posPolygon.x + size.x / 2.0f, m_posPolygon.y - size.y / 2.0f, 0.0f);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	// �Ăяo��
	SetPos(pos);
	SetSize(size);
}

//-------------------------------------------------------------------------------
// �v���C���[�̈ʒu���擾
//-------------------------------------------------------------------------------
D3DXVECTOR3 CScene2D::GetPosition(void)
{
	return m_posPolygon;
}

//=============================================================================
// �e�N�X�`��
//=============================================================================
void CScene2D::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	// �e�N�X�`���̊��蓖��
	m_pTexture = pTexture;
}

//=============================================================================
// �Z�b�g�e�N�X�`��
//=============================================================================
void CScene2D::SetTex(int m_nPatternAnim, int PatterMax)
{
	// �ϐ��錾
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f + m_nPatternAnim*(1.0f / PatterMax), 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f + m_nPatternAnim*(1.0f / PatterMax), 0.0f);
	pVtx[2].tex = D3DXVECTOR2((1.0f / PatterMax) + m_nPatternAnim*(1.0f / PatterMax), 1.0f);
	pVtx[3].tex = D3DXVECTOR2((1.0f / PatterMax) + m_nPatternAnim*(1.0f / PatterMax), 0.0f);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �e�N�X�`��(�X�N���[��)
//=============================================================================
void CScene2D::SetTexBg(float TexU, float TexV)
{
	// �ϐ��錾
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �X�N���[���̏���
	pVtx[0].tex = D3DXVECTOR2(0.0f + TexU, 1.0f + TexV);
	pVtx[1].tex = D3DXVECTOR2(0.0f + TexU, 0.0f + TexV);
	pVtx[2].tex = D3DXVECTOR2(1.0f + TexU, 1.0f + TexV);
	pVtx[3].tex = D3DXVECTOR2(1.0f + TexU, 0.0f + TexV);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �J���[�̍X�V
//=============================================================================
void CScene2D::SetCol(D3DXCOLOR col)
{
	// �ϐ��錾
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�J���[�ݒ�
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �|���S���̓����x(effect)
//=============================================================================
void CScene2D::SetEffect(D3DXCOLOR col, float nData)
{
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(col.r, col.g, col.b, nData / 100);
	pVtx[1].col = D3DXCOLOR(col.r, col.g, col.b, nData / 100);
	pVtx[2].col = D3DXCOLOR(col.r, col.g, col.b, nData / 100);
	pVtx[3].col = D3DXCOLOR(col.r, col.g, col.b, nData / 100);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
//�Փ˔���
//=============================================================================
bool CScene2D::Collision(D3DXVECTOR3 pos, D3DXVECTOR3 fsize, OBJTYPE Type, CScene **pColScene)
{
	bool bCol = false;
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		for (int nCntScene = 0; nCntScene < MAX_POLYGON; nCntScene++)
		{
			//scene���擾
			CScene *pScene;
			pScene = CScene::GetScene(nCntPriority, nCntScene);

			if (pScene != NULL)
			{
				//�I�u�W�F�N�g�^�C�v���擾
				OBJTYPE objType;
				objType = pScene->GetObjType();

				if (objType == Type)
				{

					//�ʒu���擾
					D3DXVECTOR3 Epos = pScene->GetPosition();
					//�擾�����傫��������ϐ�
					D3DXVECTOR3 fSize;

					//�T�C�Y���擾
					fSize = pScene->GetSize();

					//�����蔻�菈��
					if (Epos.y + fSize.x >= pos.y - fsize.y &&  Epos.y - fSize.x <= pos.y + fsize.y&&
						pos.x - fsize.x <= Epos.x + fSize.x && pos.x + fsize.x >= Epos.x - fSize.y)
					{
						*pColScene = pScene;

						bCol = true;
					}
				}
			}
		}
	}
	return bCol;
}