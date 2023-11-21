gcc -Wall -c hashmap.c -o hashmap.o
gcc -Wall -c word.c -o word.o
gcc -Wall -c file_operations.c -o file_operations.o
gcc -Wall -g  -c main.c -o main.o
gcc -o indexer file_operations.o hashmap.o main.o word.o -lm
rm *.o