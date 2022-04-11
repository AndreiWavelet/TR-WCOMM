#pragma once

#include <cstdint>

namespace Driver {

	namespace NRF24L01_Command {
		typedef enum {
			R_REGISTER = 0b00000000,
			W_REGISTER = 0b00100000,
			R_RX_PAYLOAD = 0b01100001,
			W_TX_PAYLOAD = 0b10100000,
			FLUSH_TX = 0b11100001,
			FLUSH_RX = 0b11100010,
			REUSE_TX_PL = 0b11100011,
			R_RX_PL_WID = 0b01100000,
			W_ACK_PAYLOAD = 0b10101000,
			W_TX_PAYLOAD_NO_ACK = 0b10110000,
			NOP = 0b11111111
		} Command;
	}

	namespace NRF24L01_Reg {

		typedef enum {
			CONFIG = 0x00,
			EN_AA = 0x01,
			EN_RXADDR = 0x02,
			SETUP_AW = 0x03,
			SETUP_RETR = 0x04,
			RF_CH = 0x05,
			RF_SETUP = 0x06,
			STATUS = 0x07,
			OBSERVE_TX = 0x08,
			RPD = 0x09,
			RX_ADDR_P0 = 0x0A,
			RX_ADDR_P1 = 0x0B,
			RX_ADDR_P2 = 0x0C,
			RX_ADDR_P3 = 0x0D,
			RX_ADDR_P4 = 0x0E,
			RX_ADDR_P5 = 0x0F,
			TX_ADDR = 0x10,
			RX_PW_P0 = 0x11,
			RX_PW_P1 = 0x12,
			RX_PW_P2 = 0x13,
			RX_PW_P3 = 0x14,
			RX_PW_P4 = 0x15,
			RX_PW_P5 = 0x16,
			FIFO_STATUS = 0x17,
			DYN_PD = 0x1C,
			FEATURE = 0x1D,

			//Not available on chip, only to ease development.
			PAYLOAD_FIFO = 0xF0,
			FLUSH = 0xF1,
			NOP = 0xF2,
		} Reg;

		typedef struct __attribute__((packed)) {
			union __attribute__((packed)) {
				struct __attribute__((packed)) {
					unsigned PRIM_RX : 1;
					unsigned PWR_UP : 1;
					unsigned CRCO : 1;
					unsigned EN_CRC : 1;
					unsigned MASK_MAX_RT : 1;
					unsigned MASK_TX_DS : 1;
					unsigned MASK_RX_DR : 1;
					unsigned : 1;
				};
				uint8_t value;
			};
		} Config_Data;

		typedef struct __attribute__((packed)) {
			union __attribute__((packed)) {
				struct __attribute__((packed)) {
					unsigned ENAA_P0 : 1;
					unsigned ENAA_P1 : 1;
					unsigned ENAA_P2 : 1;
					unsigned ENAA_P3 : 1;
					unsigned ENAA_P4 : 1;
					unsigned ENAA_P5 : 1;
					unsigned : 2;
				};
				uint8_t value;
			};

		} En_Aa_Data;

		typedef struct __attribute__((packed)) {
			union __attribute__((packed)) {
				struct __attribute__((packed)) {
					unsigned ERX_P0 : 1;
					unsigned ERX_P1 : 1;
					unsigned ERX_P2 : 1;
					unsigned ERX_P3 : 1;
					unsigned ERX_P4 : 1;
					unsigned ERX_P5 : 1;
					unsigned : 2;
				};
				uint8_t value;
			};

		} En_RxAddr_Data;

		typedef enum {
			AW_3BYTES = 0b01, AW_4BYTES = 0b10, AW_5BYTES = 0b11
		} Aw_Values;

		typedef struct __attribute__((packed)) {
			union __attribute__((packed)) {
				struct __attribute__((packed)) {
					unsigned AW : 2;
					unsigned : 6;
				};
				uint8_t value;
			};

		} Setup_Aw_Data;

		typedef enum {
			ARD_250us,ARD_500us,ARD_750us,ARD_1000us,
			ARD_1250us,ARD_1500us,ARD_1750us,ARD_2000us,
			ARD_2250us,ARD_2500us,ARD_2750us,ARD_3000us,
			ARD_3250us,ARD_3500us,ARD_3750us,ARD_4000us
		} Ard_Values;

		typedef struct __attribute__((packed)) {
			union __attribute__((packed)) {
				struct __attribute__((packed)) {
					unsigned ARC : 4;
					unsigned ARD : 4;
				};
				uint8_t value;
			};

		} Setup_Retr_Data;

		typedef struct __attribute__((packed)) {
			union __attribute__((packed)) {
				struct __attribute__((packed)) {
					unsigned RF_CH : 7;
					unsigned : 1;
				};
				uint8_t value;
			};

		} Rf_Ch_Data;

		typedef enum {
			RF_PWR_m18dBm,
			RF_PWR_m12dBm,
			RF_PWR_m6dBm,
			RF_PWR_m0dBm
		} Rf_Pwr_Value;

		typedef struct __attribute__((packed)) {
			union __attribute__((packed)) {
				struct __attribute__((packed)) {
					unsigned : 1;
					unsigned RF_PWR : 2;
					unsigned RF_DR_HIGH : 1;
					unsigned PLL_LOCK : 1;
					unsigned RF_DR_LOW : 1;
					unsigned RF : 1;
					unsigned CONT_WAVE : 1;
				};
				uint8_t value;
			};

		} Rf_Setup_Data;

		typedef struct __attribute__((packed)) {
			union __attribute__((packed)) {
				struct __attribute__((packed)) {
					unsigned TX_FULL : 1;
					unsigned RX_P_NO : 3;
					unsigned MAX_RT : 1;
					unsigned TX_DS : 1;
					unsigned RX_DR : 1;
					unsigned : 1;
				};
				uint8_t value;
			};

		} Status_Data;

		typedef struct __attribute__((packed)) {
			union __attribute__((packed)) {
				struct __attribute__((packed)) {
					unsigned ARC_CNT : 4;
					unsigned PLOS_CNT : 4;
				};
				uint8_t value;
			};

		} Observe_Tx_Data;

		typedef struct __attribute__((packed)) {
			union __attribute__((packed)) {
				struct __attribute__((packed)) {
					unsigned RPD : 1;
					unsigned : 7;
				};
				uint8_t value;
			};
		} Rpd_Data;

		typedef struct __attribute__((packed)) {
				uint8_t value[5];
		} RX_TX_ADDR_P01_Data;

		typedef struct __attribute__((packed)) {
				uint8_t value;
		} RX_ADDR_P2345_Data;

		typedef struct __attribute__((packed)) {
			union __attribute__((packed)) {
				struct __attribute__((packed)) {
					unsigned RX_PW_Px : 6;
					unsigned : 2;
				};
				uint8_t value;
			};
		} RX_PW_Px_Data;

		typedef struct __attribute__((packed)) {
			union __attribute__((packed)) {
				struct __attribute__((packed)) {
					unsigned RX_EMPTY : 1;
					unsigned RX_FULL : 1;
					unsigned : 2;
					unsigned TX_EMPTY : 1;
					unsigned TX_FULL: 1;
					unsigned TX_REUSE : 1;
					unsigned : 1;
				};
				uint8_t value;
			};
		} Fifo_Status_Data;

		typedef struct __attribute__((packed)) {
			union __attribute__((packed)) {
				struct __attribute__((packed)) {
					unsigned DPL_P0 : 1;
					unsigned DPL_P1 : 1;
					unsigned DPL_P2 : 1;
					unsigned DPL_P3 : 1;
					unsigned DPL_P4 : 1;
					unsigned DPL_P5 : 1;
					unsigned  : 2;
				};
				uint8_t value;
			};
		} Dynpd_Data;

		typedef struct __attribute__((packed)) {
			union __attribute__((packed)) {
				struct __attribute__((packed)) {
					unsigned EN_DYN_ACK : 1;
					unsigned EN_ACK_PAY : 1;
					unsigned EN_DPL : 1;
					unsigned : 5;
				};
				uint8_t value;
			};
		} Feature_Data;

	}


}

