#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <process.h>

#ifdef MRSTART
# define GOSUBDIR "\\"
# define GOEXE "mred.exe"
# define WAITTILLDONE 0
#endif

#ifdef MZSTART
# define GOSUBDIR "\\"
# define GOEXE "mzscheme.exe"
# define WAITTILDONE 1
#endif



#define MAX_ARGS 100

static char *input = "<Command Line: Replace This ************"
                     "******************************************"
  		     "*****************************************>";

static char *protect(char *s)
{
  char *naya;
  int has_space = 0, has_quote = 0, was_slash = 0;

  for (naya = s; *naya; naya++) {
    if (isspace(*naya) || (*naya == '\'')) {
      has_space = 1;
      was_slash = 0;
    } else if (*naya == '"') {
      has_quote += 1 + (2 * was_slash);
      was_slash = 0;
    } else if (*naya == '\\') {
      was_slash++;
    } else
      was_slash = 0;
  }

  if (has_space || has_quote) {
    char *p;
    int wrote_slash = 0;

    naya = malloc(strlen(s) + 3 + 3*has_quote);
    naya[0] = '"';
    for (p = naya + 1; *s; s++) {
      if (*s == '"') {
	while (wrote_slash--)
	  *(p++) = '\\';
	*(p++) = '"'; /* endquote */
	*(p++) = '\\';
	*(p++) = '"'; /* protected */
	*(p++) = '"'; /* start quote again */
	wrote_slash = 0;
      } else if (*s == '\\') {
	*(p++) = '\\';
	wrote_slash++;
      } else {
	*(p++) = *s;
	wrote_slash = 0;
      }
    }
    *(p++) = '"';
    *p = 0;

    return naya;
  }

  return s;
}

static int parse_command_line(int count, char **command, 
			      char *buf, int maxargs)

{
  char *parse, *created, *write;
  int findquote = 0;
    
  parse = created = write = buf;
  while (*parse) {
    while (*parse && isspace(*parse)) parse++;
    while (*parse && (!isspace(*parse) || findquote))	{
      if (*parse== '"') {
	findquote = !findquote;
      } else if (*parse== '\\') {
	char *next;
	for (next = parse; *next == '\\'; next++);
	if (*next == '"') {
	  /* Special handling: */
	  int count = (next - parse), i;
	  for (i = 1; i < count; i += 2)
	    *(write++) = '\\';
	  parse += (count - 1);
	  if (count & 0x1) {
	    *(write++) = '\"';
	    parse++;
	  }
	}	else
	  *(write++) = *parse;
      } else
	*(write++) = *parse;
      parse++;
    }
    if (*parse)
      parse++;
    *(write++) = 0;
	  
    if (*created)	{
      command[count++] = created;
      if (count == maxargs)
	return count;
    }
    created = write;
  }

  return count;
}

static char *make_command_line(int argc, char **argv)
{
  int i, len = 0;
  char *r;

  for (i = 0; i < argc; i++) {
    len += strlen(argv[i]) + 1;
  }
  r = malloc(len);
  len = 0;
  for (i = 0; i < argc; i++) {
    int l = strlen(argv[i]);
    if (len) r[len++] = ' ';
    memcpy(r + len, argv[i], l);
    len += l;
  }
  r[len] = 0;
  return r;
}

#ifdef MZSTART
void WriteStr(HANDLE h, const char *s) {
  DWORD done;
  WriteFile(h, s, strlen(s), &done, NULL);
}
#endif


#ifdef MRSTART
int APIENTRY WinMain(HANDLE hInstance, HANDLE hPrevInstance, 
		     LPSTR m_lpCmdLine, int nCmdShow)
#else
int main(int argc_in, char **argv_in)
#endif
{
  char *p, *s, *rest = "";
  char name[1024], go[1024], plthome[1024];
  char *args[MAX_ARGS + 1];
  int count, i;
  struct _stat st;
  STARTUPINFO si;
  PROCESS_INFORMATION pi;
#ifdef MZSTART
  HANDLE out;

  out = GetStdHandle(STD_OUTPUT_HANDLE);
#endif
 
  GetModuleFileName(NULL, name, 1024);

  p = name;
  s = name + strlen(name) - 1;
  while (*s != '\\')
    --s;
  *s = 0;

  count = 1;
  count = parse_command_line(count, args, input, MAX_ARGS);
  
  /* p should be PLTHOME path */
  strcpy(go, p);
  strcat(go, GOSUBDIR GOEXE);
  if (_stat(go, &st)) {
#ifdef MRSTART
    MessageBox(NULL, "Can't find " GOEXE, "Error", MB_OK);
#else
    WriteStr(out, "Can't find " GOEXE "\n");
#endif
    exit(-1);
  }

  strcpy(plthome, "PLTHOME=");
  strcat(plthome, p);

  args[0] = go;

#ifdef MRSTART
  {
    char *buf;
    
    buf = malloc(strlen(m_lpCmdLine) + 1);
    memcpy(buf, m_lpCmdLine, strlen(m_lpCmdLine) + 1);
    count = parse_command_line(count, args, buf, MAX_ARGS);
  }
#else
  {
    int i;
    for (i = 1; i < argc_in; i++)
      args[count++] = argv_in[i];
  }
#endif
  
  args[count] = NULL;
  
  for (i = 0; i < count; i++) {
    args[i] = protect(args[i]);
    /* MessageBox(NULL, args[i], "Argument", MB_OK); */
  }
  
  _putenv(plthome);
  
  for (i = 0; i < sizeof(si); i++)
    ((char *)&si)[i] = 0;
  si.cb = sizeof(si);
  
  if (!CreateProcess(go,
		     make_command_line(count, args),
		     NULL, NULL, TRUE,
		     0, NULL, NULL, &si, &pi)) {
    
#ifdef MRSTART
    MessageBox(NULL, "Can't start " GOEXE, "Error", MB_OK);
#else
    WriteStr(out, "Can't start " GOEXE "\n");
#endif
    return -1;
  } else {
#if WAITTILDONE
    DWORD result;
    WaitForSingleObject(pi.hProcess, INFINITE);
    GetExitCodeProcess(pi.hProcess, &result);
    return result;
#else
    return 0;
#endif
  }
}
