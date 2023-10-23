/*
***********************************************************************
  DEADASSIGN.C : IMPLEMENT THE DEAD CODE ELIMINATION OPTIMIZATION HERE
************************************************************************
*/

#include "deadassign.h"

int change;
refVar *last, *head;

/*
*************************************************************************************
   YOUR CODE IS TO BE FILLED IN THE GIVEN TODO BLANKS. YOU CAN CHOOSE TO USE ALL
   UTILITY FUNCTIONS OR NONE. YOU CAN ADD NEW FUNCTIONS. BUT DO NOT FORGET TO
   DECLARE THEM IN THE HEADER FILE.
**************************************************************************************
*/

/*
***********************************************************************
  FUNCTION TO INITIALIZE HEAD AND LAST POINTERS FOR THE REFERENCED 
  VARIABLE LIST.
************************************************************************
*/

void init()
{ 
    head = NULL;
    last = head;
}

/*
***********************************************************************
  FUNCTION TO FREE THE REFERENCED VARIABLE LIST
************************************************************************
*/

void FreeList()
{
   refVar* tmp;
   while (head != NULL)
    {
       tmp = head;
       head = head->next;
       free(tmp);
    }

}

/*
***********************************************************************
  FUNCTION TO IDENTIFY IF A VARIABLE'S REFERENCE IS ALREADY TRACKED
************************************************************************
*/
bool VarExists(char* name) {
   refVar *node;
   node = head;
   while (node != NULL) {
       if (!strcmp(name, node->name)) {
           return true;
       }
       node = node->next;
    }
    return false;
}

/*
***********************************************************************
  FUNCTION TO ADD A REFERENCE TO THE REFERENCE LIST
************************************************************************
*/
void UpdateRefVarList(char* name) {
    refVar* node = malloc(sizeof(refVar));
    if (node == NULL) return;
    node->name = name;
    node->next = NULL;
    if (head == NULL) {
        last = node;
        head = node;
    }
    else {
        last->next = node;
        last = node;
    }
}

/*
****************************************************************************
  FUNCTION TO PRINT OUT THE LIST TO SEE ALL VARIABLES THAT ARE USED/REFERRED
  AFTER THEIR ASSIGNMENT. YOU CAN USE THIS FOR DEBUGGING PURPOSES OR TO CHECK
  IF YOUR LIST IS GETTING UPDATED CORRECTLY
******************************************************************************
*/
void PrintRefVarList() {
    refVar *node;
    node = head;
    if (node == NULL) {
        printf("\nList is empty"); 
        return;
    }
    while (node != NULL) {
        printf("\t %s", node->name);
        node = node->next;
    }
}

/*
***********************************************************************
  FUNCTION TO UPDATE THE REFERENCE LIST WHEN A VARIABLE IS REFERENCED 
  IF NOT DONE SO ALREADY.
************************************************************************
*/
void UpdateRef(Node* node) {
    NodeList* arguments = NULL;
      if (node->right != NULL && node->right->exprCode == VARIABLE) {
          if (!VarExists(node->right->name)) {
              UpdateRefVarList(node->right->name);
          }
      }
      if (node->left != NULL && node->left->exprCode == VARIABLE) {
          if (!VarExists(node->left->name)) {
              UpdateRefVarList(node->left->name);
          }
      }
      if(node != NULL && node->exprCode == VARIABLE)
      {
        /* In case there is a direct assignation like: temp1 = temp2;
         OR
         Arguments inside functions that are direct */
        if (!VarExists(node->name)) {
              UpdateRefVarList(node->name);
          }
      }
      if(node != NULL && node->right == NULL && node->opCode == FUNCTIONCALL)
      {
        //Inside function, need to register use:
        /* DEAD ASSIGN FOR FUNC CALLS */
        arguments = node->arguments;
        while(arguments != NULL)
        {
          UpdateRef(arguments->node);
          arguments = arguments->next;
        }
      }
}

/*
**********************************************************************************************************************************
 YOU CAN MAKE CHANGES/ADD AUXILLIARY FUNCTIONS BELOW THIS LINE
**********************************************************************************************************************************
*/


/*
********************************************************************
  THIS FUNCTION IS MEANT TO TRACK THE REFERENCES OF EACH VARIABLE
  TO HELP DETERMINE IF IT WAS USED OR NOT LATER
********************************************************************
*/

void TrackRef(Node* funcNode) {
     NodeList* statements = funcNode->statements;
     Node *node;
     while (statements != NULL) {
        node = statements->node;
        //First lets register and check all statements:
        switch(statements->node->stmtCode)
        {
            case ASSIGN:
                UpdateRef(node->right);
                break;
            case RETURN:
                //Check return statement:
                UpdateRef(node->left);
                break;
            default:
                break;
        }      
	  statements = statements->next;
     }
}

/*
***************************************************************
  THIS FUNCTION IS MEANT TO DO THE ACTUAL DEADCODE REMOVAL
  BASED ON THE INFORMATION OF TRACKED REFERENCES
****************************************************************
*/
NodeList* RemoveDead(NodeList* statements) {
    NodeList *first;
        /*
         ****************************************
              TODO : YOUR CODE HERE
         ****************************************
        */
    Node *node, *tmpNode;
    first = statements; //Store the pointer to the statements (the first position may change in loop).
    while (statements != NULL) {
        /*
         ****************************************
              TODO : YOUR CODE HERE
         ****************************************
        */
        node = statements->node;
        //First check if current statement is used, if not, delete. Also has to be an ASSIGN
        if(node != NULL && node->stmtCode == ASSIGN && !VarExists(node->name))
        {
            tmpNode = node; //Store temp value to delete later the one we 'jumped'
            /* Update the node to point for the next one since this statement is deleted. */
            statements->node = statements->next->node;
            /* Update the pointer to the next value, since we deleted this one. Check if null in
            case we are at the end already */
            if(statements->next != NULL)
            {
                statements->next = statements->next->next;
            }
            change = true;

            //Now free the value we deleted so it is not stored in memory:
            FreeStatement(tmpNode);
        }
        else
        {
            statements = statements->next;
        }
    }
  
   return first;
}

/*
********************************************************************
  THIS FUNCTION SHOULD ENSURE THAT THE DEAD CODE REMOVAL PROCESS
  OCCURS CORRECTLY FOR ALL THE FUNCTIONS IN THE PROGRAM
********************************************************************
*/
bool DeadAssign(NodeList* funcdecls) {
    change = false;
   while (funcdecls != NULL) {
            /*
             ****************************************
              TODO : YOUR CODE HERE
             ****************************************
             */
        init();
        if(funcdecls->node != NULL && funcdecls->node->type == FUNCTIONDECL)
        {
            NodeList* statements = funcdecls->node->statements;
            TrackRef(funcdecls->node);
            RemoveDead(statements);
        }
        FreeList();//Free the list we created
        funcdecls = funcdecls->next;
    }
    return change;
}

/*
**********************************************************************************************************************************
 YOU CAN MAKE CHANGES/ADD AUXILLIARY FUNCTIONS ABOVE THIS LINE
**********************************************************************************************************************************
*/
 
