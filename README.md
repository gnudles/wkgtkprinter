# wkgtkprinter
HTML to PDF conversion library (with one simple function) using WebkitGtk (similar to wkhtmltopdf, but without the Qt part).

A simple snippet (glue code) written in C to demonstrate the conversion process of HTML to PDF using Gtk+ and WebkitGtk.

You can use this code to generate invoices from command line or from your software. you can use it from any programming language using libffi.

Very minimalistic code, and easy to tweak, embed and understand. only one function does the conversion in synchronous way.

example for use in multi-threaded non-gtk application (like web server)
```c
#include "wkgtkprinter.h"
int main()
{
 // start the main loop thread
 wkgtkprinter_gtk_mainloop_start_thread();
...

 wkgtkprinter_html2pdf(NULL, "<div style=\"background: #ff007e;overflow: hidden;\">HELLO WORLD!<br/>by wkgtkprinter</div>", NULL, (const char*)data, print_settings, NULL);

...

 //close and join the main loop
 wkgtkprinter_gtk_mainloop_stop_thread();


 return 0;
}
```
single threaded non-gtk should only contain:
```c
#include "wkgtkprinter.h"
int main()
{
 wkgtkprinter_gtk_init();
...

 wkgtkprinter_html2pdf(...);

...

 return 0;
}
```

gtk application should use only the ```wkgtkprinter_html2pdf``` function.

function usage:

```c
void wkgtkprinter_html2pdf(const char* in_uri, const char* html_txt, const char* base_uri, const char* out_uri, const char* key_file_data, const char* default_stylesheet)
// in_uri - the uri to load. eg. www.github.com
// html_txt - if in_uri is null, this html string will be rendered.
// base_uri - the uri of the environment of the html. in brief, the path that all the assets (images,css files etc.) are relative to.
// out_uri - the destination pdf file in uri. something like file:///home/user/pdf_out/x.pdf
// key_file_data - advanced printing setting in INI style. for example:
/*
"[Print Settings]\n"
    "quality=high\n"
    "resolution-x=320\n"
    "resolution-y=320\n"
    "resolution=320\n"
    "output-file-format=pdf\n"
    "printer=Print to File\n"
    "page-set=all\n"
    "\n"
            "[Page Setup]\n"
            "PPDName=A4\n"
            "DisplayName=A4\n"
            "Width=210\n"
            "Height=297\n"
            "MarginTop=6.3499999999999996\n"
            "MarginBottom=14.224\n"
            "MarginLeft=6.3499999999999996\n"
            "MarginRight=6.3499999999999996\n"
            "Orientation=landscape\n";

*/
// default_stylesheet - stylesheet to override the default css of the webview.
```

