#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

#define LIMIT_MAX 80
#define SEQ "style sequential"
#define PAR "style parallel"
#define EXIT "exit"
#define HISTORY "!!"

void style_parallel(void);
void style_sequential(void);
void bash_mode(char *file);
void input(char *line);
int handling_input (char **commandv, char *line);
void current_command(int index,  char **argumentv, char **commandv);
int is_in (char *line, char character);

pid_t pid;
int status;
char line[LIMIT_MAX];
int run = 1;
char *args, *arg;
char *commandv[LIMIT_MAX/2 + 1];
int commandc;
char *argumentv[LIMIT_MAX/2 + 1];
int limit = LIMIT_MAX/2 + 1;
char *history = '\0';


void main(int argc, char *argv[])
{
     if (argc < 2)
          style_sequential();

     else if(argc == 2)
          bash_mode(argv[1]);

     else
     {
          fprintf(stderr, "too many argument");
          exit(EXIT_FAILURE);
     }
}

void style_sequential()
{
     while (run)
     {
          printf("jaa seq> ");
          fflush(stdout);

          input(line);
          
          if (!strcmp(line, EXIT))
               exit(EXIT_SUCCESS);

          if (!strcmp(line, PAR))
               style_parallel();

          else if (!strcmp(line, SEQ))
               style_sequential();

          else
          {
               commandc = handling_input (commandv, line);

               for (size_t i = 0; i < commandc; i++)
               {
                    pid = fork();

                    if (pid < 0)
                    {
                         fprintf(stderr, "Fork Failed\n");
                         exit(EXIT_FAILURE);
                    }
                    else if (!pid)
                    {
                         current_command(i, argumentv, commandv);

                         if (execvp(argumentv[0], argumentv) < 0)
                         {             
                              fprintf(stderr,"command '%s' invalid\n", argumentv[0]);
                              exit(EXIT_FAILURE);
                         }
                    }
                    else
                         wait(&status);
               }
          }
     }
}    

void  style_parallel()
{
     while(run)
     {
          printf("jaa par> ");
          fflush(stdout);

                   input(line);
          
          if (!strcmp(line, EXIT))
               exit(EXIT_SUCCESS);

          else if (!strcmp(line, SEQ))
               style_sequential();

          else if (!strcmp(line, PAR))
               style_parallel();
          else
          {
               commandc = handling_input (commandv, line);

               for (size_t i = 0; i < commandc; i++)
               {
                    pid = fork();

                    if (pid < 0)
                    {
                         fprintf(stderr, "Fork Failed\n");
                         exit(EXIT_FAILURE);
                    }
                    else if (!pid)
                    {
                         current_command(i, argumentv, commandv);

                         if (execvp(argumentv[0], argumentv) < 0)
                         {                    
                              fprintf(stderr,"command '%s' invalid\n", argumentv[0]);
                              exit(EXIT_FAILURE);
                         }

                    }
               }

               for (size_t i = 0; i < commandc; i++)
                    wait(&status);   
          }
     }    
}

void bash_mode(char *file)
{
     FILE *file_read;
     int limit = LIMIT_MAX/2 + 1;
     int lines = 0;

     file_read = fopen(file, "r");

     if (file_read == NULL)
     {
          fprintf(stderr, "file cannot be opened\n");
          exit(EXIT_FAILURE);
     }
     
     //check number of lines
     while (!feof(file_read))
     {
          fgets(line, limit, file_read);
          lines++;

     }

     fseek(file_read, 0, SEEK_SET);

     int count_line = 1;

     while (run)
     {
          args = fgets(line, limit, file_read);
          
          if (args == NULL)
          {
               fprintf(stderr, "no argument input, bash mode finished\n");
               exit(EXIT_SUCCESS);
          }
          else
          {
               arg = strtok(args, "\n");

               if (!strcmp(arg, EXIT))
               {
                    fprintf(stderr, "exit\n");
                    exit(EXIT_SUCCESS);
               }
               else
               {
                    printf("%s\n", args);
               }
          }

          if (count_line == lines && strcmp(arg, EXIT))
          {
               while (run)
               {
                    fprintf(stderr, "enter 'exit' to finsh bash mode: \n");

                    args = fgets(line, limit, stdin);
                    arg = strtok(args, "\n");

                    if (!strcmp(arg, EXIT))
                    {
                         fprintf(stderr, "exit\n");
                         exit(EXIT_SUCCESS);
                    }
               }
          }
          if (count_line < lines)
               count_line++;
     }
     
    fclose(file_read);
}

void input (char *line)
{
     fflush(stdin);

     if (fgets(line, LIMIT_MAX, stdin) == NULL)
     {
          fprintf(stdout, "exit\n");
          exit(EXIT_SUCCESS);
     }

     for (size_t i = 0; i < strlen(line); i++)
          if (line[i] == '\t' || line[i] == '\r' || line[i] == '\a') 
               line[i] = '\0';
       
     line[strlen(line) - 1] = '\0';
}

int handling_input(char *commandv[LIMIT_MAX/2 + 1], char *line)
{
     int argc = 0;
     int count = 0;
    
     char *args;

     strcpy(args,line);

     //splitting string by character ';'
     while (count < limit)
     {
          if (!count)
               commandv[count] = strtok(args, ";");
          else
               commandv[count] = strtok(NULL, ";");
          if(commandv[count] == NULL)
               break;

        count++;
     }
     return count;
}

void current_command(int index,  char **argumentv, char **commandv)
{
     int argc = 0;

     while (argc < limit)
     {
          if (!argc)
              argumentv[argc] = strtok(commandv[index], " ");
          else
              argumentv[argc] = strtok(NULL, " ");
          if (argumentv[argc] == NULL)
              break;
         argc++;
     }
     argc = 0;

     commandv[index] = NULL;
}

int is_in (char *line, char character)
{
     int index = 0;

     while (index < strlen(line))
     {
          if (line[index] == character)
               return index;
          else
               index++;
     }
     
     return -1;
}