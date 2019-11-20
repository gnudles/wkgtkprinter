#include <stdio.h>
#include <gtk/gtk.h>
#include <webkit2/webkit2.h>


struct WkGtkPrinterUserData
{
    GtkPrintSettings * print_settings;
    WebKitPrintOperation * print_operation;
    GMainLoop * main_loop;
};
#define WK_GTK_UDATA ((struct WkGtkPrinterUserData*)user_data)
static void
print_finished (WebKitPrintOperation *print_operation,
               gpointer              user_data)
{
    g_main_loop_quit (WK_GTK_UDATA->main_loop);
}
static void web_view_load_changed (WebKitWebView  *web_view,
                                   WebKitLoadEvent load_event,
                                   gpointer        user_data)
{

    switch (load_event) {
    case WEBKIT_LOAD_STARTED:
        /* New load, we have now a provisional URI */
        //printf("WEBKIT_LOAD_STARTED\n");


        break;
    case WEBKIT_LOAD_REDIRECTED:
        break;
    case WEBKIT_LOAD_COMMITTED:
        /* The load is being performed. Current URI is
         * the final one and it won't change unless a new
         * load is requested or a navigation within the
         * same page is performed */

        break;
    case WEBKIT_LOAD_FINISHED:
        //printf("WEBKIT_LOAD_FINISHED\n");
        printf ("printing pdf file: %s\n",gtk_print_settings_get(WK_GTK_UDATA->print_settings, GTK_PRINT_SETTINGS_OUTPUT_URI));
    {


        GtkPrintSettings *print_settings = WK_GTK_UDATA->print_settings;
        WebKitPrintOperation * print_operation = WK_GTK_UDATA->print_operation;
        webkit_print_operation_print (print_operation);
    }
        break;

    }

}

void html2pdf(const char* in_uri, const char* html_txt, const char* base_uri, const char* out_uri)
{

    gtk_init_check (NULL,
                    NULL);
    struct WkGtkPrinterUserData user_data;
    GtkPrintSettings *print_settings = gtk_print_settings_new ();
    gtk_print_settings_set_printer (print_settings,
                                    "Print to File");
    gtk_print_settings_set (print_settings,
                              GTK_PRINT_SETTINGS_OUTPUT_FILE_FORMAT,
                            "pdf");
    /*gtk_print_settings_set (print_settings,
                              GTK_PRINT_SETTINGS_OUTPUT_DIR,
                            "/tmp/");
    gtk_print_settings_set (print_settings,
                                GTK_PRINT_SETTINGS_OUTPUT_BASENAME,
                                "html2pdf_test"
                                );*/ // commented out in favor of GTK_PRINT_SETTINGS_OUTPUT_URI
    gtk_print_settings_set (print_settings,
                                GTK_PRINT_SETTINGS_OUTPUT_URI,
                                out_uri
                                );
    gtk_print_settings_set_quality (print_settings,
                                    GTK_PRINT_QUALITY_HIGH);
    gtk_print_settings_set_resolution (print_settings,
                                       320);

    gtk_print_settings_set_page_set (print_settings, GTK_PAGE_SET_ALL);

    gtk_print_settings_set_orientation (print_settings,
                                        GTK_PAGE_ORIENTATION_PORTRAIT);

    user_data.print_settings = print_settings;


    WebKitWebView * web_view =  WEBKIT_WEB_VIEW( webkit_web_view_new_with_context (webkit_web_context_get_default ()) );
    g_object_ref_sink(G_OBJECT(web_view));

    WebKitPrintOperation * print_operation = webkit_print_operation_new (web_view);
    webkit_print_operation_set_print_settings(print_operation,print_settings);
    g_signal_connect (print_operation, "finished", G_CALLBACK (print_finished), &user_data);
    user_data.print_operation = print_operation;

    GMainLoop * main_loop = g_main_loop_new(NULL,false);
    user_data.main_loop = main_loop;
    g_signal_connect (web_view, "load-changed", G_CALLBACK (web_view_load_changed), &user_data);
    if (in_uri== NULL)
    {
        webkit_web_view_load_html (web_view, html_txt, base_uri);
    }
    else
    {
        webkit_web_view_load_uri(web_view, in_uri);
    }

    g_main_loop_run (main_loop);

    g_main_loop_unref (main_loop);

    g_object_unref( G_OBJECT(print_operation));
    g_object_unref( G_OBJECT(print_settings));

    gtk_widget_destroy(GTK_WIDGET(web_view));
    g_object_unref( G_OBJECT(web_view));
}

