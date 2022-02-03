#include "extension.h"
/* hi */
int main(int argc , char ** argv)
{
   int i; 
   FILE *fp;
   Program prog; 
   Turtle mover;
   SDL_Simplewin sw; 
   initMap(&prog); 
   initSDL(&sw);
   initTurtle(&mover);  
   prog.counter = 0;
   prog.randcolour = false; 
   srand(time(0)); 
   for(i = 0; i <MAXNUMTOKENS; i++){
      prog.words[i][0] = '\0'; 
   }
   if((argc < 2 ) || (argc > 2)){
      ERROR("Not enough Command Line Arguements");
      exit(2);     
   }
   if(!(fp = fopen(argv[1], "rw"))){
      fprintf(stderr, "Cannot open %s\n", argv[1]);
      exit(2); 
   }
   i = 0; 
   while (fscanf(fp, "%s", prog.words[i++]) == 1 && i<MAXNUMTOKENS){
      assert(i<MAXNUMTOKENS);    
   }
   testprogram(&prog);
   Main(&prog, &sw, &mover);
   printf("%s Parsed successfully.\n", argv[1]);  
   return 0; 
}

void testprogram(Program * p){
   int i, bracketcount = 0;
   for(i = 0; i < MAXNUMTOKENS; i++){
      if(strsame(p->words[i], "{")){
         bracketcount++;
      }
      else if(strsame(p->words[i], "}")){
         bracketcount--;
      }
   }
   if(bracketcount == 0){
      assert(bracketcount == 0);
   }
   else{
      ERROR("INCOMPLETE PROGRAM CHECK BRACKETS");
   }
}

void Main(Program * p, SDL_Simplewin * sw, Turtle * mover) {
   Loopinfo loopinfo;
   if(p->words[p->counter][0] != '{'){
      ERROR("No Initial { "); 
   }
   p->counter++;
   wordcheck(p);
   loopinfo.isLoop = false;
   TestInstructList(p , sw, mover, &loopinfo);   
}

void TestInstructList(Program *p, SDL_Simplewin * sw,
                      Turtle * mover, Loopinfo* loop) {
   while(sw->finished == 0){
      if(strsame(p->words[p->counter], "}")){
         return;
      }
      TestInstructions(p, sw, mover, loop);
      SDL_RenderPresent(sw->renderer);
      SDL_UpdateWindowSurface(sw->win);
      SDL_Delay(MILLSCS);
      p->counter++;
      wordcheck(p);
      TestInstructList(p, sw, mover, loop);
   } 
}

void TestInstructions(Program *p, SDL_Simplewin * sw,
                      Turtle * mover, Loopinfo* loop) {
   wordcheck(p);
   if(strsame(p->words[p->counter], "FD")){
      createFD(p, sw, mover);
      return; 
   }
   if((strsame(p->words[p->counter], "RT"))||
      (strsame(p->words[p->counter], "LT"))){
      turn(p,mover); 
      return; 
   }
   if(strsame(p->words[p->counter], "DO")){
      DO(p, sw, mover, loop); 
      return; 
   }
   if(strsame(p->words[p->counter], "SET")){
      Set(p); 
      return; 
   }
   if(strsame(p->words[p->counter], "PEN")){
      PENFUNCTIONS(p, sw);
      return; 
   }
   if(strsame(p->words[p->counter], "CC")){
      PENCOLOUR(p, sw);
      return; 
   }
   if(strsame(p->words[p->counter], "MT")){
      moveturtle(p, mover);
      return; 
   }
   ERROR("EXPECTED VALID INSTRUCTION");

}

void createFD(Program * p, SDL_Simplewin * sw, Turtle * mover){
   int FDLength = 0; char vname;
   if(!testVARNUM(p)){
      ERROR("Missing VAR or NUM");
   }
   vname = p->words[p->counter][0];  
   if(testNumber(p)){
      FDLength = atoi(p->words[p->counter]);
   }else if(testVAR(p)){
      FDLength = getVariable2(p, vname); 
   }
   if(p->randcolour == true){
      randsetdraw(sw);
   }
   coordinates(mover, FDLength); 
   boundarycheck(mover); 
   DRAW;
   wordcheck(p);
   newXY(mover); 
   return; 
}

void wordcheck(Program * p){
   p->cword = p->words[p->counter];
}

int testVARNUM(Program * p){
   p->counter++;   
   if(testNumber(p) == 1){
      return 1;
   }
   if(testVAR(p) == 1){
      return 1;
   }
   ERROR("EXPECTED VARNUM");
   return 0; 
}

int testNumber(Program * p){
   int i; 
   char * numstring = p->words[p->counter]; 
   if(sscanf(numstring, "%d", &i) == true){
      return 1;
   }
   return 0;
}

int testVAR(Program * p){
   p->cword = p->words[p->counter];
   if((p->words[p->counter][0] >= 'A')|| 
      (p->words[p->counter][0] <= 'Z')){
         return 1;
      }
      else{
         ERROR("VAR isnt between A or Z");
         return 0;
      }
}

void initSDL(SDL_Simplewin * sw){
   if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    printf("SDL Cannot be Initialised.\n");
    exit(1);
  }
   sw->finished = 0;
   sw->win = SDL_CreateWindow("Turtle Interpreter", SDL_WINDOWPOS_CENTERED, 
                                       SDL_WINDOWPOS_CENTERED, INITX, INITY, 0);
  if (sw->win == NULL) {
    printf("SW->Win Couldnt be initiliased.\n");
    exit(1);
  }

   sw->renderer = SDL_CreateRenderer(sw->win, -1, SDL_RENDERER_ACCELERATED);
   if (sw->renderer== NULL) {
    printf("SW->renderer couldnt be initialised.\n");
    exit(1);
  }

   SDL_SetRenderDrawColor(sw->renderer, 0, 0, 0, 255);
   SDL_RenderClear(sw->renderer);
   WHITE
   return; 
}

void coordinates(Turtle * mover, int length){
   mover->x2 = mover->x1 + (int)(length * cos((mover->angle)*(M_PI/RADIAN)));
   mover->y2 = mover->y1 + (int)(length * sin((mover->angle)*(M_PI/RADIAN)));
}

void initTurtle(Turtle * mover){
   mover->x1 = 400;
   mover->y1 = 400; 
   mover->x2 = 0; 
   mover->y2 = 0; 
   mover->angle = 0 ; 
}

void SDLEvents(SDL_Simplewin *sw){
  SDL_Event event;
  while(SDL_PollEvent(&event)) {      
    switch (event.type){
      case SDL_QUIT:
      case SDL_MOUSEBUTTONDOWN:
      case SDL_KEYDOWN:
        sw->finished = 1;
    }
  }
}

void turn(Program * p, Turtle * mover){
   int angle;
   char * tmp;
   if(!testVARNUM(p)){
      ERROR("Missing Value"); 
   }
   tmp = p->words[p->counter]; 
   angle = atoi(tmp); 
   if(strsame(p->words[p->counter - 1], "RT")){
      mover->angle -= angle;   
   }
   if(strsame(p->words[p->counter - 1], "LT")){
      mover->angle = mover->angle + (angle * -1);   
   }
   return; 
}

void newXY(Turtle * mover){
   mover->x1 = mover->x2;
   mover->y1 = mover->y2;
}

int DO(Program * p, SDL_Simplewin * sw, Turtle * mover, Loopinfo* loop){
   int i, from, to; 
   char varname; 
   Loopinfo loopinfo;
   assert(loop); 
   p->counter++; 
   varname =  p->words[p->counter][0];
   p->counter++; 
   if(strsame(p->words[p->counter], "FROM")){
      p->counter++; 
   }else {
      ERROR("MISSING FROM KEYWORD");
   }
   from = atoi(p->words[p->counter]); 
   p->counter++;
   if(strsame(p->words[p->counter], "TO")){
      p->counter++; 
   }else {
      ERROR("MISSING TO KEYWORD");
   }
   to = atoi(p->words[p->counter]); 
   p->counter++;
   if(p->words[p->counter][0] != '{'){
      ERROR("No Initial { "); 
   }
   p->counter++; 
   initLoop(p, &loopinfo, varname, to, from);
   for(i = from; i <= to; i++){
      loopinfo.currentValue = i;
      setVar(p, varname, i); 
      p->counter = loopinfo.counterOfFirstInstr;  
      TestInstructList(p , sw, mover, &loopinfo);   
   }
   return 1; 
}  

void initLoop(Program * p, Loopinfo * loop, char vn, int to, int from){
   loop->isLoop = true;
   loop->varname = vn;
   loop->from = from;
   loop->to = to;
   loop->counterOfFirstInstr = p->counter;
}

int Set(Program * p){
   char vname; 
   float setval; 
   p->counter++; 
   if(testVAR(p) != 1){
      ERROR("Missing Variable After SET");
      return 0;
   }
   vname = p->words[p->counter][0]; 
   p->counter++;
   if(!strsame(p->words[p->counter], ":=")){
      ERROR("Missing :=");
      return 0;
   }
   p->counter++;  
   if(strsame(p->words[p->counter] , ";")){
      ERROR("No Var or Num to Set"); 
      return 0; 
   } 
   setval = testPolish(p);
   setVar(p,vname, setval); 
   return 1;
} 

int testPolish(Program * p){
   Stack stk;
   float num, firstnum, secondnum, result, passout;
   initStack(&stk);   
   do{
      if(testNumber(p)){
         num = atof(p->words[p->counter]);
         push(&stk, num);
         p->counter++; 
      } else {
         if(testVAR(p)){
            float val = getVariable2(p, p->words[p->counter][0]);
            push(&stk, val);
            p->counter++;
         }
      }
      if(testOP(p)){
         firstnum = pop(&stk);
         secondnum = pop(&stk);
         result = findcomb(p, firstnum, secondnum);
         push(&stk, result); 
         p->counter++;
      }
   }while(p->words[p->counter][0] != ';'); 
   passout = pop(&stk);
   return passout; 
}

int testOP(Program * p){
   if((strsame(p->words[p->counter] , "+")) ||
      (strsame(p->words[p->counter] , "-")) ||
      (strsame(p->words[p->counter] , "*")) ||
      (strsame(p->words[p->counter] , "/"))){
      return 1;
   }
   return 0;
} 

float findcomb(Program * p, float fnum, float snum){
   float result; 
   if(p->words[p->counter][0] == '+'){
      return result = fnum + snum; 
   }
   if(p->words[p->counter][0] == '-'){
      return result = fnum - snum; 
   }
   if(p->words[p->counter][0] == '*'){
      return result = fnum * snum; 
   }
   if(p->words[p->counter][0] == '/'){
      if(snum <= 0){
         ERROR("Cant Divide by Zero");
      }
      return result = fnum / snum; 
   }
   return 0;
}

/******************************************************************\
                           Stack Functions
*******************************************************************/             

 
void initStack(Stack * stk){
   stk->top = EMPTY;
   stk->maxsize = STACKSIZE; 
   stk->count = 0; 
}

void push(Stack * stk, float x){
   if(isFull(stk)){
      ERROR("Stack Full");
   }
   stk->items[++stk->top] = x;
   stk->count++; 
}

float pop(Stack * stk){
   if(isEmpty(stk)){
      ERROR("Stack Already Empty");
   }
   return stk->items[stk->top--];
}

bool emptyStack(Stack * stk){
   if(stk->top == EMPTY){
      return true;
   }
   else return false; 
}

int isEmpty(Stack * stk){
    return stk->top == -1;    
}

int isFull(Stack * stk){
    return stk->top == stk->maxsize - 1;   
}

/******************************************************************\
                           SET FUNCTIONS
*******************************************************************/ 

void initMap(Program * p){
   p->usedVars = 0;
}
   
float getVariable2(Program * p,char name){
   int i;
   for(i = 0;i < p->usedVars; i++){
      if(p->map[i].name == name){
         return p->map[i].value;
      }
   }
   ERROR("Variable not found"); 
}

void setVar(Program * p, char name, float val){
   int i;
   bool found = false;
   for(i = 0; i<p->usedVars; i++){
      if(p->map[i].name == name){
         found = true;
         p->map[i].value = val;
      }
   }
   if(!found){
      p->map[p->usedVars++].name = name;
      p->map[p->usedVars-1].value = val;
   }
}

void printMap(Program * p){
   int i;
   for(i = 0; i < p->usedVars; i++){
   printf("map %c %f\n", p->map[i].name, p->map[i].value);
   }
}


/******************************************************************\
                           EXTENSION FUNCTIONS
*******************************************************************/ 

void PENFUNCTIONS(Program * p,SDL_Simplewin * sw){
   p->counter++;
   if(strsame(p->words[p->counter], "UP")){
      SDL_SetRenderDrawColor(sw->renderer, 0, 0, 0, 255);
      return; 
   }
   if(strsame(p->words[p->counter], "DOWN")){
      WHITE
      return; 
   }
}

void PENCOLOUR(Program * p, SDL_Simplewin * sw){
   p->counter++;
   if(strsame(p->words[p->counter], "RED")){
      RED
      return; 
   }
   if(strsame(p->words[p->counter], "BLUE")){
      BLUE
      return; 
   }
   if(strsame(p->words[p->counter], "CYAN")){
      CYAN
      return; 
   }
   if(strsame(p->words[p->counter], "PURPLE")){
      PURPLE
      return; 
   }
   if(strsame(p->words[p->counter], "YELLOW")){
      YELLOW
      return; 
   }
   if(strsame(p->words[p->counter], "PINK")){
      PINK
      return; 
   }
   if(strsame(p->words[p->counter], "GREEN")){
      GREEN
      return; 
   }
   if(strsame(p->words[p->counter], "VIOLET")){
      VIOLET
      return; 
   }
   if(strsame(p->words[p->counter], "RAND")){
      if(p->randcolour == false){
         p->randcolour = true;
      } else if(p->randcolour == true) {
         p->randcolour = false; 
      }
   }
   if(strsame(p->words[p->counter], "WHITE")){
      WHITE
      return; 
   }
}

void randsetdraw(SDL_Simplewin * sw){
   SDL_SetRenderDrawColor(sw->renderer, RANDSETDRAW, RANDSETDRAW, RANDSETDRAW, 255);
}

void moveturtle(Program * p, Turtle * mover){
   int tempx, tempy; 
   p->counter++;
   if(!strsame(p->words[p->counter], "X")){
      ERROR("Missing X");
   }
   p->counter++;
   tempx = atoi(p->words[p->counter]); 
   p->counter++;
   if(!strsame(p->words[p->counter], "Y")){
      ERROR("Missing Y");
   }
   p->counter++;
   tempy = atoi(p->words[p->counter]); 
   mover->x1 = tempx;
   mover->y1 = tempy;
   return; 
}

void boundarycheck(Turtle * mover){
   if(((mover->x1 <0) && (mover->y1 < 0)) && 
   ((mover->x2 > INITX) && (mover->y1 > INITY))){
      ERROR("Lines go beyond Program boundaries");
   }
}
