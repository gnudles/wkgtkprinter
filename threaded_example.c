
#include <stddef.h>
#include <glib.h>
#include "wkgtkprinter.h"
const char * outputs[3]=
{"file:///tmp/test1.pdf","file:///tmp/test2.pdf","file:///tmp/test3.pdf"};

gpointer t_func(gpointer data)
{
    const char* print_settings = "[Print Settings]\n"
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

    wkgtkprinter_html2pdf(NULL, "<div style=\"background: #ff007e;overflow: hidden;\">HELLO WORLD!<br/>by wkgtkprinter</div>", NULL, (const char*)data, print_settings, NULL);
}
int main()
{
 wkgtkprinter_gtk_mainloop_start_thread();
GThread *t1=
    g_thread_new ("render1",
              t_func,
              "file:///tmp/test1.pdf");
GThread *t2=
    g_thread_new ("render2",
              t_func,
              "file:///tmp/test2.pdf");
GThread *t3=
    g_thread_new ("render3",
              t_func,
              "file:///tmp/test3.pdf");
g_thread_join (t1);
g_thread_join (t2);
g_thread_join (t3);
wkgtkprinter_gtk_mainloop_stop_thread();


    return 0;
}

