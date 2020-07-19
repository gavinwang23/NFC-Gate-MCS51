/**********************RC522ʵ�鿪��������************************
*  CPU:STC89C52RC
*  ����:11.0592MHZ
------------------------------------------------*/
#include <REGX52.H>
#include <STDIO.H>
#include <string.h>
#include "main.h"
#include "mfrc522.h"

sbit s = P1^0;
sbit beep = P1^3;
sbit open1 = P1^7;
sbit open2 = P1^6;
sbit open3 = P1^5;

unsigned char code data1[16] = {0x12,0x34,0x56,0x78,0xED,0xCB,0xA9,0x87,0x12,0x34,0x56,0x78,0x01,0xFE,0x01,0xFE};
//M1����ĳһ��дΪ���¸�ʽ����ÿ�ΪǮ�����ɽ��տۿ�ͳ�ֵ����
//4�ֽڽ����ֽ���ǰ����4�ֽڽ��ȡ����4�ֽڽ�1�ֽڿ��ַ��1�ֽڿ��ַȡ����1�ֽڿ��ַ��1�ֽڿ��ַȡ�� 
unsigned char code data2[4]  = {0,0,0,0x01};
unsigned char code DefaultKey[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; 

unsigned char g_ucTempbuf[20]; 
unsigned int card_id[4]; 
int id1[4]={41,9,167,110};			//��ɫ��
int id2[4]={89,165,99,184};			//�ҵ�IPHONE�Ϻ�������
int id3[4]={54,58,231,238};			//�ҵ�ˮ��
int id4[4]={158,151,44,201};			//л��ɭˮ��
int id5[4]={19,139,139,123};			//ë���ˮ��
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
         status = PcdRequest(PICC_REQALL, g_ucTempbuf);//Ѱ��
         if (status != MI_OK)
         {    
              InitializeSystem( );
							PcdReset();
							PcdAntennaOff(); 
							PcdAntennaOn(); 
							P0=0x00;
							continue;
         }
			     
			/*printf("��������:");
            for(i=0;i<2;i++)
					{
						temp=g_ucTempbuf[i];
						printf("%X",temp);
						
					}*/
			
         status = PcdAnticoll(g_ucTempbuf);//����ײ
         if (status != MI_OK)
         {    continue;    }

         
					////////����Ϊ�������ִ�ӡ��������////////////////////////
				
					//printf("�����кţ�");	//�����ն���ʾ,
					for(i=0;i<4;i++)
					{
						temp=g_ucTempbuf[i];
						card_id[i]=g_ucTempbuf[i];
						//printf("%X",temp);
						printf("%d",card_id[i]);
						printf(";");
						
					}
					
					printf("\n");
					
					for(i=0;i<4;i++){                    //��֤��1����
						if(card_id[i]==id1[i]){p1=1;}
						else{p1=0;break;}
					}
					
					for(i=0;i<4;i++){                    //��֤��2����
						if(card_id[i]==id2[i]){p2=1;}
						else{p2=0;break;}
					}
					
					for(i=0;i<4;i++){                    //��֤��3����
						if(card_id[i]==id3[i]){p3=1;}
						else{p3=0;break;}
					}
					
					for(i=0;i<4;i++){                    //��֤��4����
						if(card_id[i]==id4[i]){p4=1;}
						else{p4=0;break;}
					}
					
					for(i=0;i<4;i++){                    //��֤��5����
						if(card_id[i]==id5[i]){p5=1;}
						else{p5=0;break;}
					}

					///////////////////////////���Ų���////////////////////////
					
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
						
						p1=0;p2=0;p3=0;p4=0;p5=0;   //��λ
						
					}
					
					
					
					//////////////////////////////////////////////////////////
					
					
         status = PcdSelect(g_ucTempbuf);//ѡ����Ƭ
         if (status != MI_OK)
         {    continue;    }
         
         status = PcdAuthState(PICC_AUTHENT1A, 1, DefaultKey, g_ucTempbuf);//��֤��Ƭ����
         if (status != MI_OK)
         {    continue;    }
         
         status = PcdWrite(1, data1);//д��
         if (status != MI_OK)
         {    continue;    }

    }
}


/////////////////////////////////////////////////////////////////////
//ϵͳ��ʼ��
/////////////////////////////////////////////////////////////////////
void InitializeSystem()
{
/*	P0M1 = 0x0; P0M2 = 0x0;
	P1M1 = 0x0;	P1M2 = 0x0;                      
	P3M1 = 0x0; P3M2 = 0xFF; 
*/	
	
	P0 = 0xff; P1 = 0x00; P3 = 0xFF;P2 = 0xFE;

 	TMOD=0x21;		   //��T0Ϊ��ʽ1��GATE=1��
	SCON=0x50;
	TH1=0xF4;          //������Ϊ2400bps
	TL1=0xF4;
	TH0=0;
	TL0=0; 
	TR0=1;  
	ET0=1;             //����T0�ж�
	TR1=1;			   //������ʱ��
	TI=1;
	EA=1;			   	//�������ж�
	
	ES = 1;
	RI = 1;
	

 
}



   
  
