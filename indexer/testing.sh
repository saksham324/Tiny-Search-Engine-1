#!/bin/bash

# testing.sh - testing the tse indexer module

# Compiling common
cd ../common
make clean
make

# Compiling crawler
cd -
make clean
make

# Variables
pagedir1=../crawler/output/letters-depth-6
pagedir2=../crawler/output/wikipedia-depth-1
index1=./letters-depth-6-index
index2=./wikipedia-depth-1-index
test1=./test1
test2=./test2

# wrong pageDirectory
./indexer ../crawler $index1

# wrong number of arguments
./indexer $pagedir1 $index1 $index2



#letters-depth-6
./indexer $pagedir1 $index1

# sorting index file
gawk -f indexsort.awk $index1 > $index1.sorted

# reloading the index file
./indextest $index1 $test1

# sorting the reloaded index file
gawk -f indexsort.awk $test1 > $test1.sorted

# comparing the loaded and reloaded index files
diff -q $test1.sorted $index1.sorted

if [ $? -eq 0 ]
then
    # if no differences, clean up the index files
    echo no differences
    rm -f *.sorted
    rm -f $index1 $test1
fi



# wikipedia-depth-1
./indexer $pagedir2 $index2

# sorting index file
gawk -f indexsort.awk $index2 > $index2.sorted

# reloading the index file
./indextest $index2 $test2

# sorting the reloaded index file
gawk -f indexsort.awk $test2 > $test2.sorted

# comparing the loaded and reloaded index files
diff -q $test2.sorted $index2.sorted

if [ $? -eq 0 ]
then
    # if no differences, clean up the index files
    echo no differences
    rm -f *.sorted
    rm -f $index2 $test2
fi

#cleanup
make clean
cd -
make clean
cd -



