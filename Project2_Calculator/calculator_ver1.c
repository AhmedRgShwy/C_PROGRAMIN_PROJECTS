

/*
  
  AUTHOR: Ahmed Ragab Shaarawy.
  DATE	: 4/9/2016.

  PROJECT =>>	Smart calculator.

		CALCULATOR Version1 FEATURS:... 

		1- suport integer and float.
		2- add, subtract, devide and multiblicate.
		4- No parenthesis x()x.
		5- Discards white space.
		6- Designed to grow.
		
*/


#include<stdio.h>
#include<string.h>
#include <stdlib.h>

#define DEBUGGING		0

#define IS_LEGAL_CHAR_1d(str,i)		(OPER_CONDITION_1d(str,i)||IS_DIGIT_1d(str,i)||str[i]=='.')
#define ILLEGAL_CHAR_1d(str,i)		(NOT_OPER_CONDITION_1d(str,i)||NOT_DIGIT_1d(str,i))
#define IS_DIGIT_1d(str,i)		(str[i]>= 48&&str[i]<=57)
#define IS_DIGIT_2d(str,i,j)		(str[i][j]>=48&&str[i][j]<=57)
#define NOT_DIGIT_1d(str,i)		(str[i]<48&&str[i]>57)
#define NOT_DIGIT_2d(str,i,j)		(str[i][j]<48 && str[i][j]>57)
#define OPER_CONDITION_1d(str,i)	(str[i]=='+'||str[i]=='-'||str[i]=='/'||str[i]=='*')
#define NOT_OPER_CONDITION_1d(str,i)	(str[i]!='+'||str[i]!='-'||str[i]!='/'||str[i]!='*')
#define DISCARD_CONDITION(str,i)	(str[i]==' ')
#define MAX_STRcalc_SIZE		((MAX_OPERAND_SIZE+SPARE_SIZE)*MAX_OPERANDS_NUM)

#define MAX_OPERAND_SIZE	20
#define MAX_OPERANDS_NUM	50
#define SPARE_SIZE		2
#define MAX_OPERS_NUM		MAX_OPERANDS_NUM
#define ILLEGALS_NUM		13
// dual legals signs are: {{+,+}, {-,-}, {/,-}, {*,+}, {*,-}, {/,+}, {*,.}, {/,.}, {+,.}, {-,.} }.
//#define LEGALS_NUM		10

typedef unsigned char  U8;
typedef signed char  S8;
typedef unsigned short U16;
typedef signed short S16;
typedef unsigned long  U32;
typedef signed long  S32;

typedef struct{
	S32 intPart;
	double floatPart;
	}Operand;


typedef struct{
	U8 (*operandsAsStr)[MAX_OPERAND_SIZE];
	Operand *operandsAsNum ;
	U8 *operators;
	U8 *str;
	U16 strLenght ;
	U16 oprndsNumUsed ; 
	} CalcControl;

U8 DualIllegalSigns[ILLEGALS_NUM][2]= { {'*','/'}, {'/','*'}, {'/','/'}, {'*','*'}, {'+','/'}, {'+','*'}, {'-','*'}, {'-','/'},
					{'.','.'}, {'.','*'}, {'.','/'}, {'.','+'}, {'.','-'} } ;

/*
** this function is to get the string equation from user 
** it discards white space
*/
U16 getStrCalc( U8 *str );
/*
** this function separates the operands from operators by order
** it stores the operands in two dimentional array
** it stores operators in another one dimentional array
*/
U8 departStrCalc( CalcControl *control ) ;
/*
** this functions verify the string input from illegal characters
** it also search for illegal and malformed signs
*/
U8 calcStrVerify( U8 *str ) ;
/*
** this function convert the operands as string to numbers 
** it converts the operand to two parts integer part and after decimal part
*/
U8 alphaToNum( const CalcControl *control ); 
/*
** this function calculates the whole input equation
** first it goes from left to right and searchs for the division processes and does it, 
** then it does the all multiplications, eventually it searches by order the addition and subtraction processes and does
*/
U8 calculate( CalcControl *control,  double *Answer );
/*
** this functions manages the whole calculator processes from other functions.
*/
U8 calcManagement(double *Answer);


/******************* main *********************/

int main(void)
{
	U8 check= 1 ;
	double Ans= 0;
	check= calcManagement(&Ans);
	printf("Answer= %lg\n", Ans); // %lg prints the important data.

	return 0;
}

/**********************************************/



U8 calcManagement(double *Answer)
{
	volatile U8 check= 1;
	U8 calcStr[MAX_STRcalc_SIZE+1] ;
	U8 calcOperands[MAX_OPERANDS_NUM][MAX_OPERAND_SIZE+1]={0} ; // MAX_OPERAND_SIZE+1 (1 bit for null).
	U8 operators[MAX_OPERS_NUM]={0} ;
	Operand oprnds[MAX_OPERANDS_NUM]={0};
	U16 length= 0 ;
	*Answer= 0;
	CalcControl cControl ;

	/* get the equation string from user */
	length= getStrCalc( calcStr );
	
	cControl.operandsAsNum= oprnds;
	cControl.operandsAsStr= calcOperands;
	cControl.operators= operators;
	cControl.str= calcStr;
	cControl.strLenght= length ;
	
	/* departs the string and separates the operands from operators */
	check= departStrCalc( &cControl );
	if( check )
	{
		/* converts the equation string operands to numbers */
		check= alphaToNum( &cControl );
		if( check )
			/* calculate the input and get the final result */
			check= calculate( &cControl, Answer );
		else
			printf("Error! no input.\n");
	}
	else 
		printf("Error! in input.\n");
	
	return check;
}



// tested with test cases & it is ready for use.
U8 calculate(  CalcControl *control,  double *Answer )
{
	volatile U8 check= 1;
	U16 i, j, k, oprndsNum= control->oprndsNumUsed ;
	double oprnds[oprndsNum];
	
	*Answer= 0;
	
	for( i=0; i< oprndsNum ;i++ )
	{
		oprnds[i]= ((double)control->operandsAsNum[i].intPart + control->operandsAsNum[i].floatPart) ;
		#if DEBUGGING
		printf("<<DEBUGGING: operand %d as a float is: %lf>>\n", i+1, oprnds[i]);
		#endif
	}
	

	/* handling the Division */
	for( i= 0 ; i< oprndsNum ; i++ )
	{
		while( control->operators[i]== '/' )
		{	
			#if DEBUGGING
			printf("\n");
			printf("<<DEBUGGING: Division process.>>\n");
			printf("<<DEBUGGING: Operators= %c, left operand= %f, right operand= %f>>\n",
				control->operators[i],oprnds[i],oprnds[i+1] );
			#endif
			oprnds[i]/= oprnds[i+1] ;
			#if DEBUGGING
			printf("<<DEBUGGING: Result= %f.>>\n",oprnds[i] );
			#endif
			for( j= i+1 ; j< oprndsNum ; j++ )
			{
				oprnds[j]= oprnds[j+1];
				control->operators[j-1]= control->operators[j] ;
			}
			oprndsNum-- ;
		}
		
	}
		
	/* handling the multiplication */
	for( i= 0 ; i< oprndsNum ; i++ )
	{
		while( control->operators[i]== '*' )
		{
			#if DEBUGGING
			printf("\n");
			printf("<<DEBUGGING: multiplication process.>>\n");
			printf("<<DEBUGGING: Operators= %c, left operand= %f, right operand= %f>>\n",
					control->operators[i],oprnds[i],oprnds[i+1] );
			#endif
			oprnds[i]*= oprnds[i+1] ;
			#if DEBUGGING
			printf("<<DEBUGGING: Result= %f.>>\n",oprnds[i] );
			#endif
			for( j= i+1 ; j< oprndsNum ; j++ )
			{
				oprnds[j]= oprnds[j+1];
				control->operators[j-1]= control->operators[j] ;
			}
			oprndsNum-- ;
		}
	}
	


	/* handling the addition and subtraction */
	for( i= 0, *Answer= oprnds[i] ; i< oprndsNum ; i++ )
	{
		switch(control->operators[i])
		{
			case '+':
			#if DEBUGGING
			printf("\n");
			printf("<<DEBUGGING: Addition process.>>\n");
			printf("<<DEBUGGING: Operators= %c, left operand= %f, right operand= %f>>\n",
				control->operators[i],*Answer,oprnds[i+1] );
			#endif
			*Answer+= oprnds[i+1];
			#if DEBUGGING
			printf("<<DEBUGGING: Result= %f.>>\n",*Answer );
			#endif
			break;
			case '-': 
			#if DEBUGGING
			printf("\n");
			printf("<<DEBUGGING: Subtraction process.>>\n");
			printf("<<DEBUGGING: Operators= %c, left operand= %f, right operand= %f>>\n",
				control->operators[i],*Answer,oprnds[i+1] );
			#endif
			*Answer-= oprnds[i+1];
			#if DEBUGGING
			printf("<<DEBUGGING: Result= %f.>>\n",*Answer );
			#endif
			break;
			default:	; break;
		}
	}
	return check;
}




// tested with test cases & it is ready for use.
U8 alphaToNum( const CalcControl *control )
{
	volatile U8 check= 1;
	U8 *ptr ;
	U16 row, column, deci;

	for( row= 0; row< control->oprndsNumUsed ;row++)
	{
		control->operandsAsNum[row].intPart= atoi(control->operandsAsStr[row]);
		for( column= 1; control->operandsAsStr[row][column]!=0 ; column++)
		{
			if( control->operandsAsStr[row][column]== '.')
			{
				ptr= &control->operandsAsStr[row][column] ;
				control->operandsAsNum[row].floatPart= atoi(ptr+1);
				if( control->operandsAsStr[row][0]== '-')
					control->operandsAsNum[row].floatPart *= -1;
				for(deci= 1; *ptr!= 0; deci*= 10, ptr++ );
				deci/= 10;
				control->operandsAsNum[row].floatPart/= deci ;
			}
		}
	}

	//printf("%p\n",control->operandsAsStr[0] );  // prints the pointer to the first row
	//printf("%p\n",control->operandsAsStr[0]+1 ); // prints to the second character in the first row
	//printf("%p\n",control->operandsAsStr[1] ); // prints the pointer to the second row
	#if DEBUGGING
	printf("<<DEBUGGING: calculator operands as numbers(two parts) are: >>\n");
	for( row= 0; row< control->oprndsNumUsed ;row++)
		printf("	Operand(%d): integer part= %d, float part= %lf, whole number= %lf\n",
			row+1,control->operandsAsNum[row].intPart,control->operandsAsNum[row].floatPart,
			control->operandsAsNum[row].intPart+control->operandsAsNum[row].floatPart );
	#endif
	return check;
}




// tested with test cases & it is ready for use.
U8 calcStrVerify( U8 *str )
{
	volatile U8 check= 1;
	U8 i, j, k;
	
	if( IS_LEGAL_CHAR_1d(str,0) )
	{
		if( OPER_CONDITION_1d(str,0) && OPER_CONDITION_1d(str,1) )
		{
			check= 0;
			#if DEBUGGING
			printf("<<DEBUGGING: Malformed expression!!, Error(1): double signs in the first of the string>\n");
			#endif
		}
		else
		{
			for( i= 0, j=0 ; /*i< MAX_STRcalc_SIZE*/str[i]!= 0 ;i++, j++ )
			{
				if( IS_LEGAL_CHAR_1d(str,i) )
				{
					for( k=0 ; k< ILLEGALS_NUM ; k++ )
					{
						if( str[i]== DualIllegalSigns[k][0] && str[i+1]== DualIllegalSigns[k][1] )
						{
							check= 0;
							#if DEBUGGING
							printf("<<DEBUGGING: Malformed expression!!, Error(2): illegal sign.>>\n");
							#endif
							break;
						}
						else if( NOT_EXIST_SIGN_1d(str,i) )
						{
							check= 0;
							#if DEBUGGING
							printf("<<DEBUGGING: Malformed expression!!, Error(3): sign is not exist between digit"
							" and parenthesis.>>\n");
							#endif
							break;
						}
					}
					if( !check )
						break;
					else if( OPER_CONDITION_1d(str,i) && OPER_CONDITION_1d(str,i+1) && OPER_CONDITION_1d(str,i+2) )
					{
						check= 0;
						#if DEBUGGING
						printf("<<DEBUGGING: Malformed expression!!, Error(4): illegal sign.>>\n");
						#endif
						break;
					}
				}
				else
				{	
					check= 0;
					#if DEBUGGING
					printf("<<DEBUGGING: Malformed expression!!, Error(5): illegal charachter>>\n");
					#endif
				}
			}	
		}
	}
	else
	{
		check= 0;
		#if DEBUGGING
		printf("<<DEBUGGING: Malformed expression!!, Error(6): illegal charachter>>\n");
		#endif
	}
	return check;
}



// tested with test cases & it is ready for use.
U16 getStrCalc( U8 *str )
{
	U16 strIndex, length= 0 ;
	printf("Calculator: ");
	for( strIndex= 0 ; strIndex<= MAX_STRcalc_SIZE ; strIndex++ )
	{
		scanf("%c", &str[strIndex]);
		if( str[strIndex]== '\n' )
		{
			length= strIndex;
			break;
		}
		else if ( DISCARD_CONDITION(str,strIndex) )
			strIndex-- ;
	}
	str[strIndex]= '\0' ;

	#if DEBUGGING
	printf("<<DEBUGGING: calculator string input is: >>\n");
	for( strIndex= 0 ; strIndex< str[strIndex]!= '\0' ; strIndex++ )
		printf("%c", str[strIndex]);
	printf("\n");
	#endif
	return length ;
}



// tested with test cases & it is ready for use.
U8 departStrCalc( CalcControl *control )
{
	volatile U8 check= 1;
	U16 row= 0, column= 0, strIndex= 0, oper= 0, k;

	check= calcStrVerify( control->str ) ;
	if( check )
	{
		control->operandsAsStr[0][0]= '+' ;
		if( control->str[0]== '+' )
			control->str[0]= ' ';
		else if( control->str[0]== '-')
		{
			control->operandsAsStr[0][0]= '-' ;
			control->str[0]= ' ';
		}

		for( row= 0, strIndex= 0;  control->str[strIndex]!= '\0' ; row++)
		{
			for( column= 1 ; control->str[strIndex]!= 0 ; column++, strIndex++ )
			{
				if ( OPER_CONDITION_1d(control->str,strIndex) )
					break ;
				
				else if( DISCARD_CONDITION(control->str,strIndex) )
					column-- ;

				else if(IS_DIGIT_1d(control->str,strIndex) || control->str[strIndex]== '.' )
					control->operandsAsStr[row][column]= control->str[strIndex] ;
			}
			if( OPER_CONDITION_1d(control->str,strIndex) )
			{
				control->operators[row]= control->str[strIndex] ;
				if( control->str[strIndex+1]== '-' )
				{
					control->operandsAsStr[row+1][0]= '-' ;
					strIndex+= 2 ;
				}
				else if( control->str[strIndex+1]== '+' )
				{
					control->operandsAsStr[row+1][0]= '+' ;
					strIndex+= 2 ;
				}
				else if( IS_DIGIT_1d(control->str,strIndex+1)|| control->str[strIndex+1]== '.' )
				{
					control->operandsAsStr[row+1][0]= '+' ;
					strIndex++ ;
				}
			}
		}
		control->oprndsNumUsed= 0;
		for( control->oprndsNumUsed=0 ; control->operandsAsStr[control->oprndsNumUsed][0]!= 0 ;control->oprndsNumUsed++);

		#if DEBUGGING
		printf("<<DEBUGGING: calculator inputs are: >>\n");
		printf("<<DEBUGGING: (1) Operands as strings by order are: >>\n");
		for( row=0  ; row< control->oprndsNumUsed; row++ )
		{
			for( column= 0;control->operandsAsStr[row][column]!= 0 ; column++ )
				printf("%c",control->operandsAsStr[row][column] );
			printf("\n");
		}
		printf("<<DEBUGGING: (2) Operators by order are: >>\n");
		for( row=0 ; row< control->operators[row]!= 0  ; row++ )
		{
			printf("%c", control->operators[row]);
			printf("\n");
		}
		printf("<<DEBUGGING: Number of operands used are: %d>>\n", control->oprndsNumUsed);
		#endif
	}
	else
		printf("Malformed expression!.\n");
	
	return check;
}



