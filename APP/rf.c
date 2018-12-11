#include "rf.h"
#include "radio_config.h"
#include <stdint.h>//声明已知大小的整数或显示特征的整数
#include "nrf51.h"//包含51822各个寄存器结构地址

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
//无线功率4分贝，2440MHz，通道0地址0x99999999C0，通道1~7地址0x66666666XX
//速率2MHz，数据字节3字节QQXXCC(QQ开始，CC结束，xx是数据char)
void radio_configure()//无线配置，准备和nrf24L01通讯
{
	//无线功率04：+4分贝，0：0分贝，FC：-4分贝，F8：-8分贝
	//    F4：-12分贝，F0：-16分贝，EC：-20分贝，D8：-30分贝
	NRF_RADIO->TXPOWER = (0x00<<0);//无线功率4分贝

	NRF_RADIO->FREQUENCY = 0UL;//无线频率0MHz+2400MHz=2400MHz

	//无线速率：00：1Mbit，01：2Mbit，02：250Kbit，03：1Mbit（蓝牙）
	NRF_RADIO->MODE = (00<<0);

	// 无线地址设置
	NRF_RADIO->PREFIX0 = 0xC3C2C1E7UL;  // 通道3 到 0 的低1字节
	NRF_RADIO->PREFIX1 = 0xC7C6C5C4UL;  // 通道7 到 4 的低1字节	
	NRF_RADIO->BASE0   = 0xE7E7E7E7UL;  // 通道0的高字节
	NRF_RADIO->BASE1   = 0x99999999UL;  // 通道1-7的高字节
	NRF_RADIO->TXADDRESS   = 0x00UL;    // 发射使用的通道号：0通道
	NRF_RADIO->RXADDRESSES = 0x01UL;    // 接收的通道号：0通道

	// 配置包0的设置
	NRF_RADIO->PCNF0 = (3<<16)| //S1领域的长度
						(0<<8) | //S0场的长度
						(6<<0);  //长度字段中的比特数
										 
	// 配置包1的设置
	NRF_RADIO->PCNF1 = (0<<25)| //效验位（0关，1开）
					  (1<<24)| //数据大小端（高低字节哪个先发 0低字节，1高字节）
					  (4<<16)| //通道0~7高字节长度（ nrf24高低字节5字节：4个高+1个低）
					  (5<<8) | //数据字节长度（255~1）3字节QxC
					  (5<<0);  //硬件传输字节长度（255~1）3字节QxC
											


	  // CRC 校验长度配置
	NRF_RADIO->CRCCNF = 2; // 校验长度 2个char
	if ((NRF_RADIO->CRCCNF & 0x03)== 2 )
	{
		NRF_RADIO->CRCINIT = 0xFFFFUL;      // 校验初始值
		NRF_RADIO->CRCPOLY = 0x11021UL;     // CRC poly: x^16+x^12^x^5+1
	}
	else if ((NRF_RADIO->CRCCNF & 0x03) == 1 )
	{
		NRF_RADIO->CRCINIT = 0xFFUL;        // 校验初始值
		NRF_RADIO->CRCPOLY = 0x107UL;       // CRC poly: x^8+x^2^x^1+1
	}	


//接收寄存器是  NRF_RADIO->PACKETPTR	
}
#endif
/** 
 * @}
 */






