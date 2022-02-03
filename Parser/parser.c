#include "parser.h"

int main(int argc, char ** argv)
{ 
   int i; 
   FILE *fp;
   Program prog; 
   prog.counter = 0;
   prog.cword = prog.words[prog.counter];
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
   
   alltest();
   testprogram(&prog);
   Main(&prog);
   printf("%s Parsed successfully.\n", argv[1]);  
   prog.cword = prog.words[prog.counter];
   return 0; 
}

int Main(Program * p) {
   if(p->words[p->counter][0] != '{'){
      ERROR("No Initial { "); 
      return 0;
   }
   p->counter = p->counter + 1;
   TestInstructList(p);   
   return 1;
}

int TestInstructList(Program *p) {
   if(strsame(p->words[p->counter], "}")){
      return 0;
   }
   TestInstructions(p);
   p->counter = p->counter + 1;
   TestInstructList(p); 
   return 1;
}

int TestInstructions(Program *p) {
   if((strsame(p->words[p->counter], "FD")) ||
      (strsame(p->words[p->counter], "RT")) ||
      (strsame(p->words[p->counter], "LT"))) {
      wordcheck(p);
      testVARNUM(p);
      return 1;
   }   
   if(strsame(p->words[p->counter], "SET")){
      wordcheck(p);
      testSet(p);
      return 1;
   }
   if(strsame(p->words[p->counter], "DO")){
      wordcheck(p);
      testDO(p);
      return 1;
   }
   ERROR("EXPECTED VALID INSTRUCTION");
   return 0; 
}

int testprogram(Program * p){
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
      return 1;
   }
   else{
      ERROR("INCOMPLETE PROGRAM CHECK BRACKETS");
      return 0;
   }
}

int testSet(Program * p){
   p->counter = p->counter + 1;
   if(testVAR(p) != 1){
      ERROR("Missing Variable After SET");  
      return 0;
   }
   p->cword = p->words[p->counter];
   p->counter = p->counter + 1;      
   wordcheck(p);
   if(strsame(p->words[p->counter], ":=")){
      testPolish(p);
   }else{
      ERROR("Missing :=");
      return 0;
   }
   return 1; 
}
int testVAR(Program * p){
   /*printf("%s\n", p->cword);*/
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
/* */
int testPolish(Program * p){
   if((p->words[p->counter][0] == ';')){
      return 1;
   }
   p->counter = p->counter + 1;
   wordcheck(p);
   if(testVAR(p) == 1){   
      return testPolish(p); 
   }
   if(testOP(p) == 1){   
      return testPolish(p); 
   }
   if(testNumber(p) == 1){   
      return testPolish(p); 
   }
   return 0;   
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

int testVARNUM(Program * p){
   p->counter = p->counter + 1;   
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

int testDO(Program * p){
   p->counter = p->counter + 1; 
   testVAR(p);
   p->counter = p->counter + 1; 
   if(strsame(p->words[p->counter], "FROM")){
      testVARNUM(p);
   } else {
      ERROR("MISSING FROM KEYWORD");
      return 0;
   }
   p->counter = p->counter + 1;
   if(strsame(p->words[p->counter], "TO")){
      testVARNUM(p); 
      p->counter = p->counter + 1;
   } else {
      ERROR("MISSING TO KEYWORD");
      return 0;
   }
   Main(p); 
   return 1;
}

void wordcheck(Program * p){
   p->cword = p->words[p->counter];
}

Program testprog1(){
   Program p; 
   strcpy(p.words[0], "{");
   strcpy(p.words[1], "FD");
   strcpy(p.words[2], "A");
   strcpy(p.words[3], "}");
   p.counter = 0; 
   return p; 
}

Program testprog2(){
   Program p; 
   strcpy(p.words[0], "{");
   strcpy(p.words[1], "FD");
   strcpy(p.words[2], "8");
   strcpy(p.words[3], "}");
   p.counter = 0; 
   return p; 
}

Program testprog3(){
   Program p; 
   strcpy(p.words[0], "{");
   strcpy(p.words[1], "SET");
   strcpy(p.words[2], "A");
   strcpy(p.words[3], ":=");
   strcpy(p.words[4], "0");
   strcpy(p.words[5], ";");
   strcpy(p.words[6], "}");
   p.counter = 0; 
   return p; 
}

Program testprog4(){
   Program p; 
   strcpy(p.words[0], "LT");
   strcpy(p.words[1], "RT");
   strcpy(p.words[2], "A");
   strcpy(p.words[3], "}");
   p.counter = 0; 
   return p; 
}

Program testprog5(){
   Program p; 
   strcpy(p.words[0], "RT");
   strcpy(p.words[1], "100");
   strcpy(p.words[2], "}");
   p.counter = 3; 
   return p; 
}

Program testprog6(){
   Program p; 
   strcpy(p.words[0], "{");
   strcpy(p.words[1], "FD");
   strcpy(p.words[2], "A");
   strcpy(p.words[1], "FD");
   strcpy(p.words[2], "20");
   strcpy(p.words[1], "FD");
   strcpy(p.words[2], "30");
   strcpy(p.words[2], "}");
   p.counter = 0; 
   return p; 
}

Program testprog7(){
   Program p; 
   strcpy(p.words[0], "LT");
   strcpy(p.words[1], "RT");
   strcpy(p.words[2], "A");
   strcpy(p.words[3], "}");
   p.counter = 0; 
   return p; 
}

Program testprog8(){
   Program p; 
   strcpy(p.words[0], "{");
   strcpy(p.words[1], "DO");
   strcpy(p.words[2], "A");
   strcpy(p.words[3], "FROM");
   strcpy(p.words[4], "1");
   strcpy(p.words[5], "TO");
   strcpy(p.words[6], "100");
   strcpy(p.words[7], "{");
   strcpy(p.words[8], "FD");
   strcpy(p.words[9], "20");
   strcpy(p.words[10], "LT");
   strcpy(p.words[11], "30");
   strcpy(p.words[12], "}");
   p.counter = 0; 
   return p; 
}

Program testprog9(){
   Program p; 
   strcpy(p.words[0], "{");
   strcpy(p.words[1], "LT");
   strcpy(p.words[2], "30");
   strcpy(p.words[3], "}");
   p.counter = 0; 
   return p; 
}

Program testprog10(){
   Program p;   
   strcpy(p.words[0], "SET");
   strcpy(p.words[1], "A");
   strcpy(p.words[2], ":=");
   strcpy(p.words[3], "C");
   strcpy(p.words[4], "A");
   strcpy(p.words[5], "5");
   strcpy(p.words[6], "/");
   strcpy(p.words[7], "+");
   strcpy(p.words[8], ";");
   strcpy(p.words[9], "}");
   p.counter = 0; 
   return p; 
}

Program testprog11(){
   Program p; 
   strcpy(p.words[0], "{");
   strcpy(p.words[1], "DO");
   strcpy(p.words[2], "A");
   strcpy(p.words[3], "FROM");
   strcpy(p.words[4], "1");
   strcpy(p.words[5], "TO");
   strcpy(p.words[6], "100");
   strcpy(p.words[7], "{");
   strcpy(p.words[8], "FD");
   strcpy(p.words[9], "20");
   strcpy(p.words[10], "LT");
   strcpy(p.words[11], "30");
   strcpy(p.words[12], "DO");
   strcpy(p.words[13], "B");
   strcpy(p.words[14], "FROM");
   strcpy(p.words[15], "1");
   strcpy(p.words[16], "TO");
   strcpy(p.words[17], "50");
   strcpy(p.words[18], "{");
   strcpy(p.words[19], "LT");
   strcpy(p.words[20], "30");
   strcpy(p.words[21], "RT");
   strcpy(p.words[22], "40");
   strcpy(p.words[23], "}");
   strcpy(p.words[24], "}");
   strcpy(p.words[25], "}");
   p.counter = 0; 
   return p; 
}

Program testprog12(){
   Program p; 
   strcpy(p.words[0], "SET");
   strcpy(p.words[1], "D");
   strcpy(p.words[2], ":=");
   strcpy(p.words[3], "A");
   strcpy(p.words[5], "}");
   p.counter = 2; 
   return p; 
}

Program testDO1(){
   Program p; 
   strcpy(p.words[0], "{");
   strcpy(p.words[1], "DO");
   strcpy(p.words[2], "A");
   strcpy(p.words[3], "FROM");
   strcpy(p.words[4], "1");
   strcpy(p.words[5], "TO");
   strcpy(p.words[6], "8");
   strcpy(p.words[7], "{");
   strcpy(p.words[8], "}");
   strcpy(p.words[9], "}");
   p.counter = 0; 
   return p; 
}

void alltest(void){
   testmain();
   assertOP();
   assertVARNUM();
   assertDO();
   assertInstructions();
   assertSET(); 
   assertPolish(); 
}

void testmain(void){
   Program M1 = testprog1(); 
   Program M2 = testprog2(); 
   Program M3 = testprog3(); 
   Program M11 = testprog11();
   
   assert(Main(&M1) == 1); 
   assert(Main(&M2) == 1);
   assert(Main(&M3) == 1);
   assert(Main(&M11) == 1); 
}
 
void testbrackcount(void){
   Program M1 = testprog1();
   assert(testprogram(&M1) == 1); 
}

void assertInstructions(void){
   Program test1 = testprog1();
   Program test2 = testprog3();  
   test1.counter = 1;
   test2.counter = 1;
   assert(TestInstructions(&test1) == 1); 
   assert(TestInstructions(&test2) == 1);  
   
}

void assertVARNUM(void){
   Program * p = (Program*)malloc(10000);
   Program test1 = testprog1(); 
   Program test2 = testprog9();
   Program test3 = testprog5();
   Program test4 = testprog7();
   Program test5 = testprog2();
   Program test6 = testprog12();
   assert(testVARNUM(&test1) == 1); 
   assert(testVARNUM(&test2) == 1);
   assert(testVARNUM(&test3) == 1); 
   assert(testVARNUM(&test4) == 1); 
   assert(testVAR(&test5) == 1); 
   assert(testVARNUM(&test6) == 1);
   strcpy(p->words[0], "A"); 
   p->counter = 0; 
   assert(testVAR(p)==1); 
   strcpy(p->words[0], "1"); 
   p->counter = 0; 
   assert(testNumber(p)==1); 
   free(p); 
}

void assertSET(void){
   Program * test = (Program *)malloc(10000); 
   Program p = testprog10(); 
   p.counter = 0;
   assert(testSet(&p) == 1);
   strcpy(test->words[0], "SET"); 
   strcpy(test->words[1], "B"); 
   strcpy(test->words[2], ":="); 
   strcpy(test->words[3], "5"); 
   strcpy(test->words[4], "7"); 
   strcpy(test->words[5], "+"); 
   strcpy(test->words[6], ";");
   strcpy(test->words[7], "}"); 
   test->counter = 0; 
   assert(testSet(test) == 1); 
   free(test); 
}

void assertOP(void){
   Program p;
   strcpy(p.words[0], "{");
   strcpy(p.words[1], "+");
   strcpy(p.words[2], "}");
   p.counter = 1;
   assert(testOP(&p) == 1); 
   strcpy(p.words[0], "{");
   strcpy(p.words[1], "-");
   strcpy(p.words[2], "}");
   assert(testOP(&p) == 1);
   strcpy(p.words[0], "{");
   strcpy(p.words[1], "-");
   strcpy(p.words[2], ";");
   p.counter = 2; 
   assert(testOP(&p) == 0);
}

void assertDO(void){
   Program * A = (Program*)malloc(10000);
   Program p = testprog11(); 
   strcpy(A->words[0], "D0");   
   strcpy(A->words[1], "B");   
   strcpy(A->words[2], "FROM");
   strcpy(A->words[3], "1");
   strcpy(A->words[4], "TO");
   strcpy(A->words[5], "8");
   strcpy(A->words[6], "{");
   strcpy(A->words[7], "FD");
   strcpy(A->words[8], "30");
   strcpy(A->words[9], "}"); 
   A->counter = 0; 
   assert(testDO(A) == 1);
   p.counter = 1; 
   assert(testDO(&p) == 1); 
   free(A); 
} 

void assertPolish(void){
   Program test = testprog3();
   Program test2 = testprog10(); 
   Program test3 = testprog12(); 
   test2.counter = 2; 
   wordcheck(&test2);
   assert(testPolish(&test2) == 1); 
   test.counter = 3;
   assert(testPolish(&test) == 1);
   test.counter = 2;
   assert(testPolish(&test3)== 1);
}
