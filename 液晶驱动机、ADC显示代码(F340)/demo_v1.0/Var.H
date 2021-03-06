
//---------------- 新定义数据类型 ---------------------------------
typedef unsigned char   uchar;
typedef unsigned int    uint;
typedef unsigned long   ulong;


union w_b
{uint  data_w;
 uchar data_b[2];
};    

/*---------------------宏定义 --------------------------------*/ 
#define WDT() {PCA0CPH4=0;PSCTL=0;}  ////看门狗刷新宏定义 关闭对flash寻址

//-------符号及常量定义----------------------------------
//----访问内部XDATA和是否关闭ALE选择---------
#define EM_DATA 3  //外部寄存器控制寄存器的写入数据

//--------中断优先级------------------------------------
#define ZD_IP2      0
#define ZD_IP1      0x80  //0x10
#define ZD_IP0      0     //0x2B   

//---------位定义------------------------------------------
sbit SPRING_A  = P0^2;         //A+相触发端口	
sbit SPRING_AF = P0^5;         //A-			
sbit SPRING_B  = P0^1;         //B+相触发端口	
sbit SPRING_BF = P0^4;         //B-				
sbit SPRING_C  = P0^0;         //C+相触发端口	
sbit SPRING_CF = P0^3;		     //C-				

//sbit AXDY_TB  = P1^5; //电压同步
sbit BXDY_TB  = P1^0;		//AB电压同步	
sbit CXDY_TB  = P0^7;	  //AC电压同步	

sbit AXDL_TB  = P0^6;   //a电流同步		
sbit BXDL_TB  = P1^1;		//b电流同步	
sbit CXDL_TB  = P1^2;		//c电流同步		

sbit ERR_JDQ  = P2^4;       //故障继电器
sbit PL_JDQ   = P2^5;       //旁路继电器
sbit RUN_JDQ  = P2^6;       //运行继电器
sbit STP      = P2^7;       //启/停输入   

sbit BH1  = P3^0;       //保护1输入		
sbit BH2  = P3^4;       //保护2输入	
					    
sbit JYDZ     = P2^2;       //测漏电		
sbit LEAK_PCT=  P2^3;       //漏电保护继电器输出	

sbit RMS_CON  = P1^7;       //RMS3485控制端 0:发送  1：接收  
//sbit RMS_TXD  = P2^0;  
//sbit RMS_RXD  = P2^1;  	 

//-----------键盘----------------------------
sbit ENT     = P3^1;
sbit DOWN    = P3^2;		
sbit UP      = P3^3;	
	
//-----------lcd驱动接口 ------------------------------------
sbit SCLK =   P1^6;  //lcd串行时钟		    
sbit SID  =   P1^3;  //lcd串行数据		    
sbit LCD_CS  =P1^4;  //LCD片选端 高电平有效 

//-----------程控放大器，多路开关接口--------------------------------
sbit CS4053  =P3^7;   //2选1多路开关 选通端  G
sbit TD4053  =P3^6;   //选择4053的通道	     S


/*--------------------------全局变量 ----------------------------*/
bit    re_hf;           //热恢复标志 为1热启动 启动和软停用
bit    up_power;        //系统上电标记，冷启动处理时置位在进入键盘处理内清除
bit    cfmc_jx;         //触发脉冲极性

uchar  data  zt;         //状态55停车5a启动
uint   data  pj_dl[2];   //三相平均峰值电流，
uint   data  axdl_jz,bxdl_jz;       //校准后的电流值
uint   data  axdl_bcd,bxdl_bcd;     //电流显示用
uchar  data  adc_amp;               //运放增益状态寄存器 0 ,2倍   1, 4倍   2 ,8倍
uchar  data  adc_t3;                //t3中断用
uchar  data  time_2500us;           //2.5ms
uint   xdata ax_dl[40],bx_dl[40];   //采样电流值
uint   xdata axdl_xs[8],bxdl_xs[8];   //显示稳定用

/*--------函数说明----------------------------*/
void jisuan(void);      //数据计算
void re_pd(void);       //热复位判断
void leng(void);        //冷启动初始化
void stop_cl(void);     //停车处理
void start_cl(void);    //启动处理
uint  adc_start(uchar td,uchar mode); //起动adc并取结果  入口：通道号,对齐方式
extern void tzd(void);   //reti指令
void wdt_initi(void);   //看门狗初始化
void vdd_rst_init(void);  //vdd监视器初始化及复位源及关闭稳压器
void delay_10us(uint t);  //延迟函数
void port_initi(void);    //IO口初始化
void port_cz(void);       //端口重置
void osc_init(void);    //振荡器初始化
void lcd_w(uchar i);    //写LCD函数(ST7920驱动芯片串口连接)
void lcd_command_w(uchar comcode); //LCD写命令函数(ST7920驱动芯片)
void lcd_data_w(uchar comcode);    //LCD写数据函数（ST7920驱动芯片
void string_display(uchar  *s); //字符串显示函数
void lcd_init (void);		//lcd初始化
void hm_cl(uchar hm_code); //画面处理函数�
void var_xr(uchar hm_code,uchar hm_line,uint *var); //变量写入函数将变量放到适当位置,并指向对应变量
uint bcd_tran(uint value);    //bcd转换
void chufa_off(void);//关闭触发、清触发标志

//--------------ADC用参数---------------------
#define LEFT  0X2C   //adc左对齐
#define RIGHT 0X28   //ADC右对齐
#define AX_ADC   0x0d    //A相电流ADC通道号	 P4.4
#define BX_ADC   0x0b    //B相电流ADC通道号	 P4.0