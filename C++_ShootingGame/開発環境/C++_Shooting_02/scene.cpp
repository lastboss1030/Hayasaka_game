//=============================================================================
//
// �I�u�W�F�N�g���� [scene.cpp]
// Author : Taiki Hayasaka
//
//=============================================================================
#include "scene.h"
#include "main.h"
#include "renderer.h"
#include "scene2D.h"

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
CScene *CScene::m_apScene[PRIORITY_MAX][MAX_POLYGON] = {};		// ��ʂɕ\������I�u�W�F�N�g��
int CScene::m_nNumAll = 0;										// �ő吔

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CScene::CScene(CScene::PRIORITY Priority)
{
	for (int nCntScene = 0; nCntScene < MAX_POLYGON; nCntScene++)
	{
		if (m_apScene[Priority][nCntScene] == NULL)
		{
			m_apScene[Priority][nCntScene] = this;
			m_nID = nCntScene;
			m_nNumAll++;
			m_nPriority = Priority;
			break;
		}
	}

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nLife = 100;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CScene::~CScene()
{

}

//=============================================================================
// �I������
//=============================================================================
void CScene::ReleaseAll()
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		for (int nCntScene = 0; nCntScene < MAX_POLYGON; nCntScene++)
		{
			if (m_apScene[nCntPriority][nCntScene] != NULL)
			{
				 // �I������
				if (nCntPriority != PRIORITY_FADE)
				{
					// �t�F�[�h�ȊO�j��
					m_apScene[nCntPriority][nCntScene]->Uninit();
					delete m_apScene[nCntPriority][nCntScene];
					m_apScene[nCntPriority][nCntScene] = NULL;
				}
			}
		}
	}
}

//=============================================================================
// �S�X�V����
//=============================================================================
void CScene::UpdateAll()
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		for (int nCntScene = 0; nCntScene < MAX_POLYGON; nCntScene++)
		{
			if (m_apScene[nCntPriority][nCntScene] != NULL)
			{
				 // �X�V����
				m_apScene[nCntPriority][nCntScene]->Update();
			}
		}
	}
}

//=============================================================================
// �S�`��
//=============================================================================
void CScene::DrawAll()
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		for (int nCntScene = 0; nCntScene < MAX_POLYGON; nCntScene++)
		{
			if (m_apScene[nCntPriority][nCntScene] != NULL)
			{
				 // �`�揈��
				m_apScene[nCntPriority][nCntScene]->Draw();
			}
		}
	}
}

//=============================================================================
// �j��
//=============================================================================
void CScene::Release(void)
{
	if (m_apScene[m_nPriority][m_nID] != NULL)
	{
		// �i�[
		int nID = m_nID;
		int nPriority = m_nPriority;

		//�C���X�^���X�̔j��
		delete m_apScene[nPriority][nID];
		m_apScene[nPriority][nID] = NULL;
		m_nNumAll--;
	}
}

//=============================================================================
// �I�u�W�F�N�g�̎�ނ̐ݒ�
//=============================================================================
void CScene::SetObjType(OBJTYPE objType)
{
	m_objType = objType;
}

//=============================================================================
// �`�揇�̐ݒ�
//=============================================================================
void CScene::SetPriority(CScene::PRIORITY priority)
{
	m_nPriority = priority;
}

//=============================================================================
// �I�u�W�F�N�g�̎�ނ̎擾
//=============================================================================
CScene::OBJTYPE CScene::GetObjType(void)
{
	return m_objType;
}

//=============================================================================
// �I�u�W�F�N�g�̎擾
//=============================================================================
CScene *CScene::GetScene(int nPriority, int nCntScene)
{
	return m_apScene[nPriority][nCntScene];
}

//=============================================================================
// �ʒu��ݒ�
//=============================================================================
void CScene::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//=============================================================================
// �T�C�Y��ݒ�
//=============================================================================
void CScene::SetSize(D3DXVECTOR3 size)
{
	m_size = size;
}

//=============================================================================
// �ʒu���擾
//=============================================================================
D3DXVECTOR3 CScene::GetPos(void)
{
	return m_pos;
}

//=============================================================================
// �T�C�Y���擾
//=============================================================================
D3DXVECTOR3 CScene::GetSize(void)
{
	return m_size;
}

//=============================================================================
//�@�ʒu�Ǘ�
//=============================================================================
D3DXVECTOR3 CScene::GetPosition(void)
{
	return D3DXVECTOR3();
}

//=============================================================================
//�@���C�t�Ǘ�
//=============================================================================
void CScene::SetLife(int nLife)
{
	m_nLife = nLife;
}

//=============================================================================
//�@Get�֐�Life
//=============================================================================
int CScene::GetLife(void)
{
	return m_nLife;
}

//=============================================================================
//�@���C�t�������֐�
//=============================================================================
void CScene::Damage(int nDamage)
{
	m_nLife -= nDamage;
}