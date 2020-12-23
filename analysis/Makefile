CC = g++

all : process_track plot_hist

process_track : process_track.cpp
	$(CC) $< -o $@ `root-config --cflags --glibs`

plot_hist : plot_hist.cpp
	$(CC) $< -o $@ `root-config --cflags --glibs`

clean :
	rm process_track
	rm plot_hist
