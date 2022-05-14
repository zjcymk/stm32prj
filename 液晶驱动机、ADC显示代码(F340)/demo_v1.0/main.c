
/*-----------------07ÑÓÊ±³ÌĞò--------------
C8051F340£¬ÏµÍ³Ê±ÖÓÎª12M
¹¦ÄÜ1£ºÊµÏÖÈıÏà½»Á÷µ÷Ñ¹
¹¦ÄÜ2£º²É¼¯A,BÁ½ÏàADCÊı¾İ£¬Í¨¹ıLCDÏÔÊ¾
*¡-----------------------------------------*/
#include "c8051f340.h"
#include <INTRINS.H>
#include "var.h"
#include "lcd.c"
//-------------------Ö÷³ÌĞò---------------------
void main(void)               
{ 
  EA=0; EMI0CF=EM_DATA;
  PCA0MD=0;        //¹Ø±ÕWDT 
  osc_init();      //Õñµ´Æ÷³õÊ¼»¯ 
  tzd();tzd();     //¸´Î»ÖĞ¶Ï
  wdt_initi();	   //¿´ÃÅ¹·³õÊ¼»¯º¯Êı
  vdd_rst_init();  //³õÊ¼»¯¸´Î»Ô´¡¢Æğ¶¯Ê±ÖÓ¶ªÊ§¼ì²â¡¢vdd¼àÊÓ¸´Î»   
  port_initi();    //io¿Ú³õÊ¼»¯
  chufa_off();  //¹Ø±Õ¶Ë¿Ú¡¢Çå´¥·¢±êÖ¾ 
  if((RSTSRC&1<<PORSF)==0)	  re_hf=1;   //ÖÃÈÈ»Ö¸´±êÖ¾
  else                       {zt=0x55;re_hf=0;PCON=0;}  // ÀäÆô¶¯

  if(re_hf==0&&zt==0x55)  leng();  //ÀäÆô¶¯\lcd³õÊ¼»¯
		
  while(1)
  {switch(zt)
	 {case 0x5a: start_cl(); break; //µ÷Ñ¹×´Ì¬  
	  default:   stop_cl(); 	      //Í£³µ×´Ì¬
	 }             
  }
}


//-----------------------------------
/*--------aÏàµçÁ÷Í¬²½ÖĞ¶Ï--------------*/                                   
void int0(void) interrupt 0        
{
		
		
}
//----------------------------------------
/*--------¿Õ--------------*/
void int1(void) interrupt 2         
{ 
  
}

/*--------aÏà¶¨Ê±Æ÷ÖĞ¶Ï------------*/  //¶¨Ê±Æ÷0
void time_ax(void) interrupt 1  
{ 
	
	

}                                
/*--------bÏà¶¨Ê±Æ÷ÖĞ¶Ï------------*/  //¶¨Ê±Æ÷1
void time_bx(void) interrupt 3  
{
	
}


/*--------cÏà¶¨Ê±Æ÷ÖĞ¶Ï------------*/ //¶¨Ê±Æ÷2
void time_cx(void) interrupt 5  
{ 
	
}


/*--------pcaÖĞ¶Ïa,b,cµçÑ¹Í¬²½¼°------
----------cµçÁ÷Í¬²½---------*/	  //PCAÖĞ¶Ï
void pca(void)  interrupt 11       
{if(_testbit_(CCF2))  //CÏàµçÁ÷Í¬²½
  {

  }
//-------------------------------------------------------------
 else if(_testbit_(CCF0))  //BÏàµçÑ¹Í¬²½(ÆäËüÒÔ´ËÎª»ù×¼¡¢ÏÂ½µÑØÓĞĞ§£©
  {

  }
//-------------------------------------------------------------
 else if(_testbit_(CCF3))  //3.333ms¶¨Ê±
  {

  }
//---------------------------------------------------------
 else if(_testbit_(CCF1)) 	 //bÏàµçÁ÷Í¬²½
  {
		
  } 
 else 
	{}  //¶¼²»ÊÇ·µ»Ø
}


//¡ª¡ª-------t3¶¨Ê±ÖĞ¶Ï ´¥·¢Âö³åÁĞ-------------
void ds_1ms(void) interrupt 14     //110us+57us¶¨Ê±*6=1ms
 {  
   uchar data temp;
   static uchar data vp_js,yc,dl_max,adc_js;
   static uint data dl_a,dl_b;
	 

   if(cfmc_jx==1)	//Âö³å´®Êä³ö¸ßµçÆ½110us£¬ÏÂ´Î½øÈëÖÜÆÚ166.6us
	   {cfmc_jx=0;
			
	  //--Æğ¶¯adcÅĞ¶Ï-----------------
		if(adc_t3>39)    adc_t3=0;  //Êı×é±äÁ¿
		if(++adc_js>5)   adc_js=0;
		 switch(adc_js)	   //Ã¿´Î167usÖ´ĞĞÒ»¸ö
			   {case 0:     
   					 dl_a=adc_start(AX_ADC,RIGHT); //²ÉÑùAµçÁ÷
			   	   dl_b=adc_start(BX_ADC,RIGHT); //²ÉÑùBµçÁ÷
			   	  break;		   	            
				 case 1: 
				 	  break;
				 case 2:   //---ÕÒ×î´óÖµ---------------
					    temp=dl_a>>8;
				 	    if(temp>dl_max)  dl_max=temp;
				 	    temp=dl_b>>8;
				 	    if(temp>dl_max)  dl_max=temp;  		  
						  break	; 	  
				 case 3: 
				    switch(adc_amp)   //---------µçÁ÷·ÅÈë¶ÓÁĞ--------------------
			         {case 0:
								   ax_dl[adc_t3]=dl_a<<2;//2±¶ÔöÒæÊ±Êı¾İ·Å´ó
							     bx_dl[adc_t3]=dl_b<<2;   	
							     break;  
				        case 1:
									 ax_dl[adc_t3]=dl_a<<1;//4±¶ÔöÒæÊ±Êı¾İ·Å´ó
							     bx_dl[adc_t3]=dl_b<<1;
							     break;  
				        case 2:
									ax_dl[adc_t3]=dl_a;//8±¶ÔöÒæ²»±ä	
							    bx_dl[adc_t3]=dl_b;
							    break;       	  	  
				 	      default: ;
							 }			
				 		 break;	
				 case 4:
					  if(++vp_js>20)   //20msÔöÒæ¸Ä±äÒ»´Î ·åÖµ±£³ÖÇåÁã
				       { //----ÅĞ¶ÏÔöÒæ Ôö¼Ó»¹ÊÇ¼õÉÙ----
					      if(dl_max>=3)
						       {if(--adc_amp>0x80) adc_amp=0;}//´óÓÚ0x300¼õĞ¡ÔöÒæ
						    else if(dl_max<1)
						       {if(++adc_amp>2)    adc_amp=2;}	//Ğ¡ÓÚ0x100¼Ó´óÔöÒæ 
						    else  ;
					        //----µ÷ÕûÔöÒæ---- G=0,S=0---4±¶
					        //---------------------G=0,S=1---2±¶
					        //---------------------------G=1 S=1---8±¶
					        switch(adc_amp)
			             {case 0:  CS4053=0; TD4053=1; break;   //2±¶ÔöÒæ //0,1
				            case 1:  CS4053=0; TD4053=0; break;   //4±¶ÔöÒæ //0,0  
				            case 2:  CS4053=1; TD4053=1; break;   //8±¶ÔöÒæ //1,1	  		   	  	  
				 	          default: adc_amp=0;CS4053=0; TD4053=1; //2±¶ÔöÒæ //0,1
						       }      
						      vp_js=0;dl_max=0;   //·åÖµ±£³ÖÇåÁã		
						     }	               
						  break; 
				 case 5: adc_t3++;	 //1msÖ´ĞĞÒ»´Î  
						 break;
						   				 	     
				 default: ;
			 } 
		  if(++yc>=15) {yc=0;time_2500us++;}
		  TMR3CN=0;
      TMR3RLL=0XD2;TMR3RLH=0XFF;	  //µÍµçÆ½¶¨Ê±46us£¨Ğ´Èë×°ÔØ¼Ä´æÆ÷ ÖĞ¶Ïºó´ËÖµÓĞĞ§£©
	    TMR3CN=4;		
	  }           	   
 else  //Âö³å´®Êä³öµÍµçÆ½
	  {cfmc_jx=1;
	   TMR3CN=0;
     TMR3RLL=0X88;TMR3RLH=0XFF;	  //¸ßµçÆ½¶¨Ê±120us £¨Ğ´Èë×°ÔØ¼Ä´æÆ÷£©
	   TMR3CN=4;
    }
 }

 
/*--------=---------------------------------
-----º¯ÊıÃû³Æ£º jisuan
-----¹¦ÄÜÃèÊö£º ÏÔÊ¾ADC²ÉÑù
-----Êä    Èë£º ÎŞ
-----Êä    ³ö£º ÎŞ
------------------------------------------*/  
void jisuan(void)
{ uchar  i;
  uint   data dl,temp;
  ulong  data temp_data;
	
//--------¼ÆËãabÆ½¾ùÖµ¼°Ğ£×¼Öµ-----------------------------------
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
  axdl_bcd=bcd_tran(temp/8);//ÏÔÊ¾AÏàADCÖµ
							  
  //----b----------------------			
  temp=0;
  for(i=1;i<=7;i++) {dl=bxdl_xs[i-1]=bxdl_xs[i];temp+=dl;}
  dl=bxdl_xs[7]=bxdl_jz;
  temp+=dl;
  bxdl_bcd=bcd_tran(temp/8);//ÏÔÊ¾BÏàADCÖµ
}
 

/*----------------------------------------
-----º¯ÊıÃû³Æ£º leng
-----¹¦ÄÜÃèÊö£º ÀäÆô¶¯³õÊ¼»¯
-----Êä    Èë£º ÎŞ
-----Êä    ³ö£º ÎŞ
------------------------------------------*/ 
void leng(void)
{uchar i;
  //uint js; 
	
  EMI0CF=EM_DATA;
  port_cz();  //¶Ë¿ÚÖØÖÃ
  chufa_off();  //¹Ø±Õ¶Ë¿Ú¡¢Çå´¥·¢±êÖ¾
  EA=0;WDT();
  
  ERR_JDQ=1;   //³õÊ¼»¯,¹ÊÕÏ´¥µã±ÕºÏ
  RUN_JDQ=0;      //³õÊ¼»¯£¬ÔËĞĞ¶Ï¿ª
  PL_JDQ=0;       //³õÊ¼»¯£¬ÅÔÂ·¶Ï¿ª
  zt=0x55;
  
  //--ÇåµçÁ÷µ¥Ôª------
 for(i=0;i<40;i++)   ax_dl[i]=bx_dl[i]=0;
 axdl_jz=bxdl_jz=0;
 axdl_bcd=bxdl_bcd=0; 	
 re_hf=0;                   //ÇåÈÈ»Ö¸´±êÖ¾
 adc_amp=2;
 CS4053=1; TD4053=1; //ÔË·Å8±¶ÔöÒæ
 
 //-----ÏÈÆô¶¯t3  ²úÉú¸ºÑ¹ ÓÃ7660ºó¿ÉÈ¥µô
 IE=0X80;EIE1=0X80;	//¿ªT3,¿ªÈ«¾Ö
 TMR3CN|=4;         //ÖØÖÁt3ÔËĞĞ
 //-------lcd³õÊ¼»¯--------------
 for(i=0;i<6;i++)  {delay_10us(1000); WDT();}

 lcd_init(); WDT();//lcd³õÊ¼»¯
 lcd_command_w(0x24);				//½øÈëÀ©Õ¹Ö¸Áî
 delay_10us(LCD_W_TIME);
 lcd_command_w(0x30);              //·µ»Ø»ù±¾Ö¸Áî	
}


/*------------------------------------------------------------------
-----º¯ÊıÃû³Æ£º stop_cl
-----¹¦ÄÜÃèÊö£º Í£³µ×´Ì¬´¦Àí
-----Êä    Èë£º ÎŞ
-----Êä    ³ö£º ÎŞ
--------------------------------------------------------------------*/
void stop_cl(void)
{uchar  js=0;
	                   //¶Ë¿ÚÖØÖÃ
  chufa_off();  //¹Ø±Õ¶Ë¿Ú¡¢Çå´¥·¢±êÖ¾
  time_2500us=0;
  do
   {EMI0CF=EM_DATA;
    port_cz();                       //¶Ë¿ÚÖØÖÃ
	  chufa_off();  //¹Ø±Õ¶Ë¿Ú¡¢Çå´¥·¢±êÖ¾
	  WDT();
    PCA0CPM0=PCA0CPM1=PCA0CPM2=PCA0CPM3=0;	//PCA¹Ø±Õ
	  TR0=TR1=TR2=0;					        //T0,1,2¹Ø±Õ
    zt=0x55;
	  IE=0X80; EIE1=0X80;//¿ªT3£¬È«¾ÖÖĞ¶Ï
	  TMR3CN|=4;         //T3ÔËĞĞ
	
	  if(time_2500us>=30)  //80msÖ´ĞĞ1´Î
	    {time_2500us=0;
	     jisuan();  
	    }
			
	 STP=1;
   if(STP==0)  {if(++js>20) zt=0x5a;} //Æğ¶¯µ÷Ñ¹¿ªÊ¼
	 else        {js=0;} 
	 hm_cl(0);	
 }while(zt==0x55);         //ÅĞ¶ÏÆô¶¯	Èç¹û¼üÅÌ×´Ì¬²»ÊÇÖ÷»­Ãæ£¬²»Æô¶¯ £¬¹ÊÕÏÓÅÏÈÍË³ö		  
}


/*-----------------------------------------
-----º¯ÊıÃû³Æ£º start_cl
-----¹¦ÄÜÃèÊö£º Æğ¶¯×´Ì¬´¦Àí
-----Êä    Èë£º ÎŞ
-----Êä    ³ö£º ÎŞ
------------------------------------------*/
void start_cl(void)
{uchar js1,js2,js3;

	  //BXDY_TB=CXDY_TB=1;
	 // AXDL_TB=BXDL_TB=CXDL_TB=1;
	 // PCA0CN=0;
	 // PCA0CPL3=PCA0L-1;PCA0CPH3=PCA0H-1; //ÏÂ´ÎÆ¥ÅäÊ±¼ä·ÅÖÃ×î´ó£¬ÒÔÃâ¿ªÖĞ¶Ïºó·¢ÉúÒâÍâÖĞ¶Ï
   // PCA0MD=0;PCA0CPM0=0x11;PCA0CPM3=0x49;//³õÊ¼»¯pca 
	 // TMR3RLL=0X3C;TMR3RLH=0XF6;TMR3CN=4;    //t3Æğ¶¯
   // PCA0CN=0;TR0=TR1=TR2=0;TCON=5;TMR2CN=0;
					
		time_2500us=0;	js1=js2=js3=0;
    //--- Æô¶¯Ñ­»· ------------------
    do
	   {EMI0CF=EM_DATA;
		  port_cz();  //¶Ë¿ÚÖØÖÃ
	    WDT();
      EIP2=ZD_IP2;EIP1=ZD_IP1;IP=ZD_IP0;  //ÖĞ¶ÏÓÅÏÈ¼¶ T3×î¸ß
		  ESPI0=ES0=EX1=0;
	    zt=0x5a;  		 
 
      if(time_2500us>=30)  //80msÖ´ĞĞ1´Îa¼ÆËã
        { time_2500us=0;
			    jisuan(); //¼ÆËã
          hm_cl(0);	
		    }
				
   //---------------ÏòÉÏ£¬¼ÓÑ¹----------------
	 UP=1;
   if(UP==0&&DOWN==1)  {if(++js1>20) {}}
	 else                {js1=0;} 
	 
	 //---------------ÏòÏÂ£¬¼õÑ¹----------------
	 DOWN=1;
   if(DOWN==0&&UP==1)  {if(++js2>20) {}}
	 else                {js2=0;} 
	 
  //--------------Í£Ö¹ÍË³ö-------------
   if(STP)  {if(++js3>20) zt=0x55;}
	 else     {js3=0;} 
	}while(zt==0x5a);         //Æô¶¯×´Ì¬¼ÌĞøÑ­»·		                      
}

				
/*-----------------------------------------
-----º¯ÊıÃû³Æ£º uint adc_start(uchar)
-----¹¦ÄÜÃèÊö£º adc²ÉÑù
-----Êä    Èë£º Í¨µÀºÅ,²ÉÑùµÈ´ıÊ±¼ä1=1us×óÓÒ
-----Êä    ³ö£º ×ª»»½á¹û
------------------------------------------*/ 
uint adc_start(uchar td,uchar mode)
{
 union  w_b adc_jg;
 uchar  js;
  
  REF0CN=3;     //Ñ¡ÔñÄÚ²¿»ù×¼¡¢³õÊ¼»¯¶Ë¿ÚÊ±ÒÑÑ¡Ôñ
  AMX0P=td;     //Ñ¡ÔñÍ¨µÀ
  AMX0N=0x1f;      //¸º¶Ë½ÓµØ¡¢µ¥¶Ë×ª»»
  ADC0CF=mode;  //0X28;  //2MhzÊ±ÖÓ¡¢ÓÒ¶ÔÆë	 0x2c×ó¶ÔÆë
  ADC0CN=0X80;  //ADC×¼±¸
   _nop_();_nop_();_nop_();_nop_();
   _nop_();_nop_();_nop_();_nop_();  //µÈ´ı±£³ÖµçÈİ³äµç Ô¼1us
  AD0BUSY=1;     //Æô¶¯×ª»»
  for(js=0;js<40;js++)
     {if(AD0INT==1) {adc_jg.data_b[0]=ADC0H;
	                 adc_jg.data_b[1]=ADC0L; break;} //×ª»»ÍêÌø³ö	     
	  else ;	 
	  }

  return(adc_jg.data_w);
}


/*--------------------------------------
¿´ÃÅ¹·³õÊ¼»¯º¯Êı
------------------------------------*/ 
void wdt_initi(void)
{
  PCA0MD=0;      //¹Ø±ÕWDT 
  PCA0CPL4=200;   //1/12Ê±ÖÓÔ´Ê± 1=256us  12mhz
  PCA0MD=1<<WDTE; //Ñ¡Ôñ1/12Ê±ÖÓÔ´	²¢Æô¶¯¿´ÃÅ¹·
  PCA0MD|=1<<WDLCK;  //Ëø¶¨¿´ÃÅ¹·
  _nop_();
  PCA0CPH4=0;		//¸üĞÂ
}



/*------------------------------------------
 vdd¼à¿ØÆ÷³õÊ¼»¯¡¢¼°¸´Î»Ô´Ñ¡Ôñº¯Êı
 ÄÚÓĞÑÓ³Ù
-----------------------------------------*/	
void vdd_rst_init(void)
{
  VDM0CN|=0X80;   //Ê¹ÄÜVDD¼à¿Ø
  delay_10us(14); //140usÑÓ³Ù
  RSTSRC=1<<MCDRSF|1<<PORSF;//Ê¹ÄÜÊ±ÖÓ¶ªÊ§¼ì²â¡¢ÉÏµç¸´Î»£¨vdd£©
  REG0CN=0X80;   //¹Ø±ÕÎÈÑ¹Æ÷
}

/*-------------------------------------------
   Ê±¼äÑÓ³Ùº¯Êı	(12mhzÊ±1=10us)
------------------------------------------*/ 
void delay_10us(uint t)
{
  uchar i;
  uint  j;
  for(j=0;j<t;j++)
    {
	  for(i=0;i<10;i++)	 //±¾Ñ­»·Ô¼9.914us
	    { _nop_();_nop_();_nop_();_nop_();}
	}
}
   

/*--------------------------------------
//IO¿ÚÅäÖÃ¸ü¸Ä 
--------------------------------------*/ 
void port_initi(void)	//V3.0
{ 
  P0MDIN= 0xff;    
  P0MDOUT=0x3f;   //P0.0~0.5(´¥·¢),ÍÆÍì,P0.6,P0.7(Í¬²½ÊäÈë),¿ªÂ©
  P0SKIP= 0xff;   //È«Ìø¹ı
  P0=0xc0;        //P0.0~0.5£¬³õÊ¼»¯µÍ£¬NPNÕıÇı

  P1MDIN =0Xdf;   
  P1MDOUT=0xd8;   //P1.0~P1.2(Í¬²½),¿ªÂ©, P1.3 P1.4,P1.6(Òº¾§Çı¶¯),ÍÆÍì,P1.5(Íâ²Î¿¼)Ä£Äâ P1.7(CON)ÍÆÍì
  P1SKIP =0Xf8;   //Ìø¹ıP1.3~P1.7 , P1.0~1.2·ÖÅä¸øPCA0~2
  P1=0xff;        

  P2MDIN =0Xff;   
  P2MDOUT=0x73;   //P2.0(TXD1),P2.1(RXD1)ÍÆÍì ,P2.2(Â©µç¼ì²â) P2.3 (Â©µç±£»¤¼ÌµçÆ÷)¿ªÂ©,2.4~2.6¼ÌµçÆ÷Çı¶¯ÍÆÍì P2.7ÆôÍ£ ¿ªÂ©
  P2SKIP =0x00;   //P2.0,P2.1·ÖÅä¸øTX1,RX1
  P2=0X8f;		    //2.4,2.5,2.6¼ÌµçÆ÷ÉÏµçµÍ
    
  P3MDIN =0Xff;  
  P3MDOUT=0Xe0;   //P3.0~P3.4 ¿ª¹ØÁ¿ÊäÈë,¿ªÂ©  P3.5(ÌúµçSCK) ÍÆÍì,P3.6,3.7 ¶àÂ·¿ª¹Ø,ÍÆÍì
  P3=0xff;		  

  P4MDIN =0X86;   
  P4MDOUT=0x86;   //P4.0,4.3~P4.6Ä£Äâ P4.1,P4.2,P4.7Ìúµç ÍÆÍì
  P4=0xff;		
  
  XBR0=0;          
  XBR2=1;         //UART1Á¬½Ó 
  XBR1=0X43;      //Ê¹ÄÜ½»²æ¿ª¹Ø¡¢ÔÊĞíPCA0-2

  IT01CF=0X76;    //int0=p0.6 int1=p0.7 µÍµçÆ½ÓĞĞ§
  REF0CN=3;       //Ê¹ÄÜÄÚ²¿µçÑ¹»ù×¼£¨ÒÑÔÚÆğ¶¯adcÊ±ÖØÖÃ£©
}

	

/*--------------------------------------
   ¶Ë¿Ú¼°¼Ä´æÆ÷ÖØÖÃ	  V3.0
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
  P4MDOUT=(P4MDOUT|0X83)&0x86;   //xxxxxxxxxxxxxxxxxxxxxxx	P4.2ĞèÒª±ä»¯

  XBR0=0;          
  XBR2=1;          //UART1Á¬½Ó 
  XBR1=0X43;       //Ê¹ÄÜ½»²æ¿ª¹Ø¡¢ÔÊĞíPCA0-2

  IT01CF=0X76;     //int0=p0.6 int1=p0.7 µÍµçÆ½ÓĞĞ§
  osc_init();      //ÖØÖÃÕñµ´Æ÷
  vdd_rst_init();  //ÖØÖÃvdd¼à¿Ø¼°¸´Î»Ô´¼°¹Ø±ÕÎÈÑ¹Æ÷
}


//-------------------------------------
//Õñµ´Æ÷³õÊ¼»¯
//----------------------------------
void osc_init(void)
{ 
  OSCICN=0X83;   //Ê¹ÄÜÄÚ²¿Õñµ´Æ÷²»·ÖÆµ  12mhz
  OSCLCN=0;      //&=~0X80; //¹Ø±ÕÄÚ²¿µÍÆµÕñµ´Æ÷
  OSCXCN=0;      //¹Ø±ÕÍâ²¿Õñµ´Æ÷
  CLKMUL=0;      //¹Ø±Õ³Ë·¨Æ÷
  CLKSEL=0;      //USBÎª4±¶³Ë·¨Æ÷£¬ÄÚ²¿Õñµ´Æ÷×÷ÎªÏµÍ³Ê±ÖÓ
  CKCON=0;       //¶¨Ê±Æ÷Ê±ÖÓÔ´²»ÊÇÏµÍ³Ê±ÖÓÓÉ¿ØÖÆÆ÷¼Ä´æÆ÷Ñ¡Ôñ
}	
		
//----------------------------------------------
//---bcd×ª»»
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
//---¹Ø±Õ´¥·¢¡¢Çå´¥·¢±êÖ¾
//--------------------------------
void chufa_off(void)
{  SPRING_A=0; SPRING_B=0; SPRING_C=0;
   SPRING_AF=0;SPRING_BF=0;SPRING_CF=0;	 //´¥·¢¶Ë¿ÚÖÃÎŞĞ§
}
	   
