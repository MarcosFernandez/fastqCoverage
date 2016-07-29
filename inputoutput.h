/*
 * inputoutput.h
 *
 *  Created on: 26 Xul, 2016
 *      Author: marcos
 */

#ifndef INPUTOUTPUT_H_
#define INPUTOUTPUT_H_

#include <stdio.h>
#include <zlib.h>

#define LENGTH 0x1000
#define READ_FILE "r"
#define WRITE_FILE "w"


struct FastqRead
{
	char * header;     /*Header name*/
	char * sequence;   /*Sequence name*/
	char * blank;     /*Blanck line*/
	char * qualities;  /*Qualities*/
};

struct StatsInfo
{
	unsigned long long int numberBases;     /*Number of bases*/
	unsigned long long int referenceLength;      /*Reference Length*/
	double coverge;                         /*Coverage*/
};

int setupFileDescriptor(char * fileName,void ** fileDescriptor,int isZip,char * openMode);
void fileCloseInput(void * fileDescriptor,int isZip);
void freeMemFastqRead(struct FastqRead * fastqRead1,struct FastqRead * fastqRead2);
int getFastqRead(void * fileDescriptor,struct FastqRead * fastqRead,int isGzip);
int writeFastqRead(void * fileDescriptor,struct FastqRead * fastqRead,int isGzip);

int getStatsPairs(void * fileDescriptor,struct StatsInfo * statsInfo);

#endif /* INPUTOUTPUT_H_ */
