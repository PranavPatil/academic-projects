
/***********************************************************************************************/
//
//        Network Programming: Milestone 5
//
//        Compile: sh compile.sh (set path in /srv/www/cgi-bin for Apache2 in openSuse 11.2)
//        Run: On the browser type: localhost//cgi-bin/cscript.cgi
//
/***********************************************************************************************/


#include "phplib.h"

 List Header;

char* trim(char *source)
{
  int i,j=0;
  char* dest = (char*) calloc(strlen(source), sizeof(char));
  memset(dest, 0, sizeof(char)*strlen(source));

  for(i=0;source[i]!='\0';i++)
  {
    if (source[i] != ' ' && source[i] != '\t')
      dest[j++]=source[i];
  }
  dest[j]='\0';
  return dest;
}

void touppercase(char *str) {

  int i = 0;
  while(i <= strlen(str)) {
      str[i] = toupper(str[i]);
      i++;
  }
}

void tolowercase(char *str) {

  int i = 0;
  while(i <= strlen(str)) {
      str[i] = tolower(str[i]);
      i++;
  }
}

char* substring(char* str, size_t begin, size_t len)
{
  if (str == 0 || strlen(str) == 0 || strlen(str) < begin || strlen(str) < (begin+len))
    return 0;

  return strndup(str + begin, len);
}

void header(char* mstring, bool replace, int http_response_code)  // true, 0
{
  char* eqpos = NULL;
  char* id = NULL;
  char* value = NULL;

  if(mstring == NULL)
    return;
  else
    mstring = strdup(mstring);

  if ((eqpos=strchr(mstring, ':')) != NULL) {
    *eqpos= '\0' ;
    value = strdup(eqpos+1) ;
  } else {
    value = strdup("") ;
  }
  id = strdup(mstring) ;

  Data* data = NULL;
  data = getData(&Header, id);

  if(data == NULL) {
    addData(&Header, id, value);
  }
  else {
    if (replace == TRUE)
      updateData(&Header, id, value);
    else
      return;
  }
}

bool setcookie (char* name, char* value, int expire, char* path, char* domain, bool secure, bool httponly)
//defaults:  0, "/", "localhost", false, false
{
  bool ret = false;
  addData(&Cookie, name, value);

  int namelen = 0;
  int vallen = 0;

  if(name != NULL)
    namelen = strlen(name);
  if(value != NULL)
    vallen = strlen(value);

  int input_size= 0;
  char* cginput = NULL;
  cginput = getenv("HTTP_COOKIE");

  if (cginput != NULL) {
    input_size = strlen(cginput);
  }

  char* headerstr = NULL;

  if(namelen > 0 && vallen > 0)
    headerstr = (char*) calloc((input_size + namelen + vallen + 100), sizeof(char));

  if(headerstr != NULL) {
    name = replacechar(name, ' ', '+');
    value = replacechar(value, ' ', '+');

    if(cginput != NULL) {
      strcpy(headerstr, cginput);
      strcat(headerstr, "; ");
      strcat(headerstr, name);
    }
    else {
      strcpy(headerstr, name);
    }

    strcat(headerstr, "=");
    strcat(headerstr, value);
    setenv("HTTP_COOKIE", headerstr, 1);
    ret = true;
  }

  return ret;
}

void print_r(int bufid) {

  if(bufid == SERVER) {
    int i = 0;
    printf("ServerArray (");

    for (i=0;i<SERVAR_NO;i++) {
        printf(" [%s] => %s ", envars[i], Server[i]);
    }

    printf(") <br><br>");
  }
  else if (bufid == GET) {
    printf("GetArray ( ");
    showList(&Get);
    printf(") <br><br>");
  }
  else if (bufid == POST) {
    printf("PostArray ( ");
    showList(&Post);
    printf(") <br><br>");
  }
  else if (bufid == COOKIE) {
    printf("CookieArray ( ");
    showList(&Cookie);
    printf(") <br><br>");
  }
}
