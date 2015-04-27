Zacharias Shufflebarger
CS360
February 2, 2015

Program interface:

	wordpairs <-count> filename1 <filename2> <filename3> ...

wordpairs reads through the files supplied and tracks each word pair that
occurs in the text as well as the frequency of each pair. The optional
argument -count allows the user to limit the number of lines printed by the
program; if the argument is not supplied the program will print every pair
that occured in every file supplied. 
The sorting algorithm used in this program is the library function qsort.
The hash table and the constituent nodes are structures defined in the 
hash.h file.
The hash table automatically grows as word pairs are insterted whenever
the number of unique pairs reaches 75% of the length of the table.
The time taken to sort the function is recorded and printed.

Note: When using the makefile, be sure to have an environment variable GET_WORD that references the directory containing getWord.h. Alternatively, use the command “export GET_WORD=<the path to getWord.h>”

Included files:
main.c
hash.c
hash.h
Makefile
README.txt
