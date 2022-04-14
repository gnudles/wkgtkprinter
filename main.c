#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <glib.h>

#include "wkgtkprinter.h"

void print_help()
{
  printf("usage: wkgtkprinter\n");
  printf("html string should be provided through stdin\n");
}
int read_file(char **bufptr, FILE *stream)
{
#define CHUNK_SIZE 4096
  *bufptr = malloc(CHUNK_SIZE+1);
  ssize_t n;
  size_t buflen = 0;
  size_t bufoff = 0;
  while ((n = fread(*bufptr+buflen,1,CHUNK_SIZE,stdin)))
  {
    buflen += n;
    if (n == CHUNK_SIZE)
    {
      *bufptr=realloc(*bufptr,buflen+CHUNK_SIZE+1);
    }
    else
      break;
  }
  (*bufptr)[buflen] = '\0';
  if (ferror(stdin))
  {
    return -1;
  }
  *bufptr=realloc(*bufptr,buflen+1);
  return buflen;
}
int main(int argc, char ** argv)
{
  const char *in_uri = NULL;
  const char *base_uri = NULL; // base path for html resources
  const char *out_uri = NULL;
  const char *key_file = NULL;
  const char *stylesheet_file = NULL;
  char *key_string = NULL;
  char *stylesheet_string = NULL;
  char *html_txt = NULL;
  
  int index;
  int c;
  while ((c = getopt (argc, argv, "i:o:k:b:s:h")) != -1)
  {
    switch (c)
      {
      case 'i':
        in_uri = optarg;
        break;
      case 'b':
        base_uri = optarg;
        break;
      case 'o':
        out_uri = optarg;
        break;
      case 'k':
        key_file = optarg;
        break;
      case 's':
        stylesheet_file = optarg;
        break;
      case 'h':
        print_help();
        return 0;
      case '?':
        if (optopt == 'i' || optopt == 'o' ||optopt == 'k' ||optopt == 'b' )
          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
        else if (isprint (optopt))
          fprintf (stderr, "Unknown option `-%c'.\n", optopt);
        else
          fprintf (stderr,
                   "Unknown option character `\\x%x'.\n",
                   optopt);
        return 1;
      default:
        return -1;
      }
  }
  if (key_file != NULL)
  {

    if (!g_file_get_contents (
    key_file,
    &key_string,
    NULL,
    NULL))
    {
      fprintf (stderr,
                   "Could not read key file successfully (%s)\n", key_file);
      return -1;
    }
  }
  if (stylesheet_file != NULL)
  {
    if (!g_file_get_contents (
    stylesheet_file,
    &stylesheet_string,
    NULL,
    NULL))
    {
      fprintf (stderr,
                   "Could not read stylesheet file successfully\n");
      return -1;
    }
  }

  if (out_uri == NULL)
  {
    fprintf (stderr,
                   "Missing out_uri argument\n");
    return -1;
  }
  
  if (in_uri == NULL)
  {
    if (base_uri == NULL)
    {
      fprintf (stderr,
                   "Missing base_uri argument\n");
      return -1;
    }
    size_t len = 0;
    ssize_t bytes_read = read_file( &html_txt, stdin);
    if ( bytes_read < 0) {
      fprintf (stderr,
                   "Could not read html content from stdin\n");
      return -1;
    }
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
    free(html_txt);
  }
  return 0;
}
