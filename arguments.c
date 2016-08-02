/*
 * arguments.c
 *
 *  Created on: 26 Xul, 2016
 *      Author: marcos
 */
#include "arguments.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

/**
 * \brief Print Program help
 */
void printHelp()
{
	printf("usage: fastqCoverage [-h] [single-end] <command>... \n");
	printf("\n");
	printf("fastqCoverage parses paired end files. Can be gzipped. \n");
	printf("It is able to reduce coverage, calculate coverage per set of Paired End files. \n");
	printf("Can also calculate global coverages for a set of files. \n");
	printf("\n");
	printf("fastqCoverage can process Single End data, you should use single-end argument before the command.\n");
	printf("\n");
	printf("\n");
	printf("\t-h\t show this help message and exit\n");
	printf("\n");
	printf("commands:\n");
	printf("\n");
	printf("\t downsampling \t Reduces the coverage for a given pair of files or a unique (single end) fastq file.\n");
	printf("\t getCoverage \t Calculates the number of bases in a pair of files (paired end) or a unique (single end) file \n");
	printf("\t \t \t and gets a coverage value.\n");
	printf("\t getStats \t From getCoverage output gets a global coverage value.\n");
}


/**
 * \brief Print Downdampling Help
 */
void printDownsamplingHelp(unsigned int seqMode)
{
	if(seqMode == PAIRED_END)
	{
		printf("usage: fastqCoverage downsampling [-h] -r ratio -a pair_1.fastq.gz -b pair_2.fastq.gz -x out_pair_1.fastq.gz -y out_pair_2.fastq.gz [-z] \n");
	}
	else
	{
		printf("usage: fastqCoverage single-end downsampling [-h] -r ratio -a file.fastq.gz -x out_file.fastq.gz [-z] \n");
	}

	printf("\n");
	if(seqMode == PAIRED_END) printf("Reduces the coverage of a couple of paired end files.\n");
	else printf("Reduces the coverage of a single end file.\n");

	printf("\n");
	printf("Optional Arguments:\n");
	printf("\n");
	printf("\t-h\t show this help message and exit \n");
	printf("\t-z\t Gzipped input files.\n");
	printf("\n");
	printf("Mandatory Arguments:\n");
	printf("\n");
	printf("\t-r\t Decreasing ratio to apply. Value must be between 0 and 1. \n");
	printf("\t \t \t 0.5 stands for 50% reduction. 0.75 stands for 75% reduction. \n");

	if(seqMode == PAIRED_END)
	{
		printf("\t-a\t First FASTQ pair file. Can be gzipped. \n");
		printf("\t-b\t Second FASTQ pair file. Can be gzipped. \n");
		printf("\t-x\t Output First FASTQ pair file. If inputs are gzipped then output will be gzipped. \n");
		printf("\t-y\t Output Second FASTQ pair file. If inputs are gzipped then output will be gzipped. \n");
	}
	else
	{
		printf("\t-a\t FASTQ input file. Can be gzipped. \n");
		printf("\t-x\t FASTQ output file. If inputs are gzipped then output will be gzipped. \n");
	}
}

/**
 * \brief Print Get Coverage Help
 */
void printGetCoverageHelp(unsigned int seqMode)
{
	if(seqMode == PAIRED_END)
	{
		printf("usage: fastqCoverage getCoverage [-h] -a pair_1.fastq.gz -b pair_2.fastq.gz -l reference_length [-z] \n");
		printf("Get total number of bases in a couple of paired end fastq files and calculates the coverage from a reference length.\n");
	}
	else
	{
		printf("usage: fastqCoverage single-end getCoverage [-h] -a file.fastq.gz -l reference_length [-z] \n");
		printf("\n");
		printf("Get total number of bases in a fastq file and calculates the coverage from a reference length.\n");
	}
	printf("\n");
	printf("\t Output format: \n");
	printf("\t [numberOfBases(tab)ReferenceLength(tab)Coverage]\n");
	printf("\n");
	printf("Optional Arguments:\n");
	printf("\n");
	printf("\t-h\t show this help message and exit \n");
	printf("\t-z\t Gzipped input files.\n");
	printf("\n");
	printf("Mandatory Arguments:\n");
	printf("\n");
	if(seqMode == PAIRED_END)
	{
		printf("\t-a\t First FASTQ pair file. Can be gzipped. \n");
		printf("\t-b\t Second FASTQ pair file. Can be gzipped. \n");
	}
	else
	{
		printf("\t-a\t First Single End file. Can be gzipped. \n");
	}
	printf("\t-l\t Reference length. \n");
}


/**
 * \brief Print Get Stats Help
 */
/**
 * \brief Print Get Coverage Help
 */
void printGetStatsHelp()
{
	printf("usage: fastqCoverage getStats [-h] -s fileStats \n");
	printf("\n");
	printf("Get total number of bases in a stats file and calculates the coverage from a reference length.\n");
	printf("\n");
	printf("Input format: \n");
	printf("\t A line per each set of files in the format: [numberOfBases(tab)ReferenceLength(tab)Coverage] \n");
	printf("\n");
	printf("Optional Arguments:\n");
	printf("\n");
	printf("\t-h\t show this help message and exit \n");
	printf("\n");
	printf("Mandatory Arguments:\n");
	printf("\n");
	printf("\t-s\t File stats. Should follow input format above defined. \n");
}


/**
 * \brief Initialization of arguments
 */
void initArgs(struct Args * arguments)
{
	arguments->perform = DOWNSAMPLING;
	arguments->seqMode = PAIRED_END;
	arguments->fastq_1_file = NULL;
	arguments->fastq_2_file = NULL;
	arguments->out_fastq_1_file = NULL;
	arguments->out_fastq_2_file = NULL;
	arguments->fileStats = NULL;
	arguments->isZipped = 0;
	arguments->reduction_ratio = 0.0;
	arguments->reference_length = 0;
}

/**
 * \brief Process arguments for downsampling command
 */
int processArgumentsDownsampling(struct Args * arguments, int argc, char *argv[])
{
	int opt = 0;

	while ((opt = getopt(argc, argv, "hr:a:b:x:y:z")) != -1)
	{
		switch(opt)
		{
		    case 'h':
		    	printDownsamplingHelp(arguments->seqMode);
			    return 0;
			    break;
			case 'r':
				arguments->reduction_ratio = atof(optarg);
				break;
			case 'a':
				arguments->fastq_1_file = optarg;
				break;
			case 'b':
				arguments->fastq_2_file = optarg;
				break;
			case 'x':
				arguments->out_fastq_1_file = optarg;
				break;
			case 'y':
				arguments->out_fastq_2_file = optarg;
				break;
			case 'z':
				arguments->isZipped = 1;
				break;
			case '?':
				printf("Invalid option received \n");
				break;
		}
	}

	if (arguments->fastq_1_file == NULL)
	{
		printf("Sorry!! Fastq file for first pair is not defined!! \n");
		return 0;
	}
	else if (arguments->fastq_2_file == NULL)
	{
		if (arguments->seqMode == PAIRED_END)
		{
			printf("Sorry!! Fastq file for second pair is not defined!! \n");
			return 0;
		}
	}
	else if (arguments->out_fastq_1_file == NULL)
	{
		if (arguments->seqMode == PAIRED_END)
		{
			printf("Sorry!! Output Fastq file for first pair is not defined!! \n");
			return 0;
		}
	}
	else if (arguments->out_fastq_2_file == NULL)
	{
		if (arguments->seqMode == PAIRED_END)
		{
			printf("Sorry!! Output Fastq file for second pair is not defined!! \n");
			return 0;
		}
	}

	return 1;
}

/**
 * \brief Process arguments for getCoverage command
 */
int processArgumentsGetCoverage(struct Args * arguments, int argc, char *argv[])
{
	int opt = 0;

	while ((opt = getopt(argc, argv, "ha:b:l:z")) != -1)
	{
		switch(opt)
		{
		    case 'h':
		    	printGetCoverageHelp(arguments->seqMode);
			    return 0;
			    break;
			case 'a':
				arguments->fastq_1_file = optarg;
				break;
			case 'b':
				arguments->fastq_2_file = optarg;
				break;
			case 'z':
				arguments->isZipped = 1;
				break;
			case 'l':
				arguments->reference_length = atoll(optarg);
				break;
			case '?':
				printf("Invalid option received \n");
				break;
		}
	}

	if (arguments->fastq_1_file == NULL)
	{
		printf("Sorry!! Fastq file for first pair is not defined!! \n");
		return 0;
	}
	else if (arguments->fastq_2_file == NULL)
	{
		if (arguments->seqMode == PAIRED_END)
		{
			printf("Sorry!! Fastq file for second pair is not defined!! \n");
			return 0;
		}
	}

	return 1;
}


/**
 * \brief Process arguments for getStats command
 */
int processArgumentsGetStats(struct Args * arguments, int argc, char *argv[])
{
	int opt = 0;

	while ((opt = getopt(argc, argv, "hs:")) != -1)
	{
		switch(opt)
		{
		    case 'h':
		    	printGetStatsHelp();
			    return 0;
			    break;
			case 's':
				arguments->fileStats = optarg;
				break;
			case '?':
				printf("Invalid option received \n");
				break;
		}
	}

	if (arguments->fileStats == NULL)
	{
		printf("Sorry!! File stats file is not defined!! \n");
		return 0;
	}

	return 1;
}



/**
 * \brief Get arguments
 * \params arguments - struct of arguments
 * \params argc - number of arguments
 * \params argv - arguments passed
 * \return 1 if everything is ok  0 if not ok
 */
int getArgs (struct Args * arguments, int argc, char *argv[])
{
	initArgs(arguments);

	char * command;

	if (argc > 2)
	{
		if (strcmp(argv[1],"-h") == 0)
		{
			printHelp();
			return 0;
		}
		else if (strcmp(argv[1],"single-end") == 0)
		{
			arguments->seqMode = SINGLE_END;
			command = argv[2];
		}
		else
		{
			command = argv[1];
		}

		if (strcmp(command,"downsampling") == 0)
		{
			arguments->perform = DOWNSAMPLING;
			return processArgumentsDownsampling(arguments,argc,argv);
		}
		else if (strcmp(command,"getCoverage") == 0)
		{
			arguments->perform = GET_COVERAGE;
			return processArgumentsGetCoverage(arguments,argc,argv);
		}
		else if (strcmp(command,"getStats") == 0)
		{
			arguments->perform = GET_STATS;
			return processArgumentsGetStats(arguments,argc,argv);
		}
		else
		{
			printf("Sorry!! Argument [%s] is not valid\n",argv[1]);
			return 0;
		}
	}
	else
	{
		printHelp();
		return 0;
	}

	return 1;
}












