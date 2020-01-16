#include <asf.h>
#include "sam.h"
#include <pio.h>
#include <interrupt.h>
#include <delay.h>
#include "sam3u_ek.h"
#include "conf_board.h"
#include "conf_clock.h"
#include "board.h"
#include "pmc.h"
#include "sleepmgr.h" //sleep manager
#include "sysclk.h" //system clock
#include "udc.h" // USB device stack
#include <string.h>

void systemStart(){
	board_init();
	sysclk_init();
	irq_initialize_vectors(); //�������������� ������� ����������
	cpu_irq_enable(); //��������� ����������
	sleepmgr_init(); //�������������� sleep manager
	sysclk_init(); //�������������� clock service
	udc_start(); //��������� USB ����
	udc_attach(); //�������������� � �����
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

void setupUart(){
		uint32_t received_data, data_to_transmit;
		//PIO_Configure(PIOA, PIO_PERIPH_A, PIO_PA8 | PIO_PA9 , PIO_DEFAULT);
		
		/* Enable the pull up on the Rx and Tx pin */
		PIOA->PIO_PUER = PIO_PA8 | PIO_PA9; 

		/*�������� UART, ������� �� ���� ������������*/
		pmc_enable_periph_clk(ID_UART);
		
		/* ��������� DMA ��� ����� � ��� �������� */
		UART->UART_PTCR = UART_PTCR_RXTDIS | UART_PTCR_TXTDIS;
		
		// Reset and disable receiver and transmitter
		UART->UART_CR = UART_CR_RSTRX | UART_CR_RSTTX | UART_CR_RXDIS | UART_CR_TXDIS;
		
		// Configure mode
		uint32_t dwMode = US_MR_CHRL_8_BIT | US_MR_NBSTOP_1_BIT | UART_MR_PAR_NO;
		uint32_t modeReg = dwMode & 0x00000E00;

		UART->UART_MR = modeReg;

		// Configure baudrate (asynchronous, no oversampling)
		uint32_t dwBaudRate = 9600;
		UART->UART_BRGR = (SystemCoreClock / dwBaudRate) >> 4;

		/*������������� ����������*/
		/*��������� ��*/
		UART->UART_IDR = 0xFFFFFFFF;
  
		/*�������� ������ ��� ����������. ���� �������� � ������ ����� �������, ��
		�������� ���������� �� �����*/
		#ifndef USE_POLLING
		UART->UART_IER = UART_IER_RXRDY | UART_IER_OVRE | UART_IER_FRAME;
		#endif
  
		NVIC_EnableIRQ(UART_IRQn);
		/*�������� �������� � ����*/
		 UART->UART_CR = UART_CR_RXEN | UART_CR_TXEN;
}

void uartRecieve(){
	//���� �������� ��������� ���� �� UART - ��������� ��� � ���������� received_data
	
	/*if ((UART->UART_SR & UART_SR_RXRDY) ==  UART_SR_RXRDY )
	{
		received_data = UART->UART_RHR;
		
	}*/
	
}

void uartTransmit(){
	//���� ������ ��������� ��������� ���� (�������� � data_to_transmit) ����� UART - ������� � ������� UART_THR
	/*if ((UART->UART_SR & UART_SR_TXRDY) == UART_SR_TXRDY)
	{
		UART->UART_THR = 5;
		//received_data;
		//data_to_transmit;
	}*/
}



int main(void) {
	
	systemStart();
	

	while(1){		
		
	 }	 
	return 0;
 }
 
 void myTest() {
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
	 /*udi_cdc_getc(); //- ���������� ������, ��������� � ��
		udi_cdc_putc(); //- ���� ������ �� ��
		udi_cdc_write_buf();// - ���� ���������� ������ (��������, ������) �� ��
		udi_cdc_read_buf(); //- ������ ������, ��������� � ��, � ��������� �� � �����.*/
		//udi_cdc_write_buf("congratulation!!!\n", sizeof("congratulation!!!\n")); 
 }
	 