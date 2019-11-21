
#include <stddef.h>
void html2pdf(const char* in_uri, const char* html_txt, const char* base_uri, const char* out_uri, const char* key_file_data);

int main()
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

    html2pdf(NULL, "<div style=\"background: #ff007e;overflow: hidden;\">HELLO simple<br/>html2pdf!</div>", NULL, "file:///tmp/test1.pdf", print_settings);
    html2pdf("https://webkitgtk.org" ,NULL, NULL, "file:///tmp/test2.pdf", NULL);


    return 0;
}

