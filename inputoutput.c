/*
 * inputoutput.c
 *
 *  Created on: 26 Xul, 2016
 *      Author: marcos
 */
#include "inputoutput.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>


/**
 * \brief Removes carry return character
 * \param string to remove last carry return character
 */
void chomp(const char *s)
{
    char *p;
    while (NULL != s && NULL != (p = strrchr(s, '\n')))
    {
        *p = '\0';
    }
}


/**
 * \brief Configure file descriptor
 * \param fileName File Name
 * \param void * fileDescriptor File Descriptor to be open
 * \param is input gzipped 1 yes 0 not
 * \param openFile Mode to read file, w write to file. r read to file.
 * \returns 1 in success otherwise 0
 */
int setupFileDescriptor(char * fileName,void ** fileDescriptor,int isZip,char * openMode)
{
	if(isZip == 0)
	{
		(*fileDescriptor) = fopen(fileName, openMode);
	}
	else
	{
		(*fileDescriptor) = gzopen(fileName, openMode);
	}

	if ((*fileDescriptor) == NULL)
	{
		printf("Sorry!! Not possible to read file: %s \n",fileName);
		printf("Errno:%i \n",errno);
		exit(EXIT_FAILURE);
	}
	return 1;
}

/**
 * \brief Close File
 * \param void * fileDescriptor File Descriptor to be open
 * \param is input gzipped 1 yes 0 not
 */
void fileCloseInput(void * fileDescriptor,int isZip)
{
	if(isZip == 1)
	{
		gzclose(fileDescriptor);
	}
	else
	{
		fclose(fileDescriptor);
	}
}


/**
 * \brief Free char * and initializates as NULL
 * \param string member to be treated
 */
void freeString(char * string)
{
	if (string != NULL)
	{
		free(string);
		string = NULL;
	}
}


/**
 * \brief Free a read fastq structure
 * \param fastqRead to be freed
 */
void freeFastqRead(struct FastqRead * fastqRead)
{
	freeString(fastqRead->header);
	freeString(fastqRead->sequence);
	freeString(fastqRead->blank);
	freeString(fastqRead->qualities);
}


/**
 * \brief Initialization Fastq Reads
 * \param fastqRead1 Record structure to be freed
 * \param fastqRead2 Record structure to be freed
 */
void freeMemFastqRead(struct FastqRead * fastqRead1,struct FastqRead * fastqRead2)
{
	freeFastqRead(fastqRead1);
	freeFastqRead(fastqRead2);
}


/**
 * \brief Get a complete FASTQ read, read new four lines from file
 * \param void * fileDescriptor File Descriptor already opened
 * \param fastqRead - FastqRead to be returned
 * \param Is input gzipped 1 yes 0 not
 * \returns 1 if there is more reads to read otherwise 0
 */
int getFastqRead(void * fileDescriptor,struct FastqRead * fastqRead,int isGzip)
{
	/*1. GET fastq READ */
	if(isGzip == 1)
	{
		char * readGz;
		char buffer[LENGTH];

		unsigned int nLines = 0;

		while (nLines < 4)
		{
			readGz = gzgets (fileDescriptor, buffer, LENGTH - 1);
			chomp(readGz);

			switch (nLines)
			{
				case 0:
					if (readGz == NULL)
					{
						return 0;
						break;
					}
					fastqRead->header = malloc (strlen (readGz) + 1);
					strcpy (fastqRead->header,readGz);
					break;
				case 1:
					fastqRead->sequence = malloc (strlen (readGz) + 1);
					strcpy (fastqRead->sequence,readGz);
					break;
				case 2:
					fastqRead->blank = malloc (strlen (readGz) + 1);
					strcpy (fastqRead->blank,readGz);
					break;
				case 3:
					fastqRead->qualities = malloc (strlen (readGz) + 1);
					strcpy (fastqRead->qualities,readGz);
					break;
			}

			nLines = nLines + 1;
		}
        return 1;
	}
	else
	{
		char * line = NULL;
		size_t len = 0;
		ssize_t read;

		unsigned int nLines = 0;

		while (nLines < 4)
		{
			read = getline(&line, &len, fileDescriptor);
			chomp(line);

			switch (nLines)
			{
				case 0:
					if (strlen(line) == 0)
					{
						return 0;
						break;
					}
					fastqRead->header = malloc (strlen (line) + 1);
					strcpy (fastqRead->header,line);
					break;
				case 1:
					fastqRead->sequence = malloc (strlen (line) + 1);
					strcpy (fastqRead->sequence,line);
					break;
				case 2:
					fastqRead->blank = malloc (strlen (line) + 1);
					strcpy (fastqRead->blank,line);
					break;
				case 3:
					fastqRead->qualities = malloc (strlen (line) + 1);
					strcpy (fastqRead->qualities,line);
					break;
			}

			nLines = nLines + 1;
		}
		return 1;
	}
    return 0;
}



/**
 * \brief Get a complete FASTQ read, read new four lines from file
 * \param void * fileDescriptor File Descriptor to be open
 * \param fastqRead - FastqRead to be returned
 * \param Is input gzipped 1 yes 0 not
 * \returns 1 if there is more reads to read otherwise 0
 */
int writeFastqRead(void * fileDescriptor,struct FastqRead * fastqRead,int isGzip)
{
	if(isGzip)
	{
		/*Header*/
		if (gzputs(fileDescriptor,fastqRead->header) < 0) return 0;
		if (gzputs(fileDescriptor,"\n") < 0) return 0;
		/*Sequence*/
		if (gzputs(fileDescriptor,fastqRead->sequence) < 0) return 0;
		if (gzputs(fileDescriptor,"\n") < 0) return 0;
		/*Blank*/
		if (gzputs(fileDescriptor,fastqRead->blank) < 0) return 0;
		if (gzputs(fileDescriptor,"\n") < 0) return 0;
		/*Qualities*/
		if (gzputs(fileDescriptor,fastqRead->qualities) < 0) return 0;
		if (gzputs(fileDescriptor,"\n") < 0) return 0;
	}
	else
	{
		/*Header*/
		if (fputs(fastqRead->header,fileDescriptor) < 0) return 0;
		if (fputs("\n",fileDescriptor) < 0) return 0;
		/*Sequence*/
		if (fputs(fastqRead->sequence,fileDescriptor) < 0) return 0;
		if (fputs("\n",fileDescriptor) < 0) return 0;
		/*Blank*/
		if (fputs(fastqRead->blank,fileDescriptor) < 0) return 0;
		if (fputs("\n",fileDescriptor) < 0) return 0;
		/*Qualities*/
		if (fputs(fastqRead->qualities,fileDescriptor) < 0) return 0;
		if (fputs("\n",fileDescriptor) < 0) return 0;
	}

	return 1;
}



 /**
  * \brief Get Stats Pairs
  * \param void * fileDescriptor File Descriptor already opened
  * \param statsInfo - StatsInfo to be returned
  * \returns 1 if there is more reads to read otherwise 0
  */
 int getStatsPairs(void * fileDescriptor,struct StatsInfo * statsInfo)
 {
 	/*Format to Parse: [numberOfBases(tab)lengthReference(tab)coverage] */
	char * line = NULL;
	size_t len = 0;
 	ssize_t read;
 	char * pch;

 	read = getline(&line, &len, fileDescriptor);

 	if (strlen(line) == 0)
	{
		return 0;
	}

 	chomp(line);

 	/* Split line in tabs and get fields*/
 	pch = strtok (line," \t");
 	unsigned int field = 0;

 	while (pch != NULL)
	{
		char * content = pch;

		switch(field)
		{
			case 0:
				statsInfo->numberBases = atoll(content);
				break;
			case 1:
				statsInfo->referenceLength = atoll(content);
				break;
			case 2:
				statsInfo->coverge = atof(content);
				break;
		}

		field = field + 1;
		pch = strtok (NULL, " \t");
	}

 	return 1;
 }
