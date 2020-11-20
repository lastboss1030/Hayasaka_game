//=============================================================================
//
// プレイヤーの処理 [player.cpp]
// Author:Taiki Hayasaka
//
//=============================================================================
#include "player.h"									//インクルードファイル
#include "bullet.h"
#include "sound.h"
#include "explosion.h"
#include "fade.h"
#include "enemy.h"
#include "effect.h"
#include "Boss.h"
#include "score.h"

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;	//頂点情報へのポインタ
LPDIRECT3DTEXTURE9 g_pTexturePlayer;				//テクスチャへのポインタ
//int g_nCounterAnim;								//アニメーションカウンター
//int g_nPatternAnim;								//アニメーションパターンNo
D3DXVECTOR3 g_posPlayer;							//ポリゴンの位置
D3DXVECTOR3 g_rotPlayer;							//ポリゴンの回転角
D3DXVECTOR3 g_movePlayer;							//ポリゴンの移動量
PLAYER g_pPlayer;									//プレイヤーの情報
float g_fLengthPlayer;								//ポリゴンの対角線の長さ
float g_fAnglePlayer;								//ポリゴンの対角線の角度
int g_nCount;										//カウント
int g_nCntWait;										//待機時間
int g_nCntDeath;									//死亡時の待機

//=============================================================================
// プレイヤーの初期化処理
//=============================================================================
HRESULT InitPlayer(void)
{
	//変数宣言
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/player001.png", &g_pTexturePlayer);

	//g_nCounterAnim = 0;							//ランニングマン用
	//g_nPatternAnim = 0;

	//初期化
	g_pPlayer.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_pPlayer.state = PLAYERSTATE_APPEAR;
	g_pPlayer.nLife = 0;
	g_pPlayer.bDisp = true;
	g_pPlayer.nCounterState = 0;
	g_pPlayer.nCounterDisp = 0;
	g_pPlayer.nRemaining = 3;

	//初期化
	g_nCount = 0;
	g_nCntWait = 0;
	g_nCntDeath = 0;

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,	//確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,		//頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffPlayer,
		NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	//対角線の長さ
	g_fLengthPlayer = sqrtf(40 * 40 + 45 * 45);	//大きさ変更はここ

	//対角線の角度
	g_fAnglePlayer = atan2f(40, 45);

	//初期位置
	g_pPlayer.pos = D3DXVECTOR3(960, 840, 0.0f);
	g_rotPlayer = D3DXVECTOR3(0, 0, 0.0f);

	//頂点情報の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);		//テクスチャ座標
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	//頂点座標の設定
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

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(0, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 0, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 0, 255);
	pVtx[3].col = D3DCOLOR_RGBA(0, 255, 0, 255);

	//頂点バッファをアンロックする
	g_pVtxBuffPlayer->Unlock();

	return S_OK;
}

//=============================================================================
// プレイヤーの終了処理
//=============================================================================
void UninitPlayer(void)
{
	//頂点バッファの開放
	if (g_pVtxBuffPlayer != NULL)
	{
		g_pVtxBuffPlayer->Release();
		g_pVtxBuffPlayer = NULL;
	}

	//テクスチャの開放
	if (g_pTexturePlayer != NULL)
	{
		g_pTexturePlayer->Release();
		g_pTexturePlayer = NULL;
	}
}

//=============================================================================
// プレイヤーの更新処理
//=============================================================================
void UpdatePlayer(void)
{
	VERTEX_2D *pVtx;
	ENEMY *pEnemy;
	BOSS *pBoss;
	//g_nCounterAnim++;														//アニメーションカウンター更新

	//受け渡し
	pEnemy = GetEnemy();
	pBoss = GetBoss();

	//if ((g_nCounterAnim % 6) == 0)										//ランニングマン用
	//{
	//	//アニメーションパターンNoを更新
	//	g_nPatternAnim = (g_nPatternAnim + 1) % 8;
	//}

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	////テクスチャ座標を更新
	//pVtx[0].tex = D3DXVECTOR2(0.0f+(0.125f*g_nPatternAnim), 1.0f);		//テクスチャ座標
	//pVtx[1].tex = D3DXVECTOR2(0.0f+ (0.125f*g_nPatternAnim), 0.0f);		//アニメーション(ランニングマン用
	//pVtx[2].tex = D3DXVECTOR2(0.125f+ (0.125f*g_nPatternAnim), 1.0f);
	//pVtx[3].tex = D3DXVECTOR2(0.125f+ (0.125f*g_nPatternAnim), 0.0f);

	//頂点座標の更新
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

	//頂点バッファをアンロックする
	g_pVtxBuffPlayer->Unlock();

	//ポリゴンの位置を更新
	if (GetKeyboardPress(DIK_D) == true)	//Dを押した
	{
		
		//右に移動
		g_pPlayer.pos.x += 2;
		g_pPlayer.pos.y += 0;
		g_pPlayer.pos.z = 0.0f;

		//慣性
		g_movePlayer.x += 2;

		//斜め移動
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
	else if (GetKeyboardPress(DIK_A) == true)	//Aを押した
	{
		//左に移動
		g_pPlayer.pos.x -= 2;
		g_pPlayer.pos.y += 0;
		g_pPlayer.pos.z = 0.0f;

		//慣性
		g_movePlayer.x -= 2;

		//斜め移動
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
	else if (GetKeyboardPress(DIK_W) == true)	//Wを押した
	{
		//上に移動
		g_pPlayer.pos.x += 0;
		g_pPlayer.pos.y -= 2;
		g_pPlayer.pos.z = 0.0f;

		//慣性
		g_movePlayer.y -= 2;
	}
	else if (GetKeyboardPress(DIK_S) == true)	//Sを押した
	{
		//下に移動
		g_pPlayer.pos.x += 0;
		g_pPlayer.pos.y += 2;
		g_pPlayer.pos.z = 0.0f;

		//慣性
		g_movePlayer.y += 2;
	}

	//if (GetKeyboardPress(DIK_RIGHT) == true)		//右回転
	//{
	//	g_rotPlayer.z += 0.1f;
	//	if (g_rotPlayer.z > D3DX_PI)
	//	{
	//		g_rotPlayer.z -= D3DX_PI * 2.0f;
	//	}
	//}
	//else if (GetKeyboardPress(DIK_LEFT) == true)	//左回転
	//{
	//	g_rotPlayer.z -= 0.1f;
	//	if (g_rotPlayer.z > D3DX_PI)
	//	{
	//		g_rotPlayer.z -= D3DX_PI * 2.0f;
	//	}
	//}

	//位置の更新
	g_pPlayer.pos.x += g_movePlayer.x;
	g_pPlayer.pos.y += g_movePlayer.y;
	g_pPlayer.pos.z += g_movePlayer.z;

	//移動量の減衰
	g_movePlayer.x += (0.0f - g_movePlayer.x) * 0.2f;
	g_movePlayer.y += (0.0f - g_movePlayer.y) * 0.2f;
	g_movePlayer.z += (0.0f - g_movePlayer.z) * 0.2f;

	//ステート判定
	switch (g_pPlayer.state)
	{
	case PLAYERSTATE_APPEAR:	//出現
		g_pPlayer.nCounterDisp++;
		g_pPlayer.bDisp = true;
		g_pPlayer.nLife = 3;
		
		if ((g_pPlayer.nCounterDisp % 5) == 0)
		{
			g_pPlayer.bDisp = g_pPlayer.bDisp ? false : true;
		}

		g_nCount++;

		//点滅後
		if (g_nCount > 100)
		{
			g_pPlayer.state = PLAYERSTATE_NORMAL;
		}

		//弾の発射
		if (GetKeyboardTrigger(DIK_SPACE) == true)
		{
			SetBullet(g_pPlayer.pos, D3DXVECTOR3(0.0f, -20.0f, 0.0f), 520, BULLETTYPE_PLAYER);
			PlaySound(SOUND_LABEL_SE_LASER2);			//レーザー発射音
		}

		//エフェクト
		SetEffect(g_pPlayer.pos, D3DXCOLOR(0.0f, 1.0f, 1.0f, 0.5f));

		break;

	case PLAYERSTATE_NORMAL:	//ノーマル

		//弾の発射
		if (GetKeyboardTrigger(DIK_SPACE) == true)
		{
			SetBullet(g_pPlayer.pos, D3DXVECTOR3(0.0f, -20.0f, 0.0f), 520, BULLETTYPE_PLAYER);
			PlaySound(SOUND_LABEL_SE_LASER2);			//レーザー発射音
		}
		else if (GetKeyboardTrigger(DIK_B) == true)
		{
			SetBullet(g_pPlayer.pos, D3DXVECTOR3(3.0f, -20.0f, 0.0f), 320, BULLETTYPE_PLAYER);
			SetBullet(g_pPlayer.pos, D3DXVECTOR3(-3.0f, -20.0f, 0.0f), 320, BULLETTYPE_PLAYER);
			PlaySound(SOUND_LABEL_SE_LASER2);			//レーザー発射音
		}

		//エフェクト
		SetEffect(g_pPlayer.pos, D3DXCOLOR(0.0f, 1.0f, 1.0f, 0.5f));

		//敵との当たり判定
		for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
		{
			if (pEnemy->bUse == true)
			{
				if (pEnemy->pos.y - 20.0f <= g_pPlayer.pos.y + 20 &&
					pEnemy->pos.y + 20.0f >= g_pPlayer.pos.y - 20 &&
					pEnemy->pos.x - 20.0f + pEnemy->move.x <= g_pPlayer.pos.x + 20 &&
					pEnemy->pos.x + 20.0f + pEnemy->move.x >= g_pPlayer.pos.x - 20)
				{
					//爆発のエフェクトを出す
					SetExplosion(g_pPlayer.pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));

					//当たったらダメージ
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
				//爆発のエフェクトを出す
				SetExplosion(g_pPlayer.pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));

				//当たったら即死
				HitPlayer(3);
			}
		}
		break;


	case PLAYERSTATE_DAMAGE:	//ダメージ
		g_pPlayer.nCounterState--;

		if (g_pPlayer.nCounterState <= 0)
		{
			g_pPlayer.state = PLAYERSTATE_NORMAL;

			//ポリゴンの色を戻す
			pVtx[0].col = D3DCOLOR_RGBA(0, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 0, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 0, 255);
			pVtx[3].col = D3DCOLOR_RGBA(0, 255, 0, 255);
		}

		//エフェクト
		SetEffect(g_pPlayer.pos, D3DXCOLOR(0.0f, 1.0f, 1.0f, 0.5f));

		break;


	case PLAYERSTATE_WAIT:		//待機
		g_nCntWait++;
		g_nCount = 0;

		//リスポーン位置
		g_pPlayer.pos = D3DXVECTOR3(960, 840, 0.0f);

		if (g_nCntWait > 100)
		{
			g_pPlayer.state = PLAYERSTATE_APPEAR;
		}
		break;

	case PLAYERSTATE_DEATH:		//死亡
		g_nCntDeath++;
		if (g_nCntDeath == 100)
		{
			SetFade(FADE_OUT, MODE_RESULT);	//リザルト画面に切り替え
		}
		break;

	}

	// 画面端の設定
	if (g_pPlayer.pos.y - 45 < 0)						//画面の上にいったら
	{
		g_pPlayer.pos.y = 45;
	}
	else if (g_pPlayer.pos.y + 45 > SCREEN_HEIGHT)	//画面の下にいったら
	{
		g_pPlayer.pos.y = SCREEN_HEIGHT - 45;
	}

	//if (g_pPlayer.pos.x - 40 <= 0)					//画面の左にいったら
	//{
	//	g_pPlayer.pos.x = 40;
	//}
	//else if (g_pPlayer.pos.x + 40 >= SCREEN_WIDTH)	//画面に右にいったら
	//{
	//	g_pPlayer.pos.x = SCREEN_WIDTH - 40;
	//}


	//画面帰宅
	if (g_pPlayer.pos.x > SCREEN_WIDTH)				//画面の右端を超えたら
	{
		g_pPlayer.pos.x = 0;
	}
	else if (g_pPlayer.pos.x < 0)						//画面の左端を超えたら
	{
		g_pPlayer.pos.x = SCREEN_WIDTH;
	}

	//if (g_pPlayer.pos.y > SCREEN_HEIGHT)			//画面の下を超えたら
	//{
	//	g_pPlayer.pos.y = 0;
	//}
	//else if (g_pPlayer.pos.y < 0)					//画面の上を超えたら
	//{
	//	g_pPlayer.pos.y = SCREEN_HEIGHT;			
	//}
}

//=============================================================================
// プレイヤーの描画処理
//=============================================================================
void DrawPlayer(void)
{
	//変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTexturePlayer);

	//ポリゴンの描画
	if (g_pPlayer.bDisp == true)
	{
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

//=============================================================================
// プレイヤーの当たり判定
//=============================================================================
bool HitPlayer(int nDamage)
{
	//変数宣言
	VERTEX_2D *pVtx;

	//プレイヤーダメージのマイナス
	g_pPlayer.nLife -= nDamage;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	if (g_pPlayer.nLife <= 0)
	{
		g_pPlayer.nRemaining -= 1;
		g_nCntWait = 0;

		if (g_pPlayer.bDisp == true)
		{
			
			//爆発をプレイヤーの座標に出す
			SetExplosion(g_pPlayer.pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f));

			//爆発効果音(爆発)
			PlaySound(SOUND_LABEL_SE_EXPLOSION);

			//ヒット音
			PlaySound(SOUND_LABEL_SE_HIT);
			
			//プレイヤーを使った判定
			g_pPlayer.bDisp = false;

			//残機残ってたら
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

		//ヒット音
		PlaySound(SOUND_LABEL_SE_HIT);

		return false;
	}
}

//=============================================================================
// プレイヤーの取得
//=============================================================================
PLAYER *GetPlayer(void)
{
	return &g_pPlayer;
}