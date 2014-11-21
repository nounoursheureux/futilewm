CFLAGS = -g -Wall -Wextra -pedantic -Wshadow 
LIBS = -lX11

all: futilewm

futilewm: futile.o
	gcc $^ -o futilewm $(CFLAGS) $(LIBS)

%.o: %.c 
	gcc -o $@ -c $< $(LIBS) $(CFLAGS)

clean: 
	rm -rf *.o 

mrproper: clean
	rm futilewm
