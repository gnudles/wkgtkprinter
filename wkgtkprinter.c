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
        //printf ("printing pdf file: %s\n",gtk_print_settings_get(WK_GTK_UDATA->print_settings, GTK_PRINT_SETTINGS_OUTPUT_URI));
    {

        WebKitPrintOperation * print_operation = WK_GTK_UDATA->print_operation;
        webkit_print_operation_print (print_operation);
    }
        break;

    }

}
static int nonthreaded_init = 0 ;
void wkgtkprinter_gtk_init()
{
	gtk_init_check (NULL,  NULL);
	nonthreaded_init = 1;
}

static GThread * main_mainloop_thread = 0;
static GMainLoop * main_mainloop = 0;
static gpointer mainloop_func(gpointer data)
{
    gtk_init_check (NULL,  NULL);
    main_mainloop = g_main_loop_new(NULL,false);

    g_main_loop_run (main_mainloop);
}
void wkgtkprinter_gtk_mainloop_start_thread()
{
	if (main_mainloop_thread == NULL)
	{
        main_mainloop_thread =
    g_thread_new ("wkgtkprinter_main_loop",
              mainloop_func,
              NULL);
	while (main_mainloop == NULL)
	{
		g_usleep(1000);
	}
	while (!g_main_loop_is_running (main_mainloop))
	{
		g_usleep(1000);
	}

	}
}
void wkgtkprinter_gtk_mainloop_stop_thread()
{
    g_main_loop_quit (main_mainloop);

    g_thread_join (main_mainloop_thread);
    g_main_loop_unref (main_mainloop);
}

struct html2pdf_params
{
const char* in_uri; const char* html_txt; const char* base_uri; const char* out_uri; const char* key_file_data; const char* default_stylesheet;
GCond* wait_cond;
GMutex* wait_mutex;
int * wait_data;
};
static int __html2pdf(struct html2pdf_params *p)
{
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
    gtk_print_settings_set_quality (print_settings,
                                    GTK_PRINT_QUALITY_HIGH);
    gtk_print_settings_set_resolution (print_settings,
                                       320);

    gtk_print_settings_set_page_set (print_settings, GTK_PAGE_SET_ALL);

    gtk_print_settings_set_orientation (print_settings,
                                        GTK_PAGE_ORIENTATION_PORTRAIT);
    gtk_print_settings_set_paper_width (print_settings,
                                        500,
                                        GTK_UNIT_MM);

    GtkPageSetup * page_setup = gtk_page_setup_new ();
    gtk_page_setup_set_orientation(page_setup, GTK_PAGE_ORIENTATION_PORTRAIT);

    if (p->key_file_data!= NULL)
    {
        GKeyFile * key_file = g_key_file_new ();
        g_key_file_load_from_data(key_file, p->key_file_data, (gsize)-1, G_KEY_FILE_NONE, NULL);
        gtk_page_setup_load_key_file(page_setup,key_file, NULL, NULL);
        gtk_print_settings_load_key_file(print_settings, key_file, NULL, NULL);


        g_key_file_free (key_file);


    }
    /*GKeyFile * key_file = g_key_file_new ();
    gtk_print_settings_to_key_file(print_settings,key_file,NULL);
    gtk_page_setup_to_key_file(page_setup, key_file, NULL);
    printf("%s\n",g_key_file_to_data (key_file, NULL, NULL));
    g_key_file_free (key_file);*/

    gtk_print_settings_set (print_settings,
                                GTK_PRINT_SETTINGS_OUTPUT_URI,
                                p->out_uri
                                );

    user_data.print_settings = print_settings;

    WebKitWebContext *  web_context = webkit_web_context_new_ephemeral ();
    WebKitWebView * web_view = 0;
    WebKitUserContentManager *user_content_manager = 0;
    WebKitUserStyleSheet * user_stylesheet = 0;

    web_view =  WEBKIT_WEB_VIEW( webkit_web_view_new_with_context (web_context) );

    WebKitSettings * view_settings = webkit_web_view_get_settings (web_view);
    webkit_settings_set_enable_javascript (  view_settings,  false);
    webkit_settings_set_enable_page_cache (  view_settings,  false);
    webkit_settings_set_enable_html5_database (  view_settings,  false);
    webkit_settings_set_enable_html5_local_storage (  view_settings,  false);
    webkit_settings_set_enable_offline_web_application_cache (  view_settings,  false);

    if (p->default_stylesheet)
    {
        user_content_manager = webkit_user_content_manager_new ();
        user_stylesheet = webkit_user_style_sheet_new (p->default_stylesheet,
    	                     WEBKIT_USER_CONTENT_INJECT_ALL_FRAMES,
    	                     WEBKIT_USER_STYLE_LEVEL_USER,
    	                     NULL,
    	                     NULL);
        webkit_user_content_manager_add_style_sheet
    	                       (user_content_manager,
    	                        user_stylesheet);
         g_object_set_property(G_OBJECT(web_view),"user-content-manager", (GValue*)(user_content_manager) );
    }
    g_object_ref_sink(G_OBJECT(web_view));

    WebKitPrintOperation * print_operation = webkit_print_operation_new (web_view);
    webkit_print_operation_set_print_settings(print_operation,print_settings);
    webkit_print_operation_set_page_setup(print_operation,page_setup);
    g_signal_connect (print_operation, "finished", G_CALLBACK (print_finished), &user_data);
    user_data.print_operation = print_operation;

    GMainLoop * main_loop = g_main_loop_new(NULL,false);
    user_data.main_loop = main_loop;
    g_signal_connect (web_view, "load-changed", G_CALLBACK (web_view_load_changed), &user_data);
    if (p->in_uri == NULL)
    {
        webkit_web_view_load_html (web_view, p->html_txt, p->base_uri);
    }
    else
    {
        webkit_web_view_load_uri(web_view, p->in_uri);
    }

    g_main_loop_run (main_loop);



    g_object_unref( G_OBJECT(print_operation));
    g_object_unref( G_OBJECT(print_settings));
    g_object_unref( G_OBJECT(page_setup));

    if (p->default_stylesheet)
    {
        g_object_unref( G_OBJECT(user_content_manager));
        webkit_user_style_sheet_unref( user_stylesheet);
    }

    
    gtk_widget_destroy(GTK_WIDGET(web_view));

    g_object_unref( G_OBJECT(web_view));

    g_object_unref( G_OBJECT(web_context));
    

    g_main_loop_unref (main_loop);

    GMutex * wait_mutex= p->wait_mutex;
    GCond * wait_cond= p->wait_cond;
    int * wait_data= p->wait_data;

    if (wait_mutex && wait_cond && wait_data)
    {
       g_mutex_lock (wait_mutex);
       (*wait_data)++;
       g_cond_signal (wait_cond);
       g_mutex_unlock (wait_mutex);
    }
    return G_SOURCE_REMOVE;
}
void wkgtkprinter_html2pdf(const char* in_uri, const char* html_txt, const char* base_uri, const char* out_uri, const char* key_file_data, const char* default_stylesheet)
{
    struct html2pdf_params *p = (struct html2pdf_params*)g_malloc(sizeof(struct html2pdf_params));
             p->in_uri=in_uri;p->html_txt=html_txt;p->base_uri=base_uri;p->out_uri=out_uri;
            p->key_file_data=key_file_data;p->default_stylesheet=default_stylesheet;
    if ((g_main_context_get_thread_default () == NULL || g_main_depth () == 0) && g_main_context_default () != NULL && !nonthreaded_init) //not in main thread
    {
        GMutex wait_mutex;
        GCond wait_cond;
	int wait_data = 0;
	g_mutex_init (&wait_mutex);
	g_cond_init (&wait_cond);
        p->wait_cond=&wait_cond;
	p->wait_mutex=&wait_mutex;
	p->wait_data=&wait_data;

        g_idle_add ((GSourceFunc)__html2pdf,
            p);
	g_mutex_lock (&wait_mutex);
	while (wait_data == 0)
	    g_cond_wait (&wait_cond, &wait_mutex);
	g_mutex_unlock (&wait_mutex);
	g_mutex_clear (&wait_mutex);
	g_cond_clear (&wait_cond);

    }
    else
    {
	p->wait_mutex = NULL;
	p->wait_cond = NULL;
	p->wait_data = NULL;
        __html2pdf(p);
    }
    g_free(p);
}

