#ifndef FAILCODE_H
#define FAILCODE_H

typedef enum {
    MODULE_OK = 0,
    MODULE_SUCCESS = MODULE_OK,
    MODULE_ERR = 1,
    MODULE_FAILURE,
    MODULE_TO_BE_CONTINUE,
		MODULE_ADU_CRC_CHECK_FAIL,
	

    /* ÆäËû */
    WX_FAIL_CODE_BUTT,
} WxFailCode;

#define VOID WX_FailCode_DbgShow(char *argv[], UINT32 argc);

#endif
