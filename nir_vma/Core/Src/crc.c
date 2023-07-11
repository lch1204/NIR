#include "crc.h"

uint16_t calculate_crc(unsigned char *data_ptr, unsigned short size)
{
    uint16_t wcrc=0xFFFF;// preset 16 position crc register , The initial values are all 1
    uint8_t temp;// Define intermediate variables
    int i=0,j=0;// Define count
    for(i=0;i<size;i++)// Cycle through each data
    {

       temp=data_ptr[i];
       wcrc^=temp;
       for(j=0;j<8;j++){

          // Judge whether what is moved to the right is 1, If it is 1 XOR with polynomials .
          if(wcrc&0x0001){

              wcrc>>=1;// First move the data one bit to the right
              wcrc^=0xA001;// XOR with the polynomial above
          }
          else// If not 1, Then directly remove
              wcrc>>=1;// Direct removal
       }
    }
    temp=wcrc;//crc Value
    return wcrc;
}

uint8_t check_crc(unsigned char *data_ptr, unsigned short size){
    unsigned char crc_low = 0;
    unsigned char crc_high = 0;
	uint16_t crc16bit = 0;
	crc16bit = calculate_crc(data_ptr,size-2);
	crc_low = (crc16bit & 0x00FF);
	crc_high = (crc16bit>>8) & 0x00FF;
	unsigned char tmp1= data_ptr[size-1];
	unsigned char tmp2= data_ptr[size-2];
	if(data_ptr[size-1]==crc_high && data_ptr[size-2]==crc_low){
		return 1;
	}
	else
		return 0;

}
