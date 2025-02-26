#define _GNU_SOURCE
/* Set HOME directory macro */
#define HOME getenv("HOME")
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <assert.h>
#include <signal.h>
#include <dirent.h>

/* counting SIGQUIT */
static char count=0;

/* store latest pid, if there's no background process
   the latestpid == -1 */
static int latestpid = -1;

/* Handling SIGQUIT */
static void quitHandler(int iSig) {
  /* First call */
  if(count==0){
    count++;
    printf("Type Ctrl-\\ again within 5 seconds to exit.\n");
    alarm(5);
  }
  /* Second call within 5 sec */
  else {
    /* cancle alarm and exit */
    alarm(0);
    exit(0);
  }
}

/* Handling SIGALRM */
static void alarmHandler(int iSig) {
  /* initialize count and return */
  count=0;
  return;
}

static void childHandler(int iSig) {
  int pid;

  while((pid = waitpid(-1,NULL,0)) > 0)
    printf("child %d terminated normally\n",pid);

  /* set latest pid = -1*/
  latestpid = -1;
}

int main() {
  /* index, process id, success function */
  int i, pid, success;
  /* 1 for background, 0 for none */
  short background = 0;
  /* store token */
  char *token;
  /* store command */
  char command[1024];
  /* store token arguments */
  char *arguments[20];
  /* signal handling */
  void (*pfRet)(int);
  /* signal unblock */
  sigset_t iSet, qSet, aSet, cSet;

  /* Unblock signals */
  sigemptyset(&iSet);
  sigemptyset(&qSet);
  sigemptyset(&aSet);
  sigemptyset(&cSet);
  sigaddset(&iSet, SIGINT);
  sigaddset(&qSet, SIGQUIT);
  sigaddset(&aSet, SIGALRM);
  sigaddset(&cSet, SIGCHLD);
  sigprocmask(SIG_UNBLOCK, &iSet, NULL);
  sigprocmask(SIG_UNBLOCK, &qSet, NULL);
  sigprocmask(SIG_UNBLOCK, &aSet, NULL);
  sigprocmask(SIG_UNBLOCK, &cSet, NULL);

  /* Ignore SIGINT in parent process */
  pfRet = signal(SIGINT, SIG_IGN);
  /* If error, assert */
  assert(pfRet != SIG_ERR);

  /* Handle SIGQUIT */
  pfRet = signal(SIGQUIT, quitHandler);
  /* If error, assert */
  assert(pfRet != SIG_ERR);

  /* Handle SIGALRM */
  pfRet = signal(SIGALRM, alarmHandler);
  /* If error, assert */
  assert(pfRet != SIG_ERR);

  /* set SIGCHLD to SIG_DFL */
  pfRet = signal(SIGCHLD, SIG_DFL);
  /* If error, assert */
  assert(pfRet != SIG_ERR);

  while(1) {
    
    /* Initialize arguments */
    for(i=0; i<20; i++)
      arguments[i]=NULL;
    
    /* Print "% " prompt */
    printf("%% ");

    fgets(command, sizeof(command), stdin);
   
    /* If the length > 1023, treat as an error */
    if( command[1023] != '\0') {
      printf(": File name too long\n");
      continue;
    }
    /* Set last element of command as '\0' */
    command[strlen(command)-1] = 0;

    token = strtok(command, " ");
    if (token == NULL) continue;

    arguments[0] = token;
    for(i = 1; i < 10; i++) {
      token = strtok(NULL, " ");
      if (token == NULL) break;
      arguments[i] = token;
    }
    arguments[i] = NULL;

    /* exit for "exit" command */
    if(strcmp(arguments[0],"exit") == 0)
      exit(0);

    /* call setenv for "setenv" command */
    else if(strcmp(arguments[0],"setenv") == 0) {
      /* if no var, print error message and continue */
      if(arguments[1]==NULL) {
	printf("./ish: setenv takes one or two parameters\n");
	continue;
      }
      /* more than two parameters, print error message and continue */
      if(arguments[3]!=NULL) {
	printf("./ish: setenv takes one or two parameters\n");
	continue;
      }
      /* call setenv */
      if(arguments[2]==NULL)
	success = setenv(arguments[1], "", 1);
      else
	success = setenv(arguments[1], arguments[2], 1);
      /* if failed, assert */
      if(success == -1)
	assert(0);
      continue;
    }

    /* call unsetenv for "unsetenv" command */
    else if(strcmp(arguments[0],"unsetenv") == 0) {
      /* if no var, print error message and continue */
      if(arguments[1]==NULL) {
	printf("./ish: unsetenv takes one parameter\n");
	continue;
      }
      /* more than one parameters, print error message and continue */
      if(arguments[2]!=NULL) {
	printf("./ish: unsetenv takes one parameter\n");
	continue;
      }
      /* call unsetenv */
      success = unsetenv(arguments[1]);
      /* if failed, assert */
      if(success == -1)
	assert(0);
      continue;
    }

    /* call chdir for "cd" command */
    else if(strcmp(arguments[0],"cd") == 0) {
      /* more than one parameters, print error message and continue */
      if(arguments[2] != NULL){
	printf("./ish: cd takes one parameter\n");
	continue;
      }
      /* call chdir */
      if(arguments[1] == NULL)
	success=chdir(HOME);
      else
	success=chdir(arguments[1]);
       /* if failed, print error message */
      if(success == -1)
	printf("./ish: No such file or directory\n");
      continue;
    }

    /* implement fg */
    /* ce*/
    else if(strcmp(arguments[0],"fg") == 0) {
      /* more than one parameters, print error message and continue */
      if(arguments[2] != NULL){
	printf("./ish: fg takes one parameter\n");
	continue;
      }
      /* if there's no background process, send error message */
      if(latestpid == -1){
	printf("./ish: There is no background process\n");
      }
      /* stop current process and continue latest process */
      else {
	kill(getpid(),SIGSTOP);
	kill(latestpid, SIGCONT);
	printf("[%d] Latest background process is executing\n",latestpid);
      }
      continue;
    }

    /* Clear all I/O buffers */
    fflush(NULL);

    /* implement background process */
    /* find if & is exist. if there's &, store 1 to background,
      and change & elements to "\0". */
    for(i=0; i<20; i++) {
      /* if no &, set background to 0 */
      if(arguments[i]==NULL) {
	background = 0;
	break;
      }
      /* if has background set arguments to NULL */
      else if(strcmp(arguments[i],"&")==0){
	arguments[i] = NULL;
	background = 1;
	break;
      }
    }
    
    /* Create child process */
    pid = fork();

    /* Parent process case 1: no background command, & */
    if ((pid != 0) && (background == 0)){
      wait(NULL);
    }
    /* Parent process case 2: background process command */
    else if((pid != 0) && (background == 1)){
      /* store latest pid*/
      latestpid = pid;
      /* Handle SIGCHLD */
      pfRet = signal(SIGCHLD, childHandler);
      /* If error, assert */
      assert(pfRet != SIG_ERR);
      continue;
    }
    else {
      /* Child process does not ignore SIGINT */
      pfRet = signal(SIGINT, SIG_DFL);
      /* If error, assert */
      assert(pfRet != SIG_ERR);
      
      execvp(arguments[0], arguments);
      /* When execvp failed, print error message
        and exit child process */
      printf("%s: No such file or directory\n",arguments[0]);
      exit(1);
    }
  }
  return 0;
}
