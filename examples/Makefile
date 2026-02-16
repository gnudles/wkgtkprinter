CC = gcc
CFLAGS = -g `pkg-config --cflags gtk+-3.0 webkit2gtk-4.1`
LIBS = `pkg-config --libs gtk+-3.0 webkit2gtk-4.1` -L.. -lwkgtkprinter

all: invoice_generator card_game_generator

invoice_generator: invoice_generator.c
	$(CC) $(CFLAGS) -o invoice_generator invoice_generator.c $(LIBS)

card_game_generator: card_game_generator.c
	$(CC) $(CFLAGS) -o card_game_generator card_game_generator.c $(LIBS)

clean:
	rm -f invoice_generator card_game_generator *.pdf
