#include "motor.h"
void Command_Recv()
{
	char* temp_command="0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000";//ÃüÁîÔÝ´æ×Ö·û´®
	int Mn_t,set_flag=0,speed;
		if(USART_RX_STA&0x8000)
		{
			USART_RX_STA=0;
			temp_command=strstr((const char *)USART1_RX_BUF,"G0");
			if(temp_command)
			{
				Mn_t=command2num(temp_command,"1M"," ");
				if(Mn_t!=0)
				{
				M1_t=Mn_t;
				M1_n=0;
				set_flag|=0x01;
				stop_flag&=0xfe;
				Mn_t=0;
				}
				Mn_t=command2num(temp_command,"2M"," ");
				if(Mn_t!=0)
				{
				M2_t=Mn_t;
				M2_n=0;
				set_flag|=0x02;
				stop_flag&=0xfd;
				Mn_t=0;
				}
				Mn_t=command2num(temp_command,"3M"," ");
				if(Mn_t!=0)
				{
				M3_t=Mn_t;
				M3_n=0;
				set_flag|=0x04;
				stop_flag&=0xfb;
				Mn_t=0;
				}
				Mn_t=command2num(temp_command,"4M"," ");
				if(Mn_t!=0)
				{
				M4_t=Mn_t;
				M4_n=0;
				set_flag|=0x08;
				stop_flag&=0xf7;
				Mn_t=0;
				}
				Mn_t=command2num(temp_command,"5M"," ");
				if(Mn_t!=0)
				{
				M5_t=Mn_t;
				M5_n=0;
				set_flag|=0x10;
				stop_flag&=0xef;
				Mn_t=0;
				}
				Mn_t=command2num(temp_command,"6M"," ");
				if(Mn_t!=0) 
				{
				M6_t=Mn_t;
				M6_n=0;
				set_flag|=0x20;
				stop_flag&=0xdf;
				Mn_t=0;
				}
				speed=command2num(temp_command,"FV","\n");
				set_speed(set_flag,speed);
				printf("OK\n");
				memset(USART1_RX_BUF,0,100);
			}
			else
			{
				temp_command=strstr((const char *)USART1_RX_BUF,"G203");
					if(temp_command)
					{
					stop_flag=0xff;
					M1_n=0;M2_n=0;M3_n=0;M4_n=0;M5_n=0;M6_n=0;
					printf("OK\n");
					memset(USART1_RX_BUF,0,100);
					}
					else
					{		
					temp_command=strstr((const char *)USART1_RX_BUF,"M200");
					if(temp_command)
							{
								if(stop_flag!=0xff)
									printf("OK V1\n");
								else
									printf("OK V0\n");
								
								memset(USART1_RX_BUF,0,100);
							}
						else
							{
								temp_command=strstr((const char *)USART1_RX_BUF,"G201");
								if(temp_command)
								{
									temp_command=strstr((const char *)USART1_RX_BUF,"D");
									if(*(temp_command+1)=='1')
									{
										HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);
										printf("OK\n");
									}
									else if(*(temp_command+1)=='0')
									{
										HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);
										printf("OK\n");
									}
								}
								else
									printf("ERROR\n");
							}
					}
			}
		}
}
int command2num(char* temp_command,const char* start_char,const char* end_char)
{
	char* temp_num="1M000000000";
	int num_len,i,Mn_t=0;
	temp_num=strstr((const char *)temp_command,start_char);
	char* temp_end;
		if(temp_num)
		{
			temp_end=strstr((const char *)temp_num,end_char);
			temp_num+=2;
			if(*(temp_num)=='-')
			{
				temp_num++;
				num_len=temp_end-temp_num;
				for(i=0;i<num_len;i++)
				{
					Mn_t+=(*(temp_num+i)-48)*pow(10,num_len-i-1);
				}
				Mn_t=(0-Mn_t);
			}
			else
			{
				num_len=temp_end-temp_num;
				for(i=0;i<num_len;i++)
				{
					Mn_t+=(*(temp_num+i)-48)*pow(10,num_len-i-1);
				}
			}
			return Mn_t;
		}
		return 0;
}
void set_speed(int set_flag,int speed)
{
	int i;
	if(speed>100)
		return;
		for(i=0;i<=5;i++)
		{
		if((set_flag&(0x01<<i))>>i)
			{
			M_v[i]=speed;
			}
		}
}
void Motor_Exec(void)
{
	if(!(stop_flag&0x01))
	{
		if(M1_t>0)
		{
		 __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, M_v[0]);
		 __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, 0);
		}
		else
		{
		 __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1,0);
		 __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, M_v[0]);			
		}
	}
	else
	{
		 __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, 0);
		 __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, 0);
	}
	
	if(!((stop_flag&0x02)>>1))
	{
		if(M2_t>0)
		{
		 __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, M_v[1]);
		 __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, 0);
		}
		else
		{
		 __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3,0);
		 __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, M_v[1]);			
		}
	}
	else
	{
		 __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, 0);
		 __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, 0);
	}
	
	if(!((stop_flag&0x04)>>2))
	{
		if(M3_t>0)
		{
		 __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, M_v[2]);
		 __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 0);
		}
		else
		{
		 __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1,0);
		 __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, M_v[2]);			
		}
	}
	else
	{
		 __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);
		 __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 0);
	}
	
	if(!((stop_flag&0x08)>>3))
	{
		if(M4_t>0)
		{
		 __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, M_v[3]);
		 __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, 0);
		}
		else
		{
		 __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3,0);
		 __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, M_v[3]);			
		}
	}
	else
	{
		 __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 0);
		 __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, 0);
	}
	
	if(!((stop_flag&0x10)>>4))
	{
		if(M5_t>0)
		{
		 __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, M_v[4]);
		 __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 0);
		}
		else
		{
		 __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1,0);
		 __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, M_v[4]);			
		}
	}
	else
	{
		 __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
		 __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 0);
	}
	
	if(!((stop_flag&0x20)>>5))
	{
		if(M6_t>0)
		{
		 __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, M_v[5]);
		 __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, 0);
		}
		else
		{
		 __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3,0);
		 __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, M_v[5]);			
		}
	}
	else
	{
		 __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 0);
		 __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, 0);
	}
	HAL_Delay(1);
}
void motor_tick_update(void)
{
	if(!(stop_flag&0x01))
	{
		if(M1_t>0)
		{
			if(M1_n<(M1_t-10))
			{
				M1_n+=10;
			}
			else
			{
				M1_n=0;
				stop_flag|=0x01;
			}
		}
		else if(M1_t<0)
		{
			if(M1_n>(M1_t+10))
			{
				M1_n-=10;
			}
			else
			{
				M1_n=0;
				stop_flag|=0x01;
			}
		}
	}
	if(!((stop_flag&0x02)>>1))
	{
		if(M2_t>0)
		{
			if(M2_n<(M2_t-10))
			{
				M2_n+=10;
			}
			else
			{
				M2_n=0;
				stop_flag|=0x02;
			}
		}
		else if(M2_t<0)
		{
			if(M2_n>(M2_t+10))
			{
				M2_n-=10;
			}
			else
			{
				M2_n=0;
				stop_flag|=0x02;
			}
		}
	}
	if(!((stop_flag&0x04)>>2))
	{
		if(M3_t>0)
		{
			if(M3_n<(M3_t-10))
			{
				M3_n+=10;
			}
			else
			{
				M3_n=0;
				stop_flag|=0x04;
			}
		}
		else if(M3_t<0)
		{
			if(M3_n>(M3_t+10))
			{
				M3_n-=10;
			}
			else
			{
				M3_n=0;
				stop_flag|=0x04;
			}
		}
	}
	if(!((stop_flag&0x08)>>3))
	{
		if(M4_t>0)
		{
			if(M4_n<(M4_t-10))
			{
				M4_n+=10;
			}
			else
			{
				M4_n=0;
				stop_flag|=0x08;
			}
		}
		else if(M4_t<0)
		{
			if(M4_n>(M4_t+10))
			{
				M4_n-=10;
			}
			else
			{
				M4_n=0;
				stop_flag|=0x08;
			}
		}
	}
	if(!((stop_flag&0x10)>>4))
	{
		if(M5_t>0)
		{
			if(M5_n<(M5_t-10))
			{
				M5_n+=10;
			}
			else
			{
				M5_n=0;
				stop_flag|=0x10;
			}
		}
		else if(M5_t<0)
		{
			if(M5_n>(M5_t+10))
			{
				M5_n-=10;
			}
			else
			{
				M5_n=0;
				stop_flag|=0x10;
			}
		}
	}
	if(!((stop_flag&0x20)>>5))
	{
		if(M6_t>0)
		{
			if(M6_n<(M6_t-10))
			{
				M6_n+=10;
			}
			else
			{
				M6_n=0;
				stop_flag|=0x20;
			}
		}
		else if(M6_t<0)
		{
			if(M6_n>(M6_t+10))
			{
				M6_n-=10;
			}
			else
			{
				M6_n=0;
				stop_flag|=0x20;
			}
		}
	}
}
