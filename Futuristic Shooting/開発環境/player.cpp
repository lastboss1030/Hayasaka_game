//=============================================================================
//
// �v���C���[�̏��� [player.cpp]
// Author:Taiki Hayasaka
//
//=============================================================================
#include "player.h"									//�C���N���[�h�t�@�C��
#include "bullet.h"
#include "sound.h"
#include "explosion.h"
#include "fade.h"
#include "enemy.h"
#include "effect.h"
#include "Boss.h"
#include "score.h"

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;	//���_���ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTexturePlayer;				//�e�N�X�`���ւ̃|�C���^
//int g_nCounterAnim;								//�A�j���[�V�����J�E���^�[
//int g_nPatternAnim;								//�A�j���[�V�����p�^�[��No
D3DXVECTOR3 g_posPlayer;							//�|���S���̈ʒu
D3DXVECTOR3 g_rotPlayer;							//�|���S���̉�]�p
D3DXVECTOR3 g_movePlayer;							//�|���S���̈ړ���
PLAYER g_pPlayer;									//�v���C���[�̏��
float g_fLengthPlayer;								//�|���S���̑Ίp���̒���
float g_fAnglePlayer;								//�|���S���̑Ίp���̊p�x
int g_nCount;										//�J�E���g
int g_nCntWait;										//�ҋ@����
int g_nCntDeath;									//���S���̑ҋ@

//=============================================================================
// �v���C���[�̏���������
//=============================================================================
HRESULT InitPlayer(void)
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/player001.png", &g_pTexturePlayer);

	//g_nCounterAnim = 0;							//�����j���O�}���p
	//g_nPatternAnim = 0;

	//������
	g_pPlayer.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_pPlayer.state = PLAYERSTATE_APPEAR;
	g_pPlayer.nLife = 0;
	g_pPlayer.bDisp = true;
	g_pPlayer.nCounterState = 0;
	g_pPlayer.nCounterDisp = 0;
	g_pPlayer.nRemaining = 3;

	//������
	g_nCount = 0;
	g_nCntWait = 0;
	g_nCntDeath = 0;

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,	//�m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,		//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffPlayer,
		NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	//�Ίp���̒���
	g_fLengthPlayer = sqrtf(40 * 40 + 45 * 45);	//�傫���ύX�͂���

	//�Ίp���̊p�x
	g_fAnglePlayer = atan2f(40, 45);

	//�����ʒu
	g_pPlayer.pos = D3DXVECTOR3(960, 840, 0.0f);
	g_rotPlayer = D3DXVECTOR3(0, 0, 0.0f);

	//���_���̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);		//�e�N�X�`�����W
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	//���_���W�̐ݒ�
	pVtx[0].pos.x = g_pPlayer.pos.x - sinf(g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[0].pos.y = g_pPlayer.pos.y + cosf(g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_pPlayer.pos.x - sinf(g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[1].pos.y = g_pPlayer.pos.y - cosf(g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_pPlayer.pos.x + sinf(g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[2].pos.y = g_pPlayer.pos.y + cosf(g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_pPlayer.pos.x + sinf(g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[3].pos.y = g_pPlayer.pos.y - cosf(g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[3].pos.z = 0.0f;

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DCOLOR_RGBA(0, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 0, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 0, 255);
	pVtx[3].col = D3DCOLOR_RGBA(0, 255, 0, 255);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPlayer->Unlock();

	return S_OK;
}

//=============================================================================
// �v���C���[�̏I������
//=============================================================================
void UninitPlayer(void)
{
	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffPlayer != NULL)
	{
		g_pVtxBuffPlayer->Release();
		g_pVtxBuffPlayer = NULL;
	}

	//�e�N�X�`���̊J��
	if (g_pTexturePlayer != NULL)
	{
		g_pTexturePlayer->Release();
		g_pTexturePlayer = NULL;
	}
}

//=============================================================================
// �v���C���[�̍X�V����
//=============================================================================
void UpdatePlayer(void)
{
	VERTEX_2D *pVtx;
	ENEMY *pEnemy;
	BOSS *pBoss;
	//g_nCounterAnim++;														//�A�j���[�V�����J�E���^�[�X�V

	//�󂯓n��
	pEnemy = GetEnemy();
	pBoss = GetBoss();

	//if ((g_nCounterAnim % 6) == 0)										//�����j���O�}���p
	//{
	//	//�A�j���[�V�����p�^�[��No���X�V
	//	g_nPatternAnim = (g_nPatternAnim + 1) % 8;
	//}

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	////�e�N�X�`�����W���X�V
	//pVtx[0].tex = D3DXVECTOR2(0.0f+(0.125f*g_nPatternAnim), 1.0f);		//�e�N�X�`�����W
	//pVtx[1].tex = D3DXVECTOR2(0.0f+ (0.125f*g_nPatternAnim), 0.0f);		//�A�j���[�V����(�����j���O�}���p
	//pVtx[2].tex = D3DXVECTOR2(0.125f+ (0.125f*g_nPatternAnim), 1.0f);
	//pVtx[3].tex = D3DXVECTOR2(0.125f+ (0.125f*g_nPatternAnim), 0.0f);

	//���_���W�̍X�V
	pVtx[0].pos.x = g_pPlayer.pos.x - sinf(g_rotPlayer.z + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[0].pos.y = g_pPlayer.pos.y + cosf(g_rotPlayer.z + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_pPlayer.pos.x - sinf(g_fAnglePlayer - g_rotPlayer.z) * g_fLengthPlayer;
	pVtx[1].pos.y = g_pPlayer.pos.y - cosf(g_fAnglePlayer - g_rotPlayer.z) * g_fLengthPlayer;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_pPlayer.pos.x + sinf(g_fAnglePlayer - g_rotPlayer.z) * g_fLengthPlayer;
	pVtx[2].pos.y = g_pPlayer.pos.y + cosf(g_fAnglePlayer - g_rotPlayer.z) * g_fLengthPlayer;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_pPlayer.pos.x + sinf(g_rotPlayer.z + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[3].pos.y = g_pPlayer.pos.y - cosf(g_rotPlayer.z + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[3].pos.z = 0.0f;

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPlayer->Unlock();

	//�|���S���̈ʒu���X�V
	if (GetKeyboardPress(DIK_D) == true)	//D��������
	{
		
		//�E�Ɉړ�
		g_pPlayer.pos.x += 2;
		g_pPlayer.pos.y += 0;
		g_pPlayer.pos.z = 0.0f;

		//����
		g_movePlayer.x += 2;

		//�΂߈ړ�
		if (GetKeyboardPress(DIK_W) == true)
		{
			g_pPlayer.pos.x += sinf(D3DX_PI) *9;
			g_pPlayer.pos.y += cosf(D3DX_PI) *9;
			g_pPlayer.pos.z = 0.0f;
		}
		else if (GetKeyboardPress(DIK_S) == true)
		{
			g_pPlayer.pos.x += sinf(D3DX_PI) * 9;
			g_pPlayer.pos.y -= cosf(D3DX_PI) * 9;
			g_pPlayer.pos.z = 0.0f;
		}
	}
	else if (GetKeyboardPress(DIK_A) == true)	//A��������
	{
		//���Ɉړ�
		g_pPlayer.pos.x -= 2;
		g_pPlayer.pos.y += 0;
		g_pPlayer.pos.z = 0.0f;

		//����
		g_movePlayer.x -= 2;

		//�΂߈ړ�
		if (GetKeyboardPress(DIK_W) == true)
		{
			g_pPlayer.pos.x += sinf(D3DX_PI) *9;
			g_pPlayer.pos.y += cosf(D3DX_PI) *9;
			g_pPlayer.pos.z = 0.0f;
		}
		else if (GetKeyboardPress(DIK_S) == true)
		{
			g_pPlayer.pos.x += sinf(D3DX_PI) *9;
			g_pPlayer.pos.y -= cosf(D3DX_PI) *9;
			g_pPlayer.pos.z = 0.0f;
		}
	}
	else if (GetKeyboardPress(DIK_W) == true)	//W��������
	{
		//��Ɉړ�
		g_pPlayer.pos.x += 0;
		g_pPlayer.pos.y -= 2;
		g_pPlayer.pos.z = 0.0f;

		//����
		g_movePlayer.y -= 2;
	}
	else if (GetKeyboardPress(DIK_S) == true)	//S��������
	{
		//���Ɉړ�
		g_pPlayer.pos.x += 0;
		g_pPlayer.pos.y += 2;
		g_pPlayer.pos.z = 0.0f;

		//����
		g_movePlayer.y += 2;
	}

	//if (GetKeyboardPress(DIK_RIGHT) == true)		//�E��]
	//{
	//	g_rotPlayer.z += 0.1f;
	//	if (g_rotPlayer.z > D3DX_PI)
	//	{
	//		g_rotPlayer.z -= D3DX_PI * 2.0f;
	//	}
	//}
	//else if (GetKeyboardPress(DIK_LEFT) == true)	//����]
	//{
	//	g_rotPlayer.z -= 0.1f;
	//	if (g_rotPlayer.z > D3DX_PI)
	//	{
	//		g_rotPlayer.z -= D3DX_PI * 2.0f;
	//	}
	//}

	//�ʒu�̍X�V
	g_pPlayer.pos.x += g_movePlayer.x;
	g_pPlayer.pos.y += g_movePlayer.y;
	g_pPlayer.pos.z += g_movePlayer.z;

	//�ړ��ʂ̌���
	g_movePlayer.x += (0.0f - g_movePlayer.x) * 0.2f;
	g_movePlayer.y += (0.0f - g_movePlayer.y) * 0.2f;
	g_movePlayer.z += (0.0f - g_movePlayer.z) * 0.2f;

	//�X�e�[�g����
	switch (g_pPlayer.state)
	{
	case PLAYERSTATE_APPEAR:	//�o��
		g_pPlayer.nCounterDisp++;
		g_pPlayer.bDisp = true;
		g_pPlayer.nLife = 3;
		
		if ((g_pPlayer.nCounterDisp % 5) == 0)
		{
			g_pPlayer.bDisp = g_pPlayer.bDisp ? false : true;
		}

		g_nCount++;

		//�_�Ō�
		if (g_nCount > 100)
		{
			g_pPlayer.state = PLAYERSTATE_NORMAL;
		}

		//�e�̔���
		if (GetKeyboardTrigger(DIK_SPACE) == true)
		{
			SetBullet(g_pPlayer.pos, D3DXVECTOR3(0.0f, -20.0f, 0.0f), 520, BULLETTYPE_PLAYER);
			PlaySound(SOUND_LABEL_SE_LASER2);			//���[�U�[���ˉ�
		}

		//�G�t�F�N�g
		SetEffect(g_pPlayer.pos, D3DXCOLOR(0.0f, 1.0f, 1.0f, 0.5f));

		break;

	case PLAYERSTATE_NORMAL:	//�m�[�}��

		//�e�̔���
		if (GetKeyboardTrigger(DIK_SPACE) == true)
		{
			SetBullet(g_pPlayer.pos, D3DXVECTOR3(0.0f, -20.0f, 0.0f), 520, BULLETTYPE_PLAYER);
			PlaySound(SOUND_LABEL_SE_LASER2);			//���[�U�[���ˉ�
		}
		else if (GetKeyboardTrigger(DIK_B) == true)
		{
			SetBullet(g_pPlayer.pos, D3DXVECTOR3(3.0f, -20.0f, 0.0f), 320, BULLETTYPE_PLAYER);
			SetBullet(g_pPlayer.pos, D3DXVECTOR3(-3.0f, -20.0f, 0.0f), 320, BULLETTYPE_PLAYER);
			PlaySound(SOUND_LABEL_SE_LASER2);			//���[�U�[���ˉ�
		}

		//�G�t�F�N�g
		SetEffect(g_pPlayer.pos, D3DXCOLOR(0.0f, 1.0f, 1.0f, 0.5f));

		//�G�Ƃ̓����蔻��
		for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
		{
			if (pEnemy->bUse == true)
			{
				if (pEnemy->pos.y - 20.0f <= g_pPlayer.pos.y + 20 &&
					pEnemy->pos.y + 20.0f >= g_pPlayer.pos.y - 20 &&
					pEnemy->pos.x - 20.0f + pEnemy->move.x <= g_pPlayer.pos.x + 20 &&
					pEnemy->pos.x + 20.0f + pEnemy->move.x >= g_pPlayer.pos.x - 20)
				{
					//�����̃G�t�F�N�g���o��
					SetExplosion(g_pPlayer.pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));

					//����������_���[�W
					HitPlayer(1);
				}
			}
		}

		if (pBoss->bUse == true)
		{
			if (pBoss->pos.y - 700.0f <= g_pPlayer.pos.y + 70 &&
				pBoss->pos.y + 70.0f >= g_pPlayer.pos.y - 70 &&
				pBoss->pos.x - 70.0f + pBoss->move.x <= g_pPlayer.pos.x + 70 &&
				pBoss->pos.x + 70.0f + pBoss->move.x >= g_pPlayer.pos.x - 70)
			{
				//�����̃G�t�F�N�g���o��
				SetExplosion(g_pPlayer.pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));

				//���������瑦��
				HitPlayer(3);
			}
		}
		break;


	case PLAYERSTATE_DAMAGE:	//�_���[�W
		g_pPlayer.nCounterState--;

		if (g_pPlayer.nCounterState <= 0)
		{
			g_pPlayer.state = PLAYERSTATE_NORMAL;

			//�|���S���̐F��߂�
			pVtx[0].col = D3DCOLOR_RGBA(0, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 0, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 0, 255);
			pVtx[3].col = D3DCOLOR_RGBA(0, 255, 0, 255);
		}

		//�G�t�F�N�g
		SetEffect(g_pPlayer.pos, D3DXCOLOR(0.0f, 1.0f, 1.0f, 0.5f));

		break;


	case PLAYERSTATE_WAIT:		//�ҋ@
		g_nCntWait++;
		g_nCount = 0;

		//���X�|�[���ʒu
		g_pPlayer.pos = D3DXVECTOR3(960, 840, 0.0f);

		if (g_nCntWait > 100)
		{
			g_pPlayer.state = PLAYERSTATE_APPEAR;
		}
		break;

	case PLAYERSTATE_DEATH:		//���S
		g_nCntDeath++;
		if (g_nCntDeath == 100)
		{
			SetFade(FADE_OUT, MODE_RESULT);	//���U���g��ʂɐ؂�ւ�
		}
		break;

	}

	// ��ʒ[�̐ݒ�
	if (g_pPlayer.pos.y - 45 < 0)						//��ʂ̏�ɂ�������
	{
		g_pPlayer.pos.y = 45;
	}
	else if (g_pPlayer.pos.y + 45 > SCREEN_HEIGHT)	//��ʂ̉��ɂ�������
	{
		g_pPlayer.pos.y = SCREEN_HEIGHT - 45;
	}

	//if (g_pPlayer.pos.x - 40 <= 0)					//��ʂ̍��ɂ�������
	//{
	//	g_pPlayer.pos.x = 40;
	//}
	//else if (g_pPlayer.pos.x + 40 >= SCREEN_WIDTH)	//��ʂɉE�ɂ�������
	//{
	//	g_pPlayer.pos.x = SCREEN_WIDTH - 40;
	//}


	//��ʋA��
	if (g_pPlayer.pos.x > SCREEN_WIDTH)				//��ʂ̉E�[�𒴂�����
	{
		g_pPlayer.pos.x = 0;
	}
	else if (g_pPlayer.pos.x < 0)						//��ʂ̍��[�𒴂�����
	{
		g_pPlayer.pos.x = SCREEN_WIDTH;
	}

	//if (g_pPlayer.pos.y > SCREEN_HEIGHT)			//��ʂ̉��𒴂�����
	//{
	//	g_pPlayer.pos.y = 0;
	//}
	//else if (g_pPlayer.pos.y < 0)					//��ʂ̏�𒴂�����
	//{
	//	g_pPlayer.pos.y = SCREEN_HEIGHT;			
	//}
}

//=============================================================================
// �v���C���[�̕`�揈��
//=============================================================================
void DrawPlayer(void)
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTexturePlayer);

	//�|���S���̕`��
	if (g_pPlayer.bDisp == true)
	{
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

//=============================================================================
// �v���C���[�̓����蔻��
//=============================================================================
bool HitPlayer(int nDamage)
{
	//�ϐ��錾
	VERTEX_2D *pVtx;

	//�v���C���[�_���[�W�̃}�C�i�X
	g_pPlayer.nLife -= nDamage;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	if (g_pPlayer.nLife <= 0)
	{
		g_pPlayer.nRemaining -= 1;
		g_nCntWait = 0;

		if (g_pPlayer.bDisp == true)
		{
			
			//�������v���C���[�̍��W�ɏo��
			SetExplosion(g_pPlayer.pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f));

			//�������ʉ�(����)
			PlaySound(SOUND_LABEL_SE_EXPLOSION);

			//�q�b�g��
			PlaySound(SOUND_LABEL_SE_HIT);
			
			//�v���C���[���g��������
			g_pPlayer.bDisp = false;

			//�c�@�c���Ă���
			if (g_pPlayer.nRemaining > 0)
			{
				g_pPlayer.state = PLAYERSTATE_WAIT;
			}
			else if (g_pPlayer.nRemaining <= 0)
			{
				g_pPlayer.state = PLAYERSTATE_DEATH;
			}
		}

		return true;
	}
	else
	{
		g_pPlayer.state = PLAYERSTATE_DAMAGE;
		g_pPlayer.nCounterState = 5;

		pVtx[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);

		//�q�b�g��
		PlaySound(SOUND_LABEL_SE_HIT);

		return false;
	}
}

//=============================================================================
// �v���C���[�̎擾
//=============================================================================
PLAYER *GetPlayer(void)
{
	return &g_pPlayer;
}