=============
fastqCoverage
=============
  
fastqCoverage allows you to have a raw coverage estimation of your paired end or single end FASTQ files. It is, also, able to downsample the coverage of your paired end 
and single end files.

If your sequenced sample has a set of Paired End or Single End files you can calculate the coverage per each one and then use getStats command to have the total coverage
of your sample. 

---------
Licensing
---------

fastqCoverage is licensed under GPL. See LICENSE for more information.

-----------
Compilation
-----------

1) For common usage compile the Release version:

    cd fastqCoverage/Release
    make clean
    make

A binary should be generated at Release folder called fastqCoverage.

2) For testing usage compile the Debug version:

    cd fastqCovergae/Debug
    make clean
    make

A debug binary should be created at Debug folder called fastqCoverage.

-------------
Documentation
-------------

fastqCoverage has three commands:

1) Downsampling 
---------------

Outputs a couple of paired end files with a reduced number of reads according to a decrease ratio coverage.

    fastqCoverage downsampling -r 0.5 -a original_pair_1.fastq.gz -b original_pair_2.fastq.gz -x output_pair_1.fastq.gz -y output_pair_2.fastq.gz -z

Reduces half coverage (-r 0.5) gzipped input Paired End fastq files. 

Outputs a single end file with a reduced number of reads according to a decrease ratio coverage.

    fastqCoverage single-end downsampling -r 0.5 -a original_single_end_file.fq.gz  -x output_single_end_file.fq.gz -z

Reduces half coverage (-r 0.5) gzipped input Single End fastq file. 


2) Raw Coverage
---------------

Outputs a coverage estimation from a given reference genome length in base pairs.

    fastqCoverage getCoverage -a pair_1.fastq.gz -b pair_2.fastq.gz -l reference_length -z

Outputs a coverage estimation from a given reference genome length in base pairs for a Single End file.

    fastqCoverage single-end getCoverage -a original_single_end_file.fq.gz -l reference_length -z

-----------------
3) Total Coverage
-----------------

From a coverage stats file outputs the total coverage.

If we have a sample with a set of paired end files as:

    file_A_1.fastq  file_A_2.fastq
    file_B_1.fastq  file_B_2.fastq

We can estimate the coverage per couple of paired end files as:

    fastqCoverage getCoverage -a file_A_1.fastq -b file_A_2.fastq -l reference_length > partialStats
    fastqCoverage getCoverage -a file_B_1.fastq -b file_B_2.fastq -l reference_length >> partialStats

And eventually get an estimation of total stats:
 
    fastqCoverage getStats -s partialStats

If we have a mixture of Single End and Paired End files then the procedure would be:

    file_A_1.fastq  file_A_2.fastq
    file_B_1.fastq  file_B_2.fastq
    file_C.fastq
    file_D.fastq

Get Coverages:

    fastqCoverage getCoverage -a file_A_1.fastq -b file_A_2.fastq -l reference_length > partialStats
    fastqCoverage getCoverage -a file_B_1.fastq -b file_B_2.fastq -l reference_length >> partialStats
    fastqCoverage single-end getCoverage -a file_C.fastq -l reference_length >> partialStats
    fastqCoverage single-end getCoverage -a file_D.fastq -l reference_length >> partialStats

Get all coverage:

    fastqCoverage getStats -s partialStats

You can get more help from:

    fastqCoverage -h

or per each command:

    fastqCoverage downsampling -h
    fastqCoverage getCoverage -h
    fastqCoverage getStats -h

------
Author
------

Marcos Fernandez-Callejo at (CNAG/CRG) Centre Nacional d’Anàlisi Genòmica / Centre de Regulació Genòmica.
marcos.fernandez@cnag.crg.eu
  
