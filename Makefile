CC = gcc
FILES = out.wav fmgen 

default: fmgen.c
	${CC} fmgen.c -o fmgen -lm -lsndfile 

clean:
	rm ${FILES}

