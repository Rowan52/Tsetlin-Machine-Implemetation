

//Number of classes
#define Number_Possible_values 2  //integers 0-9, include zero,

#define Number_Features  3 
#define Number_Automata  2*Number_Features 

//change these to find optimal values (TM Config)
#define Number_Iterations 50
#define Number_Clauses 32
static int Mid_State = 5 ;
static int THRESHOLD = 5; //cannot be zero
static float S = 3.9; // must be >1.0 , higher number increases likelyhood of reward & decreases penalty 
static float  Leniancy = 0.00; //between 0 and 1. e.g 0.2 means clause wrong if more than 20% say wrong
//leniancy optimum appears to be between 0.16 and 0.20 for small number of clauses
static int BOOST_POSITIVE = 1; 

static int Number_States = Mid_State*2 ;

extern int TM[Number_Possible_values][Number_Clauses][Number_Automata]; 


void Debug(int Option);
//initialises the states of the TM
void initialise();
//return 1 if include, 0 if exclude 
void Change_States(int New_Mid_State);
//scales the states to he new changed values. 
int Action(int state);
//type 1 fixes false negatives
void Feedback1(int Pos_Value, int Clause , int Image[Number_Features] , int Clause_Vote);
//type 2 fixes false positives
void Feedback2(int Pos_Value, int Clause, int Image[Number_Features] , int Clause_Vote);
//determines what feedback to give 
void Update( int Pos_Value ,int Answer , int Sum_Pos[Number_Possible_values], int Vote_Clauses[][Number_Clauses] , int Image[Number_Features]);
//calculates output of the tm
int Machine(int Image[Number_Features] , int Answer ,  bool Method, int debug);
//passes training or test data to tm
int TM_Image(int Size , int Data[][Number_Features] , int Answers[] , bool Method);