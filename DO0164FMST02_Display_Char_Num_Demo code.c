    	COL=280; //	COL---AMOLED横向像素(列数)
		ROW=456; // ROW---AMOLED纵向像素(行数)	
		
		
/*---------------------------------------------------------------------------
此函数用于设置DM“窗口”---GRAM
---------------------------------------------------------------------------*/
void AMOLED_Block_Write(u16 Xstart,u16 Xend,u16 Ystart,u16 Yend)
{	
	Xstart=Xstart+20;
	Xend=Xend+20;

SPI_CS=0;
	SPI_WriteComm(0x2a);   
	SPI_WriteData(Xstart>>8);
	SPI_WriteData(Xstart&0xff);
	SPI_WriteData(Xend>>8);
	SPI_WriteData(Xend&0xff);
SPI_CS=1;
	
SPI_CS=0;
	SPI_WriteComm(0x2b);   
	SPI_WriteData(Ystart>>8);
	SPI_WriteData(Ystart&0xff);
	SPI_WriteData(Yend>>8);
	SPI_WriteData(Yend&0xff);
SPI_CS=1;

SPI_CS=0;	
	SPI_WriteComm(0x2c);
SPI_CS=1;	

}



void SPI_WriteComm(u16 regval)
{ 
		SPI_1L_SendData(0x02);
		SPI_1L_SendData(0x00);
		SPI_1L_SendData(regval);
		SPI_1L_SendData(0x00);//delay_us(2);
}

void SPI_ReadComm(u16 regval)
{    
		SPI_1L_SendData(0x03);//
		SPI_1L_SendData(0x00);
		SPI_1L_SendData(regval);
		SPI_1L_SendData(0x00);//PAM
}


void SPI_WriteComm_QSPI(u16 regval)
{ 

		SPI_1L_SendData(0x32);//32  
		SPI_1L_SendData(0x00);
		SPI_1L_SendData(regval);
		SPI_1L_SendData(0x00);//delay_us(2);
}

void SPI_WriteData(u16 val)
{   
 unsigned char n;	
   for(n=0; n<8; n++)			
   {  
	if(val&0x80) SPI_SDA=1;/*SPI_SDA 为写进去的值*/
	else         SPI_SDA=0;
	 
	val<<= 1;

	SPI_SCL=0;//delay_us(2);
	SPI_SCL=1;
   }


}		


void Write_Disp_Data(u32 val)
{ 
	#ifdef Disp_Data_24bits
	  QSPI_WriteData(val>>16);
	#endif

	QSPI_WriteData(val>>8);
	QSPI_WriteData(val);
}

void QSPI_WriteData(u16 val)
{
		//SPI_CS=0; 			
		if(val&0x80) SPI_IO3=1;
		else  			 SPI_IO3=0;

		if(val&0x40) SPI_IO2=1;
		else  			 SPI_IO2=0;

		if(val&0x20) SPI_IO1=1;
		else   			 SPI_IO1=0;

		if(val&0x10) SPI_IO0=1;
		else  			 SPI_IO0=0;

		SPI_SCL=0;//delay_us(2);
		SPI_SCL=1;//delay_us(2);


		if(val&0x08) SPI_IO3=1;else SPI_IO3=0;

		if(val&0x04) SPI_IO2=1;else SPI_IO2=0;

		if(val&0x02) SPI_IO1=1;else SPI_IO1=0;

		if(val&0x01) SPI_IO0=1;else SPI_IO0=0;

		SPI_SCL=0;//delay_us(2);
		SPI_SCL=1;//delay_us(1);
}


void SPI_4wire_data_1wire_Addr(u16 First_Byte,u16 Addr)
{   
	SPI_1L_SendData(First_Byte);//
	SPI_1L_SendData(0x00);
	SPI_1L_SendData(Addr);
	SPI_1L_SendData(0x00);//PA
}


void SPI_4wire_data_4wire_Addr(u16 First_Byte,u16 Addr)
{
	SPI_1L_SendData(First_Byte);//
	QSPI_WriteData(0x00);
	QSPI_WriteData(Addr);
	QSPI_WriteData(0x00);//PA
}


void SPI_4W_DATA_4W_ADDR_START(void)
{
   SPI_4wire_data_4wire_Addr(0x12,0x2c);
}
		
	
void SPI_4W_DATA_1W_ADDR_START(void)
{
   SPI_4wire_data_1wire_Addr(0x32,0x2C);
}

void SPI_4W_DATA_1W_ADDR_END(void)
{	
	SPI_CS=0;	
	SPI_4wire_data_1wire_Addr(0x32,0x00);
	SPI_CS=1;	
}


//清屏函数   
//全屏显示单色
void AMOLED_Clear(u32 color)
{  	
	unsigned int i,j;  
	
	AMOLED_Block_Write(0,COL-1,0,ROW-1);
	
	SPI_CS=0;
	if(QSPI_DT==SPI_4W_DATA_1W_ADD){SPI_4wire_data_1wire_Addr(0x32,0x2C);}   
	if(QSPI_DT==SPI_4W_DATA_4W_ADD){SPI_4wire_data_4wire_Addr(0x12,0x2C);}//SH8601Z	

	for(i=0;i<COL;i++)
		for(j=0;j<ROW;j++)
			{
			Write_Disp_Data(color);
			}
			
		SPI_CS=1;
			
	//if(QSPI_DT==SPI_4W_DATA_1W_ADD){SPI_4W_DATA_1W_ADDR_END();}
}


//在指定区域内填充指定颜色
//区域大小:
//  (xend-xsta)*(yend-ysta)
void AMOLED_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color)
{ 
  unsigned int i,j; 
	
	AMOLED_Block_Write(xsta,xend,ysta,yend);

	SPI_CS=0;

	if(QSPI_DT==SPI_4W_DATA_1W_ADD){SPI_4wire_data_1wire_Addr(0x32,0x2C);}   
	if(QSPI_DT==SPI_4W_DATA_4W_ADD){SPI_4wire_data_4wire_Addr(0x12,0x2C);}//SH8601Z	   	

	for(i=xsta;i<xend;i++)
	for(j=ysta;j<yend;j++)
	Write_Disp_Data(color); 

	SPI_CS=1;
	
 // if(QSPI_DT==SPI_4W_DATA_1W_ADD){SPI_4W_DATA_1W_ADDR_END();}
} 



void AMOLED_DrawPoint(u16 x,u16 y,u16 frontcolor)//画点-----PEN_COLOR:此点的颜色
{
	if(x>=COL)
	   x=COL-1;
  if(y>=ROW)
     y=ROW-1;
	
	//AMOLED_Block_Write(x,x,y,y);  //最小开窗为4*4,开窗大小为2的整倍数
	
	Write_Disp_Data(frontcolor);	
							


}  

void AMOLED_Drawbigpoint(u16 x,u16 y)//画一个大点(2*2的点)
{
AMOLED_DrawPoint(x,y,PEN_COLOR);//中心点 
AMOLED_DrawPoint(x+1,y,PEN_COLOR);
AMOLED_DrawPoint(x,y+1,PEN_COLOR);
AMOLED_DrawPoint(x+1,y+1,PEN_COLOR);	
}


//在指定位置显示一个字符
//x:0~COL-6
//y:0~ROW-12
//num:要显示的字符:" "--->"~"
//size:字体大小 12/16
//mode:叠加方式(1)还是非叠加方式(0)
void AMOLED_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode)
{   
  u8 temp,t1,t;
	u8 pos;    
	u16 y0=y;
	u8 csize=(size/8+((size%8)?1:0))*(size/2);		//得到字体一个字符对应点阵集所占的字节数	
	
	//设置窗口		
	AMOLED_block_write(x,x+size/2-1,y,y+size-1);//设置窗口	
	
	num=num-' ';//得到偏移后的值
	for(t=0;t<csize;t++)
	{   
		if     (size==12)temp=asc_1206[num][t]; //调用1206字体
		else if(size==16)temp=asc_1608[num][t]; //调用1608字体
		else if(size==32)temp=asc_3216[num][t]; //调用3216字体		
		else if(size==48)temp=asc_4824[num][t]; //调用4824字体
		else return;	//没有的字库
		
		for(t1=0;t1<8;t1++)
		{			    
			if     (temp&0x80) AMOLED_DrawPoint(x,y,PEN_COLOR);
			else if(mode==0)   AMOLED_DrawPoint(x,y,PAPER_COLOR);
			temp<<=1;
		}  	 
	}  

}

//显示2个数字
//x,y:起点坐标
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~99);
void AMOLED_ShowNum(u16 x,u16 y,u16 num,u16 size,u16 mode)
{         							   
    AMOLED_ShowChar(x,y,(num/10)%10+'0',size,mode); 
    AMOLED_ShowChar(x+size/2,y,num%10+'0',size,mode); 
}

//显示4个数字
//x,y:起点坐标
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~9999);
void AMOLED_Show4Num(u16 x,u16 y,u16 num,u16 size,u16 mode)
{ 
  
AMOLED_ShowChar(x,y,(num/1000)+'0',size,mode);
AMOLED_ShowChar(x+size/2,y,(num/100)%10+'0',size,mode);      							   
AMOLED_ShowChar(x+size,y,(num/10)%10+'0',size,mode); 
AMOLED_ShowChar(x+size+size/2,y,num%10+'0',size,mode); 
}



//在指定位置开始显示一个字符串	    
//支持自动换行
//(x,y):起始坐标
//str  :字符串
//size :字体大小 默认为8*16
//mode:0,非叠加方式;1,叠加方式    		   
void AMOLED_Disp_Str(u16 x,u16 y,u16 size,u8*str)
{
	unsigned char bHz=0;     //字符或者中文  	 
	unsigned char mode=0;
	
    while(*str!=0)//数据未结束
    { 
        if(!bHz)
        {
	        if(*str>0x80)bHz=1;//中文 
	        else              //字符
	        {      
						if(x>(COL-size/2))//换行
							{				   
								y+=size;
								x=0;	   
							}							    
		        if(y>(ROW-size))break;//越界返回      
		        if(*str==13)//换行符号
							{         
								y+=size;
								x=0;
								str++; 
							}  
		        else DM_ShowChar(x,y,*str,size,mode);//有效部分写入 
				    str++; 
		        x+=size/2; //字符,为全字的一半 
	        }
        }					 
    }		
} 


	PEN_COLOR=RED;
	AMOLED_Clear(BLACK);//显示单色画面


	PEN_COLOR=WHITE;    
	AMOLED_Disp_Str(0,90,48,"1.64 inch AMOLED");	
	PEN_COLOR=GREEN;	
	AMOLED_Disp_Str(120,136,32,"280x456");		
	PEN_COLOR=BLUE;
	AMOLED_Disp_Str(120,180,48,"QSPI/MIPI");
	PEN_COLOR=GBLUE;
	AMOLED_Disp_Str(10,240,48,"www.dwo.net.cn");
