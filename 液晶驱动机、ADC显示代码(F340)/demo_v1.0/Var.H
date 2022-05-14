
//---------------- ĞÂ¶¨ÒåÊı¾İÀàĞÍ ---------------------------------
typedef unsigned char   uchar;
typedef unsigned int    uint;
typedef unsigned long   ulong;


union w_b
{uint  data_w;
 uchar data_b[2];
};    

/*---------------------ºê¶¨Òå --------------------------------*/ 
#define WDT() {PCA0CPH4=0;PSCTL=0;}  ////¿´ÃÅ¹·Ë¢ĞÂºê¶¨Òå ¹Ø±Õ¶ÔflashÑ°Ö·

//-------·ûºÅ¼°³£Á¿¶¨Òå----------------------------------
//----·ÃÎÊÄÚ²¿XDATAºÍÊÇ·ñ¹Ø±ÕALEÑ¡Ôñ---------
#define EM_DATA 3  //Íâ²¿¼Ä´æÆ÷¿ØÖÆ¼Ä´æÆ÷µÄĞ´ÈëÊı¾İ

//--------ÖĞ¶ÏÓÅÏÈ¼¶------------------------------------
#define ZD_IP2      0
#define ZD_IP1      0x80  //0x10
#define ZD_IP0      0     //0x2B   

//---------Î»¶¨Òå------------------------------------------
sbit SPRING_A  = P0^2;         //A+Ïà´¥·¢¶Ë¿Ú	
sbit SPRING_AF = P0^5;         //A-			
sbit SPRING_B  = P0^1;         //B+Ïà´¥·¢¶Ë¿Ú	
sbit SPRING_BF = P0^4;         //B-				
sbit SPRING_C  = P0^0;         //C+Ïà´¥·¢¶Ë¿Ú	
sbit SPRING_CF = P0^3;		     //C-				

//sbit AXDY_TB  = P1^5; //µçÑ¹Í¬²½
sbit BXDY_TB  = P1^0;		//ABµçÑ¹Í¬²½	
sbit CXDY_TB  = P0^7;	  //ACµçÑ¹Í¬²½	

sbit AXDL_TB  = P0^6;   //aµçÁ÷Í¬²½		
sbit BXDL_TB  = P1^1;		//bµçÁ÷Í¬²½	
sbit CXDL_TB  = P1^2;		//cµçÁ÷Í¬²½		

sbit ERR_JDQ  = P2^4;       //¹ÊÕÏ¼ÌµçÆ÷
sbit PL_JDQ   = P2^5;       //ÅÔÂ·¼ÌµçÆ÷
sbit RUN_JDQ  = P2^6;       //ÔËĞĞ¼ÌµçÆ÷
sbit STP      = P2^7;       //Æô/Í£ÊäÈë   

sbit BH1  = P3^0;       //±£»¤1ÊäÈë		
sbit BH2  = P3^4;       //±£»¤2ÊäÈë	
					    
sbit JYDZ     = P2^2;       //²âÂ©µç		
sbit LEAK_PCT=  P2^3;       //Â©µç±£»¤¼ÌµçÆ÷Êä³ö	

sbit RMS_CON  = P1^7;       //RMS3485¿ØÖÆ¶Ë 0:·¢ËÍ  1£º½ÓÊÕ  
//sbit RMS_TXD  = P2^0;  
//sbit RMS_RXD  = P2^1;  	 

//-----------¼üÅÌ----------------------------
sbit ENT     = P3^1;
sbit DOWN    = P3^2;		
sbit UP      = P3^3;	
	
//-----------lcdÇı¶¯½Ó¿Ú ------------------------------------
sbit SCLK =   P1^6;  //lcd´®ĞĞÊ±ÖÓ		    
sbit SID  =   P1^3;  //lcd´®ĞĞÊı¾İ		    
sbit LCD_CS  =P1^4;  //LCDÆ¬Ñ¡¶Ë ¸ßµçÆ½ÓĞĞ§ 

//-----------³Ì¿Ø·Å´óÆ÷£¬¶àÂ·¿ª¹Ø½Ó¿Ú--------------------------------
sbit CS4053  =P3^7;   //2Ñ¡1¶àÂ·¿ª¹Ø Ñ¡Í¨¶Ë  G
sbit TD4053  =P3^6;   //Ñ¡Ôñ4053µÄÍ¨µÀ	     S


/*--------------------------È«¾Ö±äÁ¿ ----------------------------*/
bit    re_hf;           //ÈÈ»Ö¸´±êÖ¾ Îª1ÈÈÆô¶¯ Æô¶¯ºÍÈíÍ£ÓÃ
bit    up_power;        //ÏµÍ³ÉÏµç±ê¼Ç£¬ÀäÆô¶¯´¦ÀíÊ±ÖÃÎ»ÔÚ½øÈë¼üÅÌ´¦ÀíÄÚÇå³ı
bit    cfmc_jx;         //´¥·¢Âö³å¼«ĞÔ

uchar  data  zt;         //×´Ì¬55Í£³µ5aÆô¶¯
uint   data  pj_dl[2];   //ÈıÏàÆ½¾ù·åÖµµçÁ÷£¬
uint   data  axdl_jz,bxdl_jz;       //Ğ£×¼ºóµÄµçÁ÷Öµ
uint   data  axdl_bcd,bxdl_bcd;     //µçÁ÷ÏÔÊ¾ÓÃ
uchar  data  adc_amp;               //ÔË·ÅÔöÒæ×´Ì¬¼Ä´æÆ÷ 0 ,2±¶   1, 4±¶   2 ,8±¶
uchar  data  adc_t3;                //t3ÖĞ¶ÏÓÃ
uchar  data  time_2500us;           //2.5ms
uint   xdata ax_dl[40],bx_dl[40];   //²ÉÑùµçÁ÷Öµ
uint   xdata axdl_xs[8],bxdl_xs[8];   //ÏÔÊ¾ÎÈ¶¨ÓÃ

/*--------º¯ÊıËµÃ÷----------------------------*/
void jisuan(void);      //Êı¾İ¼ÆËã
void re_pd(void);       //ÈÈ¸´Î»ÅĞ¶Ï
void leng(void);        //ÀäÆô¶¯³õÊ¼»¯
void stop_cl(void);     //Í£³µ´¦Àí
void start_cl(void);    //Æô¶¯´¦Àí
uint  adc_start(uchar td,uchar mode); //Æğ¶¯adc²¢È¡½á¹û  Èë¿Ú£ºÍ¨µÀºÅ,¶ÔÆë·½Ê½
extern void tzd(void);   //retiÖ¸Áî
void wdt_initi(void);   //¿´ÃÅ¹·³õÊ¼»¯
void vdd_rst_init(void);  //vdd¼àÊÓÆ÷³õÊ¼»¯¼°¸´Î»Ô´¼°¹Ø±ÕÎÈÑ¹Æ÷
void delay_10us(uint t);  //ÑÓ³Ùº¯Êı
void port_initi(void);    //IO¿Ú³õÊ¼»¯
void port_cz(void);       //¶Ë¿ÚÖØÖÃ
void osc_init(void);    //Õñµ´Æ÷³õÊ¼»¯
void lcd_w(uchar i);    //Ğ´LCDº¯Êı(ST7920Çı¶¯Ğ¾Æ¬´®¿ÚÁ¬½Ó)
void lcd_command_w(uchar comcode); //LCDĞ´ÃüÁîº¯Êı(ST7920Çı¶¯Ğ¾Æ¬)
void lcd_data_w(uchar comcode);    //LCDĞ´Êı¾İº¯Êı£¨ST7920Çı¶¯Ğ¾Æ¬
void string_display(uchar  *s); //×Ö·û´®ÏÔÊ¾º¯Êı
void lcd_init (void);		//lcd³õÊ¼»¯
void hm_cl(uchar hm_code); //»­Ãæ´¦Àíº¯Êı£
void var_xr(uchar hm_code,uchar hm_line,uint *var); //±äÁ¿Ğ´Èëº¯Êı½«±äÁ¿·Åµ½ÊÊµ±Î»ÖÃ,²¢Ö¸Ïò¶ÔÓ¦±äÁ¿
uint bcd_tran(uint value);    //bcd×ª»»
void chufa_off(void);//¹Ø±Õ´¥·¢¡¢Çå´¥·¢±êÖ¾

//--------------ADCÓÃ²ÎÊı---------------------
#define LEFT  0X2C   //adc×ó¶ÔÆë
#define RIGHT 0X28   //ADCÓÒ¶ÔÆë
#define AX_ADC   0x0d    //AÏàµçÁ÷ADCÍ¨µÀºÅ	 P4.4
#define BX_ADC   0x0b    //BÏàµçÁ÷ADCÍ¨µÀºÅ	 P4.0