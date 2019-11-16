#include <stdio.h>
#include <gtk/gtk.h>
#include <webkit2/webkit2.h>

static GtkWidget *window = 0;

static void
print_finished (WebKitPrintOperation *print_operation,
               gpointer              user_data)
{
#ifndef USE_GUI
    gtk_widget_destroy(window);
#endif
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
    {
        GtkPrintSettings *print_settings = gtk_print_settings_new ();
        gtk_print_settings_set_printer (print_settings,
                                        "Print to File");
        gtk_print_settings_set (print_settings,
                                    GTK_PRINT_SETTINGS_OUTPUT_BASENAME,
                                    "test"
                                    );
        gtk_print_settings_set (print_settings,
                                  GTK_PRINT_SETTINGS_OUTPUT_FILE_FORMAT,
                                "pdf");
        gtk_print_settings_set (print_settings,
                                  GTK_PRINT_SETTINGS_OUTPUT_DIR,
                                "/tmp/");
        gtk_print_settings_set_quality (print_settings,
                                        GTK_PRINT_QUALITY_HIGH);
        gtk_print_settings_set_resolution (print_settings,
                                           320);

        gtk_print_settings_set_page_set (print_settings, GTK_PAGE_SET_ALL);

        gtk_print_settings_set_orientation (print_settings,
                                            GTK_PAGE_ORIENTATION_PORTRAIT);



        WebKitPrintOperation * print_operation = webkit_print_operation_new (web_view);
          webkit_print_operation_set_print_settings(print_operation,print_settings);
          g_signal_connect (print_operation, "finished", G_CALLBACK (print_finished), NULL);
#ifdef USE_GUI
        webkit_print_operation_run_dialog (print_operation,   GTK_WINDOW(window) );
#else
        webkit_print_operation_print (print_operation);
#endif
    }
        break;

    }

}

static void
activate (GtkApplication* app,
          gpointer        user_data)
{


  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "wkgtkprinter");
  gtk_window_set_default_size (GTK_WINDOW (window), 600, 600);
  WebKitWebView * web_view =  WEBKIT_WEB_VIEW( webkit_web_view_new() );
  g_signal_connect (web_view, "load-changed", G_CALLBACK (web_view_load_changed), NULL);
  webkit_web_view_load_html (web_view, "Hello <br/>World!",  NULL);
  //webkit_web_view_load_uri(web_view,"https://webkitgtk.org");



    gtk_container_add (GTK_CONTAINER (window), GTK_WIDGET(web_view));

#ifdef USE_GUI
  gtk_widget_show_all (window);
#endif
}

int
main (int    argc,
      char **argv)
{
  GtkApplication *app;
  int status;

  app = gtk_application_new ("org.kapandaria.wkgtkprinter", G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}


