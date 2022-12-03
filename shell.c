#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define LIMIT_MAX 80
#define  EXIT "exit"

void style_parallel(void);
void style_sequential(void);
void bash_mode(char *file);


void main(int argc, char *argv[])
{
     int status;

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
     pid_t pid;
     int limit = LIMIT_MAX/2 + 1;
     char line[LIMIT_MAX];
     int run = 1;
     char *args;

     while (run)
     {
          printf("jaa seq> ");
          fflush(stdout);

          fgets(line, limit, stdin);

          args = strtok(line, "\n");
         
          if(!strcmp(args, EXIT))
          {
               exit(EXIT_SUCCESS);
          }
     }
     

}

void bash_mode(char *file)
{
     FILE *file_read;
     int limit = LIMIT_MAX/2 + 1;
     int lines = 0;
     char line[LIMIT_MAX];
     int run = 1;
     char *args, *arg;

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
               if(!strcmp(arg, EXIT))
                    exit(EXIT_SUCCESS);
               else
               {
                    printf("%s\n", args);
               }
          }
     
          if (count_line == lines)
          {
               fprintf(stderr, "enter 'exit' to finsh bash mode: \n");
               args = fgets(line, limit, stdin);
               arg = strtok(args, "\n");
               if(!strcmp(arg, EXIT))
                    exit(EXIT_SUCCESS);
          }

          count_line++;
     }
     
    fclose(file_read);

}

void  style_parallel()
{   
     pid_t pid;
     int limit = LIMIT_MAX/2 + 1;
     char line[LIMIT_MAX];
     int run = 1;
     char *args;

    while(run)
    {
          printf("jaa par> ");
          fflush(stdout);

          fgets(line, limit, stdin);

          args = strtok(line, "\n");
         
          if(!strcmp(args, EXIT))
          {
               exit(EXIT_SUCCESS);
          }
    }

}