
/*-----------------07��ʱ����--------------
C8051F340��ϵͳʱ��Ϊ12M
����1��ʵ�����ཻ����ѹ
����2���ɼ�A,B����ADC���ݣ�ͨ��LCD��ʾ
*�-----------------------------------------*/
#include "c8051f340.h"
#include <INTRINS.H>
#include "var.h"
#include "lcd.c"
//-------------------������---------------------
void main(void)               
{ 
  EA=0; EMI0CF=EM_DATA;
  PCA0MD=0;        //�ر�WDT 
  osc_init();      //������ʼ�� 
  tzd();tzd();     //��λ�ж�
  wdt_initi();	   //���Ź���ʼ������
  vdd_rst_init();  //��ʼ����λԴ����ʱ�Ӷ�ʧ��⡢vdd���Ӹ�λ   
  port_initi();    //io�ڳ�ʼ��
  chufa_off();  //�رն˿ڡ��崥����־ 
  if((RSTSRC&1<<PORSF)==0)	  re_hf=1;   //���Ȼָ���־
  else                       {zt=0x55;re_hf=0;PCON=0;}  // ������

  if(re_hf==0&&zt==0x55)  leng();  //������\lcd��ʼ��
		
  while(1)
  {switch(zt)
	 {case 0x5a: start_cl(); break; //��ѹ״̬  
	  default:   stop_cl(); 	      //ͣ��״̬
	 }             
  }
}


//-----------------------------------
/*--------a�����ͬ���ж�--------------*/                                   
void int0(void) interrupt 0        
{
		
		
}
//----------------------------------------
/*--------��--------------*/
void int1(void) interrupt 2         
{ 
  
}

/*--------a�ඨʱ���ж�------------*/  //��ʱ��0
void time_ax(void) interrupt 1  
{ 
	
	

}                                
/*--------b�ඨʱ���ж�------------*/  //��ʱ��1
void time_bx(void) interrupt 3  
{
	
}


/*--------c�ඨʱ���ж�------------*/ //��ʱ��2
void time_cx(void) interrupt 5  
{ 
	
}


/*--------pca�ж�a,b,c��ѹͬ����------
----------c����ͬ��---------*/	  //PCA�ж�
void pca(void)  interrupt 11       
{if(_testbit_(CCF2))  //C�����ͬ��
  {

  }
//-------------------------------------------------------------
 else if(_testbit_(CCF0))  //B���ѹͬ��(�����Դ�Ϊ��׼���½�����Ч��
  {

  }
//-------------------------------------------------------------
 else if(_testbit_(CCF3))  //3.333ms��ʱ
  {

  }
//---------------------------------------------------------
 else if(_testbit_(CCF1)) 	 //b�����ͬ��
  {
		
  } 
 else 
	{}  //�����Ƿ���
}


//����-------t3��ʱ�ж� ����������-------------
void ds_1ms(void) interrupt 14     //110us+57us��ʱ*6=1ms
 {  
   uchar data temp;
   static uchar data vp_js,yc,dl_max,adc_js;
   static uint data dl_a,dl_b;
	 

   if(cfmc_jx==1)	//���崮����ߵ�ƽ110us���´ν�������166.6us
	   {cfmc_jx=0;
			
	  //--��adc�ж�-----------------
		if(adc_t3>39)    adc_t3=0;  //�������
		if(++adc_js>5)   adc_js=0;
		 switch(adc_js)	   //ÿ��167usִ��һ��
			   {case 0:     
   					 dl_a=adc_start(AX_ADC,RIGHT); //����A����
			   	   dl_b=adc_start(BX_ADC,RIGHT); //����B����
			   	  break;		   	            
				 case 1: 
				 	  break;
				 case 2:   //---�����ֵ---------------
					    temp=dl_a>>8;
				 	    if(temp>dl_max)  dl_max=temp;
				 	    temp=dl_b>>8;
				 	    if(temp>dl_max)  dl_max=temp;  		  
						  break	; 	  
				 case 3: 
				    switch(adc_amp)   //---------�����������--------------------
			         {case 0:
								   ax_dl[adc_t3]=dl_a<<2;//2������ʱ���ݷŴ�
							     bx_dl[adc_t3]=dl_b<<2;   	
							     break;  
				        case 1:
									 ax_dl[adc_t3]=dl_a<<1;//4������ʱ���ݷŴ�
							     bx_dl[adc_t3]=dl_b<<1;
							     break;  
				        case 2:
									ax_dl[adc_t3]=dl_a;//8�����治��	
							    bx_dl[adc_t3]=dl_b;
							    break;       	  	  
				 	      default: ;
							 }			
				 		 break;	
				 case 4:
					  if(++vp_js>20)   //20ms����ı�һ�� ��ֵ��������
				       { //----�ж����� ���ӻ��Ǽ���----
					      if(dl_max>=3)
						       {if(--adc_amp>0x80) adc_amp=0;}//����0x300��С����
						    else if(dl_max<1)
						       {if(++adc_amp>2)    adc_amp=2;}	//С��0x100�Ӵ����� 
						    else  ;
					        //----��������---- G=0,S=0---4��
					        //---------------------G=0,S=1---2��
					        //---------------------------G=1 S=1---8��
					        switch(adc_amp)
			             {case 0:  CS4053=0; TD4053=1; break;   //2������ //0,1
				            case 1:  CS4053=0; TD4053=0; break;   //4������ //0,0  
				            case 2:  CS4053=1; TD4053=1; break;   //8������ //1,1	  		   	  	  
				 	          default: adc_amp=0;CS4053=0; TD4053=1; //2������ //0,1
						       }      
						      vp_js=0;dl_max=0;   //��ֵ��������		
						     }	               
						  break; 
				 case 5: adc_t3++;	 //1msִ��һ��  
						 break;
						   				 	     
				 default: ;
			 } 
		  if(++yc>=15) {yc=0;time_2500us++;}
		  TMR3CN=0;
      TMR3RLL=0XD2;TMR3RLH=0XFF;	  //�͵�ƽ��ʱ46us��д��װ�ؼĴ��� �жϺ��ֵ��Ч��
	    TMR3CN=4;		
	  }           	   
 else  //���崮����͵�ƽ
	  {cfmc_jx=1;
	   TMR3CN=0;
     TMR3RLL=0X88;TMR3RLH=0XFF;	  //�ߵ�ƽ��ʱ120us ��д��װ�ؼĴ�����
	   TMR3CN=4;
    }
 }

 
/*--------=---------------------------------
-----�������ƣ� jisuan
-----���������� ��ʾADC����
-----��    �룺 ��
-----��    ���� ��
------------------------------------------*/  
void jisuan(void)
{ uchar  i;
  uint   data dl,temp;
  ulong  data temp_data;
	
//--------����abƽ��ֵ��У׼ֵ-----------------------------------
  temp_data=0;
  for(i=0;i<=39;i++)	{EA=0;dl=ax_dl[i];EA=1;temp_data+=dl;}
  pj_dl[0]=(uint)(temp_data/40); 			        
  if(pj_dl[0]<=3) pj_dl[0]=0;			 
  axdl_jz=pj_dl[0];    
         
  temp_data=0;
  for(i=0;i<=39;i++) {EA=0;dl=bx_dl[i];EA=1;temp_data+=dl;}
  pj_dl[1]=(uint)(temp_data/40);                   
  if(pj_dl[1]<=3) pj_dl[1]=0;
  bxdl_jz=pj_dl[1];   

  temp=0;
  for(i=1;i<=7;i++)  {dl=axdl_xs[i-1]=axdl_xs[i];temp+=dl;}
  dl=axdl_xs[7]=axdl_jz;
  temp+=dl;
  axdl_bcd=bcd_tran(temp/8);//��ʾA��ADCֵ
							  
  //----b----------------------			
  temp=0;
  for(i=1;i<=7;i++) {dl=bxdl_xs[i-1]=bxdl_xs[i];temp+=dl;}
  dl=bxdl_xs[7]=bxdl_jz;
  temp+=dl;
  bxdl_bcd=bcd_tran(temp/8);//��ʾB��ADCֵ
}
 

/*----------------------------------------
-----�������ƣ� leng
-----���������� ��������ʼ��
-----��    �룺 ��
-----��    ���� ��
------------------------------------------*/ 
void leng(void)
{uchar i;
  //uint js; 
	
  EMI0CF=EM_DATA;
  port_cz();  //�˿�����
  chufa_off();  //�رն˿ڡ��崥����־
  EA=0;WDT();
  
  ERR_JDQ=1;   //��ʼ��,���ϴ���պ�
  RUN_JDQ=0;      //��ʼ�������жϿ�
  PL_JDQ=0;       //��ʼ������·�Ͽ�
  zt=0x55;
  
  //--�������Ԫ------
 for(i=0;i<40;i++)   ax_dl[i]=bx_dl[i]=0;
 axdl_jz=bxdl_jz=0;
 axdl_bcd=bxdl_bcd=0; 	
 re_hf=0;                   //���Ȼָ���־
 adc_amp=2;
 CS4053=1; TD4053=1; //�˷�8������
 
 //-----������t3  ������ѹ ��7660���ȥ��
 IE=0X80;EIE1=0X80;	//��T3,��ȫ��
 TMR3CN|=4;         //����t3����
 //-------lcd��ʼ��--------------
 for(i=0;i<6;i++)  {delay_10us(1000); WDT();}

 lcd_init(); WDT();//lcd��ʼ��
 lcd_command_w(0x24);				//������չָ��
 delay_10us(LCD_W_TIME);
 lcd_command_w(0x30);              //���ػ���ָ��	
}


/*------------------------------------------------------------------
-----�������ƣ� stop_cl
-----���������� ͣ��״̬����
-----��    �룺 ��
-----��    ���� ��
--------------------------------------------------------------------*/
void stop_cl(void)
{uchar  js=0;
	                   //�˿�����
  chufa_off();  //�رն˿ڡ��崥����־
  time_2500us=0;
  do
   {EMI0CF=EM_DATA;
    port_cz();                       //�˿�����
	  chufa_off();  //�رն˿ڡ��崥����־
	  WDT();
    PCA0CPM0=PCA0CPM1=PCA0CPM2=PCA0CPM3=0;	//PCA�ر�
	  TR0=TR1=TR2=0;					        //T0,1,2�ر�
    zt=0x55;
	  IE=0X80; EIE1=0X80;//��T3��ȫ���ж�
	  TMR3CN|=4;         //T3����
	
	  if(time_2500us>=30)  //80msִ��1��
	    {time_2500us=0;
	     jisuan();  
	    }
			
	 STP=1;
   if(STP==0)  {if(++js>20) zt=0x5a;} //�𶯵�ѹ��ʼ
	 else        {js=0;} 
	 hm_cl(0);	
 }while(zt==0x55);         //�ж�����	�������״̬���������棬������ �����������˳�		  
}


/*-----------------------------------------
-----�������ƣ� start_cl
-----���������� ��״̬����
-----��    �룺 ��
-----��    ���� ��
------------------------------------------*/
void start_cl(void)
{uchar js1,js2,js3;

	  //BXDY_TB=CXDY_TB=1;
	 // AXDL_TB=BXDL_TB=CXDL_TB=1;
	 // PCA0CN=0;
	 // PCA0CPL3=PCA0L-1;PCA0CPH3=PCA0H-1; //�´�ƥ��ʱ�����������⿪�жϺ��������ж�
   // PCA0MD=0;PCA0CPM0=0x11;PCA0CPM3=0x49;//��ʼ��pca 
	 // TMR3RLL=0X3C;TMR3RLH=0XF6;TMR3CN=4;    //t3��
   // PCA0CN=0;TR0=TR1=TR2=0;TCON=5;TMR2CN=0;
					
		time_2500us=0;	js1=js2=js3=0;
    //--- ����ѭ�� ------------------
    do
	   {EMI0CF=EM_DATA;
		  port_cz();  //�˿�����
	    WDT();
      EIP2=ZD_IP2;EIP1=ZD_IP1;IP=ZD_IP0;  //�ж����ȼ� T3���
		  ESPI0=ES0=EX1=0;
	    zt=0x5a;  		 
 
      if(time_2500us>=30)  //80msִ��1��a����
        { time_2500us=0;
			    jisuan(); //����
          hm_cl(0);	
		    }
				
   //---------------���ϣ���ѹ----------------
	 UP=1;
   if(UP==0&&DOWN==1)  {if(++js1>20) {}}
	 else                {js1=0;} 
	 
	 //---------------���£���ѹ----------------
	 DOWN=1;
   if(DOWN==0&&UP==1)  {if(++js2>20) {}}
	 else                {js2=0;} 
	 
  //--------------ֹͣ�˳�-------------
   if(STP)  {if(++js3>20) zt=0x55;}
	 else     {js3=0;} 
	}while(zt==0x5a);         //����״̬����ѭ��		                      
}

				
/*-----------------------------------------
-----�������ƣ� uint adc_start(uchar)
-----���������� adc����
-----��    �룺 ͨ����,�����ȴ�ʱ��1=1us����
-----��    ���� ת�����
------------------------------------------*/ 
uint adc_start(uchar td,uchar mode)
{
 union  w_b adc_jg;
 uchar  js;
  
  REF0CN=3;     //ѡ���ڲ���׼����ʼ���˿�ʱ��ѡ��
  AMX0P=td;     //ѡ��ͨ��
  AMX0N=0x1f;      //���˽ӵء�����ת��
  ADC0CF=mode;  //0X28;  //2Mhzʱ�ӡ��Ҷ���	 0x2c�����
  ADC0CN=0X80;  //ADC׼��
   _nop_();_nop_();_nop_();_nop_();
   _nop_();_nop_();_nop_();_nop_();  //�ȴ����ֵ��ݳ�� Լ1us
  AD0BUSY=1;     //����ת��
  for(js=0;js<40;js++)
     {if(AD0INT==1) {adc_jg.data_b[0]=ADC0H;
	                 adc_jg.data_b[1]=ADC0L; break;} //ת��������	     
	  else ;	 
	  }

  return(adc_jg.data_w);
}


/*--------------------------------------
���Ź���ʼ������
------------------------------------*/ 
void wdt_initi(void)
{
  PCA0MD=0;      //�ر�WDT 
  PCA0CPL4=200;   //1/12ʱ��Դʱ 1=256us  12mhz
  PCA0MD=1<<WDTE; //ѡ��1/12ʱ��Դ	���������Ź�
  PCA0MD|=1<<WDLCK;  //�������Ź�
  _nop_();
  PCA0CPH4=0;		//����
}



/*------------------------------------------
 vdd�������ʼ��������λԴѡ����
 �����ӳ�
-----------------------------------------*/	
void vdd_rst_init(void)
{
  VDM0CN|=0X80;   //ʹ��VDD���
  delay_10us(14); //140us�ӳ�
  RSTSRC=1<<MCDRSF|1<<PORSF;//ʹ��ʱ�Ӷ�ʧ��⡢�ϵ縴λ��vdd��
  REG0CN=0X80;   //�ر���ѹ��
}

/*-------------------------------------------
   ʱ���ӳٺ���	(12mhzʱ1=10us)
------------------------------------------*/ 
void delay_10us(uint t)
{
  uchar i;
  uint  j;
  for(j=0;j<t;j++)
    {
	  for(i=0;i<10;i++)	 //��ѭ��Լ9.914us
	    { _nop_();_nop_();_nop_();_nop_();}
	}
}
   

/*--------------------------------------
//IO�����ø��� 
--------------------------------------*/ 
void port_initi(void)	//V3.0
{ 
  P0MDIN= 0xff;    
  P0MDOUT=0x3f;   //P0.0~0.5(����),����,P0.6,P0.7(ͬ������),��©
  P0SKIP= 0xff;   //ȫ����
  P0=0xc0;        //P0.0~0.5����ʼ���ͣ�NPN����

  P1MDIN =0Xdf;   
  P1MDOUT=0xd8;   //P1.0~P1.2(ͬ��),��©, P1.3 P1.4,P1.6(Һ������),����,P1.5(��ο�)ģ�� P1.7(CON)����
  P1SKIP =0Xf8;   //����P1.3~P1.7 , P1.0~1.2�����PCA0~2
  P1=0xff;        

  P2MDIN =0Xff;   
  P2MDOUT=0x73;   //P2.0(TXD1),P2.1(RXD1)���� ,P2.2(©����) P2.3 (©�籣���̵���)��©,2.4~2.6�̵����������� P2.7��ͣ ��©
  P2SKIP =0x00;   //P2.0,P2.1�����TX1,RX1
  P2=0X8f;		    //2.4,2.5,2.6�̵����ϵ��
    
  P3MDIN =0Xff;  
  P3MDOUT=0Xe0;   //P3.0~P3.4 ����������,��©  P3.5(����SCK) ����,P3.6,3.7 ��·����,����
  P3=0xff;		  

  P4MDIN =0X86;   
  P4MDOUT=0x86;   //P4.0,4.3~P4.6ģ�� P4.1,P4.2,P4.7���� ����
  P4=0xff;		
  
  XBR0=0;          
  XBR2=1;         //UART1���� 
  XBR1=0X43;      //ʹ�ܽ��濪�ء�����PCA0-2

  IT01CF=0X76;    //int0=p0.6 int1=p0.7 �͵�ƽ��Ч
  REF0CN=3;       //ʹ���ڲ���ѹ��׼��������adcʱ���ã�
}

	

/*--------------------------------------
   �˿ڼ��Ĵ�������	  V3.0
--------------------------------------*/
void port_cz(void)	 //V3.0
{ 
  P0MDIN= 0xff;    
  P0MDOUT=0x3f;  
  P0SKIP= 0xff;   

  P1MDIN =0Xdf;   
  P1MDOUT=0xd8;   
  P1SKIP =0Xf8;   

  P2MDIN =0Xff;   
  P2MDOUT=0x73;		
  P2SKIP =0x00;
    
  P3MDIN =0Xff;  
  P3MDOUT=0Xe0;   
				    
  P4MDIN =0x86;  
  P4MDOUT=(P4MDOUT|0X83)&0x86;   //xxxxxxxxxxxxxxxxxxxxxxx	P4.2��Ҫ�仯

  XBR0=0;          
  XBR2=1;          //UART1���� 
  XBR1=0X43;       //ʹ�ܽ��濪�ء�����PCA0-2

  IT01CF=0X76;     //int0=p0.6 int1=p0.7 �͵�ƽ��Ч
  osc_init();      //��������
  vdd_rst_init();  //����vdd��ؼ���λԴ���ر���ѹ��
}


//-------------------------------------
//������ʼ��
//----------------------------------
void osc_init(void)
{ 
  OSCICN=0X83;   //ʹ���ڲ���������Ƶ  12mhz
  OSCLCN=0;      //&=~0X80; //�ر��ڲ���Ƶ����
  OSCXCN=0;      //�ر��ⲿ����
  CLKMUL=0;      //�رճ˷���
  CLKSEL=0;      //USBΪ4���˷������ڲ�������Ϊϵͳʱ��
  CKCON=0;       //��ʱ��ʱ��Դ����ϵͳʱ���ɿ������Ĵ���ѡ��
}	
		
//----------------------------------------------
//---bcdת��
//-----------------------------------
uint bcd_tran(uint value)
{ uint temp=0;
  if(value<10000)
   {while(value>=1000) {temp+=0x1000;value-=1000;}
    while(value>=100)  {temp+=0x100;value-=100;}
    while(value>=10)   {temp+=0x10;value-=10;}
    temp+=value;}
  else temp=value;
  return(temp);
}

//-------------------------------------------------
//---�رմ������崥����־
//--------------------------------
void chufa_off(void)
{  SPRING_A=0; SPRING_B=0; SPRING_C=0;
   SPRING_AF=0;SPRING_BF=0;SPRING_CF=0;	 //�����˿�����Ч
}
	   
