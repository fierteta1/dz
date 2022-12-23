#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <regex.h>
#include "exec.h"
#include <sys/types.h>
#include <sys/stat.h>

#define CAT_LINE_LEN 1024
#define GREP_LINE_LEN 1024
/*============================================================================
 * Функция удаления процесса с определенным pid 
 *============================================================================
*/
void del_pid(PList *p, int pid)
  {
    PList q;
    
    while (*p)
      if ((*p)->pid == pid)
        {
          q=(*p);
          *p=q->next;
          free(q);
          return ;
        }
      else
        p=&(*p)->next;
   
    return ;
  }

/*============================================================================
 * Функция удаления завершенных прцессов
 *============================================================================
*/
void clear_zombie(PList *p)
  {
    int pid;
    
    if (*p == NULL) 
      {
        //fprintf(stderr, "список пуст\n");
	      return;
      }
      
    while (*p != NULL)
      {
        if ( (pid = waitpid((*p)->pid, 0, WNOHANG)) == 0)
          ;
          //fprintf(stderr, "%d еще не завершился\n", (*p)->pid);
        else
          {
            fprintf(stderr, "процесс %d завершился\n", pid);
            del_pid(p, pid);
            return ;
          }
        p = &(*p)->next;
      }
  }

/*============================================================================
 * Функция добавления нового узла в список pid
 *============================================================================
*/
void add_pid(PList * p, int elem)
  {
    if (*p == NULL)
      {
        if ( ( *p = malloc(sizeof(struct backgrndList)) ) == NULL)
          {
            fprintf(stderr, "malloc: не выделяется память");
            return;
          }
        (*p)->pid = elem;
        (*p)->next = NULL;
        
        return ;
      }
    else
      add_pid(&(*p)->next, elem);
  }

/*============================================================================
 * Функция печати списка pid
 *============================================================================
*/
void print_pid(PList p)
  {
    if (p == NULL) 
	    return;
    
    while (p != NULL)
      {
        printf("%d\n",p->pid);
        p = p->next;
      }

  }

/*============================================================================
 * Функция удаления списка pid
 *============================================================================
*/
void del_all_pid(PList p)
{
  if (p == NULL)
    return;
  del_all_pid(p->next);
  free(p);
}

/*============================================================================
 * Функция перенаправляет стандартный поток ввода на вывод из файла
 *============================================================================
*/
void change_in(Tree tcmd)
  {
    int f_inp;
    
    if ( (f_inp = open(tcmd->infile, O_RDONLY )) == -1)
      {
        fprintf(stderr, "не удалось открыть файл %s\n", tcmd->infile);
      }
    else 
      {
        dup2(f_inp, 0);
        //fprintf(stderr, "перенаправил из файла %s\n", tcmd->infile);
        close(f_inp);
      }
  }

/*============================================================================
 * Функция перенаправляет стандартный поток вывода на вывод в файл
 *============================================================================
*/
void change_out(Tree tcmd)
  {
    int f_out;
    
    if (tcmd->append)
      f_out = open(tcmd->outfile, O_WRONLY|O_APPEND, 0666);
    else
      f_out = open(tcmd->outfile, O_WRONLY|O_CREAT|O_TRUNC, 0666);
                  
    if (f_out == -1)
      fprintf(stderr, "не удалось открыть файл %s\n", tcmd->outfile);
    else 
      {
        dup2(f_out, 1);
        //fprintf(stderr, "перенаправил в файл %s\n", tcmd->outfile);
        close(f_out);
      }
  }

/*============================================================================
 * Функция обработки команды cd
 *============================================================================
*/
void exec_cd(char ** argv)
  {
    if (argv[1] != NULL && argv[2] != NULL)
      fprintf(stderr, "не совпадает число параметров в команде cd\n");
    else
      {
        if (argv[1] != NULL)
          if (chdir(argv[1]) == -1)
            fprintf(stderr, "не удалось выполнить команду cd\n");
          else ;
        else
          if (chdir("/") == -1)
            fprintf(stderr, "не удалось выполнить команду cd\n");
          else ;
      }
  }
  /*============================================================================
 * Функция обработки команды mv
 *============================================================================
*/
void exec_mv(char ** argv)
  {
if (argv[3] != NULL) {
        fprintf(stderr, "usage: mv source destination\n");
        return;
    }

    const char *src = argv[1];
    const char *dst = argv[2];


    struct stat src_stat, dst_stat;
    if (stat(src, &src_stat) == 0 && stat(dst, &dst_stat) == 0 &&
        src_stat.st_dev == dst_stat.st_dev && src_stat.st_ino == dst_stat.st_ino) {
        fprintf(stderr, "error: source and destination are the same file\n");
        return;
    }

    struct stat st;
    if (stat(dst, &st) == 0 && S_ISDIR(st.st_mode)) {
        char *dst_path = malloc(strlen(dst) + strlen(src) + 2);
        sprintf(dst_path, "%s/%s", dst, src);

        dst = dst_path;
    }

    if (rename(src, dst) != 0) 
        perror("rename");

  }
  /*============================================================================
 * Функция обработки команды cat
 *============================================================================
*/
void exec_cat(char ** argv)
  {
    int opt;
    int line_numbering = 0;
    int argc = 0;
    while(argv[argc++]);
    --argc;


    while ((opt = getopt(argc, argv, "n")) != -1) {
        switch (opt) {
            case 'n':
                line_numbering = 1;
                break;
            default:
                fprintf(stderr, "usage: cat [-n] [file...]\n");
                return;
        }
    }

    if (optind == argc) {
        char line[CAT_LINE_LEN];
        int line_count = 0;
        while (fgets(line, CAT_LINE_LEN, stdin) != NULL) {
            if (line_numbering) {
                printf("%6d  ", ++line_count);
            }
            printf("%s", line);
        }
    } else {

        for (int i = optind; i < argc; i++) {
            const char *filename = argv[i];
            FILE *fp = fopen(filename, "r");
            if (fp == NULL) {
                perror("fopen");
                return;
            }
            char line[CAT_LINE_LEN];
            int line_count = 0;
            while (fgets(line, CAT_LINE_LEN, fp) != NULL) {
                if (line_numbering) {
                    printf("%6d  ", ++line_count);
                }
                printf("%s", line);
            }
            fclose(fp);
        }
    }

  }
  /*============================================================================
 * Функция обработки команды grep
 *============================================================================
*/
void exec_grep(char ** argv)
{
    int argc = 0;
    while(argv[argc++]);
    --argc;
    if (argc < 2) {
        fprintf(stderr, "usage: grep pattern [file...]\n");
        return ;
    }

    regex_t regex; //regular expression
    int rc = regcomp(&regex, argv[1], REG_EXTENDED | REG_NOSUB);
    if (rc != 0) {
        char error_message[GREP_LINE_LEN];
        regerror(rc, &regex, error_message, GREP_LINE_LEN);
        fprintf(stderr, "error: invalid pattern: %s\n", error_message);
        return;
    }

    if (argc == 2) {

        char line[GREP_LINE_LEN];
        while (fgets(line, GREP_LINE_LEN, stdin)) {
            if (regexec(&regex, line, 0, NULL, 0) == 0) {
                printf("%s", line);
            }
        }
    }
        if (argc == 3) {

        char line[GREP_LINE_LEN];
        FILE * f = fopen(argv[2],"r+");
        while (fgets(line, GREP_LINE_LEN, f)) {
            if (regexec(&regex, line, 0, NULL, 0) == 0) {
                printf("%s", line);
            }
        }
    }
}




/*============================================================================
 * Функция, реализующая работу конвейера. В параметрах указывается кол-во 
 * команд конвейера и структура типа Tree.
 *============================================================================
*/
void exec_conv(int cmd_count, Tree tcmd, int IsBack, PList * back_list)
  { 
    int fd[2], in, out, next_in, i, pid, fback;
    Tree term;
    PList wait_list = NULL;
    PList p;
    
    term = tcmd;
    if (cmd_count == 0)
      return ;
    if (cmd_count == 1) 
      {
        if (strcmp(term->argv[0], "cd") == 0)
          {
            exec_cd(term->argv);
            return ;
          } 
          if (strcmp(term->argv[0], "mv") == 0)
          {
            exec_mv(term->argv);
            return ;
          } 
          if (strcmp(term->argv[0], "cat") == 0)
          {
            exec_cat(term->argv);
            return ;
          } 
          if (strcmp(term->argv[0], "grep") == 0)
          {
            exec_grep(term->argv);
            return ;
          } 
        if ( (pid = fork()) == 0)
          {
            if (IsBack)
              signal(SIGINT, SIG_IGN);
              
            if (term->infile != NULL)
              change_in(term);
            if (term->outfile != NULL)
              change_out(term);
            if (IsBack)
              {
                fback = open("/dev/null", O_RDONLY);
                dup2(fback, 0);
                close(fback);
              }
            
            //fprintf(stderr, "первый пошел и все конец\n");
            execvp(term->argv[0],term->argv); 
            fprintf(stderr, "не удалось выполнить команду %s\n", term->argv[0]);
            exit(-1);
            
          }
        
        if (IsBack)
          {
            fprintf(stderr, "процесс %d работает в фоновом режиме\n", pid);
            add_pid(back_list, pid);
          }
        else 
          { 
            //fprintf(stderr, "ждем процесс %d\n", pid);
            waitpid(pid, 0, 0);
            //fprintf(stderr, "дождались (%d)\n", pid);
          }
        return ;
      }
    
    pipe(fd); 
    out=fd[1]; 
    next_in=fd[0];
    
    if (strcmp(term->argv[0], "cd") == 0)
          {
            exec_cd(term->argv);
            return ;
          } 
          if (strcmp(term->argv[0], "mv") == 0)
          {
            exec_mv(term->argv);
            return ;
          } 
          if (strcmp(term->argv[0], "cat") == 0)
          {
            exec_cat(term->argv);
            return ;
          } 
          if (strcmp(term->argv[0], "grep") == 0)
          {
            exec_grep(term->argv);
            return ;
          }
    
        if ( (pid = fork()) == 0)
          {
            if (IsBack)
              signal(SIGINT, SIG_IGN);
           
            close(next_in);
            dup2(out,1);
            close(out);
        
            if (term->infile != NULL)
              change_in(term);
            if (term->outfile != NULL)
              change_out(term);
            if (IsBack)
              {
                fback = open("/dev/null", O_RDONLY);
                dup2(fback, 0);
                close(fback);
              }
        
            //fprintf(stderr, "первый пошел\n");
            execvp(term->argv[0],term->argv); 
            fprintf(stderr, "не удалось выполнить команду %s\n", tcmd->argv[0]);
            exit(-1);
          }
    
        if (IsBack)
          {
            fprintf(stderr, "процесс %d работает в фоновом режиме\n", pid);
            add_pid(back_list, pid);
          }
        else
          {
            add_pid(&wait_list, pid);
          }
      
    
    in=next_in;
    for (i=1;i<cmd_count-1; i++)
      {
        term = term->pipe;
        close(out);
        pipe(fd);
        out=fd[1];
        next_in=fd[0];
        
        if (strcmp(term->argv[0], "cd") == 0)
          {
            continue;
          } 
          if (strcmp(term->argv[0], "mv") == 0)
          {
            continue;
          } 
          if (strcmp(term->argv[0], "cat") == 0)
          {
            continue;
          } 
          if (strcmp(term->argv[0], "grep") == 0)
          {
            continue;
          }
        
        if( (pid = fork()) == 0)
          {
            if (IsBack)
              signal(SIGINT, SIG_IGN);
          
            close(next_in);
            dup2(in,0);
            close(in);
            dup2(out,1);
            close(out);
            
            if (term->infile != NULL)
              change_in(term);
            if (term->outfile != NULL)
              change_out(term);
            
            //fprintf(stderr, "%d пошел\n", i+1);
            execvp(term->argv[0], term->argv); 
            fprintf(stderr, "не удалось выполнить команду %s\n", term->argv[i]);
            exit(-1);
          }
        
        if (IsBack)
          {
            fprintf(stderr, "процесс %d работает в фоновом режиме\n", pid);
            add_pid(back_list, pid);
          }
        else
          {
            add_pid(&wait_list, pid);
          }
        
        close(in);
        in=next_in;
      }
      
    close(out);
    term = term->pipe;
    
    if (strcmp(term->argv[0], "cd") == 0)
      {
        exec_cd(term->argv);
        if (!IsBack)
          {
            //fprintf(stderr, "++++++\n");
            print_pid(wait_list);
            //fprintf(stderr, "++++++\n");
        
            p = wait_list;
            while (p != NULL)
              {
                //printf("ждем %d\n",p->pid);
                waitpid(p->pid, 0, 0);
                //printf("дождались %d\n",p->pid);
                p = p->next;
              }
        
            del_all_pid(wait_list);
          }
          
        return ;
      }
      
    if ( (pid = fork()) == 0)
      {
        if (IsBack)
          signal(SIGINT, SIG_IGN);
      
        dup2(in,0);
        close(in);
        
        if (term->infile != NULL)
          change_in(term);
        if (term->outfile != NULL)
          change_out(term);
        
        //fprintf(stderr, "последний пошел\n");
        execvp(term->argv[0], term->argv); 
        fprintf(stderr, "не удалось выполнить команду %s\n", term->argv[cmd_count-1]);
        exit(-1);
      }
    
    close(in);
    
    if (IsBack)
      {
        fprintf(stderr, "процесс %d работает в фоновом режиме\n", pid);
        add_pid(back_list, pid);
      }
    else
      {
        add_pid(&wait_list, pid);
        
        //fprintf(stderr, "++++++\n");
        print_pid(wait_list);
        //fprintf(stderr, "++++++\n");
        
        p = wait_list;
        while (p != NULL)
          {
            //printf("ждем %d\n",p->pid);
            waitpid(p->pid, 0, 0);
            //printf("дождались %d\n",p->pid);
            p = p->next;
          }
        
        del_all_pid(wait_list);
      }  
    
    return ;
  }

/*============================================================================
 * Функция подсчитывает число команд конвейера
 *============================================================================
*/
int len_conv(Tree p)
  {
    int i;
    if (p == NULL) 
	    return 0;
    for (i=1; p->pipe != NULL; i++)
      p=p->pipe;
    return i;
  }

/*============================================================================
 * Функция подсчитывает число команд последовательности, разделенных ;
 *============================================================================
*/
int len_next(Tree p)
  {
    int i;
    if (p == NULL) 
	    return 0;
    for (i=1; p->next != NULL; i++)
      p=p->next;
    return i;
  }

/*============================================================================
 * Функция обходит структуру по полям next и вызывает функцию exec_conv
 *============================================================================
*/
void exec_com(Tree tcmd, PList * plst)
  {
    Tree term;
    int cnt_next, i;
    
    term = tcmd;
    cnt_next = len_next(term);
    
    //fprintf(stderr, "len_next=%d\n", cnt_next);
    
    for (i=0; i<cnt_next; i++)
      {
        //fprintf(stderr, "len_conv=%d\n", len_conv(term));
        if (term->backgrnd)
          {
            //fprintf(stderr, "backgrd=%d\n", 1);
            exec_conv(len_conv(term), term, 1, plst);
          }
        else
          {
            //fprintf(stderr, "backgrd=%d\n", 0);
            exec_conv(len_conv(term), term, 0, plst);
          }
        term = term->next;
      }
    return ;
  }
