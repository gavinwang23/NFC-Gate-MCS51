/**********************************************
*  CPU:STC89C52RC
*  晶振:11.0592MHZ
------------------------------------------------*/
#include <REGX52.H>
#include <STDIO.H>
#include <string.h>
#include "main.h"
#include "mfrc522.h"

sbit s = P1^0; //舵机脉冲信号
sbit beep = P1^3; //蜂鸣器信号（未用）
sbit open1 = P1^7; //预留输入开关信号
sbit open2 = P1^6; //预留输入开关信号
sbit open3 = P1^5; //预留输入开关信号

unsigned char code data1[16] = {0x12,0x34,0x56,0x78,0xED,0xCB,0xA9,0x87,0x12,0x34,0x56,0x78,0x01,0xFE,0x01,0xFE};
//M1卡的某一块写为如下格式，则该块为钱包，可接收扣款和充值命令
//4字节金额（低字节在前）＋4字节金额取反＋4字节金额＋1字节块地址＋1字节块地址取反＋1字节块地址＋1字节块地址取反 
unsigned char code data2[4]  = {0,0,0,0x01};
unsigned char code DefaultKey[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; 

unsigned char g_ucTempbuf[20]; 
unsigned int card_id[4];            //将串口输出的id填写到下面数组中，将保存作为钥匙
int id1[4]={41,9,167,110};			//蓝色卡
int id2[4]={89,165,99,184};			//我的IPHONE公交卡
int id3[4]={54,58,231,238};			//我的水卡
int id4[4]={158,151,44,201};		//谢利森水卡
int id5[4]={19,139,139,123};		//毛泽光水卡
int p1=0;
int p2=0;
int p3=0;
int p4=0;
int p5=0;

void Delay100us()		//@11.0592MHz
{
	unsigned char i;

	i = 43;
	while (--i);
}


void delay1(unsigned int z)
{
	unsigned int x,y;
	for(x=z;x>0;x--)
	for(y=110;y>0;y--);	
}  


void main( )
{    
     unsigned char status,i,j;
		 unsigned int temp;
     InitializeSystem( );
     PcdReset();
     PcdAntennaOff(); 
     PcdAntennaOn();
     while ( 1 )
     {   
             //当open1，open2，open3有信号输入时，信号保持0.7s以上，执行开门操作
			 if(open1==1 || open2==1 || open3==1){
				 delay1(700);
				 if(open1==1 || open2==1 || open3==1){
					 int x,y;
					 beep=1;
					 for(y=0;y<100;y++){
							for(x=0;x<14;x++){s=1;Delay100us();}
							for(x=0;x<186;x++){s=0;Delay100us();}
						}
						delay1(500);
						
						for(y=0;y<150;y++){
							for(x=0;x<5;x++){s=1;Delay100us();}
							for(x=0;x<195;x++){s=0;Delay100us();}
						}
				 }
			 }
         status = PcdRequest(PICC_REQALL, g_ucTempbuf);//寻卡
         if (status != MI_OK)
         {    
              InitializeSystem( );
							PcdReset();
							PcdAntennaOff(); 
							PcdAntennaOn(); 
							P0=0x00;
							continue;
         }
			     
			/*printf("卡的类型:");
            for(i=0;i<2;i++)
					{
						temp=g_ucTempbuf[i];
						printf("%X",temp);
						
					}*/
			
         status = PcdAnticoll(g_ucTempbuf);//防冲撞
         if (status != MI_OK)
         {    continue;    }

         
					////////以下为串口助手打印出的内容////////////////////////
				
					//printf("卡序列号：");	//超级终端显示,将从串口输出
					for(i=0;i<4;i++)      //卡的id存在g_ucTempbuf[i]中 i=0到3
					{
						temp=g_ucTempbuf[i];
						card_id[i]=g_ucTempbuf[i];
						//printf("%X",temp);
						printf("%d",card_id[i]); //打印卡片id，输出为10进制整数
						printf(";");             //数组元素之间用：隔开
						
					}
					
					printf("\n");
					
					//////验证读取到的卡片id是否和程序存储的卡片id相同///////
					
					for(i=0;i<4;i++){                    //验证第1个人
						if(card_id[i]==id1[i]){p1=1;}
						else{p1=0;break;}
					}
					
					for(i=0;i<4;i++){                    //验证第2个人
						if(card_id[i]==id2[i]){p2=1;}
						else{p2=0;break;}
					}
					
					for(i=0;i<4;i++){                    //验证第3个人
						if(card_id[i]==id3[i]){p3=1;}
						else{p3=0;break;}
					}
					
					for(i=0;i<4;i++){                    //验证第4个人
						if(card_id[i]==id4[i]){p4=1;}
						else{p4=0;break;}
					}
					
					for(i=0;i<4;i++){                    //验证第5个人
						if(card_id[i]==id5[i]){p5=1;}
						else{p5=0;break;}
					}

					///////////////////////////开门操作////////////////////////
					
					if(p1==1||p2==1||p3==1||p4==1||p5==1){
						
						int x,y;
						beep=1;
						
						for(y=0;y<100;y++){
							for(x=0;x<14;x++){s=1;Delay100us();}
							for(x=0;x<186;x++){s=0;Delay100us();}
						}
						delay1(500);
						
						for(y=0;y<150;y++){
							for(x=0;x<5;x++){s=1;Delay100us();}
							for(x=0;x<195;x++){s=0;Delay100us();}
						}
						
						p1=0;p2=0;p3=0;p4=0;p5=0;   //复位
						
					}
					
					
					
					//////////////////////////////////////////////////////////
					
					
         status = PcdSelect(g_ucTempbuf);//选定卡片
         if (status != MI_OK)
         {    continue;    }
         
         status = PcdAuthState(PICC_AUTHENT1A, 1, DefaultKey, g_ucTempbuf);//验证卡片密码
         if (status != MI_OK)
         {    continue;    }
         
         status = PcdWrite(1, data1);//写块
         if (status != MI_OK)
         {    continue;    }

    }
}


/////////////////////////////////////////////////////////////////////
//系统初始化
/////////////////////////////////////////////////////////////////////
void InitializeSystem()
{
/*	P0M1 = 0x0; P0M2 = 0x0;
	P1M1 = 0x0;	P1M2 = 0x0;                      
	P3M1 = 0x0; P3M2 = 0xFF; 
*/	
	
	P0 = 0xff; P1 = 0x00; P3 = 0xFF;P2 = 0xFE;

 	TMOD=0x21;		   //设T0为方式1，GATE=1；
	SCON=0x50;
	TH1=0xF4;          //波特率为2400bps
	TL1=0xF4;
	TH0=0;
	TL0=0; 
	TR0=1;  
	ET0=1;             //允许T0中断
	TR1=1;			   //开启定时器
	TI=1;
	EA=1;			   	//开启总中断
	
	ES = 1;
	RI = 1;
	

 
}



