// gcc -lreadline dumbsh.c -o dumbsh

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <readline/history.h>
#include <readline/readline.h>

void process_command( char *cmd );
char * rl_gets( char *prompt );
void make_completions(char *str);
char * get_completions(char *text, int state);
char* token(char *str, int index);
int count_tokens(char *str);
int tokcmp(char *s1, int index, char *s2);
char* dupstr(char *s);

char **completions = NULL;

int main( int argc, char *argv[] )
{
  char *line, *prompt = argv[1];
  int i;

  while ( line = rl_gets(prompt) ) {
    if( *line ) {
	printf( ":%s\n", line );
    }
  }
  
  if( completions ) {
    free( completions[0] ); // completions_buf
    free( completions );
  }
  return 0;
}

void process_command( char *cmd ) 
{
  
  switch( *token(cmd,0) )
    {
    case 'c':
      rl_bind_key('\t', rl_complete);
      rl_completion_entry_function = (Function*)get_completions;
      make_completions(readline(""));
      break;

    case 't':
      rl_bind_key('\t', rl_insert);
      break;

    default:
    }
}

void make_completions(char *str)
{
  static char *completions_buf = NULL;
  char *p;
  int length, i;

  if( completions ) {
    free( completions_buf );
    completions_buf = NULL;
    free( completions );
    completions = NULL;
  }
  completions_buf = str;

  for( p=str; *p; p++ ) {
    if( !isprint(*p) ) 
      *p = ' ';
  }
  length = count_tokens(str);
  if( length == 0 )
    return;

  completions = (char**)calloc(length+1,sizeof(char*));  
  i=0;
  while( *str ) {
    for(; *str && isspace(*str); str++);
    completions[i] = str;
    for(; *str && !isspace(*str); str++);
    *str = '\0';
    str++;
    i++;
  }
  completions[length] = NULL;

  /*
  printf("length = %d\n",length);
  for( i=0; completions[i]; i++ ) {
    printf("%d: \"%s\"\n", i, completions[i]);
  }
  */
}
    
char * get_completions(char *text, int state)
{
  static int index, len;
  char *res;
 
  if( !completions )
    return NULL;
  
  if( state == 0 ) {
    index = 0;
    len = strlen(text);
  }
  while( (res = completions[index]) ) {
    index++;
    if( strncmp(res,text,len) == 0 )
      return dupstr(res);
  }

  return NULL;
}
  
char* dupstr(char *s)
{
  char *r;
  r = malloc(strlen(s) + 1);
  strcpy (r, s);
  return (r);
}

char* token(char *str, int index)
{
  int i;

  for( ; *str && isspace(*str); str++ );
  for( i=0; i<index; i++ ) {
    for( ; *str && !isspace(*str); str++ );
    for( ; *str && isspace(*str); str++ );
  }
  return str;
}

int count_tokens(char *str)
{
  int i = 0;

  while( *str ) {
    for( ; *str && isspace(*str); str++ );
    for( ; *str && !isspace(*str); str++ );
    i++;
  }
  return i;
}

int tokcmp(char *s1, int index, char *s2)
{
  s1 = token(s1,index);
  while( *s1 && *s2 && *s1 != ' ' && *s2 != ' ' && *s1 == *s2 ) {
    s1++;
    s2++;
  }
  if( ( isspace(*s1) || *s1 == '\0') &&
      ( isspace(*s2) || *s2 == '\0') )
    return 0;

  return (int)(*s1 - *s2);
}

// Read a string, and return a pointer to it.  Returns NULL on EOF
char * rl_gets( char *prompt )
{
  static char *line_read = (char *)NULL;

  if( line_read ) {
    free( line_read );
    line_read = NULL;
  }
  
  line_read = readline(prompt);

  // If the line has any text in it, save it on the history.
  if (line_read && *line_read) {
    if( tokcmp( line_read, 0, "--" ) == 0 ) {
      process_command( token( line_read, 1 ) );
      *line_read = '\0';
    }
    else
      add_history(line_read);
  }
  return line_read;
}





