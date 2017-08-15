#include "wav.h"



void  Recoder_wav_init(WavHeader* wavhead)
{

	
	wavhead->ChunkID =0x46464952;/* RIFF */
	wavhead->ChunkSize = 0;/* N+36 */
	wavhead->Format= 0x45564157;  /* wave */
	
	wavhead->SubChunkFMT=0x20746D66;/* fmt */
	wavhead->SubChunkFMTSIZE=16;/* 16 */
	wavhead->AudioFormat=0x01;	/* PCM 1 */
	wavhead->NumChannels=1;/* ������ */
	wavhead->SampleRate=8000;/* ������ */
	wavhead->ByteRate=wavhead->SampleRate*2;/* ÿ�������� */
	wavhead->BlockAlign=2;/*  */
	wavhead->BitsPerSample=16;
	wavhead->SubChunkData=0x61746164;
	wavhead->SubChunkDataSize=0;
}
	






	
	
	


