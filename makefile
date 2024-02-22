first: parser.c
	gcc parser.c -o parser.o -c
second: parser.o operations.c first
	gcc operations.c -c -o operations.o
third: second
	gcc operations.c parser.c main.c -o ans
fourth: third
	./ans