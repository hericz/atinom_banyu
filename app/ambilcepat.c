
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"


#ifndef __AMBILCEPAT__
#define __AMBILCEPAT__

#include "monita/monita_uip.h"
xTaskHandle hdl_ambilcepat;

#ifdef BOARD_KOMON_KONTER_3_0
	#include "../modul/GPIO/gpio.h"
#endif

#ifdef DATA_RANDOM
	extern float data_f[];
#endif

portTASK_FUNCTION(ambilcepat, pvParameters )	{
  	vTaskDelay(500);
  	
  	int loopambil=0;
	
	#ifdef PAKAI_SHELL
		printf(" Monita : Ambil cepat init !!\r\n");
  	#endif
  	
  	#if defined(PAKAI_I2C) && defined(PAKAI_TSC)
		unsigned char st_tsc=0;
		char a;
		int i;
		int c=0;
		
		if (setup_fma())	printf(" NO ack !\r\n");
		else	{			
			printf("Init TSC OK ack !!!\r\n");
			st_tsc = 1;
		}
  	#endif
  	
  	#ifdef PAKAI_GPS
		int hasil_gpsnya;
		awal_gps();
  	#endif
  	
  	#ifdef PAKAI_ADC
		extern unsigned char status_adcnya; 
  	#endif
  	
  	#ifdef PAKAI_GPIO_DIMMER
		//init_remang();
  	#endif
  	
  	#ifdef PAKAI_PM
		int almtSumber=0;
		int sPM=0;
		
		#ifdef AMBIL_PM
			printf("Init ambil PM ..-ambilcepat-..!!!\r\n");
			vTaskDelay(3000);
		#endif
  	#endif
  	
  	int waktunya=0;
  	char dataserial[50];
  	
  	vTaskDelay(50);
  	for(;;) {
		vTaskDelay(1);
		
		#ifdef DATA_RANDOM
			data_f[loopambil%10] = (float) ((rand() % 100));
			//printf("%d: data: %.1f\r\n", loopambil%10, data_f[loopambil%10]);
		#endif
		
		loopambil++;
		#ifdef PAKAI_GPS
			if (serPollGPS())	
			{
				hasil_gpsnya = proses_gps();
				#if 0
					info_gps(hasilGPS);
				#endif
			}
		#endif

		#ifdef PAKAI_PM
			#ifdef AMBIL_PM			// AMBIL_PM
				sedot_pm();
			#endif
		#endif
		
		#ifdef PAKAI_ADC
			if (status_adcnya) {
				proses_data_adc();
				//printf("proses adc ... %d !!\r\n", status_adcnya);
				
				#ifdef BOARD_KOMON_420_SAJA
					hitung_datanya();
				#endif
				
				simpan_ke_data_f();
			}
		#endif
		
		#ifdef BOARD_KOMON_KONTER
			if (loopambil%20==0) {		// 5x20 = 100
				hitung_rpm();
			}
			data_frek_rpm();
		#endif

		#ifdef BOARD_KOMON_KONTER_3_0
			if (loopambil%20==0) {		// 5x20 = 100
				hitung_rpm();
				data_frek_rpm();
			}
		#endif
	
		#ifdef PAKAI_I2C
			#if 0
			if (st_tsc) {
				if (loopambil%500==0) {	// 2*250: 500ms = 0.5 detik
					printf("__detik: %3d\r\n", c++);
					//baca_register_tsc();

					#if 1
					if (int_berganti() == 0) 
					{
						printf("disentuh\r\n");
					}
					else
						printf("HIGH\r\n");
						//vSerialPutString(0, "HIGH\r\n");
					#endif
				}
			}
			#endif
			#if 0
			if (st_tsc) {
				if (xSerialGetChar(1, &c, 0xFFFF ) == pdTRUE)
				{
					vSerialPutString(0, "Tombol ditekan = ");
					xSerialPutChar(	0, (char ) c);
					vSerialPutString(0, " \r\n");
					
					if ( (char) c == 's')
					{
						printf(" Set\r\n");
						/*
						if (i2c_set_register(0x68, 1, 8))
						{
							out(" NO ACK !!\r\n");
						}
						else
							out(" OK\r\n");
						*/
						
						if (setup_fma())
						{
							printf(" NO ack !\r\n");
						}
						else
							printf(" OK ack !\r\n");
							
						
					}
					else
					{
						printf("====\r\n");
						for (i=0;i<16; i++)
						{
							if (i == 8) printf("****\r\n");
							
							if (i2c_read_register(0x68, (0x50 + i), &a))
							{
								printf(" Read failed !\r\n");
							}
							else
							{
								printf(" Read OK =");
								a = a + '0';
								printf("%c", (char) a);
								printf(" \r\n");
							}
						}
							
							printf("KEY \r\n");
							if (i2c_read_register(0x68, 0x68, &a))
							{
								printf(" Read failed !\r\n");
							}
							else
							{
								printf(" Read OK =");
								a = a + '0';
								printf("%c", (char) a);
								printf(" \r\n");
							}
							//a = read_key();
							//a = a + '0';
							//xSerialPutChar(	0, (char ) a);
					}
				}
			}
			#endif

		#endif

		
		#ifdef PAKAI_GPIO_DIMMER
			//remangkan();
			/*
			loop_pwm++;
			
			#ifdef DEBUG_PWM_GPIO
			if (loop_pwm>500) {
				loop_pwm=0;
				blink_pwm=1-blink_pwm;
				if (blink_pwm) {
					FIO1SET = BIT(30);
				} else {
					FIO1CLR = BIT(30);
				}
			}
			#endif
			//*/
		#endif
	}

	#ifdef PAKAI_GPS
		deinit_gps();
	#endif
}

int nAmbilCepat=10;

#ifdef PAKAI_GPS
//	nAmbilCepat += 5;
#endif

void init_ambilcepat(void)	{
	xTaskCreate( ambilcepat, "ambilcepat_task", (configMINIMAL_STACK_SIZE * nAmbilCepat), NULL, tskIDLE_PRIORITY+3, ( xTaskHandle * ) &hdl_ambilcepat);
}

#endif

