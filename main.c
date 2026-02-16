#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <glib.h>
#include <gio/gio.h>

#include "wkgtkprinter.h"

static gchar *in_uri = NULL;
static gchar *out_uri = NULL;
static gchar *base_uri = NULL;
static gchar *key_file = NULL;
static gchar *stylesheet_file = NULL;

static GOptionEntry entries[] =
{
  { "input", 'i', 0, G_OPTION_ARG_STRING, &in_uri, "Input URI (instead of stdin) eg. file:///home/user/in.html or https://webkit.org", "URI" },
  { "output", 'o', 0, G_OPTION_ARG_STRING, &out_uri, "Output URI (mandatory) eg. file:///home/user/x.pdf", "URI" },
  { "base", 'b', 0, G_OPTION_ARG_STRING, &base_uri, "Base URI where external resources reside (required if input is stdin)", "URI" },
  { "key-file", 'k', 0, G_OPTION_ARG_STRING, &key_file, "Key file containing print settings", "FILE" },
  { "stylesheet", 's', 0, G_OPTION_ARG_STRING, &stylesheet_file, "Default stylesheet file (css)", "FILE" },
  { NULL }
};

int main(int argc, char ** argv)
{
  GError *error = NULL;
  GOptionContext *context;
  char *key_string = NULL;
  char *stylesheet_string = NULL;
  char *html_txt = NULL;

  context = g_option_context_new ("- Convert HTML to PDF using WebkitGtk");
  g_option_context_add_main_entries (context, entries, NULL);
  
  if (!g_option_context_parse (context, &argc, &argv, &error))
  {
    g_printerr ("Option parsing failed: %s\n", error->message);
    g_error_free (error);
    return 1;
  }

  if (out_uri == NULL)
  {
    g_printerr ("Missing [Output URI] argument. Add '-o <output-uri>' to the arguments\n");
    g_printerr ("%s", g_option_context_get_help (context, TRUE, NULL));
    return 1;
  }

  if (key_file != NULL)
  {
    if (!g_file_get_contents (key_file, &key_string, NULL, &error))
    {
      g_printerr ("Could not read key file successfully (%s): %s\n", key_file, error->message);
      g_error_free (error);
      return 1;
    }
  }

  if (stylesheet_file != NULL)
  {
    if (!g_file_get_contents (stylesheet_file, &stylesheet_string, NULL, &error))
    {
      g_printerr ("Could not read stylesheet file successfully (%s): %s\n", stylesheet_file, error->message);
      g_error_free (error);
      if (key_string) g_free (key_string);
      return 1;
    }
  }
  
  if (in_uri == NULL)
  {
    if (base_uri == NULL)
    {
      g_printerr ("Missing [Base URI] argument. When you supply html string through stdin, you must provide a base uri.\n");
      if (key_string) g_free (key_string);
      if (stylesheet_string) g_free (stylesheet_string);
      return 1;
    }

    GIOChannel *io = g_io_channel_unix_new(STDIN_FILENO);
    g_io_channel_set_encoding(io, NULL, NULL); // Binary read
    gsize length = 0;
    if (g_io_channel_read_to_end(io, &html_txt, &length, &error) != G_IO_STATUS_NORMAL)
    {
        g_printerr ("Could not read html content from stdin: %s\n", error ? error->message : "Unknown error");
        if (error) g_error_free(error);
        g_io_channel_unref(io);
        if (key_string) g_free (key_string);
        if (stylesheet_string) g_free (stylesheet_string);
        return 1;
    }
    g_io_channel_unref(io);

    // Ensure null termination
    html_txt = g_realloc(html_txt, length + 1);
    html_txt[length] = '\0';
  }

  wkgtkprinter_gtk_init();
  wkgtkprinter_html2pdf(in_uri, html_txt, base_uri, out_uri, key_string, stylesheet_string);

  if (key_string != NULL)
  {
    g_free(key_string);
  }
  if (stylesheet_string != NULL)
  {
    g_free(stylesheet_string);
  }
  if (html_txt != NULL)
  {
    g_free(html_txt);
  }

  // Free option context allocated strings
  g_free(in_uri);
  g_free(out_uri);
  g_free(base_uri);
  g_free(key_file);
  g_free(stylesheet_file);
  g_option_context_free(context);

  return 0;
}
