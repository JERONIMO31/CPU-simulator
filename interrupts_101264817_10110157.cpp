#include <stdio.h>
#include <string.h>
#include <stdlib.h>
FILE *output;

int Syscall(char adress[], int time, int totaltime ){
    char *as;
    int run = 0.583*time;
    int data = 0.264*time;
    int error = 0.126*time;
    printf("%d, 1, switch to kernel mode\n", totaltime);
    totaltime += 1;
    printf("%d, 3, context saved\n", totaltime);
    totaltime += 3;
    printf("%d, 1, find vector 2 in memory position %s\n", totaltime, adress);
    totaltime += 1;
    printf("%d, 1, load adress %s into the PC\n", totaltime , adress);
    totaltime += 1;
    printf("%d, %d, SYSCALL: run the ISR\n", totaltime,run );
    totaltime += run;
    printf("%d, %d, transfer the data\n", totaltime, data );
    totaltime += data;
    printf("%d, %d, check for errors\n", totaltime, error );
    totaltime += error;
    printf("%d, 1, , IRET\n", totaltime);
    totaltime += 1;
    return totaltime;
}

int CPU ( char tim[] , int totaltime){
    char *as;
    int time = strtol(tim,&as,10);
    printf("%d, %d,  CPU execution\n", totaltime, time );
    return totaltime + time;
}

int End_IO(char tim[] , char adress[], int totaltime){
    char *as;
    int time = strtol(tim,&as,10);
    printf("%d, 1, check priority of interrupt\n", totaltime);
    totaltime += 1;
    printf("%d, 1, check if masked\n", totaltime);
    totaltime += 1;
    printf("%d, 1,  check if masked\n", totaltime);
    totaltime += 1;
    printf("%d, 3 , context saved \n", totaltime );
    totaltime += 3;
    printf("%d, 1 , find vector 5 in memory position %s\n", totaltime, adress);
    totaltime += 1;
    printf("%d, %d , END_IO\n", totaltime,time);
    totaltime += time;
    printf("%d, 1 , IRET\n", totaltime);
    totaltime += 1;
    return totaltime;
}
int parseexternal( char prog[]){
    char *a= NULL;
    int x = 0;
    int ti = strtol(prog,&a,10);
    char*b= NULL;
    int t;
    FILE *trace;
    char* as = NULL;
    char sent[100];
    char word3[10];
    trace = fopen("external_files.txt", "r");
    while (fgets(sent,100,trace )){
        char *token = strtok(sent," ");
        for (int i = 7; i <= strlen(token)-1; i++){
                word3[i-7] = token[i];
            }
        t = strtol(word3,&b,10);
        if(t == ti){
            token = strtok(0, " ");
            x = strtol(token,&as,10);
            return x;
        }
    }
    return 0;
}
int Fork(char arg[],int totaltime){
    char *a;
    int ti = strtol(arg,&a,10);
    int fork = ti*0.8;
    int scheduler = ti*0.2;
    fprintf(output,"%d, 1, switch to kernel mode\n", totaltime);
    totaltime += 1;
    fprintf(output,"%d, 3, context saved\n", totaltime);
    totaltime += 3;
    fprintf(output,"%d, 1, find vector 2 in memory position 0x0004\n", totaltime);
    totaltime += 1;
    fprintf(output,"%d, 1, load address 0X0695 into the PC\n",totaltime);
    totaltime += 1;
    fprintf(output,"%d, %d, FORK: copy parent PCB to child PCB\n",totaltime,fork);
    totaltime += fork;
    fprintf(output,"%d, %d, scheduler called\n",totaltime, scheduler);
    totaltime += scheduler;
    fprintf(output,"%d, 1, IRET\n", totaltime);
    totaltime += 1;
    return totaltime;
}
int Exec(char file[] , int arg, int mbsize,int totaltime,int *partloc){
    int ex = arg*0.4;
    int part = arg*0.3;
    int partition = arg*0.1;
    int PCB = arg*0.06;
    int sched = arg*0.16;
    fprintf(output,"%d, 1, switch to kernel mode\n", totaltime);
    totaltime += 1;
    fprintf(output,"%d, 3, context saved\n", totaltime);
    totaltime += 3;
    fprintf(output,"%d, 1, find vector 3 in memory position 0x0006\n", totaltime);
    totaltime += 1;
    fprintf(output,"%d, 1, load address 0X042B into the PC\n",totaltime);
    totaltime += 1;
    fprintf(output,"%d, %d, EXEC: load program1 of size %dMb\n",totaltime, ex,mbsize);
    totaltime += ex;
    fprintf(output,"%d, %d, partition %d marked as occupied\n",totaltime, partition,partloc);
    totaltime += partition;
    *partloc -= 1;
    fprintf(output,"%d, %d, updating PCB with new information\n",totaltime, PCB);
    totaltime += PCB;
    fprintf(output,"%d, %d, scheduler called\n",totaltime, sched);
    totaltime += sched;
    fprintf(output,"%d, 1, IRET\n", totaltime);
    return totaltime + 1;
}
int Execution (char filename[],int *partitionloc,char vector[26][7]){
    FILE *traceFile;
    traceFile = fopen(filename, "r");
    if (traceFile == NULL){
      perror("Error opening input file");
      return 1;  
    }
    int totaltime = 0;
  char line[100];
  const char *cpu = "CPU,";
  const char *io = "End_IO";
  const char *syscall = "SYSCALL";
  const char *fork = "FORK,";
  const char *exec = "EXEC";
  char *wo;
    char *wor;
    char word3[9];
    char word2[14];
    int position;
  printf("Reading %s\n", traceFile);
  while (fgets(line, 100, traceFile)) {
    wo = NULL;
    position = 0;
    
    char *token = strtok(line," ");
        if (strcmp( token , cpu) == 0){
            token = strtok(NULL, " ");
            totaltime = CPU(token,totaltime);
            token = strtok(NULL, " ");
        }
        else if (strcmp(token, syscall) == 0){
            token = strtok(0, " ");
            wor = token;
            token = strtok(0, " ");
            position = strtol(token,&wo,10);
            totaltime = Syscall(wor, position, totaltime);
        }
        else if (strcmp(token,fork) == 0){
            token = strtok(NULL, " ");
            strcpy(word2,token);
            totaltime = Fork(word2, totaltime);  // add the time taken by the fork operation to the total time
            token = strtok(NULL, " ");
        }
        else if (strcmp(token, exec) == 0){
            token = strtok(0, " ");
            token[strlen(token)-1]= '\0';
            strcpy(word2,token);
            for (int i = 7; i <= strlen(word2)-1; i++){
                word3[i-7] = word2[7];
            }
            token = strtok(0, " ");
            position = strtol(token,&wo,10);
            totaltime = Exec(word2, position, parseexternal(word3), totaltime,partitionloc);
            strcat(word2,".txt");
            Execution(word2,partitionloc,vector);
        }
        else {
            token = strtok(0, " ");
            wor= token;
            token = strtok(0, " ");
            position = strtol(token,&wor,10);
            totaltime = End_IO(wor, vector[position], totaltime);
        }
    token = strtok(NULL, " ");
    fclose(traceFile);
  }
  return 0;
}

int main(int argcount, char* args[]){
  char vector[26][7] = {"0X01E3","0X029C","0X0695","0X042B","0X0292",
                    "0X048B","0X0639","0X00BD","0X06EF","0X036C",
                    "0X07B0","0X01F8","0X03B9","0X06C7","0X0165",
                    "0X0584","0X02DF","0X05B3","0X060A","0X0765",
                    "0X07B7","0X0523","0X03B7","0X028C","0X05E8",
                    "0X05D3"};
  if (argcount < 3) {
    printf("Too few arguments! No input trace file or output file found.\n");
    return 1;
  }
  
  else if (argcount > 3){
    printf("Too many arguments! %d arguments found, where there should only be two.\n", argcount-1);
    return 1;
  }
  output = freopen(args[2], "w", stdout);
  if (output == NULL) {
      perror("Error opening output file");
      return 1;
  }
  FILE *traceFile;
  traceFile = fopen(args[1], "r");
  if (traceFile == NULL){
      perror("Error opening input file");
      return 1;  
  }
  
  
  
  int totaltime = 0;
  char line[100];
  const char *CPu = "CPU,";
  const char *IO = "End_IO";
  const char *SYSCALL = "SYSCALL";
  const char *FORK = "FORK,";
  const char *EXEC = "EXEC";
    int *partitionloc ;
    *partitionloc = 4;
  char *word1;
    char *word;
    char word3[9];
    char word2[14];
    int position;
  printf("Reading %s\n", traceFile);
  while (fgets(line, 100, traceFile)) {
    word1 = NULL;
    position = 0;
    
    char *token = strtok(line," ");
        if (strcmp( token , CPu) == 0){
            token = strtok(NULL, " ");
            totaltime = CPU(token,totaltime);
            token = strtok(NULL, " ");
        }
        else if (strcmp(token, SYSCALL) == 0){
            token = strtok(0, " ");
            word = token;
            token = strtok(0, " ");
            position = strtol(token,&word1,10);
            totaltime = Syscall(word, position, totaltime);
        }
        else if (strcmp(token,FORK) == 0){
            token = strtok(NULL, " ");
            strcpy(word2,token);
            totaltime = Fork(word2, totaltime);  // add the time taken by the fork operation to the total time
            token = strtok(NULL, " ");
        }
        else if (strcmp(token, EXEC) == 0){
            token = strtok(0, " ");
            token[strlen(token)-1]= '\0';
            strcpy(word2,token);
            for (int i = 7; i <= strlen(word2)-1; i++){
                word3[i-7] = word2[7];
            }
            token = strtok(0, " ");
            position = strtol(token,&word1,10);
            totaltime = Exec(word2, position, parseexternal(word3), totaltime,partitionloc);
            strcat(word2,".txt");
            Execution(word2,partitionloc,vector);
        }
        else {
            token = strtok(0, " ");
            word= token;
            token = strtok(0, " ");
            position = strtol(token,&word1,10);
            totaltime = End_IO(word, vector[position], totaltime);
        }
    token = strtok(NULL, " ");
      
  }
  printf("Complete!\n");
  
  fclose(traceFile);
  return 0;
}
