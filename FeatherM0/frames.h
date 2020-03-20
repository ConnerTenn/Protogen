
#include "globals.h"

extern const u8 _binary_FrameData_bin_start;
extern const u8 _binary_FrameData_bin_end;
// #define BIN_FRAME_DATA_START ((const PROGMEM u8 *const)(&_binary_FrameData_bin_start))
// extern PROGMEM const u8 *const FrameData;
// extern PROGMEM const u8 *const FrameDataEnd;

// typedef struct 
// {
// 	u16 FrameOffset;
// 	u16 FrameNext;
// 	u16 FrameDelay;
// } Frame;

// // #define FrameHeaderAcc(i) ((Frame *)(FrameData+i*sizeof(Frame)))
// // #define FrameDataAcc(i) ((u8 *)(FrameData + FrameHeaderAcc(i)->FrameOffset))


// #define FRAME_OFF_ACC_16(idx) ( (u16)(pgm_read_word_near(BIN_FRAME_DATA_START+idx*sizeof(Frame)+0)) )
// #define FRAME_NXT_ACC_16(idx) ( (u16)(pgm_read_word_near(BIN_FRAME_DATA_START+idx*sizeof(Frame)+2)) )
// #define FRAME_DLY_ACC_16(idx) ( (u16)(pgm_read_word_near(BIN_FRAME_DATA_START+idx*sizeof(Frame)+4)) )
// // #define FRAME_DAT_ACC(idx) (pgm_read_word_near(BIN_FRAME_DATA_START+FRAME_OFF_ACC(idx)))
// // #define FRAME_DAT_ADDR(idx) ( (u8 *)(BIN_FRAME_DATA_START+FRAME_OFF_ACC_16(idx)))
// #define FRAME_DAT_ACC_8(idx, off) ( (u8)(pgm_read_byte_near((u16)BIN_FRAME_DATA_START+FRAME_OFF_ACC_16(idx)+(u16)(off))) )

// // #define FrameHeaderAcc(i) ((Frame *)(BIN_FRAME_DATA_START+i*sizeof(Frame)))
// // #define FrameDataAcc(i) ((u8 *)((const PROGMEM u8 *const)(&_binary_FrameData_bin_start) + FrameHeaderAcc(i)->FrameOffset))

// void FrameTransUp(u8 *data, u8 numSegments, u8 num);
// void FrameTransDown(u8 *data, u8 numSegments, u8 num);
// void FrameTransLeft(u8 *data, u8 numSegments, u8 num);
// void FrameTransRight(u8 *data, u8 numSegments, u8 num);
