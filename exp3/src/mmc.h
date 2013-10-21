#include "integer.h"

typedef struct csd_bf_t {

    uint16_t D: 16;
    uint16_t : 16;
    uint16_t : 15;
    uint8_t C_SIZE_MULT: 3;
    uint16_t : 12;
    uint16_t C_SIZE: 12;
    uint8_t : 6;
    uint8_t READ_BL_LEN: 4;
    uint16_t CCC : 12;
    uint8_t TRAN_SPEED : 8;
    uint16_t : 16;
    uint16_t : 6;
    uint8_t CSD_STRUCTURE : 2;

} csd_bf;


uint8_t detectCard(BYTE * csd);
int rcvr_datablock (BYTE * buff, UINT btr); 
BYTE send_cmd (BYTE cmd, DWORD arg); 
DWORD get_fattime (void);

