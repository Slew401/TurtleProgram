#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h> 
#include <stdbool.h> 
#include <ctype.h>


#define MAXNUMTOKENS 100
#define MAXTOKENSIZE 10
#define strsame(A,B) (strcmp(A,B)==0)
#define ERROR(PHRASE) {fprintf(stderr, "Fatal error %s occured in %s, line %d\n",PHRASE, __FILE__, __LINE__);}
#define HI 1

struct data {
   char words[MAXNUMTOKENS][MAXTOKENSIZE];
   int counter;
   char * cword;

}; 

typedef struct data Program;

int TestInstructions(Program *p);
int Main(Program * p);
int TestInstructList(Program *p); 
int testprogram(Program * p);
int testVARNUM(Program * p);
int testNumber(Program * p); 
int testDO(Program * p);
int testVAR(Program * p);
int testSet(Program * p);
int testOP(Program * p);
int testPolish(Program * p);
void wordcheck(Program * p);

/************************************************************
 *  Test Functions
 * **********************************************************/

void alltest(void);
void testmain(void);
void testbrackcount(void);
void testassertinstructlist(void);
void assertVARNUM(void);
void assertSET(void);
void assertOP(void);
void assertDO(void);
void assertInstructions(void); 
void assertPolish(void); 

