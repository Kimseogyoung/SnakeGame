CC = g++
CCFLASGS = -g
NCURSES = -lncursesw
STD = -std=c++11
TH = -pthread

Snake: main.o Items.o Snake.o map.o display.o map_array.o mission.o
	$(CC) $(CCFLASGS) -o $@ $^ $(NCURSES) $(STD) $(TH)

clean:
	rm -f *.o

%.o : %.cpp %.h
	$(CC) $(CCFLASGS) -c $< $(NCURSES) $(STD) $(TH)

%.o : %.cpp
	$(CC) $(CCFLASGS) -c $< $(NCURSES) $(STD) $(TH)

% : %.cpp
	$(CC) $(CCFLASGS) -o $@ $< $(NCURSES) $(STD) $(TH)
