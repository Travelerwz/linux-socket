all:ser cli

LINK = -lpthread
ser:ser.c
	gcc ser.c -o ser $(LINK)
cli:cli.c
	gcc cli.c -o cli $(LINK)

clean:
	rm -f cli ser
