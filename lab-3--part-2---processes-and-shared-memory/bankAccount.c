#include  <stdio.h>
#include  <stdlib.h>
#include  <sys/types.h>
#include  <sys/wait.h>
#include  <time.h>
#include  <sys/shm.h>
#include  <unistd.h>
#include  <sys/ipc.h>

// int BankAccount = 0;
// int Turn = 0;

void deposit(int BankAccount){
  //Randomly generate a balance amount to give the Student between   0− 100
  int randomBalance = (rand() % 100);
  // If the random number is even:
  if (randomBalance % 2 == 0){
    BankAccount += randomBalance;
    printf("Dear old Dad: Deposits $%d / Balance = $%d\n", randomBalance, BankAccount);
  }
  else{
    printf("Dear old Dad: Doesn't have any money to give\n");

  }
}
void withdraw(int BankAccount){
  int studentBalance = (rand() % 50);
  printf("Poor Student needs $%d\n", studentBalance);
  if (studentBalance <= BankAccount){
    BankAccount -= studentBalance;
    printf("Poor Student: Withdraws $%d / Balance = $%d\n", studentBalance, BankAccount);
  }
  else if (studentBalance > BankAccount) {
    printf("Poor Student: Not Enough Cash ($%d)\n", BankAccount );
  }
  }
int  main(int  argc, char *argv[]){
     int    ShmID;
     int    *ShmPTR;
     pid_t  pid;
     int    BankAccount;
     int    Turn;
     ShmID = shmget(IPC_PRIVATE, 2*sizeof(int), IPC_CREAT | 0666);
     if (ShmID < 0) {
          printf("*** shmget error (server) ***\n");
          exit(1);
     }
     printf("Server has received a shared memory of four integers...\n");

     ShmPTR = (int *) shmat(ShmID, NULL, 0);
     if (ShmPTR < 0) {
          printf("*** shmat error (server) ***\n");
          exit(1);
     }
     printf("Server has attached the shared memory...\n");
     ShmPTR[0] = 0;
     ShmPTR[1] = 0;
     Turn = ShmPTR[1];
     BankAccount = ShmPTR[0];
    // Parent Process
  pid = fork();
  srand(time(0));
  if (pid > 0) {
    int incrementLoop = 0;
    while (incrementLoop < 25){
      int sleepTime = (rand() % 1);
      sleep(sleepTime);
      while (Turn != 0);
      if (BankAccount <= 100){
        deposit(BankAccount);
        printf("Dear old Dad: Thinks Student has enough Cash ($%d)\n", BankAccount);
        
      }
      Turn = 0;
        incrementLoop++;
     wait(&pid);
     printf("Server has detected the completion of its child...\n");
     shmdt((void *) ShmPTR);
     printf("Server has detached its shared memory...\n");
     shmctl(ShmID, IPC_RMID, NULL);
     printf("Server has removed its shared memory...\n");
     printf("Server exits...\n");
     exit(0);

    }
          
     }
  else {
    for (int incrementLoop = 0; incrementLoop < 25; incrementLoop++){
      int sleepTime = (rand() % 1);
      sleep(sleepTime);
      while (Turn == 1);
      withdraw(BankAccount);
      Turn = 1;
    }
    exit(0);
  }
}