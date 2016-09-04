





/*

smart calculator.

*/
#include<stdio.h>
#include<string.h>
#include <stdlib.h>


#define IS_NUM_1d(str,i)		str[i]>=48 && str[i]<=57
#define IS_NUM_2d(str,i,j)		str[i][j]>=48 && str[i][j]<=57
#define NOT_NUM_1d(str,i)		str[i]<48 && str[i]>57
#define NOT_NUM_2d(str,i,j)		str[i][j]<48 && str[i][j]>57
#define OPER_CONDITION(str,i)		str[i]=='+'||str[i]=='-'||str[i]=='/'||str[i]=='*'
#define NOT_OPER_CONDITION(str,i)	str[i]!='+'||str[i]!='-'||str[i]!='/'||str[i]!='*'
#define DISCARD_CONDITION(str,i)	str[i]==' '//||str[i]=='&'||str[i]=='@'||str[i]=='#'
#define MAX_STRcalc_SIZE		(MAX_OPERAND_SIZE+SPARE_SIZE)*MAX_OPERANDS_NUM


#define DEBUGING		1
#define MAX_OPERAND_SIZE	13
#define MAX_OPERANDS_NUM	100
#define SPARE_SIZE		5
#define MAX_OPERS_NUM		MAX_OPERANDS_NUM-1

// dual illegals signs are: {{/,/}, {*,*}, {+,/}, {+,*}, {-,*}, {-,/} }
#define ILLEGALS_NUM		6
// dual legals signs are: {{+,+}, {-,-}, {/,-}, {*,+}, {*,-}, {/,+} }
#define LEGALS_NUM		6
typedef unsigned char  U8;
typedef unsigned short U16;
typedef unsigned long  U32;


typedef struct{
	U8 (*operands)[MAX_OPERAND_SIZE];
	U8 *operators;
	U8 *str;
	U16 oprndsNumUsed ; 
	} CalcDrive;


U8 DualIllegalSigns[ILLEGALS_NUM][2]= { {'/','/'}, {'*','*'}, {'+','/'}, {'+','*'}, {'-','*'}, {'-','/'} } ;
U8 DualLegalSigns[LEGALS_NUM][2]= { {'+','+'}, {'-','-'}, {'/','-'}, {'*','+'}, {'*','-'}, {'/','+'} } ;


/*
typedef struct{
	long operand;
	U8  firstDgtIndex ;
	U8  lastDgtIndex ;
	
	}OperandProbs;
*/

typedef struct{
	U32 leftOprnd;
	U8 rightOprnd;
	U8 operator;
	U32 result;
	}Process;







/*U8 handleParenthesis( U8 *str );
U8 getProcessParts( Process *proc ); // depens on operator.
U8 operandStrToInt( U8 *str, OperandProbs *oprnd ) ;

U8 calculator( long *result ) ;
*/
// this function searsh for illegal dual signs and illegal character in the sring if it exist it returns false if thre is not illegal char's 
//it is true
U8 calcStrVerify( U8 *str ) ;
U16 getStrCalc( U8 *str );
U8 departStrCalc( CalcDrive *cDrive ) ;
U8 handleParenthesisBlocks( U8 *str );
U8 handleParenthesisOneBlock( U8 *str ); // build this function first. not in version one.
// it will search for the upper function on leagal, or illegal dual signs(//,**,-/,+/,-*,+*).
U8 searchStr( U8 *str, U8 *index );
// to do the desired process, and give result.
U8 doProcess( Process *proc );

int main(void)
{
	U8 check= 1;
	U8 calcStr[MAX_STRcalc_SIZE+1] ;
	 
	// U8 results[]; // to save results.
	U8 calcOperands[MAX_OPERANDS_NUM][MAX_OPERAND_SIZE]={0} ;
	U8 operators[MAX_OPERS_NUM]={0} ;
	CalcDrive calcdr ;
	
	getStrCalc( calcStr );

	calcdr.operands= calcOperands;
	calcdr.operators= operators;
	calcdr.str= calcStr;
	//printf("%d", atoi("-25"));
	check= departStrCalc( &calcdr ) ;
	//printf("%d", sizeof(long ) );
	
	
	return 0;
}


// not completed.
U8 calcStrVerify( U8 *str )
{
	volatile U8 check= 1;
	U16 index= 0;
	for( index= 0; str[index]!= 0 ; index++ )
	{
		if( OPER_CONDITION(str,index) )
		{
			//if( 
		}
		else if( NOT_NUM_1d(str,index) || NOT_OPER_CONDITION(str,index) )
		{
			check= 0;
			break ;
		}
	}
	
	
	
	return check ;
}

// tested= ok
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

	#if DEBUGING
	printf("<<DEBUGING: calculator string input is: >>\n");
	for( strIndex= 0 ; strIndex< str[strIndex]!= '\0' ; strIndex++ )
		printf("%c", str[strIndex]);
	printf("\n");
	#endif
	return length ;
}



// need test.
// the first columns must left for signs and operands storing begins from second columns in each row.
U8 departStrCalc( CalcDrive *cDrive )
{
	volatile U8 check= 1;
	U16 row= 0, column= 0, strIndex= 0, k ;
	
	for( row= 0, strIndex= 0;  cDrive->str[strIndex]!= '\0' && row< MAX_OPERANDS_NUM  ; row++)
	{
		for( column= 0 ; column< MAX_OPERAND_SIZE ; column++, strIndex++ )
		{
			if ( OPER_CONDITION(cDrive->str,strIndex) && NOT_OPER_CONDITION(cDrive->str,strIndex-1) )
			{
				
				break ;
			}
			else if( DISCARD_CONDITION(cDrive->str,strIndex) )
				column-- ;
			else if( NOT_OPER_CONDITION(cDrive->str,strIndex) )
				cDrive->operands[row][column]= cDrive->str[strIndex] ;
		}
		if( OPER_CONDITION(cDrive->str,strIndex) )
		{
			cDrive->operators[row]= cDrive->str[strIndex] ;
			strIndex++ ;
		}
	}
	cDrive->oprndsNumUsed= row ;	// number of rows
	
	#if DEBUGING
	printf("<<DEBUGING: calculator inputs are: >>\n");
	printf("<<DEBUGING: (1) Operands as strings by order are: >>\n");
	k= row=3 ;
	for( row=0  ; row< k ; row++ )
	{
		for( column= 0; column< MAX_OPERAND_SIZE ; column++ )
		{
			if( column> 0 && NOT_NUM_2d(cDrive->operands,row,column) )
			{
				break;
				printf("ok\n");
			}
			printf("%c",cDrive->operands[row][column] );
		}
		printf("\n");
	}
	printf("<<DEBUGING: (2) Operators by order are: >>\n");
	for( row=0 ; row< cDrive->operators[row]!= 0  ; row++ )
	{
		printf("%c", cDrive->operators[row]);
		printf("\n");
	}
	printf("<<DEBUGING: Number of operands used is: %d>>\n", cDrive->oprndsNumUsed);
	#endif

	return check;
}






















