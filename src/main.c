#include <asf.h>
#include <pio.h>
#include <interrupt.h>
#include <delay.h>
#include "sam.h"
#include "led.h"
#include "sam3u_ek.h"
#include "conf_board.h"
#include "conf_clock.h"
#include "board.h"
#include "pmc.h"
#include "sleepmgr.h" //sleep manager
#include "sysclk.h" //system clock
#include "udc.h" // USB device stack
#include <string.h>

#include "openssl/ec.h"     // for EC_GROUP_new_by_curve_name, EC_GROUP_free, EC_KEY_new, EC_KEY_set_group, EC_KEY_generate_key, EC_KEY_free
//#include <openssl/ecdsa.h>   // for ECDSA_do_sign, ECDSA_do_verify
#include <openssl/obj_mac.h> // for NID_secp192k1
//#include "cryptopp/rsa.h"

#define MSG_BUF 512

void systemStart(){
	SystemInit();/* Initialize the SAM system */
	board_init();
	irq_initialize_vectors(); //инициализируем вектора прерываний
	cpu_irq_enable(); //разрещаем прерывания
	sleepmgr_init(); //инициализируем sleep manager
	sysclk_init(); //инициализируем clock service
	udc_start(); //запускаем USB стек
	udc_attach(); //подсоединяемся к хосту
}

void blinkGreen(){
	LED_On(LED1_GPIO);
	delay_ms(1000);
	LED_Off(LED1_GPIO);
	delay_ms(1000);
}

void blinkRed(){
	LED_On(LED0_GPIO);
	delay_ms(1000);
	LED_Off(LED0_GPIO);
	delay_ms(1000);
}

 void ledTest() {
	 char *msg;
	 
	 switch(udi_cdc_getc()){
		 case '1':
		 msg="green\r\n";
		 udi_cdc_write_buf(msg, strlen(msg)*sizeof(*msg));
		 blinkGreen();
		 break;
		 case '2':
		 msg="red\r\n";
		 udi_cdc_write_buf(msg, strlen(msg)*sizeof(*msg));
		 blinkRed();
		 break;
		 default:
		 msg="wrong\r\n";
		 udi_cdc_write_buf(msg, strlen(msg)*sizeof(*msg));
		 break;
	 }
	 /*udi_cdc_getc(); //- возвращает символ, пришедший в МК
		udi_cdc_putc(); //- шлет символ из МК
		udi_cdc_write_buf();// - шлет содержимое буфера (например, строку) из МК
		udi_cdc_read_buf(); //- читает строку, пришедшую в МК, и сохраняет ее в буфер.
		udi_cdc_write_buf("congratulation!!!\n", sizeof("congratulation!!!\n")); */
 }

void textTest(){
	/*iram_size_t n_rec = udi_cdc_get_nb_received_data()+2;
		char buf[n_rec];		
		udi_cdc_read_buf(buf, n_rec-2);	
		strcat(buf,"\r\n");
		udi_cdc_write_buf(buf, n_rec);*/
		
		iram_size_t n_rec = udi_cdc_get_nb_received_data();
		uint8_t buf[n_rec];
		udi_cdc_read_buf(buf, n_rec);
		udi_cdc_write_buf(buf, n_rec);		
}



int main1 (void){
	systemStart();
	//unsigned char *hash = "c78e4d4490525662e8e11248a589916a726333b1d151b0b19f2fd67a92a078a4";
	char *msg;
	int counter=0;
	while (1)
	{		
		switch(udi_cdc_getc()){
			case '1':
			msg="green\r\n";
			udi_cdc_write_buf(msg, strlen(msg)*sizeof(*msg));
			blinkGreen();
			break;
			case '2':
			msg="red\r\n";
			udi_cdc_write_buf(msg, strlen(msg)*sizeof(*msg));
			blinkRed();
			break;
			default:
			msg="wrong\r\n";
			udi_cdc_write_buf(msg, strlen(msg)*sizeof(*msg));
			break;
		} 
	}
}
