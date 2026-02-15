PKG_CONF = `pkg-config --libs --cflags gtk+-3.0 webkit2gtk-4.1`
all: libwkgtkprinter.so wkgtkprinter
libwkgtkprinter.so: wkgtkprinter.c Makefile
	gcc -g -shared -fPIC -o libwkgtkprinter.so wkgtkprinter.c $(PKG_CONF)
threaded_example: libwkgtkprinter.so threaded_example.c Makefile
	gcc -g -o threaded_example threaded_example.c -L. -lwkgtkprinter $(PKG_CONF)
gui_example_deprecated: gui_example_deprecated.c
	gcc -o gui_example_deprecated gui_example_deprecated.c $(PKG_CONF)
wkgtkprinter: libwkgtkprinter.so main.c
	gcc -g -o wkgtkprinter main.c -L. -lwkgtkprinter $(PKG_CONF)
