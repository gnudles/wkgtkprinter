libwkgtkprinter.so: wkgtkprinter.c Makefile
	gcc  -shared -fPIC -o libwkgtkprinter.so -g wkgtkprinter.c `pkg-config --libs --cflags gtk+-3.0 webkit2gtk-4.0`
threaded_example: libwkgtkprinter.so threaded_example.c Makefile
	gcc -g -o threaded_example threaded_example.c -L. -lwkgtkprinter `pkg-config --libs --cflags gtk+-3.0 webkit2gtk-4.0`
gui_example_deprecated: gui_example_deprecated.c
	gcc -o gui_example_deprecated gui_example_deprecated.c `pkg-config --libs --cflags gtk+-3.0 webkit2gtk-4.0`
