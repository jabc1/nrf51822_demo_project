#include "rf.h"
#include "radio_config.h"
#include <stdint.h>//������֪��С����������ʾ����������
#include "nrf51.h"//����51822�����Ĵ����ṹ��ַ

#define PACKET_S1_FIELD_SIZE             (0UL)  /**< Packet S1 field size in bits. */
#define PACKET_S0_FIELD_SIZE             (0UL)  /**< Packet S0 field size in bits. */
#define PACKET_LENGTH_FIELD_SIZE         (0UL)  /**< Packet length field size in bits. */


/**
 * @brief Function for swapping/mirroring bits in a byte.
 * 
 *@verbatim
 * output_bit_7 = input_bit_0
 * output_bit_6 = input_bit_1
 *           :
 * output_bit_0 = input_bit_7
 *@endverbatim
 *
 * @param[in] inp is the input byte to be swapped.
 *
 * @return
 * Returns the swapped/mirrored input byte.
 */
static uint32_t swap_bits(uint32_t inp);

/**
 * @brief Function for swapping bits in a 32 bit word for each byte individually.
 * 
 * The bits are swapped as follows:
 * @verbatim
 * output[31:24] = input[24:31] 
 * output[23:16] = input[16:23]
 * output[15:8]  = input[8:15]
 * output[7:0]   = input[0:7]
 * @endverbatim
 * @param[in] input is the input word to be swapped.
 *
 * @return
 * Returns the swapped input byte.
 */
static uint32_t bytewise_bitswap(uint32_t inp);

static uint32_t swap_bits(uint32_t inp)
{
    uint32_t i;
    uint32_t retval = 0;
    
    inp = (inp & 0x000000FFUL);
    
    for(i = 0; i < 8; i++)
    {
        retval |= ((inp >> i) & 0x01) << (7 - i);     
    }
    
    return retval;    
}


static uint32_t bytewise_bitswap(uint32_t inp)
{
      return (swap_bits(inp >> 24) << 24)
           | (swap_bits(inp >> 16) << 16)
           | (swap_bits(inp >> 8) << 8)
           | (swap_bits(inp));
}


/** 
 * @brief Function for configuring the radio to operate in Shockburst compatible mode.
 * 
 * To configure the application running on nRF24L series devices:
 *
 * @verbatim
 * uint8_t tx_address[5] = { 0xC0, 0x01, 0x23, 0x45, 0x67 };
 * hal_nrf_set_rf_channel(7);
 * hal_nrf_set_address_width(HAL_NRF_AW_5BYTES); 
 * hal_nrf_set_address(HAL_NRF_TX, tx_address);
 * hal_nrf_set_address(HAL_NRF_PIPE0, tx_address); 
 * hal_nrf_open_pipe(0, false);
 * hal_nrf_set_datarate(HAL_NRF_1MBPS);
 * hal_nrf_set_crc_mode(HAL_NRF_CRC_16BIT);
 * hal_nrf_setup_dynamic_payload(0xFF);
 * hal_nrf_enable_dynamic_payload(false);
 * @endverbatim
 *
 * When transmitting packets with hal_nrf_write_tx_payload(const uint8_t *tx_pload, uint8_t length),
 * match the length with PACKET_STATIC_LENGTH.
 * hal_nrf_write_tx_payload(payload, PACKET_STATIC_LENGTH);
 * 
*/
#if 0
//���߹���4�ֱ���2440MHz��ͨ��0��ַ0x99999999C0��ͨ��1~7��ַ0x66666666XX
//����2MHz�������ֽ�3�ֽ�QQXXCC(QQ��ʼ��CC������xx������char)
void radio_configure()//�������ã�׼����nrf24L01ͨѶ
{
	//���߹���04��+4�ֱ���0��0�ֱ���FC��-4�ֱ���F8��-8�ֱ�
	//    F4��-12�ֱ���F0��-16�ֱ���EC��-20�ֱ���D8��-30�ֱ�
	NRF_RADIO->TXPOWER = (0x00<<0);//���߹���4�ֱ�

	NRF_RADIO->FREQUENCY = 0UL;//����Ƶ��0MHz+2400MHz=2400MHz

	//�������ʣ�00��1Mbit��01��2Mbit��02��250Kbit��03��1Mbit��������
	NRF_RADIO->MODE = (00<<0);

	// ���ߵ�ַ����
	NRF_RADIO->PREFIX0 = 0xC3C2C1E7UL;  // ͨ��3 �� 0 �ĵ�1�ֽ�
	NRF_RADIO->PREFIX1 = 0xC7C6C5C4UL;  // ͨ��7 �� 4 �ĵ�1�ֽ�	
	NRF_RADIO->BASE0   = 0xE7E7E7E7UL;  // ͨ��0�ĸ��ֽ�
	NRF_RADIO->BASE1   = 0x99999999UL;  // ͨ��1-7�ĸ��ֽ�
	NRF_RADIO->TXADDRESS   = 0x00UL;    // ����ʹ�õ�ͨ���ţ�0ͨ��
	NRF_RADIO->RXADDRESSES = 0x01UL;    // ���յ�ͨ���ţ�0ͨ��

	// ���ð�0������
	NRF_RADIO->PCNF0 = (3<<16)| //S1����ĳ���
						(0<<8) | //S0���ĳ���
						(6<<0);  //�����ֶ��еı�����
										 
	// ���ð�1������
	NRF_RADIO->PCNF1 = (0<<25)| //Ч��λ��0�أ�1����
					  (1<<24)| //���ݴ�С�ˣ��ߵ��ֽ��ĸ��ȷ� 0���ֽڣ�1���ֽڣ�
					  (4<<16)| //ͨ��0~7���ֽڳ��ȣ� nrf24�ߵ��ֽ�5�ֽڣ�4����+1���ͣ�
					  (5<<8) | //�����ֽڳ��ȣ�255~1��3�ֽ�QxC
					  (5<<0);  //Ӳ�������ֽڳ��ȣ�255~1��3�ֽ�QxC
											


	  // CRC У�鳤������
	NRF_RADIO->CRCCNF = 2; // У�鳤�� 2��char
	if ((NRF_RADIO->CRCCNF & 0x03)== 2 )
	{
		NRF_RADIO->CRCINIT = 0xFFFFUL;      // У���ʼֵ
		NRF_RADIO->CRCPOLY = 0x11021UL;     // CRC poly: x^16+x^12^x^5+1
	}
	else if ((NRF_RADIO->CRCCNF & 0x03) == 1 )
	{
		NRF_RADIO->CRCINIT = 0xFFUL;        // У���ʼֵ
		NRF_RADIO->CRCPOLY = 0x107UL;       // CRC poly: x^8+x^2^x^1+1
	}	


//���ռĴ�����  NRF_RADIO->PACKETPTR	
}
#endif
/** 
 * @}
 */






