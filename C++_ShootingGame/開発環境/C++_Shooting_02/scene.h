//=============================================================================
//
// �I�u�W�F�N�g�N���X[scene.h]
// Author : Taiki hayasaka
//
//=============================================================================
#ifndef _SCENE_H_
#define _SCENE_H_

#include "main.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_POLYGON (2048)	

//=============================================================================
// �N���X
//=============================================================================
class CScene
{
public:

	//	�`�揇�̗񋓌^
	typedef enum
	{
		PRIORITY_DEFAULT = 0,		// �f�t�H���g
		PRIORITY_BGBEHING, 			// ��둤�̔w�i
		PRIORITY_BULLET,			// �e
		PRIORITY_ENEMY,				// �G
		PRIORITY_BOSS,				// �{�X
		PRIORITY_EFFECT,			// �G�t�F�N�g
		PRIORITY_PLAYER,			// �v���C���[
		PRIORITY_EXPLOSION,			// ���j
		PRIORITY_BGFRONT,			// ��O���̔w�i	
		PRIORITY_LIFE,				// ���C�t
		PRIORITY_SCORE,				// �X�R�A
		PRIORITY_FADE,				// �t�F�[�h
		PRIORITY_PAUSE,				// �|�[�Y
		PRIORITY_MAX,
	} PRIORITY;

	// �I�u�W�F�N�g�̎��
	typedef enum
	{
		OBJTYPE_NONE = 0,
		OBJTYPE_PLAYER,				// �v���C���[
		OBJTYPE_ENEMY,				// �G
		OBJTYPE_BOSS,				// �{�X
		OBJTYPE_BOSSBREAK,			// �{�X(�j��)
		OBJTYPE_BULET,				// �e
		OBJTYPE_EFFECT,				// �G�t�F�N�g
		OBJTYPE_PAUSE,				// �|�[�Y
		OBJTYPE_MAX,
	}OBJTYPE;

	CScene(PRIORITY nPriority = PRIORITY_DEFAULT);
	virtual ~CScene();

	virtual HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	static void ReleaseAll(void);
	static void UpdateAll(void);
	static void DrawAll(void);

	void SetObjType(OBJTYPE objType);								// �I�u�W�F�N�g�̐ݒ�
	OBJTYPE GetObjType(void);										// �I�u�W�F�N�g�̎��
	static CScene *GetScene(int nPriority, int nCntScene);			// �I�u�W�F�N�g�̎擾

	void SetPos(D3DXVECTOR3 pos);									// �ʒu��ݒ�
	void SetSize(D3DXVECTOR3 size);									// �T�C�Y��ݒ�
	void SetPriority(PRIORITY priority);							// �`�揇��ݒ�

	D3DXVECTOR3 GetPos(void);										// pos���擾
	D3DXVECTOR3 GetSize(void);										// size���擾

	//���C�t�Ǘ�
	void SetLife(int nLife);
	void Damage(int nDamage);
	int GetLife(void);

	// �ʒu�Ǘ�
	virtual D3DXVECTOR3 GetPosition(void);

protected:
	void Release(void);

private:
	static CScene *m_apScene[PRIORITY_MAX][MAX_POLYGON];
	static int m_nNumAll;

	int m_nID;				// �i�[��̔ԍ�
	int m_nPriority;		// �D�揇�ʂ̔ԍ�
	int	m_nLife;			//���C�t
	D3DXVECTOR3 m_pos;		// �ʒu
	D3DXVECTOR3 m_size;		// �T�C�Y
	OBJTYPE m_objType;		// �I�u�W�F�N�g�̎��
};
#endif
