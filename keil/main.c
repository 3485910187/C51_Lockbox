#include"reg51.h"
#include"12864.h"
#include"keyboard.h"
#include"18b20.h"

uchar display_password[6]={10,10,10,10,10,10};
uchar password[]={1,2,3,4,5,6};

char* prt_password=display_password;
uchar mode = 0,error = 0;
uchar Temperature = 0;

void scan_key()
{
	uchar keydata = 0;
	uint i = 0,flag = 0;
	if(get_key() != 0xff)
	{
		keydata = get_key();
		while(get_key() != 0xff);
	}
	if(keydata > '0' && keydata < '9' && display_password[5] == 10) //����
	{
		*prt_password++=keydata-'0';
	}
	else if(keydata == '*') //ȷ��
	{
		for(i=0;i<6;i++)
		{
			if(display_password[i] != password[i])flag = 1;
		}
		if(flag == 1)
		{
			error++; //�������
		}
		else 
		{
			mode = 1; //������ȷ
			error = 0;
			ClearScreen(0);
		}
		if(error == 3)
		{
			mode = 2; //����
			ClearScreen(0);
		}
		for(i=0;i<6;i++)
		{
			display_password[i] = 10;
		}
		prt_password=display_password;
	}
	else if(keydata == '#') //ȡ��
	{
		for(i=0;i<6;i++)
		{
			display_password[i] = 10;
		}
		prt_password=display_password;
	}
	else if(keydata == 'A' && error < 3) //�ر�������
	{
		mode = 0;
		error = 0;
		ClearScreen(0);
	}
	else if(keydata == 'B')
	{
		mode = 0;
		error = 0;
		ClearScreen(0);		
	}
}


void main()
{
	InitLCD();
	while(1)
	{
		//��һ��
		Display_ZM(1,0,8*0,19);
		Display_FH(1,0,8*1,4);
		Display_SZ(1,0,8*2,Temperature/10);
		Display_SZ(1,0,8*3,Temperature%10);
		Display_HZ(1,0,8*4,16);	
		
		Display_ZM(2,0,8*0,7);
		Display_FH(2,0,8*1,4);
		Display_SZ(2,0,8*2,0);
		Display_SZ(2,0,8*3,0);		
		Display_FH(2,0,8*4,7);
		
		//�ڶ���
		if(mode == 0 || mode == 1)
		{
			Display_HZ(1,2,16*0,0);
			Display_HZ(1,2,16*1,1);
			Display_HZ(1,2,16*2,2);
			Display_FH(1,2,16*3,4);		
			Display_SZ(1,2,16*3+8,0);
			
			Display_SZ(2,2,8*0,0);
			Display_SZ(2,2,8,1);		
		}
		else if(mode == 2)
		{
			Display_HZ(1,2,16*2,12);
			Display_HZ(1,2,16*3,13);	
			
			Display_HZ(2,2,16*0,14);		
			Display_HZ(2,2,16*1,15);				
		}
		
		//������
		if(mode == 0)
		{
			Display_SZ(1,4,8*0,display_password[0]);
			Display_SZ(1,4,8*1,display_password[1]);
			Display_SZ(1,4,8*2,display_password[2]);
			Display_SZ(1,4,8*3,display_password[3]);
			Display_SZ(1,4,8*4,display_password[4]);
			Display_SZ(1,4,8*5,display_password[5]);		
			
			Display_SZ(2,4,16*3,error);
		}
		else if(mode == 1)
		{
			Display_HZ(1,4,16*0,0);
			Display_HZ(1,4,16*1,1);
			Display_HZ(1,4,16*2,2);		
			Display_HZ(1,4,16*3,3);		
			
			Display_HZ(2,4,16*0,4);		
			Display_HZ(2,4,16*1,5);					
		}
		else if(mode == 2)
		{
			Display_HZ(1,4,16*1,0);
			Display_HZ(1,4,16*2,1);
			Display_HZ(1,4,16*3,2);		
			
			Display_HZ(2,4,16*0,3);		
			Display_HZ(2,4,16*1,6);		
			Display_HZ(2,4,16*2,7);				
		}	
		
		//������
		if(mode == 0)
		{
			Display_HZ(1,6,16*0,8);
			Display_HZ(1,6,16*1,9);	

			Display_HZ(2,6,16*2,10);
			Display_HZ(2,6,16*3,11);	
		}
		
		Temperature = ReadTemperature_s()/16;
		scan_key();

	}
}