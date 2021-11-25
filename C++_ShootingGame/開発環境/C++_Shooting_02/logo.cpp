//=============================================================================
//
// ���S���� [logo.cpp]
// Author : taiki hayasaka
//
//=============================================================================
#include "logo.h"
#include "manager.h"
#include "scene2D.h"
#include "scene.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 CLogo::m_apTextureLogo[MAX_LOGO] = {};
CPlayer *CLogo::m_pPlayer = NULL;

//�O���[�o���錾
int g_nTypeLogo = -1;			// ���ꏈ��������^�C�v������
int g_nTypeMoveLogo = -1;		// ���ꏈ��������^�C�v������
int g_nTypeCollarLogo = -1;		// ���ꏈ��������^�C�v������
D3DXVECTOR3 g_posLogo;			// �ʒu
D3DXCOLOR g_col;				// �F���

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CLogo::CLogo(PRIORITY nPriority) : CScene2D(nPriority)
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CLogo::~CLogo()
{
	g_nTypeLogo = -1;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CLogo::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nType)
{
	// CScene2D�̏���������
	CScene2D::Init(pos, size);

	//�^�C�v����
	m_nType = nType;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CLogo::Uninit(void)
{
	CScene2D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CLogo::Update(void)
{
	//����X�V
	//�^�C�g���ړ�
	if (m_nType == 4)
	{
		// ���݂̈ʒu�̎擾
		D3DXVECTOR3 pos = GetPosition();
		D3DXVECTOR3 size = GetSize();

		//�ړ�������
		SetPosition(pos, size);

		//�X�V
		CScene2D::Update();
	}

	//�v���C���[�����蔻��̈ړ�
	if (m_nType == 9)
	{
		//�F��ς���
		CScene2D::SetCol(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));

		//�v���C���[�̈ʒu���擾
		D3DXVECTOR3 posPlayer = m_pPlayer->GetPos();
		D3DXVECTOR3 sizePlayer = m_pPlayer->GetSize();

		//�ړ�������
		SetPosition(posPlayer, sizePlayer);

		//�X�V
		CScene2D::Update();
	}

	//����X�V
	if (m_nType == 0)
	{
		//�F��ς���
		CScene2D::SetCol(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	}

	//�r���F�ύX
	if (m_nType == g_nTypeCollarLogo)
	{
		//�F��ς���
		CScene2D::SetCol(g_col);
	}

	//�r���ړ�
	if (m_nType == g_nTypeMoveLogo)
	{
		//������
		g_nTypeMoveLogo = -1;

		//�ړ�������
		SetPosition(g_posLogo, D3DXVECTOR3(5.0f,5.0f,0.0f));

		//�X�V
		CScene2D::Update();
	}

	//�r���j��
	if (g_nTypeLogo == m_nType)
	{
		Uninit();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CLogo::Draw(void)
{
	CScene2D::Draw();
}

//=============================================================================
// ��������
//=============================================================================
CLogo * CLogo::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nType)
{
	// ���[�J���ϐ��錾
	CLogo *pLogo = NULL;
	pLogo = new CLogo;

	// ������
	if (pLogo != NULL)
	{
		pLogo->Init(pos, size, nType);
	}

	// �e�N�X�`�����蓖��
	pLogo->BindTexture(m_apTextureLogo[nType]);

	return pLogo;
}

//=============================================================================
// �e�N�X�`���̓ǂݍ���
//=============================================================================
HRESULT CLogo::Load(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̓ǂݍ���	
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/gameover1.jpg", &m_apTextureLogo[PAUSE_RESTART]);		// �|�[�Y���X�^�[�g
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/gameover2.jpg", &m_apTextureLogo[PAUSE_CONTINUE]);		// �|�[�Y�R���e�B�j���[
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/gameover3.jpg", &m_apTextureLogo[PAUSE_TITLE]);		// �|�[�Y�^�C�g��

	return S_OK;
}

//=============================================================================
// �e�N�X�`���̔j��
//=============================================================================
void CLogo::Unload(void)
{
	for (int nCnt = 0; nCnt < MAX_LOGO; nCnt++)
	{
		if (m_apTextureLogo != NULL)
		{
			m_apTextureLogo[nCnt]->Release();
			m_apTextureLogo[nCnt] = NULL;
		}
	}
}

//=============================================================================
// �j���������^�C�v
//=============================================================================
void CLogo::TypeUninit(int nType)
{
	g_nTypeLogo = nType;
}

//=============================================================================
// �ʒu�����炵�����^�C�v
//=============================================================================
void CLogo::TypeMove(D3DXVECTOR3 pos, int nTypeMove)
{
	g_posLogo = pos;
	g_nTypeMoveLogo = nTypeMove;
}

//=============================================================================
// �w�肵���^�C�v�̐F��ς���
//=============================================================================
void CLogo::TypeChangeCollar(D3DXCOLOR col, int nTypeCollar)
{
	g_nTypeCollarLogo = nTypeCollar;
	g_col = col;
}



//=============================================================================
//
// �|�[�Y���S���� [ logo.cpp (pause) ]
// Author : taiki hayasaka
//
//=============================================================================

// �|�[�Y��ʂɕ\�����������ꍇ

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CLogoPause::CLogoPause(PRIORITY nPriority) : CLogo(nPriority)
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CLogoPause::~CLogoPause()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CLogoPause::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nType)
{
	CScene2D::Init(pos, size);

	CLogoPause::m_nType = nType;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CLogoPause::Uninit(void)
{
	CScene2D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CLogoPause::Update(void)
{
	// ����X�V
	// �^�C�g���ړ�
	if (m_nType == 0)
	{
		// ���݂̈ʒu�̎擾
		D3DXVECTOR3 pos = GetPosition();
		D3DXVECTOR3 size = GetSize();

		// �ړ�������
		SetPosition(pos, size);

		// �X�V
		CScene2D::Update();
	}

	// �v���C���[������|�C���^�[����̈ړ�
	if (m_nType == 5)
	{
		// �F��ς���
		CScene2D::SetCol(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));

		// �v���C���[�̈ʒu���擾
		D3DXVECTOR3 posPlayer = m_pPlayer->GetPos();
		D3DXVECTOR3 sizePlayer = m_pPlayer->GetSize();

		// �ړ�������
		SetPosition(posPlayer, sizePlayer);

		// �X�V
		CScene2D::Update();
	}

	if (m_nType == 9)
	{
		// �F��ς���
		CScene2D::SetCol(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	}

	if (m_nType == g_nTypeCollarLogo)
	{
		// �F��ς���
		CScene2D::SetCol(g_col);
	}

	// �r���ړ�
	if (m_nType == g_nTypeMoveLogo)
	{
		// ������
		g_nTypeMoveLogo = -1;

		// �ړ�������
		SetPosition(g_posLogo, D3DXVECTOR3(5.0f,5.0f,0.0f));

		// �X�V
		CScene2D::Update();
	}

	// �r���I��
	if (g_nTypeLogo == m_nType)
	{
		Uninit();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CLogoPause::Draw(void)
{
	CScene2D::Draw();
}

//=============================================================================
// ��������
//=============================================================================
CLogoPause * CLogoPause::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nType)
{
	// ���[�J���ϐ��錾
	CLogoPause *pLogoPause = NULL;
	pLogoPause = new CLogoPause;

	// ������
	if (pLogoPause != NULL)
	{
		pLogoPause->Init(pos, size, nType);
	}

	// �e�N�X�`�����蓖��
	pLogoPause->BindTexture(m_apTextureLogo[nType]);

	return pLogoPause;
}

//=============================================================================
// �e�N�X�`���̔j��
//=============================================================================
void CLogoPause::Unload(void)
{
	for (int nCnt = 0; nCnt < MAX_LOGO; nCnt++)
	{
		if (m_apTextureLogo != NULL)
		{
			m_apTextureLogo[nCnt]->Release();
			m_apTextureLogo[nCnt] = NULL;
		}
	}
}

//=============================================================================
// �j���������^�C�v
//=============================================================================
void CLogoPause::TypeUninit(int nType)
{
	g_nTypeLogo = nType;
}

//=============================================================================
// �ʒu�����炵�����^�C�v
//=============================================================================
void CLogoPause::TypeMove(D3DXVECTOR3 pos, int nTypeMove)
{
	g_posLogo = pos;
	g_nTypeMoveLogo = nTypeMove;
}

//=============================================================================
// �F��ς������^�C�v
//=============================================================================
void CLogoPause::TypeChangeCollar(D3DXCOLOR col, int nTypeCollar)
{
	g_nTypeCollarLogo = nTypeCollar;
	g_col = col;
}
