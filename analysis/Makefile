CC = g++

all : process_track

process_track : process_track.cpp
	$(CC) process_track.cpp -o process_track `root-config --cflags --glibs`

clean :
	rm process_track
