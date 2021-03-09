
//########## ヘッダーファイル読み込み #########
#include "DxLib.h"

//########## マクロ定義 ##########
#define GAME_WIDTH			800	//画面の横の大きさ
#define GAME_HEIGHT			600	//画面の縦の大きさ
#define GAME_COLOR			32	//画面のカラービット

#define GAME_WINDOW_BAR		0	//タイトルバーはデフォルトにする
#define GAME_WINDOW_NAME	"Speacs Escape"	//ウィンドウのタイトル

#define GAME_FPS			60	//FPSの数値	

//マウスのボタン
#define MOUSE_BUTTON_CODE	129		//0x0000～0x0080まで

//パスの長さ
#define PATH_MAX			255	//255文字まで
#define NAME_MAX			255	//255文字まで



//エラーメッセージ
#define IMAGE_LOAD_ERR_TITLE	TEXT("画像読み込みエラー")

//画像のパス
#define IMAGE_BACK_PATH			TEXT(".\\IMAGE\\stage.png")	//背景の画像
#define IMAGE_PLAYER_PATH		TEXT(".\\IMAGE\\player1.png")	//プレイヤーの画像

#define IMAGE_TITLE_BK_PATH			TEXT(".\\IMAGE\\title.png")		//タイトル背景の画像
#define IMAGE_TITLE_ROGO_PATH		TEXT(".\\IMAGE\\title_rogo.png")	//タイトルロゴの画像
#define IMAGE_TITLE_ROGO_ROTA		0.005		//拡大率
#define IMAGE_TITLE_ROGO_ROTA_MAX	1.0			//拡大率MAX
#define IMAGE_TITLE_ROGO_X_SPEED	1			//X移動速度
#define IMAGE_TITLE_START_PATH		TEXT(".\\IMAGE\\title_start.png")	//タイトルスタートの画像
#define IMAGE_TITLE_START_CNT		1			//点滅カウンタ
#define IMAGE_TITLE_START_CNT_MAX	30			//点滅カウンタMAX

#define IMAGE_END_COMP_PATH		TEXT(".\\IMAGE\\game_clear.png")	//エンドコンプ画像
#define IMAGE_END_COMP_CNT		1			//点滅カウンタ
#define IMAGE_END_COMP_CNT_MAX	30			//点滅カウンタMAX

#define IMAGE_BACK_NUM			3								//背景の画像の数

//弾の設定
#define TAMA_CHANGE_MAX		 5	//5フレーム目で弾の画像を変える
#define TAMA_MAX			50	//最大16発まで
#define TAMA_SPEED			 4
#define TAMA_RENSHA_TIME	 8

#define TAMA_YELLOW_PATH		TEXT(".\\IMAGE\\TAMA\\yellow.png")	//黄弾の画像

#define TAMA_DIV_WIDTH		16	//画像を分割する幅サイズ
#define TAMA_DIV_HEIGHT		16	//画像を分割する高さサイズ

#define TAMA_DIV_TATE		3	//画像を縦に分割する数
#define TAMA_DIV_YOKO		1	//画像を横に分割する数

#define TAMA_DIV_NUM	TAMA_DIV_TATE * TAMA_DIV_YOKO	//画像を分割する総数

//エラーメッセージ
#define START_ERR_TITLE		TEXT("スタート位置エラー")
#define START_ERR_CAPTION	TEXT("スタート位置が決まっていません")

#define GOAL_ERR_TITLE		TEXT("ゴール位置エラー")
#define GOAL_ERR_CAPTION	TEXT("ゴール位置が決まっていません")

//終了ダイアログ用
#define MOUSE_R_CLICK_TITLE		TEXT("ゲーム中断")
#define MOUSE_R_CLICK_CAPTION	TEXT("ゲームを中断し、タイトル画面に戻りますか？")

#define CHARA_SPEED			 1	//プレイヤー

enum GAME_SCENE {
	GAME_SCENE_START,
	GAME_SCENE_PLAY,
	GAME_SCENE_END,
};	//ゲームのシーン

enum GAME_END {
	GAME_END_COMP,	//コンプリート
	GAME_END_FAIL	//フォールト
};	//ゲームの終了状態


//int型のPOINT構造体
typedef struct STRUCT_I_POINT
{
	int x = -1;	//座標を初期化
	int y = -1;	//座標を初期化
}iPOINT;

//マウス構造体
typedef struct STRUCT_MOUSE
{
	int InputValue = 0;	//GetMouseInputの値を入れる
	int WheelValue = 0;	//マウスホイールの回転量(奥はプラス値 / 手前はマイナス値)
	iPOINT Point;		//マウスの座標が入る
	iPOINT OldPoint;	//マウスの座標(直前)が入る
	int OldButton[MOUSE_BUTTON_CODE] = { 0 };	//マウスのボタン入力(直前)が入る
	int Button[MOUSE_BUTTON_CODE] = { 0 };	//マウスのボタン入力が入る
}MOUSE;



typedef struct STRUCT_IMAGE
{
	char path[PATH_MAX];		//パス
	int handle;					//ハンドル
	int x;						//X位置
	int y;						//Y位置
	int width;					//幅
	int height;					//高さ
}IMAGE;	//画像構造体


typedef struct STRUCT_TAMA
{
	char path[PATH_MAX];				//パス
	int handle[TAMA_DIV_NUM];			//分割したの弾の画像ハンドルを取得
	int x;								//X位置
	int y;								//Y位置
	int width;							//幅
	int height;							//高さ
	BOOL IsDraw;						//弾を表示できるか
	int nowImageKind;					//弾の現在の画像
	int changeImageCnt;					//画像を変えるためのカウント
	int changeImageCntMAX;				//画像を変えるためのカウント(MAX)
	int speed;							//スピード
}TAMA;	//弾の構造体

typedef struct STRUCT_CHARA
{
	IMAGE image;				//IMAGE構造体
	int speed;					//速さ

	BOOL CanShot;				//ショットできるか
	int ShotReLoadCnt;			//ショットリロード時間
	int ShotReLoadCntMAX;		//ショットリロード時間(MAX)

	TAMA tama[TAMA_MAX];		//ショットする弾

	RECT coll;					//当たり判定

}CHARA;	//キャラクター構造体

typedef struct STRUCT_IMAGE_BACK
{
	IMAGE image;		//IMAGE構造体
	BOOL IsDraw;		//弾を表示できるか
}IMAGE_BACK;	//背景画像の構造体


typedef struct STRUCT_IMAGE_ROTA
{
	IMAGE image;		//IMAGE構造体
	double angle;		//回転率
	double angleMAX;	//回転率MAX
	double rate;		//拡大率
	double rateMAX;		//拡大率MAX

}IMAGE_ROTA;	//回転拡大する画像の構造体

typedef struct STRUCT_IMAGE_BLINK
{
	IMAGE image;		//IMAGE構造体
	int Cnt;			//点滅カウンタ
	int CntMAX;			//点滅する時間MAX
	BOOL IsDraw;		//描画できるか？

}IMAGE_BLINK;	//点滅する画像の構造体

//########## グローバル変数 ##########
//FPS関連
int StartTimeFps;				//測定開始時刻
int CountFps;					//カウンタ
float CalcFps;					//計算結果
int SampleNumFps = GAME_FPS;	//平均を取るサンプル数

//キーボードの入力を取得
char AllKeyState[256] = { '\0' };			//すべてのキーの状態が入る
char OldAllKeyState[256] = { '\0' };	//すべてのキーの状態(直前)が入る

//マウスの座標を取得
MOUSE mouse;

int GameScene;		//ゲームシーンを管理

int GameEndKind;					//ゲームの終了状態
RECT GoalRect = { -1,-1, -1, -1 };	//ゴールの当たり判定

IMAGE_BACK ImageBack[IMAGE_BACK_NUM];	//ゲームの背景

IMAGE ImageTitleBK;						//タイトル背景の画像
IMAGE_ROTA ImageTitleROGO;				//タイトルロゴの画像
IMAGE_BLINK ImageTitleSTART;			//タイトルスタートの画像

IMAGE_BLINK ImageEndCOMP;				//エンドコンプの画像
IMAGE_BLINK ImageEndFAIL;				//エンドフォールの画像

CHARA player;		//ゲームのキャラ

//########## プロトタイプ宣言 ##########
VOID MY_FPS_UPDATE(VOID);			//FPS値を計測、更新する
VOID MY_FPS_DRAW(VOID);				//FPS値を描画する
VOID MY_FPS_WAIT(VOID);				//FPS値を計測し、待つ

VOID MY_ALL_KEYDOWN_UPDATE(VOID);	//キーの入力状態を更新する
BOOL MY_KEY_DOWN(int);				//キーを押しているか、キーコードで判断する
BOOL MY_KEY_UP(int);				//キーを押し上げたか、キーコードで判断する
BOOL MY_KEYDOWN_KEEP(int, int);		//キーを押し続けているか、キーコードで判断する

VOID MY_MOUSE_UPDATE(VOID);			//マウスの入力情報を更新する
BOOL MY_MOUSE_DOWN(int);			//ボタンを押しているか、マウスコードで判断する
BOOL MY_MOUSE_UP(int);				//ボタンを押し上げたか、マウスコードで判断する
BOOL MY_MOUSEDOWN_KEEP(int, int);	//ボタンを押し続けているか、マウスコードで判断する

VOID MY_START(VOID);		//スタート画面
VOID MY_START_PROC(VOID);	//スタート画面の処理
VOID MY_START_DRAW(VOID);	//スタート画面の描画

VOID MY_PLAY(VOID);			//プレイ画面
VOID MY_PLAY_PROC(VOID);	//プレイ画面の処理
VOID MY_PLAY_DRAW(VOID);	//プレイ画面の描画

VOID MY_END(VOID);			//エンド画面
VOID MY_END_PROC(VOID);		//エンド画面の処理
VOID MY_END_DRAW(VOID);		//エンド画面の描画

BOOL MY_LOAD_IMAGE(VOID);		//画像をまとめて読み込む関数
VOID MY_DELETE_IMAGE(VOID);		//画像をまとめて削除する関数

BOOL MY_CHECK_RECT_COLL(RECT, RECT);	//領域の当たり判定をする関数

//########## プログラムで最初に実行される関数 ##########
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ChangeWindowMode(TRUE);								//ウィンドウモードに設定
	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);	//指定の数値でウィンドウを表示する
	SetWindowStyleMode(GAME_WINDOW_BAR);				//タイトルバーはデフォルトにする
	SetMainWindowText(TEXT(GAME_WINDOW_NAME));			//ウィンドウのタイトルの文字
	SetAlwaysRunFlag(TRUE);								//非アクティブでも実行する
	/*SetWindowIconID(IDI_ICON);*/							//アイコンファイルを読込

	if (DxLib_Init() == -1) { return -1; }	//ＤＸライブラリ初期化処理

	//画像を読み込む
	if (MY_LOAD_IMAGE() == FALSE) { return -1; }

	
	//プレイヤーの設定
	player.CanShot = TRUE;
	player.ShotReLoadCnt = 0;
	player.ShotReLoadCntMAX = TAMA_RENSHA_TIME;

	GameScene = GAME_SCENE_START;	//ゲームシーンはスタート画面から

	SetDrawScreen(DX_SCREEN_BACK);	//Draw系関数は裏画面に描画

	//無限ループ
	while (TRUE)
	{
		if (ProcessMessage() != 0) { break; }	//メッセージ処理の結果がエラーのとき、強制終了
		if (ClearDrawScreen() != 0) { break; }	//画面を消去できなかったとき、強制終了

		MY_ALL_KEYDOWN_UPDATE();				//押しているキー状態を取得

		MY_MOUSE_UPDATE();						//マウスの状態を取得

		MY_FPS_UPDATE();	//FPSの処理[更新]

		//シーンごとに処理を行う
		switch (GameScene)
		{
		case GAME_SCENE_START:
			MY_START();	//スタート画面
			break;
		case GAME_SCENE_PLAY:
			MY_PLAY();	//プレイ画面
			break;
		case GAME_SCENE_END:
			MY_END();	//エンド画面
			break;
		}

		MY_FPS_DRAW();		//FPSの処理[描画]

		ScreenFlip();		//モニタのリフレッシュレートの速さで裏画面を再描画

		MY_FPS_WAIT();		//FPSの処理[待つ]
	}

	//画像ハンドルを破棄
	MY_DELETE_IMAGE();


	DxLib_End();	//ＤＸライブラリ使用の終了処理

	return 0;
}

//########## FPS値を計測、更新する関数 ##########
VOID MY_FPS_UPDATE(VOID)
{
	if (CountFps == 0) //1フレーム目なら時刻を記憶
	{
		StartTimeFps = GetNowCount();
	}

	if (CountFps == SampleNumFps) //60フレーム目なら平均を計算
	{
		int now = GetNowCount();
		//現在の時間から、0フレーム目の時間を引き、FPSの数値で割る＝1FPS辺りの平均時間が計算される
		CalcFps = 1000.f / ((now - StartTimeFps) / (float)SampleNumFps);
		CountFps = 0;
		StartTimeFps = now;
	}
	CountFps++;
	return;
}

//########## FPS値を描画する関数 ##########
VOID MY_FPS_DRAW(VOID)
{
	//文字列を描画
	DrawFormatString(0, GAME_HEIGHT - 20, GetColor(255, 255, 255), "FPS:%.1f", CalcFps);
	return;
}

//########## FPS値を計測し、待つ関数 ##########
VOID MY_FPS_WAIT(VOID)
{
	int resultTime = GetNowCount() - StartTimeFps;					//かかった時間
	int waitTime = CountFps * 1000 / GAME_FPS - resultTime;	//待つべき時間

	if (waitTime > 0)		//指定のFPS値よりも早い場合
	{
		WaitTimer(waitTime);	//待つ
	}
	return;
}

//########## キーの入力状態を更新する関数 ##########
VOID MY_ALL_KEYDOWN_UPDATE(VOID)
{
	char TempKey[256];	//一時的に、現在のキーの入力状態を格納する

	//直前のキー入力をとっておく
	for (int i = 0; i < 256; i++)
	{
		OldAllKeyState[i] = AllKeyState[i];
	}

	GetHitKeyStateAll(TempKey); // 全てのキーの入力状態を得る

	for (int i = 0; i < 256; i++)
	{
		if (TempKey[i] != 0)	//押されているキーのキーコードを押しているとき
		{
			AllKeyState[i]++;	//押されている
		}
		else
		{
			AllKeyState[i] = 0;	//押されていない
		}
	}
	return;
}

//キーを押しているか、キーコードで判断する
//引　数：int：キーコード：KEY_INPUT_???
BOOL MY_KEY_DOWN(int KEY_INPUT_)
{
	//キーコードのキーを押している時
	if (AllKeyState[KEY_INPUT_] != 0)
	{
		return TRUE;	//キーを押している
	}
	else
	{
		return FALSE;	//キーを押していない
	}
}

//キーを押し上げたか、キーコードで判断する
//引　数：int：キーコード：KEY_INPUT_???
BOOL MY_KEY_UP(int KEY_INPUT_)
{
	if (OldAllKeyState[KEY_INPUT_] >= 1	//直前は押していて
		&& AllKeyState[KEY_INPUT_] == 0)	//今は押していないとき
	{
		return TRUE;	//キーを押し上げている
	}
	else
	{
		return FALSE;	//キーを押し上げていない
	}
}

//キーを押し続けているか、キーコードで判断する
//引　数：int：キーコード：KEY_INPUT_???
//引　数：int：キーを押し続ける時間
BOOL MY_KEYDOWN_KEEP(int KEY_INPUT_, int DownTime)
{
	//押し続ける時間=秒数×FPS値
	//例）60FPSのゲームで、1秒間押し続けるなら、1秒×60FPS
	int UpdateTime = DownTime * GAME_FPS;

	if (AllKeyState[KEY_INPUT_] > UpdateTime)
	{
		return TRUE;	//押し続けている
	}
	else
	{
		return FALSE;	//押し続けていない
	}
}

//マウスの入力情報を更新する
VOID MY_MOUSE_UPDATE(VOID)
{
	//マウスの以前の座標を取得
	mouse.OldPoint = mouse.Point;

	//マウスの以前のボタン入力を取得
	for (int i = 0; i < MOUSE_BUTTON_CODE; i++) { mouse.OldButton[i] = mouse.Button[i]; }

	//マウスの座標を取得
	GetMousePoint(&mouse.Point.x, &mouse.Point.y);

	//マウスの押しているボタンを取得
	mouse.InputValue = GetMouseInput();

	//左ボタンを押しているかチェック(TRUEは0以外なので、2も4もTRUE)
	if ((mouse.InputValue & MOUSE_INPUT_LEFT) == MOUSE_INPUT_LEFT) { mouse.Button[MOUSE_INPUT_LEFT]++; }		//押している
	if ((mouse.InputValue & MOUSE_INPUT_LEFT) != MOUSE_INPUT_LEFT) { mouse.Button[MOUSE_INPUT_LEFT] = 0; }		//押していない

	//中ボタンを押しているかチェック(TRUEは0以外なので、2も4もTRUE)
	if ((mouse.InputValue & MOUSE_INPUT_MIDDLE) == MOUSE_INPUT_MIDDLE) { mouse.Button[MOUSE_INPUT_MIDDLE]++; }	//押している
	if ((mouse.InputValue & MOUSE_INPUT_MIDDLE) != MOUSE_INPUT_MIDDLE) { mouse.Button[MOUSE_INPUT_MIDDLE] = 0; }	//押していない

	//右ボタンを押しているかチェック(TRUEは0以外なので、2も4もTRUE)
	if ((mouse.InputValue & MOUSE_INPUT_RIGHT) == MOUSE_INPUT_RIGHT) { mouse.Button[MOUSE_INPUT_RIGHT]++; }		//押している
	if ((mouse.InputValue & MOUSE_INPUT_RIGHT) != MOUSE_INPUT_RIGHT) { mouse.Button[MOUSE_INPUT_RIGHT] = 0; }	//押していない

	//ホイールの回転量を取得
	mouse.WheelValue = GetMouseWheelRotVol();

	return;
}

//ボタンを押しているか、マウスーコードで判断する
//引　数：int：マウスコード：MOUSE_INPUT_???
BOOL MY_MOUSE_DOWN(int MOUSE_INPUT_)
{
	//マウスコードのボタンを押している時
	if (mouse.Button[MOUSE_INPUT_] != 0)
	{
		return TRUE;	//ボタンを押している
	}
	else
	{
		return FALSE;	//ボタンを押していない
	}
}

//ボタンを押し上げたか、マウスコード判断する
//引　数：int：マウスコード：MOUSE_INPUT_???
BOOL MY_MOUSE_UP(int MOUSE_INPUT_)
{
	if (mouse.OldButton[MOUSE_INPUT_] >= 1	//直前は押していて
		&& mouse.Button[MOUSE_INPUT_] == 0)	//今は押していないとき
	{
		return TRUE;	//ボタンを押し上げている
	}
	else
	{
		return FALSE;	//ボタンを押し上げていない
	}
}

//ボタンを押し続けているか、マウスコード判断する
//引　数：int：マウスコード：MOUSE_INPUT_???
//引　数：int：ボタンを押し続ける時間
BOOL MY_MOUSEDOWN_KEEP(int MOUSE_INPUT_, int DownTime)
{
	//押し続ける時間=秒数×FPS値
	//例）60FPSのゲームで、1秒間押し続けるなら、1秒×60FPS
	int UpdateTime = DownTime * GAME_FPS;

	if (mouse.Button[MOUSE_INPUT_] > UpdateTime)
	{
		return TRUE;	//押し続けている
	}
	else
	{
		return FALSE;	//押し続けていない
	}
}


//スタート画面
VOID MY_START(VOID)
{
	MY_START_PROC();	//スタート画面の処理
	MY_START_DRAW();	//スタート画面の描画

	return;
}

//スタート画面の処理
VOID MY_START_PROC(VOID)
{

	//エンターキーを押したら、プレイシーンへ移動する
	if (MY_KEY_DOWN(KEY_INPUT_RETURN) == TRUE)
	{
		//プレイヤーの画像の位置を設定する
		player.image.x = GAME_WIDTH /2-player.image.width/2;
		player.image.y = GAME_HEIGHT -player.image.height;

		//弾を全て非表示にする
		for (int cnt = 0; cnt < TAMA_MAX; cnt++)
		{
			player.tama[cnt].IsDraw = FALSE;
		}

		//ゲームの終了状態を初期化する
		GameEndKind = GAME_END_FAIL;

		GameScene = GAME_SCENE_PLAY;
	}

	//タイトルロゴを拡大
	if (ImageTitleROGO.rate < ImageTitleROGO.rateMAX)
	{
		ImageTitleROGO.rate += IMAGE_TITLE_ROGO_ROTA;
	}

	//タイトルロゴを移動
	if (ImageTitleROGO.image.x < GAME_WIDTH / 2)
	{
		ImageTitleROGO.image.x += IMAGE_TITLE_ROGO_X_SPEED;
	}
	else
	{
		//タイトルロゴが移動しきったら

		//スタートを点滅
		if (ImageTitleSTART.Cnt < ImageTitleSTART.CntMAX)
		{
			ImageTitleSTART.Cnt += IMAGE_TITLE_START_CNT;
		}
		else
		{
			//描画する/しないを決める
			if (ImageTitleSTART.IsDraw == FALSE)
			{
				ImageTitleSTART.IsDraw = TRUE;
			}
			else if (ImageTitleSTART.IsDraw == TRUE)
			{
				ImageTitleSTART.IsDraw = FALSE;
			}
			ImageTitleSTART.Cnt = 0;
		}
	}

	return;
}

//スタート画面の描画
VOID MY_START_DRAW(VOID)
{
	//背景を描画
	DrawGraph(ImageTitleBK.x, ImageTitleBK.y, ImageTitleBK.handle, TRUE);	//タイトル背景の描画

	//タイトルロゴを回転しながら描画
	DrawRotaGraph(
		ImageTitleROGO.image.x, ImageTitleROGO.image.y,	//画像の座標
		ImageTitleROGO.rate,							//画像の拡大率
		ImageTitleROGO.angle,							//画像の回転率
		ImageTitleROGO.image.handle, TRUE);				//画像のハンドル

	//点滅
	if (ImageTitleSTART.IsDraw == TRUE)
	{
		//タイトルスタートの描画
		DrawGraph(ImageTitleSTART.image.x, ImageTitleSTART.image.y, ImageTitleSTART.image.handle, TRUE);
	}

	DrawString(0, 0, "スタート画面(エンターキーを押して下さい)", GetColor(255, 255, 255));
	return;
}

//プレイ画面
VOID MY_PLAY(VOID)
{
	MY_PLAY_PROC();	//プレイ画面の処理
	MY_PLAY_DRAW();	//プレイ画面の描画

	return;
}

//プレイ画面の処理
VOID MY_PLAY_PROC(VOID)
{
	//背景画像を動かす
	for (int num = 0; num < IMAGE_BACK_NUM; num++)
	{
		//画像を移動させる
		ImageBack[num].image.y++;

		//背景画像を描画できないとき
		if (ImageBack[num].IsDraw == FALSE)
		{
			//背景画像が画面内にいるとき
			if (ImageBack[num].image.y + ImageBack[num].image.height > 0)
			{
				ImageBack[num].IsDraw = TRUE;	//画像を描画する
			}
		}

		//背景画像が画面を通り越したとき
		if (ImageBack[num].image.y > GAME_HEIGHT)
		{
			ImageBack[num].image.y = 0 - ImageBack[0].image.height * 3;	//画像の高さ２つ分、上に移動させる
			ImageBack[num].IsDraw = FALSE;								//画像を描画しない
		}
	}
	
	//上に移動
	if (MY_KEY_DOWN(KEY_INPUT_UP) == TRUE)
	{
		player.image.y -= player.speed;
	}
	

	//当たり判定が追いつかないので、画像の位置を当たり判定に使用する
	//当たり判定の領域は、画像よりも少し縮小する。縮小加減は、お好みで。
	//当たり判定の範囲が、画像の幅や高さよりも大きいと、バグるかも！？
	RECT PlayerRect;
	int CollRange = 5;	//当たり判定の範囲
	PlayerRect.left = player.image.x + player.image.width / 2 - CollRange;
	PlayerRect.top = player.image.y + player.image.height / 2 - CollRange;
	PlayerRect.right = player.image.x + player.image.width / 2 + CollRange;
	PlayerRect.bottom = player.image.y + player.image.height / 2 + CollRange;

	//ゴールに触れているかチェック
	if (MY_CHECK_RECT_COLL(PlayerRect, GoalRect) == TRUE)
	{
		GameEndKind = GAME_END_COMP;	//ミッションコンプリート！

		GameScene = GAME_SCENE_END;

		return;	//強制的にエンド画面に飛ぶ
	}

	//プレイヤーが画面外に出たら
	if (player.image.x > GAME_WIDTH || player.image.y > GAME_HEIGHT
		|| player.image.x + player.image.width < 0 || player.image.y + player.image.height < 0)
	{
		GameEndKind = GAME_END_FAIL;	//ミッションフォールト！

		GameScene = GAME_SCENE_END;

		return;	//強制的にエンド画面に飛ぶ
	}

	
	//ショットが撃てるとき
	if (player.CanShot == TRUE)
	{
		//ショット発射！！
		player.CanShot = FALSE;

		//空いているスロットで、弾の描画をする
		for (int cnt = 0; cnt < TAMA_MAX; cnt++)
		{
			if (player.tama[cnt].IsDraw == FALSE)
			{
				//弾のX位置はプレイヤーの中心から発射
				player.tama[cnt].x = (player.image.x + player.image.width / 2) - player.tama[cnt].width / 2;

				//弾のY位置はプレイヤーの上部分から発射
				player.tama[cnt].y = player.image.y;

				//弾を描画する
				player.tama[cnt].IsDraw = TRUE;

				break;
			}
		}
	}

	//ショットが撃てないとき
	if (player.CanShot == FALSE)
	{
		//リロード時間が終わったとき
		if (player.ShotReLoadCnt == player.ShotReLoadCntMAX)
		{
			player.ShotReLoadCnt = 0;
			player.CanShot = TRUE;		//再びショットできる
		}

		player.ShotReLoadCnt++;	//リロードする
	}

	

	return;
}

//プレイ画面の描画
VOID MY_PLAY_DRAW(VOID)
{
	//背景を描画する
	for (int num = 0; num < IMAGE_BACK_NUM; num++)
	{
		//描画できるときは・・・
		if (ImageBack[num].IsDraw == TRUE)
		{
			//背景を描画
			DrawGraph(ImageBack[num].image.x, ImageBack[num].image.y, ImageBack[num].image.handle, TRUE);

			////背景画像の数字をテスト的に表示
			//DrawFormatString(
			//	ImageBack[num].image.x,
			//	ImageBack[num].image.y,
			//	GetColor(255, 255, 255), "背景画像：%d", num + 1);
		}
	}


	//プレイヤーのを描画する
	DrawGraph(player.image.x, player.image.y, player.image.handle, TRUE);


	//弾の情報を生成
	for (int cnt = 0; cnt < TAMA_MAX; cnt++)
	{
		//描画できる弾の処理
		if (player.tama[cnt].IsDraw == TRUE)
		{
			//弾を描画する
			DrawGraph(
				player.tama[cnt].x,
				player.tama[cnt].y,
				player.tama[cnt].handle[player.tama[cnt].nowImageKind],	//現在の画像の種類にあったハンドル
				TRUE);

			//弾の表示フレームを増やす
			if (player.tama[cnt].changeImageCnt < player.tama[cnt].changeImageCntMAX)
			{
				player.tama[cnt].changeImageCnt++;
			}
			else
			{
				//現在表示している弾の種類が、まだあるとき
				if (player.tama[cnt].nowImageKind < TAMA_DIV_NUM - 1)	//-1しないと、最後の種類のときに++されてしまう
				{
					player.tama[cnt].nowImageKind++;	//弾を次の種類にする
				}
				else
				{
					player.tama[cnt].nowImageKind = 0;	//弾の種類をリセットする
				}

				player.tama[cnt].changeImageCnt = 0;
			}

			//弾を上に移動させる
			if (player.tama[cnt].y < 0)
			{
				player.tama[cnt].IsDraw = FALSE;	//描画終了
			}
			else
			{
				player.tama[cnt].y -= player.tama[cnt].speed;
			}
		}
	}

	return;
}

//エンド画面
VOID MY_END(VOID)
{
	MY_END_PROC();	//エンド画面の処理
	MY_END_DRAW();	//エンド画面の描画
	return;
}

//エンド画面の処理
VOID MY_END_PROC(VOID)
{
	//エスケープキーを押したら、スタートシーンへ移動する
	if (MY_KEY_DOWN(KEY_INPUT_ESCAPE) == TRUE)
	{
		GameScene = GAME_SCENE_START;

		return;
	}

	switch (GameEndKind)
	{
	case GAME_END_COMP:
		//コンプリートのとき

		//コンプリートを点滅
		if (ImageEndCOMP.Cnt < ImageEndCOMP.CntMAX)
		{
			ImageEndCOMP.Cnt += IMAGE_END_COMP_CNT;
		}
		else
		{
			//描画する/しないを決める
			if (ImageEndCOMP.IsDraw == FALSE)
			{
				ImageEndCOMP.IsDraw = TRUE;
			}
			else if (ImageEndCOMP.IsDraw == TRUE)
			{
				ImageEndCOMP.IsDraw = FALSE;
			}
			ImageEndCOMP.Cnt = 0;
		}
		break;

	}

	return;
}

//エンド画面の描画
VOID MY_END_DRAW(VOID)
{
	MY_PLAY_DRAW();	//プレイ画面を描画

	//ゲームの終了状態により、描画を変える
	switch (GameEndKind)
	{
	case GAME_END_COMP:
		//コンプリートのとき

		//点滅
		if (ImageEndCOMP.IsDraw == TRUE)
		{
			//コンプリートの描画
			DrawGraph(ImageEndCOMP.image.x, ImageEndCOMP.image.y, ImageEndCOMP.image.handle, TRUE);
		}
		break;

	case GAME_END_FAIL:
		//フォールトのとき

		//点滅
		if (ImageEndFAIL.IsDraw == TRUE)
		{
			//フォールトの描画
			DrawGraph(ImageEndFAIL.image.x, ImageEndFAIL.image.y, ImageEndFAIL.image.handle, TRUE);
		}
		break;

	}

	DrawString(0, 0, "エンド画面(エスケープキーを押して下さい)", GetColor(255, 255, 255));
	return;
}

//画像をまとめて読み込む関数
BOOL MY_LOAD_IMAGE(VOID)
{
	//タイトル背景
	strcpy_s(ImageTitleBK.path, IMAGE_TITLE_BK_PATH);			//パスの設定
	ImageTitleBK.handle = LoadGraph(ImageTitleBK.path);			//読み込み
	if (ImageTitleBK.handle == -1)
	{
		//エラーメッセージ表示
		MessageBox(GetMainWindowHandle(), IMAGE_TITLE_BK_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}
	GetGraphSize(ImageTitleBK.handle, &ImageTitleBK.width, &ImageTitleBK.height);	//画像の幅と高さを取得
	ImageTitleBK.x = GAME_WIDTH / 2 - ImageTitleBK.width / 2;		//左右中央揃え
	ImageTitleBK.y = GAME_HEIGHT / 2 - ImageTitleBK.height / 2;		//上下中央揃え

	//タイトルロゴ
	strcpy_s(ImageTitleROGO.image.path, IMAGE_TITLE_ROGO_PATH);					//パスの設定
	ImageTitleROGO.image.handle = LoadGraph(ImageTitleROGO.image.path);			//読み込み
	if (ImageTitleROGO.image.handle == -1)
	{
		//エラーメッセージ表示
		MessageBox(GetMainWindowHandle(), IMAGE_TITLE_ROGO_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}
	GetGraphSize(ImageTitleROGO.image.handle, &ImageTitleROGO.image.width, &ImageTitleROGO.image.height);	//画像の幅と高さを取得
	ImageTitleROGO.image.x = 230;							//光っている部分から描画したい
	ImageTitleROGO.image.y = GAME_HEIGHT / 2;				//中央寄せ
	ImageTitleROGO.angle = DX_PI * 2;						//回転率
	ImageTitleROGO.angleMAX = DX_PI * 2;					//回転率MAX
	ImageTitleROGO.rate = 0.0;								//拡大率
	ImageTitleROGO.rateMAX = IMAGE_TITLE_ROGO_ROTA_MAX;		//拡大率MAX

	//タイトルスタート
	strcpy_s(ImageTitleSTART.image.path, IMAGE_TITLE_START_PATH);					//パスの設定
	ImageTitleSTART.image.handle = LoadGraph(ImageTitleSTART.image.path);			//読み込み
	if (ImageTitleSTART.image.handle == -1)
	{
		//エラーメッセージ表示
		MessageBox(GetMainWindowHandle(), IMAGE_TITLE_START_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}
	GetGraphSize(ImageTitleSTART.image.handle, &ImageTitleSTART.image.width, &ImageTitleSTART.image.height);	//画像の幅と高さを取得
	ImageTitleSTART.image.x = GAME_WIDTH / 2 - ImageTitleSTART.image.width / 2;			//左右中央揃え
	ImageTitleSTART.image.y = ImageTitleROGO.image.y + ImageTitleROGO.image.height / 2 + 10;	//ロゴの下に表示
	ImageTitleSTART.Cnt = 0.0;								//カウンタ
	ImageTitleSTART.CntMAX = IMAGE_TITLE_START_CNT_MAX;		//カウンタMAX
	ImageTitleSTART.IsDraw = FALSE;							//描画させない

	//エンドコンプ
	strcpy_s(ImageEndCOMP.image.path, IMAGE_END_COMP_PATH);					//パスの設定
	ImageEndCOMP.image.handle = LoadGraph(ImageEndCOMP.image.path);			//読み込み
	if (ImageEndCOMP.image.handle == -1)
	{
		//エラーメッセージ表示
		MessageBox(GetMainWindowHandle(), IMAGE_END_COMP_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}
	GetGraphSize(ImageEndCOMP.image.handle, &ImageEndCOMP.image.width, &ImageEndCOMP.image.height);	//画像の幅と高さを取得
	ImageEndCOMP.image.x = GAME_WIDTH / 2 - ImageEndCOMP.image.width / 2;			//左右中央揃え
	ImageEndCOMP.image.y = GAME_HEIGHT / 2 - ImageEndCOMP.image.height / 2;			//上下中央揃え
	ImageEndCOMP.Cnt = 0.0;									//カウンタ
	ImageEndCOMP.CntMAX = IMAGE_END_COMP_CNT_MAX;			//カウンタMAX
	ImageEndCOMP.IsDraw = FALSE;							//描画させない

	//背景画像
	strcpy_s(ImageBack[0].image.path, IMAGE_BACK_PATH);			//パスの設定
	strcpy_s(ImageBack[1].image.path, IMAGE_BACK_PATH);			//パスの設定
	strcpy_s(ImageBack[2].image.path, IMAGE_BACK_PATH);			//パスの設定
	
	//画像を連続して読み込み
	for (int num = 0; num < IMAGE_BACK_NUM; num++)
	{
		ImageBack[num].image.handle = LoadGraph(ImageBack[num].image.path);	//読み込み
		if (ImageBack[num].image.handle == -1)
		{
			//エラーメッセージ表示
			MessageBox(GetMainWindowHandle(), IMAGE_BACK_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
			return FALSE;
		}
		//画像の幅と高さを取得
		GetGraphSize(ImageBack[num].image.handle, &ImageBack[num].image.width, &ImageBack[num].image.height);
	}
	//背景画像①の設定
	ImageBack[0].image.x = GAME_WIDTH / 2 - ImageBack[0].image.width / 2;	//左右中央揃え
	ImageBack[0].image.y = 0 - ImageBack[0].image.height * 0;				//yは原点から
	ImageBack[0].IsDraw = FALSE;

	//背景画像②の設定
	ImageBack[1].image.x = GAME_WIDTH / 2 - ImageBack[1].image.width / 2;	//左右中央揃え
	ImageBack[1].image.y = 0 - ImageBack[0].image.height * 1;				//画像の高さ１つ分、上に移動させる
	ImageBack[1].IsDraw = FALSE;

	//背景画像③の設定
	ImageBack[2].image.x = GAME_WIDTH / 2 - ImageBack[2].image.width / 2;	//左右中央揃え
	ImageBack[2].image.y = 0 - ImageBack[0].image.height * 2;				//画像の高さ２つ分、上に移動させる
	ImageBack[2].IsDraw = FALSE;

	//プレイヤーの画像
	strcpy_s(player.image.path, IMAGE_PLAYER_PATH);		//パスの設定
	player.image.handle = LoadGraph(player.image.path);	//読み込み
	if (player.image.handle == -1)
	{
		//エラーメッセージ表示
		MessageBox(GetMainWindowHandle(), IMAGE_PLAYER_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}
	GetGraphSize(player.image.handle, &player.image.width, &player.image.height);	//画像の幅と高さを取得
	player.image.x = GAME_WIDTH / 2 - player.image.width / 2;		//左右中央揃え
	player.image.y = GAME_HEIGHT / 2 - player.image.height / 2;		//上下中央揃え
	player.speed = CHARA_SPEED;									//スピードを設定

	//弾の画像を分割する
	int tamaRedRes = LoadDivGraph(
		TAMA_YELLOW_PATH,										//赤弾のパス
		TAMA_DIV_NUM, TAMA_DIV_TATE, TAMA_DIV_YOKO,			//赤弾を分割する数
		TAMA_DIV_WIDTH, TAMA_DIV_HEIGHT,					//画像を分割するの幅と高さ
		&player.tama[0].handle[0]);							//分割した画像が入るハンドル

	if (tamaRedRes == -1)
	{
		//エラーメッセージ表示
		MessageBox(GetMainWindowHandle(), TAMA_YELLOW_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}

	//幅と高さを取得
	GetGraphSize(player.tama[0].handle[0], &player.tama[0].width, &player.tama[0].height);

	//弾の情報を生成
	for (int cnt = 0; cnt < TAMA_MAX; cnt++)
	{
		//パスをコピー
		strcpyDx(player.tama[cnt].path, TEXT(TAMA_YELLOW_PATH));

		for (int i_num = 0; i_num < TAMA_DIV_NUM; i_num++)
		{
			//ハンドルをコピー
			player.tama[cnt].handle[i_num] = player.tama[0].handle[i_num];
		}

		//幅をコピー
		player.tama[cnt].width = player.tama[0].width;

		//高さをコピー
		player.tama[cnt].height = player.tama[0].height;

		//弾のX位置はプレイヤーの中心から発射
		player.tama[cnt].x = (player.image.x + player.image.width / 2) - player.tama[cnt].width / 2;

		//弾のY位置はプレイヤーの上部分から発射
		player.tama[cnt].y = player.image.y;

		//弾は最初は非表示
		player.tama[cnt].IsDraw = FALSE;

		//弾の表示カウントを0にする
		player.tama[cnt].changeImageCnt = 0;

		//弾の表示カウントMAXを設定する
		player.tama[cnt].changeImageCntMAX = TAMA_CHANGE_MAX;

		//現在の画像の種類を初期化する
		player.tama[cnt].nowImageKind = 0;

		//弾のスピードを設定する
		player.tama[cnt].speed = TAMA_SPEED;
	}
	return TRUE;
}

//画像をまとめて削除する関数
VOID MY_DELETE_IMAGE(VOID)
{
	for (int num = 0; num < IMAGE_BACK_NUM; num++)
	{
		DeleteGraph(ImageBack[0].image.handle);
	}

	DeleteGraph(player.image.handle);

	DeleteGraph(ImageTitleBK.handle);
	DeleteGraph(ImageTitleROGO.image.handle);
	DeleteGraph(ImageTitleSTART.image.handle);
	DeleteGraph(ImageEndCOMP.image.handle);
	DeleteGraph(ImageEndFAIL.image.handle);

	for (int i_num = 0; i_num < TAMA_DIV_NUM; i_num++) { DeleteGraph(player.tama[0].handle[i_num]); }

	return;
}

//領域の当たり判定をする関数
BOOL MY_CHECK_RECT_COLL(RECT a, RECT b)
{
	if (a.left < b.right &&
		a.top < b.bottom &&
		a.right > b.left &&
		a.bottom > b.top
		)
	{
		return TRUE;	//当たっている
	}

	return FALSE;		//当たっていない
}