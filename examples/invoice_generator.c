#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include "../wkgtkprinter.h"

// Function to replace placeholders
char* replace_placeholder(const char* template, const char* placeholder, const char* value) {
    GRegex *regex;
    char *result;
    GError *error = NULL;

    regex = g_regex_new(placeholder, 0, 0, &error);
    if (error) {
        g_warning("Error creating regex: %s", error->message);
        g_error_free(error);
        return g_strdup(template);
    }

    result = g_regex_replace_literal(regex, template, -1, 0, value, 0, &error);
    if (error) {
        g_warning("Error replacing string: %s", error->message);
        g_error_free(error);
        g_regex_unref(regex);
        return g_strdup(template);
    }

    g_regex_unref(regex);
    return result;
}

int main(int argc, char *argv[]) {
    // Initialize wkgtkprinter (single-threaded mode)
    wkgtkprinter_gtk_init();

    char *template_path = "invoice_template.html";
    char *template_content;
    gsize length;
    GError *error = NULL;

    if (!g_file_get_contents(template_path, &template_content, &length, &error)) {
        // Try looking in examples/ if running from root
        g_clear_error(&error);
        template_path = "examples/invoice_template.html";
        if (!g_file_get_contents(template_path, &template_content, &length, &error)) {
            fprintf(stderr, "Error loading template: %s\n", error->message);
            g_error_free(error);
            return 1;
        }
    }

    // Sample data
    const char *customer_name = "John Doe";
    const char *date = "2023-10-27";
    const char *invoice_number = "INV-001";
    const char *total = "$1,250.00";

    // Items rows HTML
    const char *items_rows =
        "<tr><td>Web Design Services</td><td>1</td><td>$500.00</td><td>$500.00</td></tr>"
        "<tr><td>Backend Development</td><td>15</td><td>$50.00</td><td>$750.00</td></tr>";

    // Perform replacements. Note: {{ and }} must be escaped for regex
    char *step1 = replace_placeholder(template_content, "\\{\\{customer_name\\}\\}", customer_name);
    char *step2 = replace_placeholder(step1, "\\{\\{date\\}\\}", date);
    char *step3 = replace_placeholder(step2, "\\{\\{invoice_number\\}\\}", invoice_number);
    char *step4 = replace_placeholder(step3, "\\{\\{total\\}\\}", total);
    char *final_html = replace_placeholder(step4, "\\{\\{items_rows\\}\\}", items_rows);

    g_free(template_content);
    g_free(step1);
    g_free(step2);
    g_free(step3);
    g_free(step4);

    // Output PDF path
    char *cwd = g_get_current_dir();
    char *output_uri = g_strdup_printf("file://%s/invoice.pdf", cwd);
    g_free(cwd);

    printf("Generating invoice to %s...\n", output_uri);

    // base_uri is NULL, so relative paths in HTML might not work unless full path is provided.
    // For this example, everything is embedded or absolute.
    wkgtkprinter_html2pdf(NULL, final_html, NULL, output_uri, NULL, NULL);

    printf("Done.\n");

    g_free(final_html);
    g_free(output_uri);

    return 0;
}
