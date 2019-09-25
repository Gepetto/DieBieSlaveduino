#ifndef CUSTOM_PDO_NAME_H
#define CUSTOM_PDO_NAME_H

//-------------------------------------------------------------------//
//                                                                   //
//     This file has been created by the Easy Configurator tool      //
//                                                                   //
//     Easy Configurator project DieBieSlaveduino.prj
//                                                                   //
//-------------------------------------------------------------------//


#define CUST_BYTE_NUM_OUT	10
#define CUST_BYTE_NUM_IN	10
#define TOT_BYTE_NUM_ROUND_OUT	12
#define TOT_BYTE_NUM_ROUND_IN	12


typedef union												//---- output buffer ----
{
	uint8_t  Byte [TOT_BYTE_NUM_ROUND_OUT];
	struct
	{
		float       output_float;
		uint16_t    output_16;
		int16_t     output_int16;
		int8_t      output_int8;
		uint8_t     output_8;
	}Cust;
} PROCBUFFER_OUT;


typedef union												//---- input buffer ----
{
	uint8_t  Byte [TOT_BYTE_NUM_ROUND_IN];
	struct
	{
		float       input_float;
		uint16_t    input_16;
		int16_t     input_int16;
		int8_t      input_int8;
		uint8_t     input_8;
	}Cust;
} PROCBUFFER_IN;

#endif