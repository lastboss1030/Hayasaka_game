//=============================================================================
//
// �p�[�e�B�N���̏��� [particle.cpp]
// Author : Taiki Hayasaka
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "particle.h"
#include "manager.h"
#include "renderer.h"
#include "explosion.h"
#include "Input_Keyboard.h"
#include "enemy.h"
#include "scene.h"
#include "scene2D.h"
#include "effect.h"
#include "game.h"
#include "player.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9 CParticle::m_pTexture = NULL;

// �p�[�e�B�N��
CParticle::PARTICLE CParticle::m_aParticle[MAX_PARTICLE] =
{
	20,										// ������
	8,										// ���x
	20.0f,									// ���a
	70,										// ����
	1.00f,									// ����
	628,									// �͈�
	1.0f,									// �p�x
	D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),			// �F(R,G,B,A)
};

int CParticle::m_nIndexParticle = 0;			// �ۑ��ԍ�

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CParticle::CParticle(PRIORITY nPriority) :CScene2D(nPriority)
{
	// ������
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �p�[�e�B�N���p���l������
	for (int nCnt = 0; nCnt < MAX_PARTICLE; nCnt++)
	{
		m_aParticle[nCnt].nCreateNum = 20;							// ������
		m_aParticle[nCnt].nSpeed = 8;								// ���x
		m_aParticle[nCnt].fRadius = 20.0f;							// ���a
		m_aParticle[nCnt].nLife = 70;								// ����
		m_aParticle[nCnt].fInertia = 1.00f;							// ����
		m_aParticle[nCnt].nRange = 628;								// �͈�
		m_aParticle[nCnt].fAngle = 1.0f;							// �p�x
		m_aParticle[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �F(R,G,B,A)
	}
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CParticle::~CParticle()
{

}

//=============================================================================
// ��������
//=============================================================================
CParticle *CParticle::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 size, D3DXCOLOR col, int nLife, float fInertia)
{
	// �|�C���^
	CParticle *pParticle = NULL;

	if (pParticle == NULL)
	{
		// ���I�m��
		pParticle = new CParticle;

		if (pParticle != NULL)
		{
			pParticle->Init(pos, move, size, col, nLife, fInertia);

			pParticle->BindTexture(m_pTexture);
		}
	}

	return pParticle;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CParticle::Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 size, D3DXCOLOR col, int nLife, float fInertia)
{
	// �����o�ϐ��̏�����
	m_move = move;
	m_size = size;
	m_life = m_aParticle[m_nIndexParticle].nLife;
	m_fInertia = fInertia;
	m_col = col;

	// �p�[�e�B�N���p���l������
	m_aParticle[m_nIndexParticle].nCreateNum = 20;	// ������
	m_aParticle[m_nIndexParticle].nSpeed = 8;		// ���x
	m_aParticle[m_nIndexParticle].fRadius = 20.0f;	// ���a
	m_aParticle[m_nIndexParticle].nLife = 70;		// ����
	m_aParticle[m_nIndexParticle].fInertia = 0.95f;	// ����
	m_aParticle[m_nIndexParticle].nRange = 628;		// �͈�
	m_aParticle[m_nIndexParticle].fAngle = 1.0;		// �p�x
	m_aParticle[m_nIndexParticle].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �F(R,G,B,A)

	// CScene2D�̏���������
	CScene2D::Init(pos, size);

	// �I�u�W�F�N�g�̎�ސݒ�
	SetObjType(CScene::OBJTYPE_PARTICLE);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CParticle::Uninit(void)
{
	// CScene2D�̏I������
	CScene2D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CParticle::Update(void)
{
	// �ϐ��錾
	D3DXVECTOR3 Pos;
	Pos = GetPosition();

	// �J���[�ݒ�
	SetCol(m_aParticle[m_nIndexParticle].col);

	// pos��move�̒l�𖈕b+����
	Pos += m_move;

	// ����
	m_move *= m_fInertia;

	// ���W
	SetPosition(Pos, m_size);

	// �G�t�F�N�g�����񂾂񓧖��ɂ���
	SetEffect(m_col, (float)m_life);

	// ���C�t����
	m_life--;

	// �t�@�C�����[�h
	LoadData();

	//- - - - - - - - - - - - - - - - - - - - - - - - - - -
	// ���C�t��0�ȉ��ɂȂ����Ƃ�
	//- - - - - - - - - - - - - - - - - - - - - - - - - - -
	if (m_life <= 0)
	{
		Uninit();

		return;
	}

	// �p�[�e�B�N������ʊO�ɍs�������������
	if (Pos.x - m_size.x / 2.0f < 0.0f || Pos.x + m_size.x / 2.0f > SCREEN_WIDTH ||
		Pos.y - m_size.y / 2.0f < 0.0f || Pos.y + m_size.y / 2.0f > SCREEN_HEIGHT)
	{
		// �e������
		Uninit();

		return;
	}
}

//=============================================================================
// �`��
//=============================================================================
void CParticle::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	// ���Z����
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	CScene2D::Draw();

	// ���Z�������O��
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=============================================================================
// ���[�h����
//=============================================================================
HRESULT CParticle::Load(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect000.jpg", &m_pTexture);

	return S_OK;
}

//=============================================================================
// �A�����[�h����
//=============================================================================
void CParticle::Unload(void)
{
	// �e�N�X�`���̔j��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
// �p�[�e�B�N���\���̎擾
//=============================================================================
CParticle::PARTICLE CParticle::GetParticle(void)
{
	return m_aParticle[m_nIndexParticle];
}

//=============================================================================
// �p�[�e�B�N���p�擾
// �F���擾
//=============================================================================
D3DXCOLOR CParticle::GetCol(void)
{
	return m_aParticle[m_nIndexParticle].col;
}

//=============================================================================
// �p�[�e�B�N���������擾
//=============================================================================
int CParticle::GetCreateNum(void)
{
	return m_aParticle[m_nIndexParticle].nCreateNum;
}

//=============================================================================
// �p�[�e�B�N�����x�擾
//=============================================================================
int CParticle::GetSpeed(void)
{
	return m_aParticle[m_nIndexParticle].nSpeed;
}

//=============================================================================
// �p�[�e�B�N�����a�擾
//=============================================================================
float CParticle::GetRadius(void)
{
	return m_aParticle[m_nIndexParticle].fRadius;
}

//=============================================================================
// �p�[�e�B�N�������擾
//=============================================================================
int CParticle::GetLife(void)
{
	return m_aParticle[m_nIndexParticle].nLife;
}

//=============================================================================
// �p�[�e�B�N�������擾
//=============================================================================
float CParticle::GetInertia(void)
{
	return m_aParticle[m_nIndexParticle].fInertia;
}

//=============================================================================
// �p�[�e�B�N���͈͎擾
//=============================================================================
int CParticle::GetRange(void)
{
	return m_aParticle[m_nIndexParticle].nRange;
}

//=============================================================================
// �p�[�e�B�N���p�x�擾
//=============================================================================
float CParticle::GetAngle(void)
{
	return m_aParticle[m_nIndexParticle].fAngle;
}

//=============================================================================
// �p�[�e�B�N���ۑ��ԍ��擾
//=============================================================================
int CParticle::GetIndexParticle(void)
{
	return m_nIndexParticle;
}

//=============================================================================
// �t�@�C�����[�h����
//=============================================================================
void CParticle::LoadData(void)
{
	FILE *pFile;

	// �t�@�C���J��
	pFile = fopen(PARTICLE_FILE_NAME, "r");

	if (pFile != NULL)
	{
		// �t�@�C�����J������
		for (int nCnt = 0; nCnt < MAX_PARTICLE; nCnt++)
		{
			fscanf(pFile, "%d\n", &m_aParticle[nCnt].nCreateNum);		// ������
			fscanf(pFile, "%d\n", &m_aParticle[nCnt].nSpeed);			// ���x
			fscanf(pFile, "%f\n", &m_aParticle[nCnt].fRadius);			// ���a
			fscanf(pFile, "%d\n", &m_aParticle[nCnt].nLife);			// ����

			fscanf(pFile, "%f\n", &m_aParticle[nCnt].fInertia);			// ����
			fscanf(pFile, "%d\n", &m_aParticle[nCnt].nRange);			// �͈�
			fscanf(pFile, "%f\n", &m_aParticle[nCnt].fAngle);			// �p�x

			fscanf(pFile, "%f %f %f\n", &m_aParticle[nCnt].col.r, &m_aParticle[nCnt].col.g, &m_aParticle[nCnt].col.b);	// �F(R,G,B,A)
		}
	}

	fclose(pFile);
}
