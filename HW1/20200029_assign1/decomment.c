#include <stdio.h>
/*stdlib is for EXIT_SUCCESS and EXIT_FAILURE*/
#include <stdlib.h>
/*Declare four DFAState: Success for normal, Failure for the comment,
  and Single and Double for single and double quotation mark.*/
enum DFAState {Success, Failure, Single, Double};

/*The role of main function is printing characters on the standard output stream except for comment line,
  and returning correct result. If the file ends before the comment is terminated, program would returns error message
  with the line number at which the error occurs.*/
int main(void)
{
  /*Variable for read characters in the file.
    Read character by function getchar().*/
  int c = getchar();
  /*Variable for marking error line number*/
  int eline_num=0;
  /*Variable for counting # of lines*/
  int line_num=1;
  /*Variable for being assigned the state*/
  enum DFAState state = Success;

  /*Run until c gets EOF*/
  while (c != EOF) {
    switch (state) {
      /*In case of (state==Success), program will print the characters in this state unless
	we met comment symbol('/'+'*') or quotation marks('\''or'\"').
	The 'state' variable would be assigned Failure if variable c gets '/' and '*',
	Single if c gets single quotaion mark, and Double if c gets double quotation mark.*/
    case Success:
      /*Make decision whether it is comment symbol('/'+'*')or not.
       In case of (c == '/'), investigate the next character.*/
      if (c == '/'){
	/*If the next character is '*',*/
	if ((c = getchar()) == '*') {
	  state = Failure;  /*change state into Failure,*/
	  eline_num=line_num; /*mark current line # to the eline_num,*/
	  printf(" "); /*print a space character,*/
	  c = getchar(); /*and gets next character.*/
	}
	/*If it is not,*/
	else printf("%c",'/'); /*print '/' and does not get next character because it already got it
				at the condition expression.*/
      }
      /*If c equals to a single quotation mark,*/
      else if (c == '\'') {
	printf("\'"); /*print a single quotation mark,*/
	state = Single; /*change state into Single,*/
	c = getchar(); /*and gets next character.*/
      }
      /*If c equals to a single quotation mark,*/
      else if (c == '\"') {
	printf("\""); /*print a double quotation mark,*/
	state = Double; /*change state into Double,*/
	c = getchar(); /*and gets next character.*/
	}
      /*Else,*/
      else {
	if (c == '\n') line_num++; /*increase line number if the character equals to the nextline character,*/
	printf("%c",c); /*print the character,*/
	c = getchar(); /*and gets next character.*/
      }
      break;
      /*In case of (state==Single), program print all the characters until c gets a single quotation mark again.
       But in case of a quatation mark located right behind the backslash, print them without changing state.*/
    case Single:
      /*If c equals to backslash, print it with the next character.*/
      if (c == '\\') {
	c = getchar(); /*gets next character,*/
	printf("%c%c",'\\',c); /*print the string "\"",*/
	c = getchar(); /*and gets next character.*/
      }
      /*If c equals to a single quotation mark,*/
      else if (c == '\'') {
	printf("\'"); /*print a single quotation mark,*/
	state = Success; /*change state into Success,*/
	c = getchar(); /*and gets next character.*/
      }
      /*Else,*/
      else {
	if (c == '\n') line_num++; /*increase line number if the character equals to the nextline character,*/
	printf("%c",c); /*print the character,*/
	c = getchar(); /*and gets next character.*/
      }
      break;
      /*In case of (state==Double), program print all the characters until c gets a double quotation mark again.
	But in case of a quatation mark located right behind the backslash, print them without changing state.*/
    case Double:
      /*If c equals to backslash, print it with the next character.*/
      if (c == '\\') {
	c = getchar(); /*gets next character,*/
	printf("%c%c",'\\',c); /*print the string "\"",*/
	c = getchar(); /*and gets next character.*/
      }
      /*If c equals to a double quotation mark,*/
      else if (c == '\"') {
	printf("\""); /*print a double quotation mark,*/
	state = Success; /*change state into Success,*/
	c = getchar(); /*and gets next character.*/
      }
      /*Else,*/
      else {
	if (c == '\n') line_num++; /*increase line number if the character equals to the nextline character,*/
	printf("%c",c); /*print the character,*/
	c = getchar(); /*and gets next character.*/
      }
      break;
      /*In case of the character in the comment(state==Failure), program does not print all the characters
	except for a nextline character until c gets a comment symbol('*'+'/').*/
    case Failure:
      /*If c equals to the nextline character,*/
      if (c == '\n') {
	line_num++; /*increase line number,*/
	printf("%c",c); /*print the character,*/
	c = getchar(); /*and gets next character.*/
      }
      /*Make decision whether it is comment symbol('*'+'/')or not.
       If c equals to '*', investigate the next character.*/
      else if ( c == '*') {
	/*If the next character is '/',*/
	if ((c = getchar()) == '/') {
	  state = Success;  /*change state into Success,*/
	  c = getchar(); /*and gets next character.*/
	}
       	else break; /*If it is not, break without getting next character, because it already got it
		      at the condition expression.*/
      }
      /*Else,*/
      else {
	c = getchar(); /*gets next character.*/
	break;
      }
      break;
    }
  }
  /* If the file ends before the comment is terminated, program would returns error message
  with the line number at which the error occurs.*/
  if (state == Failure) {
    fprintf(stderr,"Error: line %d: unterminated comment\n",eline_num); /*print error message with error line number.*/
    return EXIT_FAILURE; /*returns EIXT_FAILURE*/
  }
  else return EXIT_SUCCESS; /*Else, returns EXIT_SUCCESS*/
}
