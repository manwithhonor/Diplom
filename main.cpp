#include <asf.h>
#include <interrupt.h>
#include <delay.h>
#undef min
#undef max
#include <string>
#undef min
#undef max
#include <string.h>
#include "sam.h"
#include "sam3u_ek.h"
#include "conf_board.h"
#include "conf_clock.h"
#include "board.h"
#include "sleepmgr.h" //sleep manager
#include "sysclk.h" //system clock
#include "udc.h" // USB device stack
#include "G:\diploma\backup\sectry\savetest\external\Bitcoin.h"
#include "G:\diploma\backup\sectry\savetest\external\PSBT.h"

//#include "stdafx.h"

using namespace std;
using std::string;

void systemStart();
void blinkGreen();
void blinkRed();
void ledTest();
void textTest();
void getNAdresses(string userMasterPubKey, int amountOfChildKeys);
void getNChildKey(string userMasterPubKey, int amountOfChildKeys);
void genMasterKeys();
void signTransaction();

int main (void){
	systemStart();
	char *msg;
	
	
	while (1)
	{	//getNAdresses
		// printAddresses("xpub6BoiLSuHTDytgQejDauyXkBqQ4xTw2tSFKgkHfmZky7jDQQecUKwbFocxZXSJMCSp8gFNBbD9Squ3etZbJkE2qQqVBrypLjWJaWUNmHh3LT");
		
		
			 			
	}
}


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
/*
	LED_On(LED1_GPIO);
	delay_ms(1000);
	LED_Off(LED1_GPIO);
	delay_ms(1000);*/
}

void blinkRed(){
	/*LED_On(LED0_GPIO);
	delay_ms(1000);
	LED_Off(LED0_GPIO);
	delay_ms(1000);*/
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
		
		/*udi_cdc_getc(); //- возвращает символ, пришедший в МК
		udi_cdc_putc(); //- шлет символ из МК
		udi_cdc_write_buf();// - шлет содержимое буфера (например, строку) из МК
		udi_cdc_read_buf(); //- читает строку, пришедшую в МК, и сохраняет ее в буфер.
		udi_cdc_write_buf("congratulation!!!\n", sizeof("congratulation!!!\n")); */
	 /*
	 iram_size_t n_rec = udi_cdc_get_nb_received_data();
	 char buf[n_rec];
	 udi_cdc_read_buf(buf, n_rec);
	 udi_cdc_putc('\n');
	 udi_cdc_putc('\r');
	 udi_cdc_write_buf(buf, n_rec);
	 UDI_CDC_TX_EMPTY_NOTIFY
	 USB_END
	 (tx_counter != CDC_STREAM_OUT_SIZE))*/	
}

/*
 * This example shows how to derive bitcoin addresses from the master public key (bip32 and bip 44)
 * Enter account master public key to the serial console and get the addresses
 * Use this tool to check: https://iancoleman.io/bip39/
 */
void getNAdresses(string userMasterPubKey, int amountOfChildKeys ){

	HDPublicKey hd(userMasterPubKey.c_str() );
	char *msg, *msg1;
	
	
	if(!hd){ // check if it is valid		
		msg="Invalid xpub\r\n";
		udi_cdc_write_buf(msg, strlen(msg)*sizeof(*msg));
		return;
	}
// 	msg="Master public key:\r\n";	
// 	udi_cdc_write_buf(msg, strlen(msg)*sizeof(*msg));
	
	msg="First N receiving addresses:\r\n";
	udi_cdc_write_buf(msg, strlen(msg)*sizeof(*msg));
	
	for(int i=0; i<amountOfChildKeys; i++){
		
		msg="Path:\r\n";
		udi_cdc_write_buf(msg, strlen(msg)*sizeof(*msg));
		
		sprintf(msg1, "%d", i);
		string path = string("m/0/") + msg1;		
		udi_cdc_write_buf(path.c_str() , path.size() * sizeof(path));
		
		msg="Address: \r\n";
		udi_cdc_write_buf(msg, strlen(msg)*sizeof(*msg));
		
		string uAdress="";
		HDPublicKey childPubKey;
		childPubKey = hd.derive(path.c_str());
		//childPubKey.address();
		//udi_cdc_write_buf(uAdress.c_str(), uAdress.size() * sizeof(uAdress));
		
	}	
	udi_cdc_putc('\n');
}

void getNChildKey(string userMasterPubKey, int amountOfChildKeys){

	HDPublicKey hd(userMasterPubKey.c_str() );
	char *msg, *msg1;
	
	
	if(!hd){ // check if it is valid
		msg="Invalid xpub\r\n";
		udi_cdc_write_buf(msg, strlen(msg)*sizeof(*msg));
		return;
	}
	msg="Master public key:\r\n";
	udi_cdc_write_buf(msg, strlen(msg)*sizeof(*msg));
	
	msg="First N receiving addresses:\r\n";
	udi_cdc_write_buf(msg, strlen(msg)*sizeof(*msg));
	
	for(int i=0; i<amountOfChildKeys; i++){
		
		msg="Path:\r\n";
		udi_cdc_write_buf(msg, strlen(msg)*sizeof(*msg));
		
		sprintf(msg1, "%d", i);
		string path = string("m/0/") + msg1;
		udi_cdc_write_buf(path.c_str() , path.size() * sizeof(path));
		
		msg="Address of path: \r\n";
		udi_cdc_write_buf(msg, strlen(msg)*sizeof(*msg));
		
		string uAdress="";
		HDPublicKey childPubKey;
		childPubKey = hd.derive(path.c_str());
		//childPubKey.address();	
		//udi_cdc_write_buf(uAdress.c_str(), uAdress.size() * sizeof(uAdress));
		
	}
	udi_cdc_putc('\n');
}

void genMasterKeys(){	 
	char *msg;
	char *myMnemonic="flight canvas heart purse potato mixed offer tooth maple blue kitten salute almost staff physical remain coral clump midnight rotate innocent shield inch ski";
	char *myPassword="213";
	int mnemonicLen= strlen(myMnemonic)*sizeof(*myMnemonic);
	int pasLen= strlen(myPassword)*sizeof(*myPassword);
	HDPrivateKey hd(myMnemonic, mnemonicLen, myPassword, pasLen, &Testnet, NULL);
	
	msg="bip84 master private key:\r\n";
	udi_cdc_write_buf(msg, strlen(msg)*sizeof(*msg));
		
	char *path="m/84'/0'/0'/";
	HDPrivateKey masterKey = hd.derive(path);
	//output of private Key
	//udi_cdc_write_buf(account, strlen(account)*sizeof(*account));
			
	msg="bip84 master public key:\r\n";
	udi_cdc_write_buf(msg, strlen(msg)*sizeof(*msg));
	
	//buf=masterKey.xpub().to_str();
	//msg= 
	//udi_cdc_write_buf(msg, strlen(msg)*sizeof(*msg));
	
	
	msg="first address:\r\n";
	udi_cdc_write_buf(msg, strlen(msg)*sizeof(*msg));
	
	path= "m/0/0/";
	HDPrivateKey childKey = masterKey.derive(path);
	//string hh;
	//childKey.publicKey().legacyAddress();
	udi_cdc_write_buf(msg, strlen(msg)*sizeof(*msg));
	
	udi_cdc_putc('\n');
}

void signTransaction(){
	char *myMnemonic="flight canvas heart purse potato mixed offer tooth maple blue kitten salute almost staff physical remain coral clump midnight rotate innocent shield inch ski";
	char *myPassword="213";	
	int mnemonicLen= strlen(myMnemonic)*sizeof(*myMnemonic); 
	int pasLen= strlen(myPassword)*sizeof(*myPassword); 
	HDPrivateKey hd(myMnemonic, mnemonicLen, myPassword, pasLen, &Testnet, NULL);
		
	PSBT tx;
	tx.parse("70736274ff01007202000000014b30bba309cd2b8a3d51030f7731c283d8ca7bd03a750fd712206bf432bfe89c0000000000fdffffff02a08601000000000017a914c43ffc04e8fd52e70988da5a81347396e73882f28712bb0d0000000000160014d924a0e0e40ca2766bacba5db53cdc35eb3532d8f0e517000001011f40420f000000000016001470e54ab67c6b6c4eeb6a019a83332a43b909b1992206037b3a3a013c18228e7070c97bb5d8d2afe2fd749ee9ef77dc4ac20c208a4795a8180464238c54000080010000800000008000000000000000000000220203d3b09e954f3e05800bf3bb4aec4734f8d7e3a233d90bfd75c234df702a132bfe180464238c540000800100008000000080010000000000000000");
	char *msg;
	
	msg="Transactions details:\r\n";
	udi_cdc_write_buf(msg, strlen(msg)*sizeof(*msg));
	
	msg="Outputs:\r\n";
	udi_cdc_write_buf(msg, strlen(msg)*sizeof(*msg));	
	 
	 for(int i=0; i<tx.tx.outputsNumber; i++){
		 
// 		 msg=tx.tx.txOuts[i].address(&Testnet);
// 		 udi_cdc_write_buf(msg, strlen(msg)*sizeof(*msg));
		 
		 if(tx.txOutsMeta[i].derivationsLen > 0){  
			 PSBTDerivation der = tx.txOutsMeta[i].derivations[0];
			 if(hd.derive(der.derivation, der.derivationLen).publicKey() == der.pubkey){
				 msg=" (change) \r\n";
				 udi_cdc_write_buf(msg, strlen(msg)*sizeof(*msg));
			 }
		 }
		 msg=" -> \r\n";
		 udi_cdc_write_buf(msg, strlen(msg)*sizeof(*msg));
		 		 
		
		 sprintf(msg, "%g", tx.tx.txOuts[i].btcAmount()*1e3);
		 udi_cdc_write_buf(msg, strlen(msg)*sizeof(*msg));
		 
		 msg=" mBTC\r\n";
		 udi_cdc_write_buf(msg, strlen(msg)*sizeof(*msg));
	 }
	 //Serial.print(float(tx.fee())/100); 
		 
	 msg="Fee: N bits";
	 udi_cdc_write_buf(msg, strlen(msg)*sizeof(*msg));

	 uint8_t buf= tx.sign(hd); // !transaction signing!
	 msg= (char*) buf;
	 udi_cdc_write_buf(msg, strlen(msg)*sizeof(*msg));// now you can combine and finalize PSBTs in Bitcoin Core
}