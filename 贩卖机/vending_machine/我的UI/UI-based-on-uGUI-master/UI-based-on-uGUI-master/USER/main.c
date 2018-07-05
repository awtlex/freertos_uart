#include "include.h"


/**********************************************************************
** 函数名称         :   
** 创建人           :liupengcheng
** 创建日期         :2017/9/8 12:48
** 最新修改人       :liupengcheng
** 最近修改日期      :2017/9/8 12:48
** 功能描述         :
** 入口参数         :
** 返回参数         :
** 备注/注意        :
** QQ              : 1240612852
***********************************************************************/
void BSP_Init()
{
	LcdInit();
	KeyInit();
	delay_init();
}

/**********************************************************************
** 函数名称         :   
** 创建人           :liupengcheng
** 创建日期         :2017/9/8 12:48
** 最新修改人       :liupengcheng
** 最近修改日期      :2017/9/8 12:48
** 功能描述         :
** 入口参数         :
** 返回参数         :
** 备注/注意        :
** QQ              : 1240612852
***********************************************************************/
void MiddleLayer_Init()
{
	//ugui 初始化
	////////////*******************************************************/
	UG_Init(&GUI_Struct, LCDDrawDot, 132, 64);
	UG_SelectGUI(&GUI_Struct);
	UG_FontSelect(&FONT_8X14);	
	UG_SetForecolor(C_BLACK);
	UG_SetBackcolor(C_WHITE);
	UG_FontSetHSpace(0);
	UG_FontSetVSpace(0);
	
	UG_ConsoleSetArea(0, 0, 131, 63);
	UG_ConsoleSetForecolor(C_BLACK);
	UG_ConsoleSetBackcolor(C_WHITE);

	UPDATE_ScreenRAM();
	/*********************************************************///////////
	//ui.c
	//按键输入和屏幕输出通信邮箱的初始化
//	BoxConfig();
	
}


/**********************************************************************
** 函数名称         : main
** 创建人           :liupengcheng
** 创建日期         :2017/9/8 12:48
** 最新修改人       :liupengcheng
** 最近修改日期      :2017/9/8 12:48
** 功能描述         :
** 入口参数         :
** 返回参数         :
** 备注/注意        :
** QQ              : 1240612852
***********************************************************************/
int main()
{
	
	////////////****************初始化***************************************/
	BSP_Init();
	MiddleLayer_Init();
	/*********************************************************///////////

	while (1)
	{
		KeyScanMain();
		LcdUpdateMain();
		delay_ms(10);
	}

}


