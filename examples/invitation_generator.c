#include <stdio.h>
#include <glib.h>
#include "../wkgtkprinter.h"

int main(int argc, char *argv[]) {
    wkgtkprinter_gtk_init();

    const char *groom = "John";
    const char *bride = "Jane";
    const char *date = "October 27, 2023";

    if (argc > 1) groom = argv[1];
    if (argc > 2) bride = argv[2];
    if (argc > 3) date = argv[3];

    // Create HTML using g_strdup_printf
    char *html = g_strdup_printf(
        "<html><head><style>"
        "body { font-family: 'Times New Roman', serif; text-align: center; border: 5px double gold; padding: 50px; background-color: #fffaf0; height: 100vh; box-sizing: border-box; }"
        "h1 { color: #8b4513; font-size: 48px; margin-bottom: 0; }"
        "h2 { color: #a0522d; font-size: 32px; margin-top: 10px; }"
        ".names { font-size: 36px; font-weight: bold; margin: 40px 0; }"
        ".date { font-size: 24px; color: #555; }"
        "</style></head><body>"
        "<h1>Wedding Invitation</h1>"
        "<h2>You are cordially invited to celebrate the wedding of</h2>"
        "<div class='names'>%s &amp; %s</div>"
        "<div class='date'>on %s</div>"
        "</body></html>",
        groom, bride, date
    );

    char *cwd = g_get_current_dir();
    char *output_uri = g_strdup_printf("file://%s/invitation.pdf", cwd);
    g_free(cwd);

    printf("Generating invitation for %s and %s to %s...\n", groom, bride, output_uri);

    wkgtkprinter_html2pdf(NULL, html, NULL, output_uri, NULL, NULL);

    g_free(html);
    g_free(output_uri);

    return 0;
}
