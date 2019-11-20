#include <stddef.h>
void html2pdf(const char* in_uri, const char* html_txt, const char* base_uri, const char* out_uri);

int main()
{
    html2pdf(NULL, "<div style=\"background: #ff007e;overflow: hidden;\">HELLO simple<br/>html2pdf!</div>", NULL, "file:///tmp/test1.pdf");
    html2pdf("https://webkitgtk.org" ,NULL, NULL, "file:///tmp/test2.pdf");


    return 0;
}
