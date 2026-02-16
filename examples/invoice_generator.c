#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include "../wkgtkprinter.h"

// Helper function to replace placeholders
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

// Dedicated function to create HTML
char* create_invoice_html(const char* template_path,
                          const char* customer_name,
                          const char* date,
                          const char* invoice_number,
                          const char* total,
                          const char* items_rows) {
    char *template_content;
    gsize length;
    GError *error = NULL;

    // Try current directory first, then examples/
    if (!g_file_get_contents(template_path, &template_content, &length, &error)) {
        g_clear_error(&error);
        char *alt_path = g_strdup_printf("examples/%s", template_path);
        if (!g_file_get_contents(alt_path, &template_content, &length, &error)) {
            fprintf(stderr, "Error loading template '%s': %s\n", template_path, error->message);
            g_error_free(error);
            g_free(alt_path);
            return NULL;
        }
        g_free(alt_path);
    }

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

    return final_html;
}

static gchar *customer_name = "John Doe";
static gchar *date = "2023-10-27";
static gchar *invoice_number = "INV-001";
static gchar *total = "$0.00";
static gchar *items_rows = "<tr><td>Sample Item</td><td>1</td><td>$0.00</td><td>$0.00</td></tr>";
static gchar *output_file = "invoice.pdf";
static gchar *template_file = "invoice_template.html";

static GOptionEntry entries[] = {
    { "customer", 'c', 0, G_OPTION_ARG_STRING, &customer_name, "Customer Name", "NAME" },
    { "date", 'd', 0, G_OPTION_ARG_STRING, &date, "Invoice Date", "DATE" },
    { "number", 'n', 0, G_OPTION_ARG_STRING, &invoice_number, "Invoice Number", "NUMBER" },
    { "total", 't', 0, G_OPTION_ARG_STRING, &total, "Total Amount", "AMOUNT" },
    { "items", 'i', 0, G_OPTION_ARG_STRING, &items_rows, "Items Rows (HTML <tr>...</tr>)", "HTML" },
    { "output", 'o', 0, G_OPTION_ARG_STRING, &output_file, "Output PDF file", "FILE" },
    { "template", 'T', 0, G_OPTION_ARG_STRING, &template_file, "Template HTML file", "FILE" },
    { NULL }
};

int main(int argc, char *argv[]) {
    GError *error = NULL;
    GOptionContext *context;

    context = g_option_context_new ("- Generate PDF invoice from HTML template");
    g_option_context_add_main_entries (context, entries, NULL);
    /* Parse options */
    if (!g_option_context_parse (context, &argc, &argv, &error)) {
        g_print ("option parsing failed: %s\n", error->message);
        exit (1);
    }

    // Initialize wkgtkprinter
    wkgtkprinter_gtk_init();

    char *final_html = create_invoice_html(template_file, customer_name, date, invoice_number, total, items_rows);
    if (!final_html) {
        return 1;
    }

    char *cwd = g_get_current_dir();
    char *output_uri = NULL;

    if (g_path_is_absolute(output_file)) {
        output_uri = g_strdup_printf("file://%s", output_file);
    } else {
        output_uri = g_strdup_printf("file://%s/%s", cwd, output_file);
    }
    g_free(cwd);

    printf("Generating invoice to %s...\n", output_uri);
    wkgtkprinter_html2pdf(NULL, final_html, NULL, output_uri, NULL, NULL);
    printf("Done.\n");

    g_free(final_html);
    g_free(output_uri);
    g_option_context_free(context);

    return 0;
}
