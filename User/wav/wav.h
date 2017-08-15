#ifndef __WAV_H
#define	__WAV_H
#include "stm32f10x.h"
typedef struct
{
	
	u32 ChunkID; //RIFF 0x46464952
	u32 ChunkSize;//文件总大小
	u32 Format;//格式wav 0x45564157
	
	u32 SubChunkFMT;// "fmt"ID 0x20746D66
	u32 SubChunkFMTSIZE;//大小
	u16 AudioFormat;//0x01表示PCM
	u16 NumChannels;//  1 单通道
	u32 SampleRate;//采样率0x1f40
	u32 ByteRate;//采样率*2*通道数 
	u16 BlockAlign;//块大小
	u16 BitsPerSample;//16位
	
	u32 SubChunkData;//0x61746164
	u32 SubChunkDataSize;	
} WavHeader;




void  Recoder_wav_init(WavHeader* wavhead);




#endif
