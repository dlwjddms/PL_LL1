/* front.c - a lexical analyzer system for simple arithmetic expressions */ 
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>  
/* Global declarations */
//jjeong
int V_OPT;
#define MAX_LEN 100
/* Variables */
int charClass;
char lexeme[MAX_LEN]; 
char nextChar;
int lexLen;
int token;
int nextToken;
FILE *in_fp, *fopen();
//jjeong
char* output;

/* errorvariables*/
#define ASSIGN_SHAPE_ERROR 0
#define RESERVE_ERROR 1
#define IDENT_NAME_ERROR 2
#define ASSIGN_ERROR 3
#define IDENT_ERROR 4
#define RPAREN_ERROR 5
#define MISSING_ERROR 6
#define NO_SEMI_ERROR 7
#define SEMI_ERROR 8
#define UNDEFINE_ERROR 9

/* Character classes */ 
#define LETTER 0
#define DIGIT 1
#define UNKNOWN INT_MAX

/* Token codes */ 
#define INT_LIT 10
#define IDENT 11 
#define ASSIGN_OP 20 
#define ADD_OP 21 
#define SUB_OP 22 
#define MULT_OP 23
#define DIV_OP 24 
#define LEFT_PAREN 25 
#define RIGHT_PAREN 26
#define SEMI 27

/******************************************************/ 
/* Function declarations */
int lex();
int lookup(char ch);
void getNonBlank(); 
void getChar(); 
void addChar();

void error(int ERROR_NUM);
void syn();
/******************************************************/ 
/* main driver */
int  main(int argc, char *argv[]) {

	if(argc <=1)
		V_OPT = false;
	else if(strcmp(argv[1], "-v")==0)
		V_OPT = true;
	else
		return 0;
    //jjeong
   /* Open the input data file and process its contents */
   if ((in_fp = fopen("front.in", "r")) == NULL)
	printf("ERROR - cannot open front.in \n"); 
   else {
	   getChar();
	   syn();
   }
	return 0;
}
/* lookup - a function to lookup operators and parentheses and return the token */
int lookup(char ch) {
	switch (ch) {
		//jjeong
		case ';': addChar();
			  nextToken =SEMI; 
			  break;

		case ':': 
			  addChar();
			  getChar();
			  if(nextChar == '='){
			  	addChar();
			  	nextToken = ASSIGN_OP;
			  }else{
				error(ASSIGN_SHAPE_ERROR);
				lexLen++;
			  	nextToken = ASSIGN_OP;
			  }
			  break;
		case '(': addChar();
	 		nextToken = LEFT_PAREN; 
			  break; 
		case ')': addChar(); 
			  nextToken = RIGHT_PAREN;
			  break;
		case '+': addChar();
			  nextToken = ADD_OP;
			  break;
		case '-': addChar();
			  nextToken = SUB_OP;
			  break;
		case '*': addChar();
			  nextToken = MULT_OP;
			  break;
		case '/': addChar();
			  nextToken = DIV_OP;
			  break;
		default: addChar();
			 nextToken = EOF; 
			 break;
	}
	return nextToken; 
}

/*****************************************************/ 
/* addChar - a function to add nextChar to lexeme */ 
void addChar() {
	if (lexLen <= 98)
       	{ 
		lexeme[lexLen++] = nextChar; 
		lexeme[lexLen] = 0;
	}
	else
		printf("Error - lexeme is too long \n");
}


/* getChar - a function to get the next character of 
 * input and determine its character class */
void getChar() {
	if ((nextChar = getc(in_fp)) != EOF) {
		if (isalpha(nextChar)) 
			charClass = LETTER;
		else if (isdigit(nextChar)) 
			charClass = DIGIT;
		else charClass = UNKNOWN; }
	else charClass = EOF; 
}


/* getNonBlank - a function to call getChar 
 * until it returns a non-whitespace character */
void getNonBlank() {
	while (isspace(nextChar))
		getChar();
}


bool reserve_word(char* cmp){
	if(strcmp(cmp,"if")==0)
		return true;
	else if(strcmp(cmp,"for")==0)
		return true;
	else if(strcmp(cmp,"while")==0)
		return true;
	else if(strcmp(cmp,"do")==0)
		return true;
	else if(strcmp(cmp,"goto")==0)
		return true;
	else if(strcmp(cmp,"int")==0)
		return true;
	else if(strcmp(cmp,"char")==0)
		return true;
	else if(strcmp(cmp,"struct")==0)
		return true;

	else
		return false;
}

/* check_reserve_word - a function to check 
 * if the identifier is using reserved word or not */
bool check_reserve_word(){
	char* cmp = (char*)malloc(sizeof(char)*lexLen);
       	strncpy(cmp, lexeme, lexLen); 
	return reserve_word(cmp);
}
/* lex - a simple lexical analyzer for arithmetic expressions */ 
int lex() {
	lexLen = 0; 
	getNonBlank(); 
	switch (charClass) {
	/* Parse identifiers */
		case LETTER:
			addChar(); getChar();
			while (charClass == LETTER || charClass == DIGIT) {
				addChar();
				getChar(); }
			nextToken = IDENT;
		       	break;
	/* Parse integer literals */
	       	case DIGIT:
			addChar(); getChar(); 
			while (charClass == DIGIT) {
				addChar(); getChar(); }
			nextToken = INT_LIT;
			break;
	/* Parentheses and operators */
		case UNKNOWN: 
			lookup(nextChar); getChar();
			      break;
		/* EOF */ 
		case EOF:
			nextToken = EOF;
		       	lexeme[0] = 'E';
		       	lexeme[1] = 'O';
		       	lexeme[2] = 'F';
		       	lexeme[3] = 0;
		       	break;
	} /* End of switch */
	//if option v
	if(V_OPT)
		printf("Next token is: %d, Next lexeme is %s\n", nextToken, lexeme);
	output = (char*)malloc(sizeof(char)*lexLen);
       	strncpy(output, lexeme, lexLen); 
       	return nextToken;
} /* End of function lex */

/*****************************************************/ 
#define SUCCESS 0
#define WARNING 1
#define ERROR 2
int RESULT =-1;
int ID;
int CON;
int OP;
bool LAST =false;

struct ERR{
	int ERROR_RET;
	char *log;
	struct ERR*err;

};
struct identTable{
	char* name;
	bool KNOWN;
	int value;
	struct identTable* next;
};
struct parseTree{
	char*name;
	char*value;
	struct parseTree *peer;
	struct parseTree *child;
	struct parseTree *parent;
};
struct ERR * h_err;
struct ERR * curr_err;
struct identTable *i_head;
struct identTable *i_tail;
struct parseTree *p_head;
struct parseTree *curr;
struct parseTree *p_ident;


/******************************************************/ 


/*init finction*/
void init_tree(){
	h_err = (struct ERR*)malloc(sizeof(struct ERR));
	curr_err = h_err;
	p_head  =(struct parseTree*) malloc(sizeof(struct parseTree));
	p_head -> peer = NULL;
	p_head -> child = NULL;
	p_head -> parent = p_head;
	curr = p_head;
}
void init_table(){
	i_head = (struct identTable*)malloc(sizeof(struct identTable));
	i_head-> name = NULL;
	i_head-> KNOWN = false;
	i_head-> value = UNKNOWN;
	i_tail = i_head;
}
/*tree function*/
struct parseTree*  make_parseTree_child(char* name, struct parseTree* preNode){
	struct parseTree * node;
	node =(struct parseTree*) malloc(sizeof(struct parseTree));
	node->peer = NULL;
	node-> parent = preNode;
	node->child =NULL;
	node->name = name;
	preNode->child = node;
	node->value =(char*)malloc(sizeof(char)*15);
	strcpy(node->value, output);
	return node;
}
struct parseTree*  make_parseTree_peer(char* name, struct parseTree* preNode){
	struct parseTree * node;
	node =(struct parseTree*) malloc(sizeof(struct parseTree));
	preNode->peer = node;
	node->peer = NULL;
	node-> child = NULL;
	node-> parent = NULL;
	node->name = name;
	node->value =(char*)malloc(sizeof(char)*15);
	strcpy(node->value, output);
	return node;
}
void test_parse_tree(struct parseTree* node){
	if (node == NULL) 
		  return; 
	/* first recur on left child */
	test_parse_tree(node->child); 	    
	/* then print the data of node */
       	if(node->child == NULL && strcmp(node-> name , "missing")){
		printf("%s  ", node->value);  
  	}
	/* now recur on right child */
	test_parse_tree(node->peer); 

}

/*table function*/
bool check_existence(){
	for(struct identTable * iter = i_head; iter != NULL; iter = iter -> next){
		//if(iter->name !=NULL && strcmp(iter->name,output)==0)
		if(iter->name !=NULL && strcmp(iter->name,output)==0 ){
			return true;
		}
	}
	return false;
}

int find_ident_value(){
	for(struct identTable * iter = i_head; iter != NULL; iter = iter -> next){
		if(iter->name !=NULL && strcmp(iter->name,output)==0 ){
			return iter->value;
		}
	}
	return UNKNOWN;
}

void make_ident_table(){
     if(check_existence()==false){
	struct identTable *node;
	node = (struct identTable*)malloc(sizeof(struct identTable));
	node-> name =(char*)malloc(sizeof(char)*lexLen);
	strncpy(node->name, output,lexLen);
	if(check_reserve_word())
		error(RESERVE_ERROR);
	if(atoi(output) > 0)
		error(IDENT_NAME_ERROR);
	node-> KNOWN = false;
	node-> value = UNKNOWN;
	node-> next = NULL;
	i_tail->next = node;
	i_tail = node;
	}
}

void test_ident_table(){
	for(struct identTable * iter = i_head->next; iter != NULL; iter = iter -> next){
		if(iter->value <  UNKNOWN)
			printf("%s : %d \n", iter-> name, iter->value);
		else
			printf("%s : UNKNOWN \n", iter-> name);
	}

}
void insert_to_ident_table(int tmp,char* name){
	for(struct identTable * iter = i_head->next; iter != NULL; iter = iter -> next){
		if(iter->name !=NULL && strcmp(iter->name,name)==0){
			if(reserve_word(name))
				iter->value = UNKNOWN;
			else
				iter->value = tmp;
			return ;
		}
	}
}
void make_error_node(char*name, char*value, bool child){
		    struct parseTree* node;
		    node = (struct parseTree *)malloc(sizeof(struct parseTree));
		    if(child ==false){
	    	    	curr->peer = node;
		    	node-> parent = NULL;}
		    else{
		    	curr->child = node;
		    	node-> parent = curr;}
		   node->peer = NULL;
		   node-> child = NULL;
		   node->name = name;
	           node->value =(char*)malloc(sizeof(char)*15);
	           strcpy(node->value, value);
	  	   curr = node; 

}
/*for handling error*/
void error(int ERROR_NUM){
	//printf("ERROR = %d", ERROR_NUM);
	struct parseTree* node;
	struct ERR * tmp_err = (struct ERR*)malloc(sizeof(struct ERR));
	switch(ERROR_NUM){
		case ASSIGN_SHAPE_ERROR:
		    tmp_err->ERROR_RET = WARNING;
		    tmp_err-> log = "your assignment operation is wrong it should be lik ':='\n";
		     lexeme[lexLen]='=';
		       break;
		case RESERVE_ERROR :
		    tmp_err->ERROR_RET = ERROR;
		    tmp_err-> log = "you can't use reserved word to identifier \n";
		      break;
		case IDENT_NAME_ERROR :
		    tmp_err->ERROR_RET = ERROR;
		    tmp_err-> log = "you can't use identifier starting woth digit\n";
	      	      break;
		case ASSIGN_ERROR :
		    tmp_err->ERROR_RET = WARNING;
		    tmp_err-> log = "your missing assgin ':='\n";

		    make_error_node("assginment_op", ":=", false);
		    /*
		    node = (struct parseTree *)malloc(sizeof(struct parseTree));
	    	    curr->peer = node;
		    node->peer = NULL;
		    node-> child = NULL;
		    node-> parent = NULL;
		    node->name = "assginment_op";
	            node->value =(char*)malloc(sizeof(char)*15);
	            strcpy(node->value, ":=");
	  	    curr = node;*/ 
		    break;
		case IDENT_ERROR :

		    make_error_node("missing_element", " ", true);
		    /*
			  node = (struct parseTree *)malloc(sizeof(struct parseTree));
	    	    curr->child = node;
		    node->peer = NULL;
		    node-> child = NULL;
		    node-> parent = curr;
		    node->name = "missing element";
	            node->value =(char*)malloc(sizeof(char)*15);
	            strcpy(node->value, " ");
	  	    curr = node;
		  */  
		    tmp_err->ERROR_RET = ERROR;
		    tmp_err-> log = "your missing identifier\n";
		     break;
		case RPAREN_ERROR :
		    tmp_err->ERROR_RET = WARNING;
		    tmp_err-> log = "your missing right paren\n";

		    make_error_node("right_paren", ")", true);
		    /*
		    node = (struct parseTree *)malloc(sizeof(struct parseTree));
	    	    curr->peer = node;
		    node->peer = NULL;
		    node-> child = NULL;
		    node-> parent = NULL;
		    node->name = "right_paren";
	            node->value =(char*)malloc(sizeof(char)*15);
	            strcpy(node->value, ")");
	  	    curr = node;  
		    i*/
		      break;
		case MISSING_ERROR :
		    tmp_err->ERROR_RET = ERROR;
		    tmp_err-> log = "you are missing 'identifier' or 'const' or 'left-paren' becareful\n";

		    make_error_node("missing_element", " ", true);
		    /*
		    node = (struct parseTree *)malloc(sizeof(struct parseTree));
	    	    curr->child = node;
		    node->peer = NULL;
		    node-> child = NULL;
		    node-> parent = curr;
		    node->name = "missing element";
	            node->value =(char*)malloc(sizeof(char)*15);
	            strcpy(node->value, " ");
	  	    curr = node;  
		   */   
		    	break;
		case NO_SEMI_ERROR :
		    tmp_err->ERROR_RET = ERROR;
		    tmp_err-> log = "you need semi colon after this statement\n It could effect code comming after \n";
		       break;
		case SEMI_ERROR :
	 	   tmp_err->ERROR_RET = WARNING;
		   tmp_err-> log = "you need erase semi colon after this statement\n";
		      break;
		case UNDEFINE_ERROR :
		    

		  tmp_err->ERROR_RET = ERROR;
		  tmp_err-> log = "your identifier id undefined\n";
		      break;
		default : return;
	}
       	tmp_err-> err =NULL;
	curr_err->err = tmp_err;
        curr_err = tmp_err;

}
/*for printing the result*/
void print_err(){
	bool NO_ERROR =true;
	for(struct ERR * iter = h_err->err; iter != NULL; iter = iter -> err){
		RESULT = iter->ERROR_RET;
		NO_ERROR = false;
	switch(RESULT){
		case 0: printf("(OK)");
			break;
		case 1: printf("(WARNING)");
			break;
		case 2: printf("(ERROR)");
			break;
		default : break;
		}
	printf("   %s\n", iter->log);
	}
	if(NO_ERROR)
		printf("(OK)\n");
}
void check_and_print(){
	printf("\n");
	//print statement
	if(ID ==0 && CON ==0 && OP ==0)
		return ;
	printf("---------------stmt------------\n");
	test_parse_tree(p_head);
	printf("\nID : %d, CONST:%d OP: %d \n", ID, CON, OP);
	print_err();
}
void print_result(){
		printf("-------------result------------\n");
		printf("RESULT ==> ");
		test_ident_table();
}

int term();
int factor();
/* Function expr parses strings in the language generated by the rule:
 *          <expr> → <term> {(+ | -) <term>} */
/* Whenever a parsing function begins, it assumes
 * that nextToken has the code for the leftmost token of the input that has not yet been used in parsing.*/
int expr() {
	int tmp =UNKNOWN;
	struct parseTree *local = make_parseTree_child("term",curr);
	curr = local;
	/* Parse the first term */
	tmp = term();
	/* If the next token is + or -, call lex to get the next token and parse the next term */
	while (nextToken == ADD_OP || nextToken == SUB_OP){ 
		int option =nextToken;
		OP++;
		local = make_parseTree_peer("term_tail",local);
		local = make_parseTree_child("add_operator",local);
		lex();
		local = make_parseTree_peer("term",local);
		curr=local;
		int ret = term();
		if(tmp != UNKNOWN && ret!=UNKNOWN){
			if(option == ADD_OP)
	       			tmp += ret;
			else
				tmp -=ret;
		}else
			tmp = UNKNOWN;
	}
	return tmp;
}

/* Function ‘term’ parses strings in the language generated by the rule:
 *               <term> -> <factor> {(* | /) <factor>)
 *               */
int term() {
	int tmp =UNKNOWN;
	struct parseTree *local = make_parseTree_child("factor",curr);
	curr = local;
	/* Parse the first factor */
	  tmp = factor();
	  /* If the next token is * or /, get the next token and parse the next factor */
	  while (nextToken == MULT_OP || nextToken == DIV_OP) { 
		 int option =nextToken;
		  OP++;
		 local = make_parseTree_peer("factor_tail",local);
		 local = make_parseTree_child("multi_operator",local);
		 lex();

		 local = make_parseTree_peer("factor",local);
		 curr=local;
		 int ret = factor();
		 if(tmp != UNKNOWN && ret!= UNKNOWN){
		 	if(option==MULT_OP)
		 		tmp =tmp*ret;
			 else{
				if(ret!=0)
					tmp = tmp/ret;
		    	}
		 }
	  }
	  return tmp;
} /* End of function term */

/* Function ‘factor’ parses strings in the language generated by the rule:
 * <factor> -> id | int_constant | (<expr>) */
int factor() {
	int tmp =UNKNOWN;
	struct parseTree *local; 
	/* Determine which RHS */
	if (nextToken == IDENT || nextToken == INT_LIT){
		if(nextToken == IDENT){
			ID++;
			local = make_parseTree_child("ident",curr);
			make_ident_table();
			tmp = find_ident_value();
			
		}else{
			CON++;
			make_parseTree_child("const",curr);
			tmp = atoi(output);
		}
		/* For the RHS id, just call ‘lex’ to get the next token */
		lex();
	}
	/* If the RHS is (<expr>), call ‘lex’ to pass over the left parenthesis,
	 * call ‘expr’, and check for the right parenthesis */
	else if (nextToken == LEFT_PAREN) {
		local = make_parseTree_child("left_paren",curr);
	       	lex();
		local = make_parseTree_peer("expression",local);
		curr=local;
		tmp = expr();
		//
		curr= local;
		if (nextToken == RIGHT_PAREN){
			local = make_parseTree_peer("right_paren",local);
			lex();
		}
		else{error(RPAREN_ERROR); 
		}
	} /* End of else if (nextToken == ... */
	else{ 
		error(MISSING_ERROR); 
		return UNKNOWN;
	}
	return tmp;
}

bool SEMI_ON = true;
void stmt(){
	//head 초기화
	init_tree();
	struct parseTree *local = make_parseTree_child("statement",curr);
	char* name = (char*)malloc(sizeof(char)*lexLen);
	strcpy(name, output);
	

	curr = local;
	if(nextToken == IDENT){
		ID++;
		local = make_parseTree_child("ident",local);
		make_ident_table();
		lex();
		curr = local;
		if(nextToken == ASSIGN_OP){
			local = make_parseTree_peer("assignment_op",local);
			lex();
			
			local = make_parseTree_peer("expression",local);
		}
		else {
			error(ASSIGN_ERROR);// assign 필요 -2
			local = make_parseTree_peer("expression",curr);
		}
		curr = local;
		int tmp = expr();
		if(tmp != UNKNOWN)
			insert_to_ident_table(tmp,name);
		else{ 
			error(UNDEFINE_ERROR);
		}
			
	}else{ error(IDENT_ERROR); // idx없음-1
		if(nextToken == ASSIGN_OP){
			local = make_parseTree_peer("assignment_op",local);
			lex();
			
			local = make_parseTree_peer("expression",local);
		}
		else {
			error(ASSIGN_ERROR);// assign 필요 -2
			local = make_parseTree_peer("expression",curr);
		}
		curr = local;
		int tmp = expr();

	}
	if(nextToken == SEMI){
			//local = make_parseTree_peer("semi_colon",local);
			SEMI_ON = true;
	}else
		SEMI_ON = false;
	curr = local;
}

void stmts(){
	ID =0;
	CON=0;
	OP=0;
	/*parse the first statement*/
	stmt();
	lex();
	// no finish semi , finish no semi
	if(SEMI_ON && nextToken ==EOF){ // finish but semi
		error(SEMI_ERROR);
	}else if(SEMI_ON == false && nextToken > EOF){// not finish but no semi -> semi needed
		error(NO_SEMI_ERROR);
	 }else if(SEMI_ON == true &&nextToken >EOF){
	// no finish semi 
		     struct parseTree* node = (struct parseTree *)malloc(sizeof(struct parseTree));
	    	    curr->peer = node;
		    node->peer = NULL;
		    node-> child = NULL;
		    node-> parent = NULL;
		    node->name = "semi_colon";
	            node->value =(char*)malloc(sizeof(char)*15);
	            strcpy(node->value, ";");
	  	    curr = node;  
		
	 }//else// if(SEMI_ON == false && nextToken == EOF){// finish but no semi 
	
		if(V_OPT == false)
			check_and_print();

		if(nextToken > EOF)
		stmts();
}

void program(){
	init_table();
	lex();
	stmts();
	if(V_OPT == false)
		print_result();
	//test_ident_table();
}

void syn(){
	program();
} 
/*****************************************************/ 
