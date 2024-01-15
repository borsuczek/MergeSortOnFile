# MergeSortOnFile
The project implements the natural merge sort algorithm on 2+1 tapes. The program sorts sets of real numbers (up to 15 numbers per set) in non-decreasing order based on the sum of numbers in those sets. The buffer size is constant and accommodates 6 records.

# File format
The file contains records, each record is written in a new line and consists of 15 real numbers separated by spaces. If a set contains fewer than 15 numbers, it should be padded with zeros (zeros do not affect the sum of numbers in the set and do not influence the sorting outcome). Each record has a size of 15*8 bytes (double has 8 bytes), totaling 120 bytes.

# Input data
There are three ways to provide data for sorting. Random sets can be generated (the program prompts for the number of records to generate and the range for the generated numbers). Alternatively, you can specify an input file containing unsorted records or manually input data in the console. When entering data in the console, the input should end with the word "end" on a new line. 

# Output data
If an unsorted file is not specified, the program operates on the "file.txt" file, where the sorted data is also stored at the end. Additionally, the program displays the content of the initial and final (sorted) file in the console. It also provides an option to display the file's content after each sorting phase.
