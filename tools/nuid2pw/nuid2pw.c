
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAX_TEXT_LINE 1000
#define MAX_LINES 1000

void strdncpy (char *s, const char *ct, size_t n)
{
  // copies string with a maximum of size_t chars from a null terminated string.
  // the result is always null terminated

  if (n>0)
  {
    strncpy (s, ct, n-1);
    s[n-1] = '\0';
  }
  else
  {
    s[0] = '\0';
  }
}

int strbegins(const char *str, const char *prefix)
{
  while(*prefix)
  {
    if(*prefix++ != *str++) return 0;
  }

  return 1;
}

int update_passwd_notes (const char *passwd_file, const char *uid_str, const char *homedir)
{
  FILE *fp;
  int  count;
  int  i;
  char line[MAX_LINES+1][MAX_TEXT_LINE+1];
  char num_user[255];

  sprintf (num_user, "%s:", uid_str);

  count = 0;
  fp = fopen (passwd_file, "r");

  if (fp == NULL)
  {
    printf ("error -- canot open [%s] for reading\n", passwd_file);
    return 1;
  }

  while (fgets(line[count], MAX_TEXT_LINE, fp))
  {
    if (count >= MAX_LINES) break;
    count++;
  }
  
  fclose (fp);
  fp = NULL;

  // printf ("entries %d read\n", count);

  fp = fopen (passwd_file, "w");

  if (fp == NULL)
  {
    printf ("error -- canot open [%s] for writing\n", passwd_file);
    return 1;
  }

  for (i=0; i<count; i++)
  {
    if (strbegins (line[i], "notes:"))
    {
      fprintf (fp, "notes:x:%s:0::%s:/bin/bash\n", uid_str, homedir);  
    }
    else if (strbegins (line[i], num_user))
    {
      // skip
    }
    else
    {
      fprintf (fp, "%s", line[i]);  
    } 
  }

  fclose (fp);
  fp = NULL;

  printf("updated notes uid [%s] in [%s] \n", uid_str, passwd_file);
}

int main (int argc, char *argv[])
{
  int  ret;
  char uid_str[255];
   
  ret = 0;
 
  if (argc > 1)
  {
    strdncpy (uid_str, argv[1], 250);
  }
  else
  {
    return ret;
  }
  ret = update_passwd_notes ("/etc/passwd", uid_str, "/local/notesdata");

  return ret;
}


