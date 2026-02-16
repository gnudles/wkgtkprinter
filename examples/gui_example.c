#include <gtk/gtk.h>
#include <webkit2/webkit2.h>
#include "wkgtkprinter.h"

static WebKitWebView *web_view;
static GtkWidget *window;

static void
print_activated (GtkButton *button,
                 gpointer   user_data)
{
    wkgtkprinter_html2pdf(NULL, "<h1>Hello World</h1><p>This is a test.</p>", "file://.", "file:///tmp/gui_example.pdf", NULL, NULL);

    GtkWidget *dialog = gtk_message_dialog_new (GTK_WINDOW (window),
                                               GTK_DIALOG_DESTROY_WITH_PARENT,
                                               GTK_MESSAGE_INFO,
                                               GTK_BUTTONS_OK,
                                               "PDF saved to /tmp/gui_example.pdf");
    gtk_dialog_run (GTK_DIALOG (dialog));
    gtk_widget_destroy (dialog);
}

static void
activate (GtkApplication* app,
          gpointer        user_data)
{
    window = gtk_application_window_new (app);
    gtk_window_set_title (GTK_WINDOW (window), "wkgtkprinter GUI Example");
    gtk_window_set_default_size (GTK_WINDOW (window), 800, 600);

    GtkWidget *header_bar = gtk_header_bar_new ();
    gtk_header_bar_set_show_close_button (GTK_HEADER_BAR (header_bar), TRUE);
    gtk_window_set_titlebar (GTK_WINDOW (window), header_bar);

    GtkWidget *print_button = gtk_button_new_with_label ("Print");
    gtk_header_bar_pack_end (GTK_HEADER_BAR (header_bar), print_button);
    g_signal_connect (print_button, "clicked", G_CALLBACK (print_activated), NULL);

    GtkWidget *scrolled_window = gtk_scrolled_window_new (NULL, NULL);
    web_view = WEBKIT_WEB_VIEW (webkit_web_view_new ());
    webkit_web_view_load_html (web_view, "<h1>Hello World</h1><p>This is a test.</p>", NULL);

    gtk_container_add (GTK_CONTAINER (scrolled_window), GTK_WIDGET (web_view));
    gtk_container_add (GTK_CONTAINER (window), scrolled_window);

    gtk_widget_show_all (window);
}

int
main (int    argc,
      char **argv)
{
    wkgtkprinter_gtk_init();
    GtkApplication *app = gtk_application_new ("org.wkgtkprinter.gui-example", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
    int status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);
    return status;
}
