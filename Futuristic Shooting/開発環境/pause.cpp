//=============================================================================
//
// ポーズ処理 [pause.cpp]
// Author : Taiki Hayasaka
//
//=============================================================================
#include "pause.h"
#include "input.h"
#include "game.h"
#include "fade.h"
#include "sound.h"

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9 g_pTexturePause[5] = {};		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;	//頂点バッファへのポインタ
bool g_bPause2 = false;							//ポーズ中かどうか
int g_State;									//ステート
D3DXCOLOR PauseCol[5];							//ポーズカラー

//=============================================================================
// ポーズの初期化処理
//=============================================================================
HRESULT InitPause(void)
{
	//変数宣言
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;

	//デバイスの取得
	pDevice = GetDevice();
	
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, NULL, &g_pTexturePause[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/pause100.png", &g_pTexturePause[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/pause000.png", &g_pTexturePause[2]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/pause001.png", &g_pTexturePause[3]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/pause002.png", &g_pTexturePause[4]);

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * 5,	//確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,		//頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffPause,
		NULL)))
	{
		return E_FAIL;
	}

	//初期化
	g_State = PAUSE_MENU_CONTINUE;
	PauseCol[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPause = 0; nCntPause < 5; nCntPause++)
	{
		//頂点情報の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);			//テクスチャ座標
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		//頂点座標の設定
		SetTexturePause(nCntPause);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//４つの頂点を進める
		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffPause->Unlock();

	return S_OK;
}

//=============================================================================
// ポーズの終了処理
//=============================================================================
void UninitPause(void)
{
	//頂点バッファの開放
	if (g_pVtxBuffPause != NULL)
	{
		g_pVtxBuffPause->Release();
		g_pVtxBuffPause = NULL;
	}

	//テクスチャの開放
	for (int nCntTexture = 0; nCntTexture < 4; nCntTexture++)
	{
		if (g_pTexturePause[nCntTexture] != NULL)
		{
			g_pTexturePause[nCntTexture]->Release();
			g_pTexturePause[nCntTexture] = NULL;
		}
	}
}

//=============================================================================
// ポーズの更新処理
//=============================================================================
void UpdatePause(void)
{
	VERTEX_2D *pVtx;

	//セレクト
	if (GetKeyboardTrigger(DIK_DOWN) == true)
	{
		g_State++;
		if (g_State > 2)
		{
			g_State = 0;
		}
		PlaySound(SOUND_LABEL_SE_SELECT);
	}

	if (GetKeyboardTrigger(DIK_UP) == true)
	{
		g_State--;
		if (g_State < 0)
		{
			g_State = 2;
		}
		PlaySound(SOUND_LABEL_SE_SELECT);
	}

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntPause = 0; nCntPause < 5; nCntPause++, pVtx += 4)
	{
		switch (g_State)
		{
		case PAUSE_MENU_CONTINUE:	//コンティニュー選択時

			PauseCol[2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			PauseCol[3] = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			PauseCol[4] = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);

			break;

		case PAUSE_MENU_RETRY:		//リトライ選択時
			PauseCol[2] = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			PauseCol[3] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			PauseCol[4] = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);

			break;

		case PAUSE_MENU_QUIT:		//終了選択時
			PauseCol[2] = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			PauseCol[3] = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			PauseCol[4] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			break;
		}
		pVtx[0].col = PauseCol[nCntPause];
		pVtx[1].col = PauseCol[nCntPause];
		pVtx[2].col = PauseCol[nCntPause];
		pVtx[3].col = PauseCol[nCntPause];
	}

	//エンター押したら
	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{
		if (g_State == PAUSE_MENU_RETRY)	//リトライ選択時
		{
			//ゲーム画面へ
			SetFade(FADE_OUT, MODE_GAME);

			//決定音
			PlaySound(SOUND_LABEL_SE_DECISION);
		}
		else if (g_State == PAUSE_MENU_QUIT)	//終了選択時
		{
			//タイトル画面へ
			SetFade(FADE_OUT, MODE_TITLE);

			//決定音
			PlaySound(SOUND_LABEL_SE_DECISION);
		}

		SetPause(false);
	}

	//頂点バッファをアンロックする
	g_pVtxBuffPause->Unlock();
}
//=============================================================================
// ポーズの描画処理
//=============================================================================
void DrawPause(void)
{
	//変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPause, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//ポリゴンの描画
	for (int nCntTexture = 0; nCntTexture < 5; nCntTexture++)
	{
		pDevice->SetTexture(0, g_pTexturePause[nCntTexture]);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTexture * 4, 2);
	}
}

//=============================================================================
// ポーズのテクスチャの設定
//=============================================================================
void SetTexturePause(int nCntPause)
{
	VERTEX_2D *pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += nCntPause * 4;

	if (nCntPause == 0)			//黒背景
	{
		//頂点座標
		pVtx[0].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);	//Zは0.0固定
		pVtx[1].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);

		//カラー
		PauseCol[nCntPause] = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
	
	}
	 else if (nCntPause == 1)	//ポーズメニュ
	{
		 //頂点座標
		pVtx[0].pos = D3DXVECTOR3(720, 850, 0.0f);	//Zは0.0固定
		pVtx[1].pos = D3DXVECTOR3(720, 250, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(1200, 850, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(1200, 250, 0.0f);

		//カラー
		PauseCol[nCntPause] = D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.6f);
	}
	else if (nCntPause == 2)	//コンティニュー
	{
		//頂点座標
		pVtx[0].pos = D3DXVECTOR3(720, 400, 0.0f);	//Zは0.0固定
		pVtx[1].pos = D3DXVECTOR3(720, 300, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(1200, 400, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(1200, 300, 0.0f);

		//カラー
		PauseCol[nCntPause] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}
	else if (nCntPause == 3)	//リトライ
	{
		//頂点座標
		pVtx[0].pos = D3DXVECTOR3(720, 600, 0.0f);	//Zは0.0固定
		pVtx[1].pos = D3DXVECTOR3(720, 500, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(1200, 600, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(1200, 500, 0.0f);

		//カラー
		PauseCol[nCntPause] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}
	else if (nCntPause == 4)	//終了
	{
		//頂点座標
		pVtx[0].pos = D3DXVECTOR3(720, 800, 0.0f);	//Zは0.0固定
		pVtx[1].pos = D3DXVECTOR3(720, 700, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(1200, 800, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(1200, 700, 0.0f);

		//カラー
		PauseCol[nCntPause] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	pVtx[0].col = PauseCol[nCntPause];
	pVtx[1].col = PauseCol[nCntPause];
	pVtx[2].col = PauseCol[nCntPause];
	pVtx[3].col = PauseCol[nCntPause];


	//頂点バッファをアンロックする
	g_pVtxBuffPause->Unlock();
}