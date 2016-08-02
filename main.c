/*
 * main.c
 *
 *  Created on: 26 Xul, 2016
 *      Author: marcos
 */
#include <stdio.h>
#include <unistd.h>
#include "arguments.h"
#include "inputoutput.h"
#include <string.h>
#include <stdlib.h>

#define READ_FILE "r"
#define WRITE_FILE "w"

/**
 * \brief Downsampling input reads according to a percentage ratio
 * \param reduction_ration Ratio to decrease the number of reads
 * \param fastq_1_file File name of the first FASTQ input paired file
 * \param fastq_2_file File name of the second FASTQ input paired file
 * \param isZipped isCompressed input
 * \param out_fastq_1_file File name of the first FASTQ output paired file
 * \param out_fastq_1_file File name of the second FASTQ output paired file
 * \param return 1 if everything is correct otherwise 0
 */
int runDownsampling(float reduction_ratio,char * fastq_1_file,char * fastq_2_file,int isZipped,char * out_fastq_1_file,char * out_fastq_2_file)
{
	/*1. File Descriptors*/
	void * input_file_1_descriptor;
	void * input_file_2_descriptor;
	void * output_file_1_descriptor;
	void * output_file_2_descriptor;

	if(isZipped)
	{
		gzFile * z_input_descriptor_one;
		input_file_1_descriptor = z_input_descriptor_one;
		gzFile * z_input_descriptor_two;
		input_file_2_descriptor = z_input_descriptor_two;
		gzFile * z_output_descriptor_one;
		output_file_1_descriptor = z_output_descriptor_one;
		gzFile * z_output_descriptor_two;
		output_file_2_descriptor = z_output_descriptor_two;
	}
	else
	{
		FILE * text_input_descriptor_one;
		input_file_1_descriptor = text_input_descriptor_one;
		FILE * text_input_descriptor_two;
		input_file_2_descriptor = text_input_descriptor_two;
		FILE * text_output_descriptor_one;
		output_file_1_descriptor = text_output_descriptor_one;
		FILE * text_output_descriptor_two;
		output_file_2_descriptor = text_output_descriptor_two;
	}

	/*1.1 File Descriptors Initialization*/
	if (!setupFileDescriptor(fastq_1_file,&input_file_1_descriptor,isZipped,READ_FILE))
	{
		printf ("Sorry!! File %s can not be opened \n",fastq_1_file);
		return 0;
	}

	if (!setupFileDescriptor(fastq_2_file,&input_file_2_descriptor,isZipped,READ_FILE))
	{
		printf ("Sorry!! File %s can not be opened \n",fastq_2_file);
		return 0;
	}

	if (!setupFileDescriptor(out_fastq_1_file,&output_file_1_descriptor,isZipped,WRITE_FILE))
	{
		printf ("Sorry!! File %s can not be opened \n",out_fastq_1_file);
		return 0;
	}

	if (!setupFileDescriptor(out_fastq_2_file,&output_file_2_descriptor,isZipped,WRITE_FILE))
	{
		printf ("Sorry!! File %s can not be opened \n",out_fastq_2_file);
		return 0;
	}

	/*2. Parse Couple of FASTQ files*/
	struct FastqRead read_one, read_two;
	time_t t;

	/*2.1. Intializes random number generator */
	srand((unsigned) time(&t));
	unsigned int threshold_decision = reduction_ratio * 100;

	/*2.1 Get Reads from each pair*/
	while (getFastqRead(input_file_1_descriptor,&read_one,isZipped) && getFastqRead(input_file_2_descriptor,&read_two,isZipped))
	{
		int random_value = rand() % 100;

		if (random_value <= threshold_decision)
		{
			if (!writeFastqRead(output_file_1_descriptor,&read_one,isZipped))
			{
				printf("Sorry!! It was not possible to write fastq record in file: %s \n", out_fastq_1_file);
			}

			if (!writeFastqRead(output_file_2_descriptor,&read_two,isZipped))
			{
				printf("Sorry!! It was not possible to write fastq record in file: %s \n", out_fastq_2_file);
			}

		}
		freeMemFastqRead(&read_one,&read_two);
	}

	/*3. Close files*/
	fileCloseInput(input_file_1_descriptor,isZipped);
	fileCloseInput(input_file_2_descriptor,isZipped);
	fileCloseInput(output_file_1_descriptor,isZipped);
	fileCloseInput(output_file_2_descriptor,isZipped);

	return 1;
}


/**
 * \brief Downsampling input reads in single end according to a percentage ratio
 * \param reduction_ration Ratio to decrease the number of reads
 * \param fastq_file File name of the FASTQ input single end file
 * \param isZipped isCompressed input
 * \param out_fastq_file File name of the FASTQ output single end file
 * \param return 1 if everything is correct otherwise 0
 */
int runDownsamplingSE(float reduction_ratio,char * fastq_file,int isZipped,char * out_fastq_file)
{
	/*1. File Descriptors*/
	void * input_file_descriptor;
	void * output_file_descriptor;

	if(isZipped)
	{
		gzFile * z_input_descriptor;
		input_file_descriptor = z_input_descriptor;
		gzFile * z_output_descriptor;
		output_file_descriptor = z_output_descriptor;
	}
	else
	{
		FILE * text_input_descriptor;
		input_file_descriptor = text_input_descriptor;
		FILE * text_output_descriptor;
		output_file_descriptor = text_output_descriptor;
	}

	/*1.1 File Descriptors Initialization*/
	if (!setupFileDescriptor(fastq_file,&input_file_descriptor,isZipped,READ_FILE))
	{
		printf ("Sorry!! File %s can not be opened \n",fastq_file);
		return 0;
	}

	if (!setupFileDescriptor(out_fastq_file,&output_file_descriptor,isZipped,WRITE_FILE))
	{
		printf ("Sorry!! File %s can not be opened \n",out_fastq_file);
		return 0;
	}

	/*2. Parse FASTQ files*/
	struct FastqRead read;
	time_t t;

	/*2.1. Intializes random number generator */
	srand((unsigned) time(&t));
	unsigned int threshold_decision = reduction_ratio * 100;

	/*2.1 Get Reads from FASTQ file*/
	while (getFastqRead(input_file_descriptor,&read,isZipped))
	{
		int random_value = rand() % 100;

		if (random_value <= threshold_decision)
		{
			if (!writeFastqRead(output_file_descriptor,&read,isZipped))
			{
				printf("Sorry!! It was not possible to write fastq record in file: %s \n", out_fastq_file);
			}
		}
		freeFastqRead(&read);
	}

	/*3. Close files*/
	fileCloseInput(input_file_descriptor,isZipped);
	fileCloseInput(output_file_descriptor,isZipped);

	return 1;
}


/**
 * \brief Run Coverage Calculation from two fastq files
 * \param fastq_1_file Pair One name
 * \param fastq_2_file Pair Two name
 * \param isZipped isCompressed input
 * \param reference_length Assembly reference length
 * \param return 1 if everything is correct otherwise 0
 */
int runGetCoverage(char * fastq_1_file,char * fastq_2_file,int isZipped,unsigned long long int reference_length)
{
	/*1. File Descriptors*/
	void * file_1_descriptor;
	void * file_2_descriptor;
	if(isZipped)
	{
		gzFile * zDescriptor_one;
		file_1_descriptor = zDescriptor_one;
		gzFile * zDescriptor_two;
		file_2_descriptor = zDescriptor_two;
	}
	else
	{
		FILE * textDescriptor_one;
		file_1_descriptor = textDescriptor_one;
		FILE * textDescriptor_two;
		file_2_descriptor = textDescriptor_two;
	}


	/*1.1 File Descriptors Initialization*/
	if (!setupFileDescriptor(fastq_1_file,&file_1_descriptor,isZipped,READ_FILE))
	{
		printf ("Sorry!! File %s can not be opened \n",fastq_1_file);
		return 0;
	}

	if (!setupFileDescriptor(fastq_2_file,&file_2_descriptor,isZipped,READ_FILE))
	{
		printf ("Sorry!! File %s can not be opened \n",fastq_2_file);
		return 0;
	}

	/*2. Parse Couple of FASTQ files*/
	struct FastqRead read_one, read_two;

	/*2.1 Get Reads from each pair*/
	unsigned long long int nTotalBases = 0;

	while (getFastqRead(file_1_descriptor,&read_one,isZipped) && getFastqRead(file_2_descriptor,&read_two,isZipped))
	{
		nTotalBases = nTotalBases + strlen(read_one.sequence) + strlen(read_two.sequence);
		freeMemFastqRead(&read_one,&read_two);
	}

	/*3. Calculate stats*/
	double raw_coverage = (double) nTotalBases / (double) reference_length;

	/*4. Close files*/
	fileCloseInput(file_1_descriptor,isZipped);
	fileCloseInput(file_2_descriptor,isZipped);

	/*5. Print Results*/
	printf("%llu\t%llu\t%.2f\n",nTotalBases,reference_length,raw_coverage);

	return 1;
}

/**
 * \brief Run Coverage Calculation from single end fastq file
 * \param fastq_file Fastq file name
 * \param isZipped isCompressed input
 * \param reference_length Assembly reference length
 * \param return 1 if everything is correct otherwise 0
 */
int runGetCoverageSE(char * fastq_file,int isZipped,unsigned long long int reference_length)
{
	/*1. File Descriptors*/
	void * file_descriptor;
	if(isZipped)
	{
		gzFile * zDescriptor;
		file_descriptor = zDescriptor;
	}
	else
	{
		FILE * textDescriptor;
		file_descriptor = textDescriptor;
	}

	/*1.1 File Descriptors Initialization*/
	if (!setupFileDescriptor(fastq_file,&file_descriptor,isZipped,READ_FILE))
	{
		printf ("Sorry!! File %s can not be opened \n",fastq_file);
		return 0;
	}

	/*2. Parse Couple of FASTQ files*/
	struct FastqRead read;

	/*2.1 Get Reads from each pair*/
	unsigned long long int nTotalBases = 0;

	while (getFastqRead(file_descriptor,&read,isZipped))
	{
		nTotalBases = nTotalBases + strlen(read.sequence) + strlen(read.sequence);
		freeFastqRead(&read);
	}

	/*3. Calculate stats*/
	double raw_coverage = (double) nTotalBases / (double) reference_length;

	/*4. Close files*/
	fileCloseInput(file_descriptor,isZipped);

	/*5. Print Results*/
	printf("%llu\t%llu\t%.2f\n",nTotalBases,reference_length,raw_coverage);

	return 1;
}





/**
 * \brief Run Coverage Calculation from a set of coverage files
 * \param fileStats Stats for fastq pairs files. Format [numberOfBases(tab)lengthReference(tab)coverage]
 * \param return 1 if everything is correct otherwise 0
 */
int runGetStats(char * fileStats)
{
   FILE * statsDescriptor;

   /*1. File Descriptors Initialization*/
   if (!setupFileDescriptor(fileStats,&statsDescriptor,0,READ_FILE))
   {
	   printf ("Sorry!! File %s can not be opened \n",fileStats);
	   return 0;
   }

   /*2. File Parsing */
   struct StatsInfo stats;

   /*2.1 Get Reads from each pair*/
   unsigned long long int nTotalBases = 0;
   unsigned long long int referenceLength = 0;

   while (getStatsPairs(statsDescriptor,&stats))
   {
	   nTotalBases = nTotalBases + stats.numberBases;
	   if(referenceLength == 0)
	   {
		   referenceLength = stats.referenceLength;
	   }
   }

   /*3. Calculate Coverage*/
   double raw_coverage = (double) nTotalBases / (double) referenceLength;

   /*4. Close files*/
   fileCloseInput(statsDescriptor,0);

   /*5. Print Results*/
   printf("%llu\t%llu\t%.2f\n",nTotalBases,referenceLength,raw_coverage);

   return 1;
}


int main (int argc, char *argv[])
{
    /*0. PARSE ARGUMENTS*/
	struct Args arguments;

	if (!getArgs(&arguments,argc,argv))
	{
		return 0;
	}

	/*1. Run Command*/
	switch (arguments.perform)
	{
		case DOWNSAMPLING:

			if(arguments.seqMode == SINGLE_END)
			{
				if (!runDownsamplingSE(arguments.reduction_ratio,arguments.fastq_1_file,arguments.isZipped,arguments.out_fastq_1_file))
				{
					return 1;
				}
			}
			else
			{
				if(!runDownsampling(arguments.reduction_ratio,arguments.fastq_1_file,arguments.fastq_2_file,arguments.isZipped,arguments.out_fastq_1_file,arguments.out_fastq_2_file))
				{
					return 1;
				}
			}

			break;
		case GET_COVERAGE:
			if(arguments.seqMode == SINGLE_END)
			{
				if(!runGetCoverageSE(arguments.fastq_1_file,arguments.isZipped,arguments.reference_length))
				{
					return 1;
				}
			}
			else
			{
				if (!runGetCoverage(arguments.fastq_1_file,arguments.fastq_2_file,arguments.isZipped,arguments.reference_length))
				{
					return 1;
				}
			}

			break;
		case GET_STATS:
			if (!runGetStats(arguments.fileStats))
			{
				return 1;
			}
			break;
		default:
			break;
	}

	return 0;
}
