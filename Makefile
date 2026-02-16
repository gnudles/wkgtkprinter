#PKG_CONF = `pkg-config --libs --cflags gtk4 webkitgtk-6.0`
PKG_CONF = `pkg-config --libs --cflags gtk+-3.0 webkit2gtk-4.1`
CFLAGS = -g -Werror=return-type -Werror=implicit-function-declaration -Wall 
all: libwkgtkprinter.so wkgtkprinter
libwkgtkprinter.so: wkgtkprinter.c Makefile
	gcc $(CFLAGS) -shared -fPIC -o libwkgtkprinter.so wkgtkprinter.c $(PKG_CONF)
threaded_example: libwkgtkprinter.so threaded_example.c Makefile
	gcc $(CFLAGS) -o threaded_example threaded_example.c -L. -lwkgtkprinter $(PKG_CONF)
wkgtkprinter: libwkgtkprinter.so main.c
	gcc $(CFLAGS) -o wkgtkprinter main.c -L. -lwkgtkprinter $(PKG_CONF)
clean:
	rm -f libwkgtkprinter.so wkgtkprinter threaded_example gui_example_deprecated
