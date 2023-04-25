#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <glib.h>

#include "wkgtkprinter.h"

void print_help()
{
  printf("usage: wkgtkprinter [-i <input_uri>] [-b <base_uri>] [-k <key_file>] [-s <default_css>] -o <out_uri>\n");
  printf("html string should be provided through stdin. (if input_uri was not provided.)\n");
  printf("Pdf result must be written to file, no option for outputing to stdout. (but you may write to a pipe file.)\n");
  printf("Options:\n"
"-i <uri>         [Input URI] Supply input uri for the html source (instead of stdin)\n"
"                 eg. file:///home/user/in.html or https://webkit.org ...\n"
"-o <uri>         [Output URI] Supply output uri for the pdf output (mandatory) \n"
"                 eg. file:///home/user/x.pdf\n"
"-b <uri>         [Base URI] When you supply html string, you must provide\n"
"                 a base uri where all your external resources (images, css, etc) sits in\n"
"                 (similar to the <base> tag in html). You must provide it (and in uri format)\n"
"                 even if your html string do not contain any external reference.\n"
"-k <file>        [Key file] key file contains settings for the pdf printer.\n"
"                 a typical key file should look like:\n"
"\n"
"     ~~~~~~~~~~~~~~~~~~~\\n"
"     [Print Settings]\n"
"     quality=high\n"
"     resolution-x=320\n"
"     resolution-y=320\n"
"     resolution=320\n"
"     output-file-format=pdf       <--- mandatory\n"
"     printer=Print to File        <--- mandatory\n"
"     page-set=all\n"
"\n"
"     [Page Setup]\n"
"     PPDName=A4\n"
"     DisplayName=A4\n"
"     Width=210\n"
"     Height=297\n"
"     MarginTop=6.35\n"
"     MarginBottom=14.224\n"
"     MarginLeft=6.35\n"
"     MarginRight=6.35\n"
"     Orientation=landscape|portrait\n"
"     ~~~~~~~~~~~~~~~~~~~\n"
"-s <file>        [Default stylesheet file] in css format, optional.\n"
"-h               print this help and exits.\n");
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
        if (optopt == 'i' || optopt == 'o' ||optopt == 'k' ||optopt == 'b' || optopt == 's' )
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
                   "Missing [Output URI] argument. Add '-o <output-uri>' to the arguments\n");
    return -1;
  }
  
  if (in_uri == NULL)
  {
    if (base_uri == NULL)
    {
      fprintf (stderr,
                   "Missing [Base URI] argument\n");
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
