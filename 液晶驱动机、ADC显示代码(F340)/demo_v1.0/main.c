
/*-----------------07延时程序--------------
C8051F340，系统时钟为12M
功能1：实现三相交流调压
功能2：采集A,B两相ADC数据，通过LCD显示
*?-----------------------------------------*/
#include "c8051f340.h"
#include <INTRINS.H>
#include "var.h"
#include "lcd.c"
//-------------------主程序---------------------
void main(void)               
{ 
  EA=0; EMI0CF=EM_DATA;
  PCA0MD=0;        //关闭WDT 
  osc_init();      //振荡器初始化 
  tzd();tzd();     //复位中断
  wdt_initi();	   //看门狗初始化函数
  vdd_rst_init();  //初始化复位源、起动时钟丢失检测、vdd监视复位   
  port_initi();    //io口初始化
  chufa_off();  //关闭端口、清触发标志 
  if((RSTSRC&1<<PORSF)==0)	  re_hf=1;   //置热恢复标志
  else                       {zt=0x55;re_hf=0;PCON=0;}  // 冷启动

  if(re_hf==0&&zt==0x55)  leng();  //冷启动\lcd初始化
		
  while(1)
  {switch(zt)
	 {case 0x5a: start_cl(); break; //调压状态  
	  default:   stop_cl(); 	      //停车状态
	 }             
  }
}


//-----------------------------------
/*--------a相电流同步中断--------------*/                                   
void int0(void) interrupt 0        
{
		
		
}
//----------------------------------------
/*--------空--------------*/
void int1(void) interrupt 2         
{ 
  
}

/*--------a相定时器中断------------*/  //定时器0
void time_ax(void) interrupt 1  
{ 
	
	

}                                
/*--------b相定时器中断------------*/  //定时器1
void time_bx(void) interrupt 3  
{
	
}


/*--------c相定时器中断------------*/ //定时器2
void time_cx(void) interrupt 5  
{ 
	
}


/*--------pca中断a,b,c电压同步及------
----------c电流同步---------*/	  //PCA中断
void pca(void)  interrupt 11       
{if(_testbit_(CCF2))  //C相电流同步
  {

  }
//-------------------------------------------------------------
 else if(_testbit_(CCF0))  //B相电压同步(其它以此为基准、下降沿有效）
  {

  }
//-------------------------------------------------------------
 else if(_testbit_(CCF3))  //3.333ms定时
  {

  }
//---------------------------------------------------------
 else if(_testbit_(CCF1)) 	 //b相电流同步
  {
		
  } 
 else 
	{}  //都不是返回
}


//——-------t3定时中断 触发脉冲列-------------
void ds_1ms(void) interrupt 14     //110us+57us定时*6=1ms
 {  
   uchar data temp;
   static uchar data vp_js,yc,dl_max,adc_js;
   static uint data dl_a,dl_b;
	 

   if(cfmc_jx==1)	//脉冲串输出高电平110us，下次进入周期166.6us
	   {cfmc_jx=0;
			
	  //--起动adc判断-----------------
		if(adc_t3>39)    adc_t3=0;  //数组变量
		if(++adc_js>5)   adc_js=0;
		 switch(adc_js)	   //每次167us执行一个
			   {case 0:     
   					 dl_a=adc_start(AX_ADC,RIGHT); //采样A电流
			   	   dl_b=adc_start(BX_ADC,RIGHT); //采样B电流
			   	  break;		   	            
				 case 1: 
				 	  break;
				 case 2:   //---找最大值---------------
					    temp=dl_a>>8;
				 	    if(temp>dl_max)  dl_max=temp;
				 	    temp=dl_b>>8;
				 	    if(temp>dl_max)  dl_max=temp;  		  
						  break	; 	  
				 case 3: 
				    switch(adc_amp)   //---------电流放入队列--------------------
			         {case 0:
								   ax_dl[adc_t3]=dl_a<<2;//2倍增益时数据放大
							     bx_dl[adc_t3]=dl_b<<2;   	
							     break;  
				        case 1:
									 ax_dl[adc_t3]=dl_a<<1;//4倍增益时数据放大
							     bx_dl[adc_t3]=dl_b<<1;
							     break;  
				        case 2:
									ax_dl[adc_t3]=dl_a;//8倍增益不变	
							    bx_dl[adc_t3]=dl_b;
							    break;       	  	  
				 	      default: ;
							 }			
				 		 break;	
				 case 4:
					  if(++vp_js>20)   //20ms增益改变一次 峰值保持清零
				       { //----判断增益 增加还是减少----
					      if(dl_max>=3)
						       {if(--adc_amp>0x80) adc_amp=0;}//大于0x300减小增益
						    else if(dl_max<1)
						       {if(++adc_amp>2)    adc_amp=2;}	//小于0x100加大增益 
						    else  ;
					        //----调整增益---- G=0,S=0---4倍
					        //---------------------G=0,S=1---2倍
					        //---------------------------G=1 S=1---8倍
					        switch(adc_amp)
			             {case 0:  CS4053=0; TD4053=1; break;   //2倍增益 //0,1
				            case 1:  CS4053=0; TD4053=0; break;   //4倍增益 //0,0  
				            case 2:  CS4053=1; TD4053=1; break;   //8倍增益 //1,1	  		   	  	  
				 	          default: adc_amp=0;CS4053=0; TD4053=1; //2倍增益 //0,1
						       }      
						      vp_js=0;dl_max=0;   //峰值保持清零		
						     }	               
						  break; 
				 case 5: adc_t3++;	 //1ms执行一次  
						 break;
						   				 	     
				 default: ;
			 } 
		  if(++yc>=15) {yc=0;time_2500us++;}
		  TMR3CN=0;
      TMR3RLL=0XD2;TMR3RLH=0XFF;	  //低电平定时46us（写入装载寄存器 中断后此值有效）
	    TMR3CN=4;		
	  }           	   
 else  //脉冲串输出低电平
	  {cfmc_jx=1;
	   TMR3CN=0;
     TMR3RLL=0X88;TMR3RLH=0XFF;	  //高电平定时120us （写入装载寄存器）
	   TMR3CN=4;
    }
 }

 
/*--------=---------------------------------
-----函数名称： jisuan
-----功能描述： 显示ADC采样
-----输    入： 无
-----输    出： 无
------------------------------------------*/  
void jisuan(void)
{ uchar  i;
  uint   data dl,temp;
  ulong  data temp_data;
	
//--------计算ab平均值及校准值-----------------------------------
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
  axdl_bcd=bcd_tran(temp/8);//显示A相ADC值
							  
  //----b----------------------			
  temp=0;
  for(i=1;i<=7;i++) {dl=bxdl_xs[i-1]=bxdl_xs[i];temp+=dl;}
  dl=bxdl_xs[7]=bxdl_jz;
  temp+=dl;
  bxdl_bcd=bcd_tran(temp/8);//显示B相ADC值
}
 

/*----------------------------------------
-----函数名称： leng
-----功能描述： 冷启动初始化
-----输    入： 无
-----输    出： 无
------------------------------------------*/ 
void leng(void)
{uchar i;
  //uint js; 
	
  EMI0CF=EM_DATA;
  port_cz();  //端口重置
  chufa_off();  //关闭端口、清触发标志
  EA=0;WDT();
  
  ERR_JDQ=1;   //初始化,故障触点闭合
  RUN_JDQ=0;      //初始化，运行断开
  PL_JDQ=0;       //初始化，旁路断开
  zt=0x55;
  
  //--清电流单元------
 for(i=0;i<40;i++)   ax_dl[i]=bx_dl[i]=0;
 axdl_jz=bxdl_jz=0;
 axdl_bcd=bxdl_bcd=0; 	
 re_hf=0;                   //清热恢复标志
 adc_amp=2;
 CS4053=1; TD4053=1; //运放8倍增益
 
 //-----先启动t3  产生负压 用7660后可去掉
 IE=0X80;EIE1=0X80;	//开T3,开全局
 TMR3CN|=4;         //重至t3运行
 //-------lcd初始化--------------
 for(i=0;i<6;i++)  {delay_10us(1000); WDT();}

 lcd_init(); WDT();//lcd初始化
 lcd_command_w(0x24);				//进入扩展指令
 delay_10us(LCD_W_TIME);
 lcd_command_w(0x30);              //返回基本指令	
}


/*------------------------------------------------------------------
-----函数名称： stop_cl
-----功能描述： 停车状态处理
-----输    入： 无
-----输    出： 无
--------------------------------------------------------------------*/
void stop_cl(void)
{uchar  js=0;
	                   //端口重置
  chufa_off();  //关闭端口、清触发标志
  time_2500us=0;
  do
   {EMI0CF=EM_DATA;
    port_cz();                       //端口重置
	  chufa_off();  //关闭端口、清触发标志
	  WDT();
    PCA0CPM0=PCA0CPM1=PCA0CPM2=PCA0CPM3=0;	//PCA关闭
	  TR0=TR1=TR2=0;					        //T0,1,2关闭
    zt=0x55;
	  IE=0X80; EIE1=0X80;//开T3，全局中断
	  TMR3CN|=4;         //T3运行
	
	  if(time_2500us>=30)  //80ms执行1次
	    {time_2500us=0;
	     jisuan();  
	    }
			
	 STP=1;
   if(STP==0)  {if(++js>20) zt=0x5a;} //起动调压开始
	 else        {js=0;} 
	 hm_cl(0);	
 }while(zt==0x55);         //判断启动	如果键盘状态不是主画面，不启动 ，故障优先退出		  
}


/*-----------------------------------------
-----函数名称： start_cl
-----功能描述： 起动状态处理
-----输    入： 无
-----输    出： 无
------------------------------------------*/
void start_cl(void)
{uchar js1,js2,js3;

	  //BXDY_TB=CXDY_TB=1;
	 // AXDL_TB=BXDL_TB=CXDL_TB=1;
	 // PCA0CN=0;
	 // PCA0CPL3=PCA0L-1;PCA0CPH3=PCA0H-1; //下次匹配时间放置最大，以免开中断后发生意外中断
   // PCA0MD=0;PCA0CPM0=0x11;PCA0CPM3=0x49;//初始化pca 
	 // TMR3RLL=0X3C;TMR3RLH=0XF6;TMR3CN=4;    //t3起动
   // PCA0CN=0;TR0=TR1=TR2=0;TCON=5;TMR2CN=0;
					
		time_2500us=0;	js1=js2=js3=0;
    //--- 启动循环 ------------------
    do
	   {EMI0CF=EM_DATA;
		  port_cz();  //端口重置
	    WDT();
      EIP2=ZD_IP2;EIP1=ZD_IP1;IP=ZD_IP0;  //中断优先级 T3最高
		  ESPI0=ES0=EX1=0;
	    zt=0x5a;  		 
 
      if(time_2500us>=30)  //80ms执行1次a计算
        { time_2500us=0;
			    jisuan(); //计算
          hm_cl(0);	
		    }
				
   //---------------向上，加压----------------
	 UP=1;
   if(UP==0&&DOWN==1)  {if(++js1>20) {}}
	 else                {js1=0;} 
	 
	 //---------------向下，减压----------------
	 DOWN=1;
   if(DOWN==0&&UP==1)  {if(++js2>20) {}}
	 else                {js2=0;} 
	 
  //--------------停止退出-------------
   if(STP)  {if(++js3>20) zt=0x55;}
	 else     {js3=0;} 
	}while(zt==0x5a);         //启动状态继续循环		                      
}

				
/*-----------------------------------------
-----函数名称： uint adc_start(uchar)
-----功能描述： adc采样
-----输    入： 通道号,采样等待时间1=1us左右
-----输    出： 转换结果
------------------------------------------*/ 
uint adc_start(uchar td,uchar mode)
{
 union  w_b adc_jg;
 uchar  js;
  
  REF0CN=3;     //选择内部基准、初始化端口时已选择
  AMX0P=td;     //选择通道
  AMX0N=0x1f;      //负端接地、单端转换
  ADC0CF=mode;  //0X28;  //2Mhz时钟、右对齐	 0x2c左对齐
  ADC0CN=0X80;  //ADC准备
   _nop_();_nop_();_nop_();_nop_();
   _nop_();_nop_();_nop_();_nop_();  //等待保持电容充电 约1us
  AD0BUSY=1;     //启动转换
  for(js=0;js<40;js++)
     {if(AD0INT==1) {adc_jg.data_b[0]=ADC0H;
	                 adc_jg.data_b[1]=ADC0L; break;} //转换完跳出	     
	  else ;	 
	  }

  return(adc_jg.data_w);
}


/*--------------------------------------
看门狗初始化函数
------------------------------------*/ 
void wdt_initi(void)
{
  PCA0MD=0;      //关闭WDT 
  PCA0CPL4=200;   //1/12时钟源时 1=256us  12mhz
  PCA0MD=1<<WDTE; //选择1/12时钟源	并启动看门狗
  PCA0MD|=1<<WDLCK;  //锁定看门狗
  _nop_();
  PCA0CPH4=0;		//更新
}



/*------------------------------------------
 vdd监控器初始化、及复位源选择函数
 内有延迟
-----------------------------------------*/	
void vdd_rst_init(void)
{
  VDM0CN|=0X80;   //使能VDD监控
  delay_10us(14); //140us延迟
  RSTSRC=1<<MCDRSF|1<<PORSF;//使能时钟丢失检测、上电复位（vdd）
  REG0CN=0X80;   //关闭稳压器
}

/*-------------------------------------------
   时间延迟函数	(12mhz时1=10us)
------------------------------------------*/ 
void delay_10us(uint t)
{
  uchar i;
  uint  j;
  for(j=0;j<t;j++)
    {
	  for(i=0;i<10;i++)	 //本循环约9.914us
	    { _nop_();_nop_();_nop_();_nop_();}
	}
}
   

/*--------------------------------------
//IO口配置更改 
--------------------------------------*/ 
void port_initi(void)	//V3.0
{ 
  P0MDIN= 0xff;    
  P0MDOUT=0x3f;   //P0.0~0.5(触发),推挽,P0.6,P0.7(同步输入),开漏
  P0SKIP= 0xff;   //全跳过
  P0=0xc0;        //P0.0~0.5，初始化低，NPN正驱

  P1MDIN =0Xdf;   
  P1MDOUT=0xd8;   //P1.0~P1.2(同步),开漏, P1.3 P1.4,P1.6(液晶驱动),推挽,P1.5(外参考)模拟 P1.7(CON)推挽
  P1SKIP =0Xf8;   //跳过P1.3~P1.7 , P1.0~1.2分配给PCA0~2
  P1=0xff;        

  P2MDIN =0Xff;   
  P2MDOUT=0x73;   //P2.0(TXD1),P2.1(RXD1)推挽 ,P2.2(漏电检测) P2.3 (漏电保护继电器)开漏,2.4~2.6继电器驱动推挽 P2.7启停 开漏
  P2SKIP =0x00;   //P2.0,P2.1分配给TX1,RX1
  P2=0X8f;		    //2.4,2.5,2.6继电器上电低
    
  P3MDIN =0Xff;  
  P3MDOUT=0Xe0;   //P3.0~P3.4 开关量输入,开漏  P3.5(铁电SCK) 推挽,P3.6,3.7 多路开关,推挽
  P3=0xff;		  

  P4MDIN =0X86;   
  P4MDOUT=0x86;   //P4.0,4.3~P4.6模拟 P4.1,P4.2,P4.7铁电 推挽
  P4=0xff;		
  
  XBR0=0;          
  XBR2=1;         //UART1连接 
  XBR1=0X43;      //使能交叉开关、允许PCA0-2

  IT01CF=0X76;    //int0=p0.6 int1=p0.7 低电平有效
  REF0CN=3;       //使能内部电压基准（已在起动adc时重置）
}

	

/*--------------------------------------
   端口及寄存器重置	  V3.0
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
  P4MDOUT=(P4MDOUT|0X83)&0x86;   //xxxxxxxxxxxxxxxxxxxxxxx	P4.2需要变化

  XBR0=0;          
  XBR2=1;          //UART1连接 
  XBR1=0X43;       //使能交叉开关、允许PCA0-2

  IT01CF=0X76;     //int0=p0.6 int1=p0.7 低电平有效
  osc_init();      //重置振荡器
  vdd_rst_init();  //重置vdd监控及复位源及关闭稳压器
}


//-------------------------------------
//振荡器初始化
//----------------------------------
void osc_init(void)
{ 
  OSCICN=0X83;   //使能内部振荡器不分频  12mhz
  OSCLCN=0;      //&=~0X80; //关闭内部低频振荡器
  OSCXCN=0;      //关闭外部振荡器
  CLKMUL=0;      //关闭乘法器
  CLKSEL=0;      //USB为4倍乘法器，内部振荡器作为系统时钟
  CKCON=0;       //定时器时钟源不是系统时钟由控制器寄存器选择
}	
		
//----------------------------------------------
//---bcd转换
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
//---关闭触发、清触发标志
//--------------------------------
void chufa_off(void)
{  SPRING_A=0; SPRING_B=0; SPRING_C=0;
   SPRING_AF=0;SPRING_BF=0;SPRING_CF=0;	 //触发端口置无效
}
	   
