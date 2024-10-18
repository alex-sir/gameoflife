# Name of executable
BINS = game_of_life

# Compile using MPI
CC = mpicc

# Setting the flags
CFLAGS = -g -Wall

# Link in support for debugging
LDFLAGS = -g

# Default target produced by entering "make" alone
.PHONY: default
default: $(BINS)

# Compile *.c into *.o
src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Remove *.o files, but leave executable
.PHONY: clean
clean:
	rm -f core* src/*.o *~

# Link *.o files into an executable
game_of_life: src/game_of_life.o src/board.o
	$(CC) $(LDFLAGS) $^ -o $@

# Remove all files that can be reconstructed through "make"
.PHONY: immaculate
immaculate: clean
	rm -f $(BINS)
