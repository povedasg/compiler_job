/*
   CONSTFOLDING.C : THIS FILE IMPLEMENTS THE CONSTANT FOLDING OPTIMIZATION
*/

#include "constfolding.h"
/*
*************************************************************************************
   YOUR CODE IS TO BE FILLED IN THE GIVEN TODO BLANKS. YOU CAN CHOOSE TO USE ALL 
   UTILITY FUNCTIONS OR NONE. YOU CAN ADD NEW FUNCTIONS. BUT DO NOT FORGET TO 
   DECLARE THEM IN THE HEADER FILE.
**************************************************************************************
*/                                                                                                          
static bool gChanged = false;

/*
******************************************************************************************
FUNCTION TO CALCULATE THE CONSTANT EXPRESSION VALUE 
OBSERVE THAT THIS IMPLEMENTATION CONSIDERS ADDITIONAL OPTIMIZATIONS SUCH AS:
1.  IDENTITY MULTIPLY = 1 * ANY_VALUE = ANY_VALUE - AVOID MULTIPLICATION CYCLE IN THIS CASE
2.  ZERO MULTIPLY = 0 * ANY_VALUE = 0 - AVOID MULTIPLICATION CYCLE
3.  DIVIDE BY ONE = ORIGINAL_VALUE - AVOID DIVISION CYCLE
4.  SUBTRACT BY ZERO = ORIGINAL_VALUE - AVOID SUBTRACTION
5.  MULTIPLICATION BY 2 = ADDITION BY SAME VALUE [STRENGTH REDUCTION]
******************************************************************************************
*/
long CalcExprValue(Node* node)
{
     long result;
     Node *leftNode, *rightNode;
     leftNode = node->left;
     rightNode = node->right; 
     switch(node->opCode){
         case MULTIPLY:
             if (leftNode->value == 1) {
                 result = rightNode->value;
             } 
             else if (rightNode->value == 1) {
                 result = leftNode->value;
             }
             else if (leftNode->value == 0 || rightNode->value == 0) {
                 result = 0;
             }
             else if (leftNode->value == 2) {
                 result = rightNode->value + rightNode->value;
             }              
             else if (rightNode->value == 2) {
                 result = leftNode->value + leftNode->value;
             }
             else {
                 result = leftNode->value * rightNode->value;
             }
             break;
         case DIVIDE:
             if (rightNode->value == 1) {
                 result = leftNode->value;
             }
             else {
                 result = leftNode->value / rightNode->value;
             }
             break;
         case ADD:
             result = leftNode->value + rightNode->value;
             break;
         case SUBTRACT:
             result = leftNode->value - rightNode->value;
             break;
         case NEGATE:
             result = -leftNode->value;
             break;
         default:
             break;
     }
     return result;
}


/*
**********************************************************************************************************************************
// YOU CAN MAKE CHANGES/ADD AUXILLIARY FUNCTIONS BELOW THIS LINE
**********************************************************************************************************************************
*/

/*
*****************************************************************************************************
THIS FUNCTION IS MEANT TO PROCESS THE CANDIDATE STATEMENTS AND PERFORM CONSTANT FOLDING 
WHEREVER APPLICABLE.
******************************************************************************************************
*/
long ConstFoldPerStatement(Node* stmtNodeRight){
    long result = -1;
    /*
    *************************************************************************************
          TODO: YOUR CODE HERE
    **************************************************************************************
    */             
        result = CalcExprValue(stmtNodeRight);
        if(result != stmtNodeRight->value)
        {
            stmtNodeRight->exprCode = CONSTANT;
            stmtNodeRight->opCode = O_NONE;
            stmtNodeRight->value = result;  
            gChanged = true;

            //Now we need to free the children since we deleate them:
            FreeExpression(stmtNodeRight->left);//Left operand
            FreeExpression(stmtNodeRight->right);//Right operand
        }                                                            

    return result;
}


/*
*****************************************************************************************************
THIS FUNCTION IS MEANT TO IDENTIFY THE STATEMENTS THAT ARE ACTUAL CANDIDATES FOR CONSTANT FOLDING
AND CALL THE APPROPRIATE FUNCTION FOR THE IDENTIFIED CANDIDATE'S CONSTANT FOLDING
******************************************************************************************************
*/
void ConstFoldPerFunction(Node* funcNode) {
      Node *rightNode, *leftNode, *stmtNodeRight;
      long result;
      NodeList* statements = funcNode->statements;
      NodeList* arguments;

      while (statements != NULL) {

                  /*
          *************************************************************************************
        TODO: YOUR CODE HERE
          **************************************************************************************
          */

        switch(statements->node->stmtCode)
        {
        case ASSIGN:
            stmtNodeRight = statements->node->right;//Right part of statement
            break;
        case RETURN:
            //Check return statement:
            stmtNodeRight = statements->node->left;//left part statement
            break;
        default:
            break;
        }

        //Check normal statements
        if (stmtNodeRight != NULL && stmtNodeRight->exprCode == OPERATION)
        { 
          rightNode = stmtNodeRight->right;
          leftNode = stmtNodeRight->left;
          if(rightNode != NULL && 
              leftNode != NULL && 
              rightNode->exprCode == CONSTANT && 
              leftNode->exprCode == CONSTANT)
          {
              ConstFoldPerStatement(stmtNodeRight);
          }
          else if(rightNode == NULL && stmtNodeRight->opCode == FUNCTIONCALL)
          {
            /* CONST FOLDING FOR FUNC CALLS */
            arguments = stmtNodeRight->arguments;
            while(arguments != NULL)
            {
                if(arguments->node->exprCode == OPERATION)
                {
                    ConstFoldPerStatement(arguments->node);
                }
                arguments = arguments->next;
            }
          }
        }
                                                                                                            
	  statements = statements->next;
      }
     
}



/*
**********************************************************************************************************************************
// YOU CAN MAKE CHANGES/ADD AUXILLIARY FUNCTIONS ABOVE THIS LINE
********************************************************************************************************************************
*/

/*
*****************************************************************************************************
THIS FUNCTION ENSURES THAT THE CONSTANT FOLDING OPTIMIZATION IS DONE FOR EVERY FUNCTION IN THE PROGRAM
******************************************************************************************************
*/

bool ConstantFolding(NodeList* list) {
    gChanged = false;
    while (list != NULL) {
          /*
          *************************************************************************************
		TODO: YOUR CODE HERE
          **************************************************************************************
          */
        if(list->node->type == FUNCTIONDECL)
        {
            ConstFoldPerFunction(list->node);
        }
	    list = list->next;
    }
    return gChanged;
}

/*
****************************************************************************************************************************
 END OF CONSTANT FOLDING
*****************************************************************************************************************************
*/                
