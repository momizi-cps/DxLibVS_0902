
//########## �w�b�_�[�t�@�C���ǂݍ��� #########
#include "DxLib.h"

//########## �}�N����` ##########
#define GAME_WIDTH			800	//��ʂ̉��̑傫��
#define GAME_HEIGHT			600	//��ʂ̏c�̑傫��
#define GAME_COLOR			32	//��ʂ̃J���[�r�b�g

#define GAME_WINDOW_BAR		0	//�^�C�g���o�[�̓f�t�H���g�ɂ���
#define GAME_WINDOW_NAME	"Speacs Escape"	//�E�B���h�E�̃^�C�g��

#define GAME_FPS			60	//FPS�̐��l	

//�}�E�X�̃{�^��
#define MOUSE_BUTTON_CODE	129		//0x0000�`0x0080�܂�

//�p�X�̒���
#define PATH_MAX			255	//255�����܂�
#define NAME_MAX			255	//255�����܂�



//�G���[���b�Z�[�W
#define IMAGE_LOAD_ERR_TITLE	TEXT("�摜�ǂݍ��݃G���[")

//�摜�̃p�X
#define IMAGE_BACK_PATH			TEXT(".\\IMAGE\\stage.png")	//�w�i�̉摜
#define IMAGE_PLAYER_PATH		TEXT(".\\IMAGE\\player1.png")	//�v���C���[�̉摜

#define IMAGE_TITLE_BK_PATH			TEXT(".\\IMAGE\\title.png")		//�^�C�g���w�i�̉摜
#define IMAGE_TITLE_ROGO_PATH		TEXT(".\\IMAGE\\title_rogo.png")	//�^�C�g�����S�̉摜
#define IMAGE_TITLE_ROGO_ROTA		0.005		//�g�嗦
#define IMAGE_TITLE_ROGO_ROTA_MAX	1.0			//�g�嗦MAX
#define IMAGE_TITLE_ROGO_X_SPEED	1			//X�ړ����x
#define IMAGE_TITLE_START_PATH		TEXT(".\\IMAGE\\title_start.png")	//�^�C�g���X�^�[�g�̉摜
#define IMAGE_TITLE_START_CNT		1			//�_�ŃJ�E���^
#define IMAGE_TITLE_START_CNT_MAX	30			//�_�ŃJ�E���^MAX

#define IMAGE_END_COMP_PATH		TEXT(".\\IMAGE\\game_clear.png")	//�G���h�R���v�摜
#define IMAGE_END_COMP_CNT		1			//�_�ŃJ�E���^
#define IMAGE_END_COMP_CNT_MAX	30			//�_�ŃJ�E���^MAX

#define IMAGE_BACK_NUM			3								//�w�i�̉摜�̐�

//�e�̐ݒ�
#define TAMA_CHANGE_MAX		 5	//5�t���[���ڂŒe�̉摜��ς���
#define TAMA_MAX			50	//�ő�16���܂�
#define TAMA_SPEED			 4
#define TAMA_RENSHA_TIME	 8

#define TAMA_YELLOW_PATH		TEXT(".\\IMAGE\\TAMA\\yellow.png")	//���e�̉摜

#define TAMA_DIV_WIDTH		16	//�摜�𕪊����镝�T�C�Y
#define TAMA_DIV_HEIGHT		16	//�摜�𕪊����鍂���T�C�Y

#define TAMA_DIV_TATE		3	//�摜���c�ɕ������鐔
#define TAMA_DIV_YOKO		1	//�摜�����ɕ������鐔

#define TAMA_DIV_NUM	TAMA_DIV_TATE * TAMA_DIV_YOKO	//�摜�𕪊����鑍��

//�G���[���b�Z�[�W
#define START_ERR_TITLE		TEXT("�X�^�[�g�ʒu�G���[")
#define START_ERR_CAPTION	TEXT("�X�^�[�g�ʒu�����܂��Ă��܂���")

#define GOAL_ERR_TITLE		TEXT("�S�[���ʒu�G���[")
#define GOAL_ERR_CAPTION	TEXT("�S�[���ʒu�����܂��Ă��܂���")

//�I���_�C�A���O�p
#define MOUSE_R_CLICK_TITLE		TEXT("�Q�[�����f")
#define MOUSE_R_CLICK_CAPTION	TEXT("�Q�[���𒆒f���A�^�C�g����ʂɖ߂�܂����H")

#define CHARA_SPEED			 1	//�v���C���[

enum GAME_SCENE {
	GAME_SCENE_START,
	GAME_SCENE_PLAY,
	GAME_SCENE_END,
};	//�Q�[���̃V�[��

enum GAME_END {
	GAME_END_COMP,	//�R���v���[�g
	GAME_END_FAIL	//�t�H�[���g
};	//�Q�[���̏I�����


//int�^��POINT�\����
typedef struct STRUCT_I_POINT
{
	int x = -1;	//���W��������
	int y = -1;	//���W��������
}iPOINT;

//�}�E�X�\����
typedef struct STRUCT_MOUSE
{
	int InputValue = 0;	//GetMouseInput�̒l������
	int WheelValue = 0;	//�}�E�X�z�C�[���̉�]��(���̓v���X�l / ��O�̓}�C�i�X�l)
	iPOINT Point;		//�}�E�X�̍��W������
	iPOINT OldPoint;	//�}�E�X�̍��W(���O)������
	int OldButton[MOUSE_BUTTON_CODE] = { 0 };	//�}�E�X�̃{�^������(���O)������
	int Button[MOUSE_BUTTON_CODE] = { 0 };	//�}�E�X�̃{�^�����͂�����
}MOUSE;



typedef struct STRUCT_IMAGE
{
	char path[PATH_MAX];		//�p�X
	int handle;					//�n���h��
	int x;						//X�ʒu
	int y;						//Y�ʒu
	int width;					//��
	int height;					//����
}IMAGE;	//�摜�\����


typedef struct STRUCT_TAMA
{
	char path[PATH_MAX];				//�p�X
	int handle[TAMA_DIV_NUM];			//���������̒e�̉摜�n���h�����擾
	int x;								//X�ʒu
	int y;								//Y�ʒu
	int width;							//��
	int height;							//����
	BOOL IsDraw;						//�e��\���ł��邩
	int nowImageKind;					//�e�̌��݂̉摜
	int changeImageCnt;					//�摜��ς��邽�߂̃J�E���g
	int changeImageCntMAX;				//�摜��ς��邽�߂̃J�E���g(MAX)
	int speed;							//�X�s�[�h
}TAMA;	//�e�̍\����

typedef struct STRUCT_CHARA
{
	IMAGE image;				//IMAGE�\����
	int speed;					//����

	BOOL CanShot;				//�V���b�g�ł��邩
	int ShotReLoadCnt;			//�V���b�g�����[�h����
	int ShotReLoadCntMAX;		//�V���b�g�����[�h����(MAX)

	TAMA tama[TAMA_MAX];		//�V���b�g����e

	RECT coll;					//�����蔻��

}CHARA;	//�L�����N�^�[�\����

typedef struct STRUCT_IMAGE_BACK
{
	IMAGE image;		//IMAGE�\����
	BOOL IsDraw;		//�e��\���ł��邩
}IMAGE_BACK;	//�w�i�摜�̍\����


typedef struct STRUCT_IMAGE_ROTA
{
	IMAGE image;		//IMAGE�\����
	double angle;		//��]��
	double angleMAX;	//��]��MAX
	double rate;		//�g�嗦
	double rateMAX;		//�g�嗦MAX

}IMAGE_ROTA;	//��]�g�傷��摜�̍\����

typedef struct STRUCT_IMAGE_BLINK
{
	IMAGE image;		//IMAGE�\����
	int Cnt;			//�_�ŃJ�E���^
	int CntMAX;			//�_�ł��鎞��MAX
	BOOL IsDraw;		//�`��ł��邩�H

}IMAGE_BLINK;	//�_�ł���摜�̍\����

//########## �O���[�o���ϐ� ##########
//FPS�֘A
int StartTimeFps;				//����J�n����
int CountFps;					//�J�E���^
float CalcFps;					//�v�Z����
int SampleNumFps = GAME_FPS;	//���ς����T���v����

//�L�[�{�[�h�̓��͂��擾
char AllKeyState[256] = { '\0' };			//���ׂẴL�[�̏�Ԃ�����
char OldAllKeyState[256] = { '\0' };	//���ׂẴL�[�̏��(���O)������

//�}�E�X�̍��W���擾
MOUSE mouse;

int GameScene;		//�Q�[���V�[�����Ǘ�

int GameEndKind;					//�Q�[���̏I�����
RECT GoalRect = { -1,-1, -1, -1 };	//�S�[���̓����蔻��

IMAGE_BACK ImageBack[IMAGE_BACK_NUM];	//�Q�[���̔w�i

IMAGE ImageTitleBK;						//�^�C�g���w�i�̉摜
IMAGE_ROTA ImageTitleROGO;				//�^�C�g�����S�̉摜
IMAGE_BLINK ImageTitleSTART;			//�^�C�g���X�^�[�g�̉摜

IMAGE_BLINK ImageEndCOMP;				//�G���h�R���v�̉摜
IMAGE_BLINK ImageEndFAIL;				//�G���h�t�H�[���̉摜

CHARA player;		//�Q�[���̃L����

//########## �v���g�^�C�v�錾 ##########
VOID MY_FPS_UPDATE(VOID);			//FPS�l���v���A�X�V����
VOID MY_FPS_DRAW(VOID);				//FPS�l��`�悷��
VOID MY_FPS_WAIT(VOID);				//FPS�l���v�����A�҂�

VOID MY_ALL_KEYDOWN_UPDATE(VOID);	//�L�[�̓��͏�Ԃ��X�V����
BOOL MY_KEY_DOWN(int);				//�L�[�������Ă��邩�A�L�[�R�[�h�Ŕ��f����
BOOL MY_KEY_UP(int);				//�L�[�������グ�����A�L�[�R�[�h�Ŕ��f����
BOOL MY_KEYDOWN_KEEP(int, int);		//�L�[�����������Ă��邩�A�L�[�R�[�h�Ŕ��f����

VOID MY_MOUSE_UPDATE(VOID);			//�}�E�X�̓��͏����X�V����
BOOL MY_MOUSE_DOWN(int);			//�{�^���������Ă��邩�A�}�E�X�R�[�h�Ŕ��f����
BOOL MY_MOUSE_UP(int);				//�{�^���������グ�����A�}�E�X�R�[�h�Ŕ��f����
BOOL MY_MOUSEDOWN_KEEP(int, int);	//�{�^�������������Ă��邩�A�}�E�X�R�[�h�Ŕ��f����

VOID MY_START(VOID);		//�X�^�[�g���
VOID MY_START_PROC(VOID);	//�X�^�[�g��ʂ̏���
VOID MY_START_DRAW(VOID);	//�X�^�[�g��ʂ̕`��

VOID MY_PLAY(VOID);			//�v���C���
VOID MY_PLAY_PROC(VOID);	//�v���C��ʂ̏���
VOID MY_PLAY_DRAW(VOID);	//�v���C��ʂ̕`��

VOID MY_END(VOID);			//�G���h���
VOID MY_END_PROC(VOID);		//�G���h��ʂ̏���
VOID MY_END_DRAW(VOID);		//�G���h��ʂ̕`��

BOOL MY_LOAD_IMAGE(VOID);		//�摜���܂Ƃ߂ēǂݍ��ފ֐�
VOID MY_DELETE_IMAGE(VOID);		//�摜���܂Ƃ߂č폜����֐�

BOOL MY_CHECK_RECT_COLL(RECT, RECT);	//�̈�̓����蔻�������֐�

//########## �v���O�����ōŏ��Ɏ��s�����֐� ##########
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ChangeWindowMode(TRUE);								//�E�B���h�E���[�h�ɐݒ�
	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);	//�w��̐��l�ŃE�B���h�E��\������
	SetWindowStyleMode(GAME_WINDOW_BAR);				//�^�C�g���o�[�̓f�t�H���g�ɂ���
	SetMainWindowText(TEXT(GAME_WINDOW_NAME));			//�E�B���h�E�̃^�C�g���̕���
	SetAlwaysRunFlag(TRUE);								//��A�N�e�B�u�ł����s����
	/*SetWindowIconID(IDI_ICON);*/							//�A�C�R���t�@�C����Ǎ�

	if (DxLib_Init() == -1) { return -1; }	//�c�w���C�u��������������

	//�摜��ǂݍ���
	if (MY_LOAD_IMAGE() == FALSE) { return -1; }

	
	//�v���C���[�̐ݒ�
	player.CanShot = TRUE;
	player.ShotReLoadCnt = 0;
	player.ShotReLoadCntMAX = TAMA_RENSHA_TIME;

	GameScene = GAME_SCENE_START;	//�Q�[���V�[���̓X�^�[�g��ʂ���

	SetDrawScreen(DX_SCREEN_BACK);	//Draw�n�֐��͗���ʂɕ`��

	//�������[�v
	while (TRUE)
	{
		if (ProcessMessage() != 0) { break; }	//���b�Z�[�W�����̌��ʂ��G���[�̂Ƃ��A�����I��
		if (ClearDrawScreen() != 0) { break; }	//��ʂ������ł��Ȃ������Ƃ��A�����I��

		MY_ALL_KEYDOWN_UPDATE();				//�����Ă���L�[��Ԃ��擾

		MY_MOUSE_UPDATE();						//�}�E�X�̏�Ԃ��擾

		MY_FPS_UPDATE();	//FPS�̏���[�X�V]

		//�V�[�����Ƃɏ������s��
		switch (GameScene)
		{
		case GAME_SCENE_START:
			MY_START();	//�X�^�[�g���
			break;
		case GAME_SCENE_PLAY:
			MY_PLAY();	//�v���C���
			break;
		case GAME_SCENE_END:
			MY_END();	//�G���h���
			break;
		}

		MY_FPS_DRAW();		//FPS�̏���[�`��]

		ScreenFlip();		//���j�^�̃��t���b�V�����[�g�̑����ŗ���ʂ��ĕ`��

		MY_FPS_WAIT();		//FPS�̏���[�҂�]
	}

	//�摜�n���h����j��
	MY_DELETE_IMAGE();


	DxLib_End();	//�c�w���C�u�����g�p�̏I������

	return 0;
}

//########## FPS�l���v���A�X�V����֐� ##########
VOID MY_FPS_UPDATE(VOID)
{
	if (CountFps == 0) //1�t���[���ڂȂ玞�����L��
	{
		StartTimeFps = GetNowCount();
	}

	if (CountFps == SampleNumFps) //60�t���[���ڂȂ畽�ς��v�Z
	{
		int now = GetNowCount();
		//���݂̎��Ԃ���A0�t���[���ڂ̎��Ԃ������AFPS�̐��l�Ŋ��遁1FPS�ӂ�̕��ώ��Ԃ��v�Z�����
		CalcFps = 1000.f / ((now - StartTimeFps) / (float)SampleNumFps);
		CountFps = 0;
		StartTimeFps = now;
	}
	CountFps++;
	return;
}

//########## FPS�l��`�悷��֐� ##########
VOID MY_FPS_DRAW(VOID)
{
	//�������`��
	DrawFormatString(0, GAME_HEIGHT - 20, GetColor(255, 255, 255), "FPS:%.1f", CalcFps);
	return;
}

//########## FPS�l���v�����A�҂֐� ##########
VOID MY_FPS_WAIT(VOID)
{
	int resultTime = GetNowCount() - StartTimeFps;					//������������
	int waitTime = CountFps * 1000 / GAME_FPS - resultTime;	//�҂ׂ�����

	if (waitTime > 0)		//�w���FPS�l���������ꍇ
	{
		WaitTimer(waitTime);	//�҂�
	}
	return;
}

//########## �L�[�̓��͏�Ԃ��X�V����֐� ##########
VOID MY_ALL_KEYDOWN_UPDATE(VOID)
{
	char TempKey[256];	//�ꎞ�I�ɁA���݂̃L�[�̓��͏�Ԃ��i�[����

	//���O�̃L�[���͂��Ƃ��Ă���
	for (int i = 0; i < 256; i++)
	{
		OldAllKeyState[i] = AllKeyState[i];
	}

	GetHitKeyStateAll(TempKey); // �S�ẴL�[�̓��͏�Ԃ𓾂�

	for (int i = 0; i < 256; i++)
	{
		if (TempKey[i] != 0)	//������Ă���L�[�̃L�[�R�[�h�������Ă���Ƃ�
		{
			AllKeyState[i]++;	//������Ă���
		}
		else
		{
			AllKeyState[i] = 0;	//������Ă��Ȃ�
		}
	}
	return;
}

//�L�[�������Ă��邩�A�L�[�R�[�h�Ŕ��f����
//���@���Fint�F�L�[�R�[�h�FKEY_INPUT_???
BOOL MY_KEY_DOWN(int KEY_INPUT_)
{
	//�L�[�R�[�h�̃L�[�������Ă��鎞
	if (AllKeyState[KEY_INPUT_] != 0)
	{
		return TRUE;	//�L�[�������Ă���
	}
	else
	{
		return FALSE;	//�L�[�������Ă��Ȃ�
	}
}

//�L�[�������グ�����A�L�[�R�[�h�Ŕ��f����
//���@���Fint�F�L�[�R�[�h�FKEY_INPUT_???
BOOL MY_KEY_UP(int KEY_INPUT_)
{
	if (OldAllKeyState[KEY_INPUT_] >= 1	//���O�͉����Ă���
		&& AllKeyState[KEY_INPUT_] == 0)	//���͉����Ă��Ȃ��Ƃ�
	{
		return TRUE;	//�L�[�������グ�Ă���
	}
	else
	{
		return FALSE;	//�L�[�������グ�Ă��Ȃ�
	}
}

//�L�[�����������Ă��邩�A�L�[�R�[�h�Ŕ��f����
//���@���Fint�F�L�[�R�[�h�FKEY_INPUT_???
//���@���Fint�F�L�[�����������鎞��
BOOL MY_KEYDOWN_KEEP(int KEY_INPUT_, int DownTime)
{
	//���������鎞��=�b���~FPS�l
	//��j60FPS�̃Q�[���ŁA1�b�ԉ���������Ȃ�A1�b�~60FPS
	int UpdateTime = DownTime * GAME_FPS;

	if (AllKeyState[KEY_INPUT_] > UpdateTime)
	{
		return TRUE;	//���������Ă���
	}
	else
	{
		return FALSE;	//���������Ă��Ȃ�
	}
}

//�}�E�X�̓��͏����X�V����
VOID MY_MOUSE_UPDATE(VOID)
{
	//�}�E�X�̈ȑO�̍��W���擾
	mouse.OldPoint = mouse.Point;

	//�}�E�X�̈ȑO�̃{�^�����͂��擾
	for (int i = 0; i < MOUSE_BUTTON_CODE; i++) { mouse.OldButton[i] = mouse.Button[i]; }

	//�}�E�X�̍��W���擾
	GetMousePoint(&mouse.Point.x, &mouse.Point.y);

	//�}�E�X�̉����Ă���{�^�����擾
	mouse.InputValue = GetMouseInput();

	//���{�^���������Ă��邩�`�F�b�N(TRUE��0�ȊO�Ȃ̂ŁA2��4��TRUE)
	if ((mouse.InputValue & MOUSE_INPUT_LEFT) == MOUSE_INPUT_LEFT) { mouse.Button[MOUSE_INPUT_LEFT]++; }		//�����Ă���
	if ((mouse.InputValue & MOUSE_INPUT_LEFT) != MOUSE_INPUT_LEFT) { mouse.Button[MOUSE_INPUT_LEFT] = 0; }		//�����Ă��Ȃ�

	//���{�^���������Ă��邩�`�F�b�N(TRUE��0�ȊO�Ȃ̂ŁA2��4��TRUE)
	if ((mouse.InputValue & MOUSE_INPUT_MIDDLE) == MOUSE_INPUT_MIDDLE) { mouse.Button[MOUSE_INPUT_MIDDLE]++; }	//�����Ă���
	if ((mouse.InputValue & MOUSE_INPUT_MIDDLE) != MOUSE_INPUT_MIDDLE) { mouse.Button[MOUSE_INPUT_MIDDLE] = 0; }	//�����Ă��Ȃ�

	//�E�{�^���������Ă��邩�`�F�b�N(TRUE��0�ȊO�Ȃ̂ŁA2��4��TRUE)
	if ((mouse.InputValue & MOUSE_INPUT_RIGHT) == MOUSE_INPUT_RIGHT) { mouse.Button[MOUSE_INPUT_RIGHT]++; }		//�����Ă���
	if ((mouse.InputValue & MOUSE_INPUT_RIGHT) != MOUSE_INPUT_RIGHT) { mouse.Button[MOUSE_INPUT_RIGHT] = 0; }	//�����Ă��Ȃ�

	//�z�C�[���̉�]�ʂ��擾
	mouse.WheelValue = GetMouseWheelRotVol();

	return;
}

//�{�^���������Ă��邩�A�}�E�X�[�R�[�h�Ŕ��f����
//���@���Fint�F�}�E�X�R�[�h�FMOUSE_INPUT_???
BOOL MY_MOUSE_DOWN(int MOUSE_INPUT_)
{
	//�}�E�X�R�[�h�̃{�^���������Ă��鎞
	if (mouse.Button[MOUSE_INPUT_] != 0)
	{
		return TRUE;	//�{�^���������Ă���
	}
	else
	{
		return FALSE;	//�{�^���������Ă��Ȃ�
	}
}

//�{�^���������グ�����A�}�E�X�R�[�h���f����
//���@���Fint�F�}�E�X�R�[�h�FMOUSE_INPUT_???
BOOL MY_MOUSE_UP(int MOUSE_INPUT_)
{
	if (mouse.OldButton[MOUSE_INPUT_] >= 1	//���O�͉����Ă���
		&& mouse.Button[MOUSE_INPUT_] == 0)	//���͉����Ă��Ȃ��Ƃ�
	{
		return TRUE;	//�{�^���������グ�Ă���
	}
	else
	{
		return FALSE;	//�{�^���������グ�Ă��Ȃ�
	}
}

//�{�^�������������Ă��邩�A�}�E�X�R�[�h���f����
//���@���Fint�F�}�E�X�R�[�h�FMOUSE_INPUT_???
//���@���Fint�F�{�^�������������鎞��
BOOL MY_MOUSEDOWN_KEEP(int MOUSE_INPUT_, int DownTime)
{
	//���������鎞��=�b���~FPS�l
	//��j60FPS�̃Q�[���ŁA1�b�ԉ���������Ȃ�A1�b�~60FPS
	int UpdateTime = DownTime * GAME_FPS;

	if (mouse.Button[MOUSE_INPUT_] > UpdateTime)
	{
		return TRUE;	//���������Ă���
	}
	else
	{
		return FALSE;	//���������Ă��Ȃ�
	}
}


//�X�^�[�g���
VOID MY_START(VOID)
{
	MY_START_PROC();	//�X�^�[�g��ʂ̏���
	MY_START_DRAW();	//�X�^�[�g��ʂ̕`��

	return;
}

//�X�^�[�g��ʂ̏���
VOID MY_START_PROC(VOID)
{

	//�G���^�[�L�[����������A�v���C�V�[���ֈړ�����
	if (MY_KEY_DOWN(KEY_INPUT_RETURN) == TRUE)
	{
		//�v���C���[�̉摜�̈ʒu��ݒ肷��
		player.image.x = GAME_WIDTH /2-player.image.width/2;
		player.image.y = GAME_HEIGHT -player.image.height;

		//�e��S�Ĕ�\���ɂ���
		for (int cnt = 0; cnt < TAMA_MAX; cnt++)
		{
			player.tama[cnt].IsDraw = FALSE;
		}

		//�Q�[���̏I����Ԃ�����������
		GameEndKind = GAME_END_FAIL;

		GameScene = GAME_SCENE_PLAY;
	}

	//�^�C�g�����S���g��
	if (ImageTitleROGO.rate < ImageTitleROGO.rateMAX)
	{
		ImageTitleROGO.rate += IMAGE_TITLE_ROGO_ROTA;
	}

	//�^�C�g�����S���ړ�
	if (ImageTitleROGO.image.x < GAME_WIDTH / 2)
	{
		ImageTitleROGO.image.x += IMAGE_TITLE_ROGO_X_SPEED;
	}
	else
	{
		//�^�C�g�����S���ړ�����������

		//�X�^�[�g��_��
		if (ImageTitleSTART.Cnt < ImageTitleSTART.CntMAX)
		{
			ImageTitleSTART.Cnt += IMAGE_TITLE_START_CNT;
		}
		else
		{
			//�`�悷��/���Ȃ������߂�
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

//�X�^�[�g��ʂ̕`��
VOID MY_START_DRAW(VOID)
{
	//�w�i��`��
	DrawGraph(ImageTitleBK.x, ImageTitleBK.y, ImageTitleBK.handle, TRUE);	//�^�C�g���w�i�̕`��

	//�^�C�g�����S����]���Ȃ���`��
	DrawRotaGraph(
		ImageTitleROGO.image.x, ImageTitleROGO.image.y,	//�摜�̍��W
		ImageTitleROGO.rate,							//�摜�̊g�嗦
		ImageTitleROGO.angle,							//�摜�̉�]��
		ImageTitleROGO.image.handle, TRUE);				//�摜�̃n���h��

	//�_��
	if (ImageTitleSTART.IsDraw == TRUE)
	{
		//�^�C�g���X�^�[�g�̕`��
		DrawGraph(ImageTitleSTART.image.x, ImageTitleSTART.image.y, ImageTitleSTART.image.handle, TRUE);
	}

	DrawString(0, 0, "�X�^�[�g���(�G���^�[�L�[�������ĉ�����)", GetColor(255, 255, 255));
	return;
}

//�v���C���
VOID MY_PLAY(VOID)
{
	MY_PLAY_PROC();	//�v���C��ʂ̏���
	MY_PLAY_DRAW();	//�v���C��ʂ̕`��

	return;
}

//�v���C��ʂ̏���
VOID MY_PLAY_PROC(VOID)
{
	//�w�i�摜�𓮂���
	for (int num = 0; num < IMAGE_BACK_NUM; num++)
	{
		//�摜���ړ�������
		ImageBack[num].image.y++;

		//�w�i�摜��`��ł��Ȃ��Ƃ�
		if (ImageBack[num].IsDraw == FALSE)
		{
			//�w�i�摜����ʓ��ɂ���Ƃ�
			if (ImageBack[num].image.y + ImageBack[num].image.height > 0)
			{
				ImageBack[num].IsDraw = TRUE;	//�摜��`�悷��
			}
		}

		//�w�i�摜����ʂ�ʂ�z�����Ƃ�
		if (ImageBack[num].image.y > GAME_HEIGHT)
		{
			ImageBack[num].image.y = 0 - ImageBack[0].image.height * 3;	//�摜�̍����Q���A��Ɉړ�������
			ImageBack[num].IsDraw = FALSE;								//�摜��`�悵�Ȃ�
		}
	}
	
	//��Ɉړ�
	if (MY_KEY_DOWN(KEY_INPUT_UP) == TRUE)
	{
		player.image.y -= player.speed;
	}
	

	//�����蔻�肪�ǂ����Ȃ��̂ŁA�摜�̈ʒu�𓖂��蔻��Ɏg�p����
	//�����蔻��̗̈�́A�摜���������k������B�k�������́A���D�݂ŁB
	//�����蔻��͈̔͂��A�摜�̕��⍂�������傫���ƁA�o�O�邩���I�H
	RECT PlayerRect;
	int CollRange = 5;	//�����蔻��͈̔�
	PlayerRect.left = player.image.x + player.image.width / 2 - CollRange;
	PlayerRect.top = player.image.y + player.image.height / 2 - CollRange;
	PlayerRect.right = player.image.x + player.image.width / 2 + CollRange;
	PlayerRect.bottom = player.image.y + player.image.height / 2 + CollRange;

	//�S�[���ɐG��Ă��邩�`�F�b�N
	if (MY_CHECK_RECT_COLL(PlayerRect, GoalRect) == TRUE)
	{
		GameEndKind = GAME_END_COMP;	//�~�b�V�����R���v���[�g�I

		GameScene = GAME_SCENE_END;

		return;	//�����I�ɃG���h��ʂɔ��
	}

	//�v���C���[����ʊO�ɏo����
	if (player.image.x > GAME_WIDTH || player.image.y > GAME_HEIGHT
		|| player.image.x + player.image.width < 0 || player.image.y + player.image.height < 0)
	{
		GameEndKind = GAME_END_FAIL;	//�~�b�V�����t�H�[���g�I

		GameScene = GAME_SCENE_END;

		return;	//�����I�ɃG���h��ʂɔ��
	}

	
	//�V���b�g�����Ă�Ƃ�
	if (player.CanShot == TRUE)
	{
		//�V���b�g���ˁI�I
		player.CanShot = FALSE;

		//�󂢂Ă���X���b�g�ŁA�e�̕`�������
		for (int cnt = 0; cnt < TAMA_MAX; cnt++)
		{
			if (player.tama[cnt].IsDraw == FALSE)
			{
				//�e��X�ʒu�̓v���C���[�̒��S���甭��
				player.tama[cnt].x = (player.image.x + player.image.width / 2) - player.tama[cnt].width / 2;

				//�e��Y�ʒu�̓v���C���[�̏㕔�����甭��
				player.tama[cnt].y = player.image.y;

				//�e��`�悷��
				player.tama[cnt].IsDraw = TRUE;

				break;
			}
		}
	}

	//�V���b�g�����ĂȂ��Ƃ�
	if (player.CanShot == FALSE)
	{
		//�����[�h���Ԃ��I������Ƃ�
		if (player.ShotReLoadCnt == player.ShotReLoadCntMAX)
		{
			player.ShotReLoadCnt = 0;
			player.CanShot = TRUE;		//�ĂуV���b�g�ł���
		}

		player.ShotReLoadCnt++;	//�����[�h����
	}

	

	return;
}

//�v���C��ʂ̕`��
VOID MY_PLAY_DRAW(VOID)
{
	//�w�i��`�悷��
	for (int num = 0; num < IMAGE_BACK_NUM; num++)
	{
		//�`��ł���Ƃ��́E�E�E
		if (ImageBack[num].IsDraw == TRUE)
		{
			//�w�i��`��
			DrawGraph(ImageBack[num].image.x, ImageBack[num].image.y, ImageBack[num].image.handle, TRUE);

			////�w�i�摜�̐������e�X�g�I�ɕ\��
			//DrawFormatString(
			//	ImageBack[num].image.x,
			//	ImageBack[num].image.y,
			//	GetColor(255, 255, 255), "�w�i�摜�F%d", num + 1);
		}
	}


	//�v���C���[�̂�`�悷��
	DrawGraph(player.image.x, player.image.y, player.image.handle, TRUE);


	//�e�̏��𐶐�
	for (int cnt = 0; cnt < TAMA_MAX; cnt++)
	{
		//�`��ł���e�̏���
		if (player.tama[cnt].IsDraw == TRUE)
		{
			//�e��`�悷��
			DrawGraph(
				player.tama[cnt].x,
				player.tama[cnt].y,
				player.tama[cnt].handle[player.tama[cnt].nowImageKind],	//���݂̉摜�̎�ނɂ������n���h��
				TRUE);

			//�e�̕\���t���[���𑝂₷
			if (player.tama[cnt].changeImageCnt < player.tama[cnt].changeImageCntMAX)
			{
				player.tama[cnt].changeImageCnt++;
			}
			else
			{
				//���ݕ\�����Ă���e�̎�ނ��A�܂�����Ƃ�
				if (player.tama[cnt].nowImageKind < TAMA_DIV_NUM - 1)	//-1���Ȃ��ƁA�Ō�̎�ނ̂Ƃ���++����Ă��܂�
				{
					player.tama[cnt].nowImageKind++;	//�e�����̎�ނɂ���
				}
				else
				{
					player.tama[cnt].nowImageKind = 0;	//�e�̎�ނ����Z�b�g����
				}

				player.tama[cnt].changeImageCnt = 0;
			}

			//�e����Ɉړ�������
			if (player.tama[cnt].y < 0)
			{
				player.tama[cnt].IsDraw = FALSE;	//�`��I��
			}
			else
			{
				player.tama[cnt].y -= player.tama[cnt].speed;
			}
		}
	}

	return;
}

//�G���h���
VOID MY_END(VOID)
{
	MY_END_PROC();	//�G���h��ʂ̏���
	MY_END_DRAW();	//�G���h��ʂ̕`��
	return;
}

//�G���h��ʂ̏���
VOID MY_END_PROC(VOID)
{
	//�G�X�P�[�v�L�[����������A�X�^�[�g�V�[���ֈړ�����
	if (MY_KEY_DOWN(KEY_INPUT_ESCAPE) == TRUE)
	{
		GameScene = GAME_SCENE_START;

		return;
	}

	switch (GameEndKind)
	{
	case GAME_END_COMP:
		//�R���v���[�g�̂Ƃ�

		//�R���v���[�g��_��
		if (ImageEndCOMP.Cnt < ImageEndCOMP.CntMAX)
		{
			ImageEndCOMP.Cnt += IMAGE_END_COMP_CNT;
		}
		else
		{
			//�`�悷��/���Ȃ������߂�
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

//�G���h��ʂ̕`��
VOID MY_END_DRAW(VOID)
{
	MY_PLAY_DRAW();	//�v���C��ʂ�`��

	//�Q�[���̏I����Ԃɂ��A�`���ς���
	switch (GameEndKind)
	{
	case GAME_END_COMP:
		//�R���v���[�g�̂Ƃ�

		//�_��
		if (ImageEndCOMP.IsDraw == TRUE)
		{
			//�R���v���[�g�̕`��
			DrawGraph(ImageEndCOMP.image.x, ImageEndCOMP.image.y, ImageEndCOMP.image.handle, TRUE);
		}
		break;

	case GAME_END_FAIL:
		//�t�H�[���g�̂Ƃ�

		//�_��
		if (ImageEndFAIL.IsDraw == TRUE)
		{
			//�t�H�[���g�̕`��
			DrawGraph(ImageEndFAIL.image.x, ImageEndFAIL.image.y, ImageEndFAIL.image.handle, TRUE);
		}
		break;

	}

	DrawString(0, 0, "�G���h���(�G�X�P�[�v�L�[�������ĉ�����)", GetColor(255, 255, 255));
	return;
}

//�摜���܂Ƃ߂ēǂݍ��ފ֐�
BOOL MY_LOAD_IMAGE(VOID)
{
	//�^�C�g���w�i
	strcpy_s(ImageTitleBK.path, IMAGE_TITLE_BK_PATH);			//�p�X�̐ݒ�
	ImageTitleBK.handle = LoadGraph(ImageTitleBK.path);			//�ǂݍ���
	if (ImageTitleBK.handle == -1)
	{
		//�G���[���b�Z�[�W�\��
		MessageBox(GetMainWindowHandle(), IMAGE_TITLE_BK_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}
	GetGraphSize(ImageTitleBK.handle, &ImageTitleBK.width, &ImageTitleBK.height);	//�摜�̕��ƍ������擾
	ImageTitleBK.x = GAME_WIDTH / 2 - ImageTitleBK.width / 2;		//���E��������
	ImageTitleBK.y = GAME_HEIGHT / 2 - ImageTitleBK.height / 2;		//�㉺��������

	//�^�C�g�����S
	strcpy_s(ImageTitleROGO.image.path, IMAGE_TITLE_ROGO_PATH);					//�p�X�̐ݒ�
	ImageTitleROGO.image.handle = LoadGraph(ImageTitleROGO.image.path);			//�ǂݍ���
	if (ImageTitleROGO.image.handle == -1)
	{
		//�G���[���b�Z�[�W�\��
		MessageBox(GetMainWindowHandle(), IMAGE_TITLE_ROGO_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}
	GetGraphSize(ImageTitleROGO.image.handle, &ImageTitleROGO.image.width, &ImageTitleROGO.image.height);	//�摜�̕��ƍ������擾
	ImageTitleROGO.image.x = 230;							//�����Ă��镔������`�悵����
	ImageTitleROGO.image.y = GAME_HEIGHT / 2;				//������
	ImageTitleROGO.angle = DX_PI * 2;						//��]��
	ImageTitleROGO.angleMAX = DX_PI * 2;					//��]��MAX
	ImageTitleROGO.rate = 0.0;								//�g�嗦
	ImageTitleROGO.rateMAX = IMAGE_TITLE_ROGO_ROTA_MAX;		//�g�嗦MAX

	//�^�C�g���X�^�[�g
	strcpy_s(ImageTitleSTART.image.path, IMAGE_TITLE_START_PATH);					//�p�X�̐ݒ�
	ImageTitleSTART.image.handle = LoadGraph(ImageTitleSTART.image.path);			//�ǂݍ���
	if (ImageTitleSTART.image.handle == -1)
	{
		//�G���[���b�Z�[�W�\��
		MessageBox(GetMainWindowHandle(), IMAGE_TITLE_START_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}
	GetGraphSize(ImageTitleSTART.image.handle, &ImageTitleSTART.image.width, &ImageTitleSTART.image.height);	//�摜�̕��ƍ������擾
	ImageTitleSTART.image.x = GAME_WIDTH / 2 - ImageTitleSTART.image.width / 2;			//���E��������
	ImageTitleSTART.image.y = ImageTitleROGO.image.y + ImageTitleROGO.image.height / 2 + 10;	//���S�̉��ɕ\��
	ImageTitleSTART.Cnt = 0.0;								//�J�E���^
	ImageTitleSTART.CntMAX = IMAGE_TITLE_START_CNT_MAX;		//�J�E���^MAX
	ImageTitleSTART.IsDraw = FALSE;							//�`�悳���Ȃ�

	//�G���h�R���v
	strcpy_s(ImageEndCOMP.image.path, IMAGE_END_COMP_PATH);					//�p�X�̐ݒ�
	ImageEndCOMP.image.handle = LoadGraph(ImageEndCOMP.image.path);			//�ǂݍ���
	if (ImageEndCOMP.image.handle == -1)
	{
		//�G���[���b�Z�[�W�\��
		MessageBox(GetMainWindowHandle(), IMAGE_END_COMP_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}
	GetGraphSize(ImageEndCOMP.image.handle, &ImageEndCOMP.image.width, &ImageEndCOMP.image.height);	//�摜�̕��ƍ������擾
	ImageEndCOMP.image.x = GAME_WIDTH / 2 - ImageEndCOMP.image.width / 2;			//���E��������
	ImageEndCOMP.image.y = GAME_HEIGHT / 2 - ImageEndCOMP.image.height / 2;			//�㉺��������
	ImageEndCOMP.Cnt = 0.0;									//�J�E���^
	ImageEndCOMP.CntMAX = IMAGE_END_COMP_CNT_MAX;			//�J�E���^MAX
	ImageEndCOMP.IsDraw = FALSE;							//�`�悳���Ȃ�

	//�w�i�摜
	strcpy_s(ImageBack[0].image.path, IMAGE_BACK_PATH);			//�p�X�̐ݒ�
	strcpy_s(ImageBack[1].image.path, IMAGE_BACK_PATH);			//�p�X�̐ݒ�
	strcpy_s(ImageBack[2].image.path, IMAGE_BACK_PATH);			//�p�X�̐ݒ�
	
	//�摜��A�����ēǂݍ���
	for (int num = 0; num < IMAGE_BACK_NUM; num++)
	{
		ImageBack[num].image.handle = LoadGraph(ImageBack[num].image.path);	//�ǂݍ���
		if (ImageBack[num].image.handle == -1)
		{
			//�G���[���b�Z�[�W�\��
			MessageBox(GetMainWindowHandle(), IMAGE_BACK_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
			return FALSE;
		}
		//�摜�̕��ƍ������擾
		GetGraphSize(ImageBack[num].image.handle, &ImageBack[num].image.width, &ImageBack[num].image.height);
	}
	//�w�i�摜�@�̐ݒ�
	ImageBack[0].image.x = GAME_WIDTH / 2 - ImageBack[0].image.width / 2;	//���E��������
	ImageBack[0].image.y = 0 - ImageBack[0].image.height * 0;				//y�͌��_����
	ImageBack[0].IsDraw = FALSE;

	//�w�i�摜�A�̐ݒ�
	ImageBack[1].image.x = GAME_WIDTH / 2 - ImageBack[1].image.width / 2;	//���E��������
	ImageBack[1].image.y = 0 - ImageBack[0].image.height * 1;				//�摜�̍����P���A��Ɉړ�������
	ImageBack[1].IsDraw = FALSE;

	//�w�i�摜�B�̐ݒ�
	ImageBack[2].image.x = GAME_WIDTH / 2 - ImageBack[2].image.width / 2;	//���E��������
	ImageBack[2].image.y = 0 - ImageBack[0].image.height * 2;				//�摜�̍����Q���A��Ɉړ�������
	ImageBack[2].IsDraw = FALSE;

	//�v���C���[�̉摜
	strcpy_s(player.image.path, IMAGE_PLAYER_PATH);		//�p�X�̐ݒ�
	player.image.handle = LoadGraph(player.image.path);	//�ǂݍ���
	if (player.image.handle == -1)
	{
		//�G���[���b�Z�[�W�\��
		MessageBox(GetMainWindowHandle(), IMAGE_PLAYER_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}
	GetGraphSize(player.image.handle, &player.image.width, &player.image.height);	//�摜�̕��ƍ������擾
	player.image.x = GAME_WIDTH / 2 - player.image.width / 2;		//���E��������
	player.image.y = GAME_HEIGHT / 2 - player.image.height / 2;		//�㉺��������
	player.speed = CHARA_SPEED;									//�X�s�[�h��ݒ�

	//�e�̉摜�𕪊�����
	int tamaRedRes = LoadDivGraph(
		TAMA_YELLOW_PATH,										//�Ԓe�̃p�X
		TAMA_DIV_NUM, TAMA_DIV_TATE, TAMA_DIV_YOKO,			//�Ԓe�𕪊����鐔
		TAMA_DIV_WIDTH, TAMA_DIV_HEIGHT,					//�摜�𕪊�����̕��ƍ���
		&player.tama[0].handle[0]);							//���������摜������n���h��

	if (tamaRedRes == -1)
	{
		//�G���[���b�Z�[�W�\��
		MessageBox(GetMainWindowHandle(), TAMA_YELLOW_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}

	//���ƍ������擾
	GetGraphSize(player.tama[0].handle[0], &player.tama[0].width, &player.tama[0].height);

	//�e�̏��𐶐�
	for (int cnt = 0; cnt < TAMA_MAX; cnt++)
	{
		//�p�X���R�s�[
		strcpyDx(player.tama[cnt].path, TEXT(TAMA_YELLOW_PATH));

		for (int i_num = 0; i_num < TAMA_DIV_NUM; i_num++)
		{
			//�n���h�����R�s�[
			player.tama[cnt].handle[i_num] = player.tama[0].handle[i_num];
		}

		//�����R�s�[
		player.tama[cnt].width = player.tama[0].width;

		//�������R�s�[
		player.tama[cnt].height = player.tama[0].height;

		//�e��X�ʒu�̓v���C���[�̒��S���甭��
		player.tama[cnt].x = (player.image.x + player.image.width / 2) - player.tama[cnt].width / 2;

		//�e��Y�ʒu�̓v���C���[�̏㕔�����甭��
		player.tama[cnt].y = player.image.y;

		//�e�͍ŏ��͔�\��
		player.tama[cnt].IsDraw = FALSE;

		//�e�̕\���J�E���g��0�ɂ���
		player.tama[cnt].changeImageCnt = 0;

		//�e�̕\���J�E���gMAX��ݒ肷��
		player.tama[cnt].changeImageCntMAX = TAMA_CHANGE_MAX;

		//���݂̉摜�̎�ނ�����������
		player.tama[cnt].nowImageKind = 0;

		//�e�̃X�s�[�h��ݒ肷��
		player.tama[cnt].speed = TAMA_SPEED;
	}
	return TRUE;
}

//�摜���܂Ƃ߂č폜����֐�
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

//�̈�̓����蔻�������֐�
BOOL MY_CHECK_RECT_COLL(RECT a, RECT b)
{
	if (a.left < b.right &&
		a.top < b.bottom &&
		a.right > b.left &&
		a.bottom > b.top
		)
	{
		return TRUE;	//�������Ă���
	}

	return FALSE;		//�������Ă��Ȃ�
}