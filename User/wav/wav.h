#ifndef __WAV_H
#define	__WAV_H
#include "stm32f10x.h"
typedef struct
{
	
	u32 ChunkID; //RIFF 0x46464952
	u32 ChunkSize;//�ļ��ܴ�С
	u32 Format;//��ʽwav 0x45564157
	
	u32 SubChunkFMT;// "fmt"ID 0x20746D66
	u32 SubChunkFMTSIZE;//��С
	u16 AudioFormat;//0x01��ʾPCM
	u16 NumChannels;//  1 ��ͨ��
	u32 SampleRate;//������0x1f40
	u32 ByteRate;//������*2*ͨ���� 
	u16 BlockAlign;//���С
	u16 BitsPerSample;//16λ
	
	u32 SubChunkData;//0x61746164
	u32 SubChunkDataSize;	
} WavHeader;




void  Recoder_wav_init(WavHeader* wavhead);




#endif
