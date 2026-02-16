#include <stdio.h>
#include <glib.h>
#include "../wkgtkprinter.h"

int main(int argc, char *argv[]) {
    wkgtkprinter_gtk_init();

    // A4 size is roughly 210mm x 297mm.
    // To fit 25 cards (5x5 grid), we need to size them appropriately.
    // 210mm / 5 = 42mm width
    // 297mm / 5 = 59.4mm height
    // We'll leave some margin, so let's aim for cards approx 38mm x 55mm.

    GString *html = g_string_new(
        "<html><head><style>"
        "@page { size: A4; margin: 0; }"
        "body { font-family: Arial, sans-serif; background-color: white; margin: 0; padding: 0mm 4mm 0mm 4mm; }"
        "table { width: 100%; border-collapse: separate; border-spacing: 2mm; }"
        "td { padding: 0; }"
        ".card { "
        "   width: 36mm; height: 53mm; "
        "   background-color: white; border: 1px solid #ccc; border-radius: 4px; "
        "   position: relative; box-shadow: 1px 1px 3px rgba(0,0,0,0.2); "
        "   display: flex; flex-direction: column; justify-content: space-between; padding: 2mm; box-sizing: border-box;"
        "}"
        ".card.red { color: red; }"
        ".card.black { color: black; }"
        ".rank-suit { font-size: 16px; font-weight: bold; display: flex; flex-direction: column; align-items: center; line-height: 1; }"
        ".center { font-size: 40px; align-self: center; }"
        ".bottom-right { transform: rotate(180deg); }"
        "</style></head><body>"
        "<table>"
    );

    const char *suits[] = { "♠", "♥", "♦", "♣" };
    const char *suit_classes[] = { "black", "red", "red", "black" };
    const char *ranks[] = { "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K" };

    int count = 0;
    int cards_per_row = 5;
    int rows_per_page = 5;
    int cards_per_page = cards_per_row * rows_per_page;

    // We want 25 cards for the per page
    // We have 52 cards total, so we'll print all of them and they will spill to next pages.

    int total_cards_to_print = 52;

    // To show variety, let's just loop through deck and stop at 25.
    int card_idx = 0;

    for (int i = 0; i < total_cards_to_print; i++) {
        int s = (card_idx / 13) % 4;
        int r = card_idx % 13;

        if (count % cards_per_row == 0) {
            g_string_append(html, "<tr>");
        }

        g_string_append_printf(html,
            "<td><div class='card %s'>"
            "<div class='rank-suit'><span>%s</span><span>%s</span></div>"
            "<div class='center'>%s</div>"
            "<div class='rank-suit bottom-right'><span>%s</span><span>%s</span></div>"
            "</div></td>",
            suit_classes[s], ranks[r], suits[s], suits[s], ranks[r], suits[s]
        );

        count++;
        if (count % cards_per_row == 0) {
            g_string_append(html, "</tr>");
        }
        if (count % cards_per_page == 0 && count < total_cards_to_print)
        {
          g_string_append_printf(html,
            "</table><table>");
        }

        card_idx++;
        if (card_idx >= 52) card_idx = 0; // Wrap around if we wanted more than 52
    }

    // Close last row if not full
    if (count % cards_per_row != 0) {
        g_string_append(html, "</tr>");
    }

    g_string_append(html, "</table></body></html>");

    char *cwd = g_get_current_dir();
    char *output_uri = g_strdup_printf("file://%s/cards.pdf", cwd);
    g_free(cwd);

    printf("Generating card sheet to %s...\n", output_uri);

    wkgtkprinter_html2pdf(NULL, html->str, NULL, output_uri, NULL, NULL);
    printf("%s", html->str);
    g_string_free(html, TRUE);
    g_free(output_uri);

    return 0;
}
