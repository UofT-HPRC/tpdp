/*********************************************************************
 * Copyright (c) 2017 Xilinx, Inc.
 *
 **********************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

typedef struct XvcClient XvcClient;

/*
 * XVC server callback function table.
 */
typedef struct XvcServerHandlers {
	void (*set_tck)(
		unsigned long nsperiod,
		unsigned long * result);
	void (*shift_tms_tdi)(
		void * map_base,
		unsigned long count,
		unsigned char * tms_buf,
		unsigned char * tdi_buf,
		unsigned char * tdo_buf);
} XvcServerHandlers;

int xvcserver_start(
	const char * url,
	void * map_base,
	XvcServerHandlers * handlers);

#ifdef __cplusplus
}
#endif
