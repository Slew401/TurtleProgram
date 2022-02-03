#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h> 
#include <stdbool.h> 
#include <ctype.h>

#include <SDL2/SDL.h>

#define MAXNUMTOKENS 100
#define MAXTOKENSIZE 10
#define PROGNAME "settest.ttl"
#define strsame(A,B) (strcmp(A,B)==0)
#define ERROR(PHRASE) {fprintf(stderr, "Fatal error %s occured in %s, line %d\n",PHRASE, __FILE__, __LINE__);}
#define HI 1
/* New Defines */
#define INITX 800
#define INITY 800
#define INITZ 600
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#define RADIAN 180
#define MILLSCS 0.05
#define EMPTY -1
#define STACKSIZE 25
/* New Defines */
#define MALLSIZE 10000
#define MAX_VARS 100


struct data {
   char words[MAXNUMTOKENS][MAXTOKENSIZE];
   int counter;
   char * cword;
   int usedVars;
   struct setVariable{
      char name;
      float value;
   }setVariable;
   struct setVariable map[MAX_VARS];
}; 
typedef struct data Program;

struct SDL_Simplewin {
   SDL_bool finished;
   SDL_Window *win;
   SDL_Renderer *renderer;
   SDL_Texture *display;
};
typedef struct SDL_Simplewin SDL_Simplewin;

struct turtle {
   int x1;
   int x2;
   int y1;
   int y2;
   int angle; 
};
typedef struct turtle Turtle; 

struct loopinfo {
   bool isLoop;
   char varname;
   int from;
   int to;
   int currentValue;
   int counterOfFirstInstr;
};
typedef struct loopinfo Loopinfo; 

struct stack{
    int maxsize;    
    int top;        
    float items[25];
    int count;
};
typedef struct stack Stack;


int TestInstructions(Program *p , SDL_Simplewin * sw, Turtle * mover, Loopinfo* loop);
void Main(Program * p, SDL_Simplewin * sw, Turtle * mover);
int TestInstructList(Program *p, SDL_Simplewin * sw, Turtle * mover, Loopinfo* loop); 
int testprogram(Program * p);
int testVARNUM(Program * p);
int testNumber(Program * p); 
int testVAR(Program * p);
int testOP(Program * p);
int testPolish(Program * p);
void wordcheck(Program * p);
int DO(Program * p, SDL_Simplewin * sw, Turtle * mover, Loopinfo* loop);
int Set(Program * p); 
float operations(Program * p, float fnum, float snum);
/* SDL FUNCS*/
void initSDL(SDL_Simplewin * sw);
int createFD(Program * p, SDL_Simplewin * , Turtle * mover); 
void initTurtle(Turtle * mover);  
void SDLEvents(SDL_Simplewin * sw); 
void coordinates(Turtle * mover, int length);
int turn(Program * p, Turtle * mover); 
void newXY(Turtle * mover);
void initLoop(Program * p, Loopinfo * loop, char vn, int to, int from);
void initMap(Program * p);

/* Stack functions */
void initStack(Stack * stk); 
void push(Stack * stk, float x);
float pop(Stack * stk);
bool emptyStack(Stack * stk);
int isFull(Stack * stk);
int isEmpty(Stack * stk);
void setVar(Program * p, char name, float val);
float getVariable2(Program * p,char name);

/* Test functions */ 
void coretests(void);
void assertCreateFD(void); 
void assertTurn(void); 
void assertDO(void); 
void assertSet(void);

