/*
 * arguments.h
 *
 *  Created on: 26 Xul, 2016
 *      Author: marcos
 */

#ifndef ARGUMENTS_H_
#define ARGUMENTS_H_

#define DOWNSAMPLING 0
#define GET_COVERAGE 1
#define GET_STATS 2


struct Args
{
	int perform;
	char * fastq_1_file;
	char * fastq_2_file;
	char * out_fastq_1_file;
	char * out_fastq_2_file;
	char * fileStats;
	int isZipped;
	float reduction_ratio;
	unsigned long long int reference_length;
};


int getArgs (struct Args * arguments, int argc, char *argv[]);


#endif /* ARGUMENTS_H_ */
