#include <stdio.h>
#include <glib.h>
#include "../wkgtkprinter.h"

int main(int argc, char *argv[]) {
    wkgtkprinter_gtk_init();

    GString *html = g_string_new(
        "<html><head><style>"
        "body { font-family: Arial, sans-serif; background-color: #333; padding: 20px; display: flex; flex-wrap: wrap; }"
        ".card { width: 100px; height: 140px; background-color: white; border-radius: 8px; margin: 10px; padding: 10px; position: relative; box-shadow: 2px 2px 5px rgba(0,0,0,0.5); display: flex; flex-direction: column; justify-content: space-between; }"
        ".card.red { color: red; }"
        ".card.black { color: black; }"
        ".rank-suit { font-size: 20px; font-weight: bold; display: flex; flex-direction: column; align-items: center; line-height: 1; }"
        ".center { font-size: 40px; align-self: center; }"
        ".bottom-right { transform: rotate(180deg); }"
        "</style></head><body>"
    );

    const char *suits[] = { "♠", "♥", "♦", "♣" };
    const char *suit_classes[] = { "black", "red", "red", "black" };
    const char *ranks[] = { "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K" };

    for (int s = 0; s < 4; s++) {
        for (int r = 0; r < 13; r++) {
            g_string_append_printf(html,
                "<div class='card %s'>"
                "<div class='rank-suit'><span>%s</span><span>%s</span></div>"
                "<div class='center'>%s</div>"
                "<div class='rank-suit bottom-right'><span>%s</span><span>%s</span></div>"
                "</div>",
                suit_classes[s], ranks[r], suits[s], suits[s], ranks[r], suits[s]
            );
        }
    }

    g_string_append(html, "</body></html>");

    char *cwd = g_get_current_dir();
    char *output_uri = g_strdup_printf("file://%s/cards.pdf", cwd);
    g_free(cwd);

    printf("Generating card sheet to %s...\n", output_uri);

    wkgtkprinter_html2pdf(NULL, html->str, NULL, output_uri, NULL, NULL);

    g_string_free(html, TRUE);
    g_free(output_uri);

    return 0;
}
