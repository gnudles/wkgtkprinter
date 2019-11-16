all:wkgtkprinter_test
libwkgtkprinter.so:
	gcc -shared -fPIC -o libwkgtkprinter.so wkgtkprinter.c `pkg-config --libs --cflags gtk+-3.0 webkit2gtk-4.0`
wkgtkprinter_test: libwkgtkprinter.so
	gcc  -o wkgtkprinter_test main.c -L. -lwkgtkprinter `pkg-config --libs gtk+-3.0 webkit2gtk-4.0`
