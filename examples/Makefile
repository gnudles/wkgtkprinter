CC = gcc
CFLAGS = -g `pkg-config --cflags gtk+-3.0 webkit2gtk-4.1` -I..
LIBS = `pkg-config --libs gtk+-3.0 webkit2gtk-4.1` -L.. -lwkgtkprinter

all: invoice_generator card_game_generator threaded_example gui_example

invoice_generator: invoice_generator.c ../libwkgtkprinter.so
	$(CC) $(CFLAGS) -o invoice_generator invoice_generator.c $(LIBS)

card_game_generator: card_game_generator.c ../libwkgtkprinter.so
	$(CC) $(CFLAGS) -o card_game_generator card_game_generator.c $(LIBS)

threaded_example: threaded_example.c ../libwkgtkprinter.so
	$(CC) $(CFLAGS) -o threaded_example threaded_example.c $(LIBS)

gui_example: gui_example.c ../libwkgtkprinter.so
	$(CC) $(CFLAGS) -o gui_example gui_example.c $(LIBS)
clean:
	rm -f invoice_generator card_game_generator threaded_example gui_example *.pdf
