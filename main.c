#include <stddef.h>
void html2pdf(const char* html_txt, const char* base_uri, const char* out_uri);

int main()
{
    html2pdf("HELLO simple<br/>html2pdf", NULL, "file:///tmp/test.pdf");

    return 0;
}
