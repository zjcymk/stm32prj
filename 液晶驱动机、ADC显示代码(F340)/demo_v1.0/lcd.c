
//---------------------------------------
//LCD使用字符串数组定义
//---------------------------------------
#define HM_N          1 //画面总数
#define SING_N        16  //每行的字符（英文）限制
#define LINE_N        4   //每屏幕的文字行数
#define FANB_FLAG	    1	  //增加1个指针，指向配置字节
#define PZZJ_WZ       4   //配置字节位置，在画面数组的位置
#define LCD_L         64  //液晶长像素
#define LCD_W         128 //液晶宽像素						  
#define SXH           2   //每次刷新图像的行数
		
		
#define LCD_SX_TIME   10   //lcd刷新时间1=10ms,
#define LCD_W_TIME    2    //lcd写入数据、命令间等待时间1=10us
#define LCD_CLR_TIME  160  //lcd清屏时间1=10us
#define SX_TIME       240  //背景图刷新时间1=2.5ms

//---------------lcd 命令 st7920芯片-----------------
#define LCD_CODE_W 0xf8  //写命令代码
#define LCD_CODE_R 0xfc  //读命令代码
#define LCD_DATA_W 0xfa  //写数据代码
#define LCD_DATA_R 0xfe  //读数据代码

//------------------------------------------------------------
//-----------------配置字说明
//--配置字组成
//--配置字节0、配置字节1、配置字节2、步长、上限高字节、上限低字节、
//--下限高字节、下限低字节(即是无数据也保留位置用0代替)
//--
//---不反白的行不作为选中项，光标跳过此行
//----------------------------------------------
//--画面配置字节0 
//--第7，6，5，4  保留
//--3-0指示本画面那一行为反白 1为需要反白
//--------------------------------------------
//--画面配置字节1  （一页最多只能有2个数据变量）
//--7-4位指示那一行需要写数据变量 7对应第一行lcd，发字符时遇到“*”字符
//--此位开始为修改单元数据，遇到“#”时停止，'#'位也为变量值，直接遇到‘#’
//---为1个字符
//--表示单个位显示。发完数据后继续发剩下的本行字符。如有一行以上需要显示
//--数据则对应其余的修改单元。

//--3-2位为那个数据变量不用code区的步长、上下限字节  3位对应第一个数据变量（如果3，2都为1 画面指针中
//--配置字节数组后面为中文字符串数组指针（个数在配置字节2中4-0确定），在后面就为步长、上下限指针，第2变量的步长、
//--上下限排在后面。
//------如：第3位为1则第1个数据变量不在code区提取步长、上下限,此时应在画面指针后面取它们的地址（需检查中文数组指针个数）。
//---排列：hm*_0,hm*_1,hm*_2,hm*_3,hm*_pz,中文数组指针1，......,中文数组指针n，第1变量步长地址，第1变量上限地址，
//---第1变量下限地址，第2变量步长地址，第2变量上限地址，第2变量下限地址。（无第2变量时则省略后3个地址）

//--1-0位  1对应第一个数据变量需要中文，
//--如1位为1代表此数据需要用中文代表(对应一个字符数组，包括数字和中英文)

//-----------------------------------------------------------------------------------
//--画面配置字节2（对应第一个数据变量，一个以上再增加字节如：“画面配置3.....”）
//--位7代表调整最大值需要中英文
//--位6代表调整最小值需要中英文
//--位5代表调整值全部都需要中英文
//--位4 代表中文个数超过15个，要读取配置自己3
//--位3-0 代表配置指针后还有几个指针（每个指针对应一个字符串）,此字符串用于数据变量的中文,
//--------第1个指针对应最大值，第2个指针对应最小值，第3个为中间值需要的数组，此字符数组完全替代数据调整行，
//--------如果7，6，5都为0则无这3个指针
//--------如：有最小值中文没有最大值中文用空字符数组代替，最小值始终在最大值后面。
//--------如：全部都需要中文则只有一个指针指向一个数组
//----------------------------------------------------
//-----配置字节1，2主要是对变量调整使用，画面无变量此配置字节都为0
//----------------------------------------------------------------------------------------------

//--------画面1,平均电流，电压，通讯地址----------------------
uchar code hm_0[]=  {"数据1：   *  #  "}; 
uchar code hm_1[]=  {"数据2:    *  #  "}; 
uchar code hm_2[]=  {"                "}; 
uchar code hm_3[]=  {"                "}; 
                  
uchar code hm_pz[]={0x00,0xc0,0x00};//1,2行有变量
uchar code *code hm[]={hm_0,hm_1,hm_2,hm_3,hm_pz}; 
uint *code hm_var[]={&axdl_bcd,&bxdl_bcd}; //电压

 	
//---------------画面显示指针需放在最后定义的字符文件结尾
uchar code ** code hm_pointer[]={hm};  

uint ** code hm_var_p[]={hm_var};
					
//------变量中的数值转字符数组-----------
uchar code v_s[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F',','};

//-----------------------------------------------------------------
//写LCD函数(ST7920驱动芯片串口连接)
//--------------------------------------
void delay()
{
 _nop_();_nop_();_nop_(); _nop_();  
 _nop_();_nop_();
}

void lcd_w(uchar i)
{ WDT(); 
  LCD_CS=0;	
  SCLK=0;
  LCD_CS=1;      //lcd有效
  delay();_nop_();_nop_(); _nop_(); //CS稳定后
	
  if((i&0x80)!=0)  SID=1;	        //判断第7位
  else             SID=0;
  SCLK=1;delay();
  SCLK=0;_nop_();
	
  if((i&0x40)!=0)   SID=1;	      //判断第6位
  else              SID=0;
	SCLK=1;delay();
  SCLK=0;_nop_();
	
  if((i&0x20)!=0)   SID=1;	      //判断第5位
  else              SID=0;
  SCLK=1;delay();
  SCLK=0;_nop_();

  if((i&0x10)!=0)   SID=1;	     //判断第4位
  else              SID=0;
  SCLK=1;delay();
  SCLK=0;_nop_();

  if((i&0x08)!=0)  SID=1; 	     //判断第3位
  else             SID=0;
  SCLK=1;delay();
  SCLK=0;_nop_();

  if((i&0x04)!=0)   SID=1;	     //判断第2位
  else              SID=0;
  SCLK=1;delay();
  SCLK=0;_nop_();

  if((i&0x02)!=0)   SID=1;	     //判断第1位
  else              SID=0;
  SCLK=1;delay();
  SCLK=0;_nop_();

  if((i&0x01)!=0)   SID=1;	     //判断第0位
  else              SID=0;
  SCLK=1;delay();
  SCLK=0;
	_nop_();_nop_();_nop_();_nop_();
  LCD_CS=0;    //lcd无效
}
 
//-------------------------------------
//LCD写命令函数(ST7920驱动芯片)
//-------------------------------------
void lcd_command_w(uchar comcode)
{
  lcd_w(LCD_CODE_W);  //写入写命令代码
  lcd_w(comcode&0xf0);
  lcd_w(comcode<<4);
}
//------------------------------------
//LCD写数据函数（ST7920驱动芯片）
//------------------------------------
void lcd_data_w(uchar comcode)
{
 lcd_w(LCD_DATA_W);  //写入写数据代码
 lcd_w(comcode&0xf0);
 lcd_w(comcode<<4);
}

//----------------------------------------
//LCD初始化上电到执行此函数应有50ms的延迟
//-----------------------------------------
void  lcd_init( void)
{ 
	WDT();	
  lcd_command_w(0x30); delay_10us(8);
  lcd_command_w(0x30); delay_10us(4);	 //选择基本指令
  lcd_command_w(0x0c); delay_10us(10);	 //开显示
  lcd_command_w(1);    delay_10us(900);  //清屏
  lcd_command_w(0x34); delay_10us(8);
  lcd_command_w(0x30); delay_10us(6);
}

//--------------------------------------
//字符串显示函数
//---------------------------------------
void string_display(uchar *s)
{ uchar js=0;

  while(*s>0&&js++<SING_N)
   { lcd_data_w(*s);
	 s++;
	 delay_10us(LCD_W_TIME);	
   }
}


/**********************************************************************************
函数功能： 画面刷新显示
入口数据： 画面序号hm_code
***********************************************************************************/
void hm_cl(uchar hm_code)
{
   uchar temp_pz,temp_pz2,i=0;
   uchar *p;

   lcd_command_w(0x30); delay_10us(LCD_W_TIME);	//选择基本指令	 
   lcd_command_w(0x0c); delay_10us(LCD_W_TIME);	 //开显示
   temp_pz =((hm_pointer[hm_code])[PZZJ_WZ])[1]; //提取配置字节1
   temp_pz2=((hm_pointer[hm_code])[PZZJ_WZ])[2]; //提取配置字节2

  //------判断lcd第0行-----------------------------
   if((temp_pz&1<<7)==0)   //配置字节1,第7位为0第0行无“变量”显示
    {lcd_command_w(0x80);delay_10us(LCD_W_TIME);
	   p=(hm_pointer[hm_code])[0];	
     string_display(p);
	  }	  
   else	 				  //本行有变量写入
   	{var_xr(hm_code,0,(hm_var_p[hm_code])[i]);	// 显示普通型16位 
	   i++;	//变量的数量     
	  }
	 
    //------判断lcd第1行 ----------------------------------------------------
   if((temp_pz&1<<6)==0) //配置字节1 第6位为0第1行无“变量”显示
     {lcd_command_w(0x90);delay_10us(LCD_W_TIME);
	    p=(hm_pointer[hm_code])[1];	  
      string_display(p);
	   }	
    else	             //本行有变量写入
   	 {var_xr(hm_code,1,(hm_var_p[hm_code])[i]);//都未写汉字，写入数值
		  i++; //变量个数加1     	  
	   } 
	 	  
    //-----判断lcd第2行-----------
   if((temp_pz&1<<5)==0)   //配置字节1 第5位为0第2行无“变量”显示
     {lcd_command_w(0x88);delay_10us(LCD_W_TIME);
	    p=(hm_pointer[hm_code])[2];	  //提取画面字符的地址
      string_display(p);
	   }	
    else	              //本行有变量写入  
     {var_xr(hm_code,2,(hm_var_p[hm_code])[i]);//都未写汉字，写入数值
		  i++;  //变量个数加1
	   }							   	   
	   
	  //-----判断lcd第3行-----------	  
	  if((temp_pz&1<<4)==0)   //配置字节1 第4位为0第3行无“变量”显示
      {lcd_command_w(0x98);delay_10us(LCD_W_TIME);
	     p=(hm_pointer[hm_code])[3];	  
       string_display(p);
	   }	//本行没有变量直接写字符
    else	
     {var_xr(hm_code,3,(hm_var_p[hm_code])[i]);//都未写汉字，写入数值
		  i++; //变量个数加1
	    } 			 					  	  
 }

//-------------------------------------------------
//--变量写入函数将变量放到适当位置
//------------------------------------------
void var_xr(uchar hm_code,uchar hm_line,uint *var)
{ uchar i,js;
  uchar *p;
  bit   var_zt;

  p=(hm_pointer[hm_code])[hm_line];	  //提取画面字符的地址
  i=0;var_zt=0;js=0;
  while(*p!='#'&&js++<SING_N)//计算变量所占位数,并限定循环次数防止死循环
	{if(*p=='*') var_zt=1;
	 if(var_zt==1) i++;
	 p++;
	} 
	i++;	        //补上缺少的一个，变量个数在i中
	//-----------------------------------
  lcd_command_w(0x20); delay_10us(LCD_W_TIME); //基本指令
	if(hm_line==0)	{lcd_command_w(0x80);delay_10us(LCD_W_TIME);} //对应第一行第一位（80地址）
	else if(hm_line==1)	{lcd_command_w(0x90);delay_10us(LCD_W_TIME);}
	else if(hm_line==2)	{lcd_command_w(0x88);delay_10us(LCD_W_TIME);}
	else if(hm_line==3)	{lcd_command_w(0x98);delay_10us(LCD_W_TIME);}
	else {}
		
	p=(hm_pointer[hm_code])[hm_line];	  //提取画面字符的地址
	var_zt=0;js=0;
	while(*p>0&&js++<SING_N)  //检查字符串尾部并限定循环次数防止死循环
    {if((*p=='*'||*p=='#')&&i!=0) var_zt=1;//遇到*、#（变量为1位）开始写变量
		 if(var_zt==1) //写变量
		   {switch(i)
			   {case 4:  lcd_data_w(v_s[(*var&0xf000)>>12]); break;
				  case 3:  lcd_data_w(v_s[(*var&0x0f00)>>8]);  break;
				  case 2:  lcd_data_w(v_s[(*var&0xf0)>>4]);	  break;
				  case 1:  lcd_data_w(v_s[*var&0xf]);		  break;
				  default:	 ;
				 }
			  delay_10us(LCD_W_TIME);
			  if(--i==0) var_zt=0;
			 }
		 else	   //未碰到变量
		  {lcd_data_w(*p);delay_10us(LCD_W_TIME);} //写字符     
	  p++;	  	
	 }
}  