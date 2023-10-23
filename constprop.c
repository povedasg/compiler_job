/*
********************************************************************************
  CONSTPROP.C : IMPLEMENT THE DOWNSTREAM CONSTANT PROPOGATION OPTIMIZATION HERE
*********************************************************************************
*/

#include "constprop.h"

refConst *lastNode, *headNode;

static bool gChanged = false;
/*
*************************************************************************************
   YOUR CODE IS TO BE FILLED IN THE GIVEN TODO BLANKS. YOU CAN CHOOSE TO USE ALL
   UTILITY FUNCTIONS OR NONE. YOU CAN ADD NEW FUNCTIONS. BUT DO NOT FORGET TO
   DECLARE THEM IN THE HEADER FILE.
**************************************************************************************
*/

/*
***********************************************************************
  FUNCTION TO FREE THE CONSTANTS-ASSOCIATED VARIABLES LIST
************************************************************************
*/
void FreeConstList()
{
   refConst* tmp;
   while (headNode != NULL)
    {
       tmp = headNode;
       headNode = headNode->next;
       free(tmp);
    }

}

/*
*************************************************************************
  FUNCTION TO ADD A CONSTANT VALUE AND THE ASSOCIATED VARIABLE TO THE LIST
**************************************************************************
*/
void UpdateConstList(char* name, long val) {
    refConst* node = malloc(sizeof(refConst));
    if (node == NULL) return;
    node->name = name;
    node->val = val;
    node->next = NULL;
    if (headNode == NULL) {
        lastNode = node;
        headNode = node;
    }
    else {
        lastNode->next = node;
        lastNode = node;
    }
}

/*
*****************************************************************************
  FUNCTION TO LOOKUP IF A CONSTANT ASSOCIATED VARIABLE IS ALREADY IN THE LIST
******************************************************************************
*/
refConst* LookupConstList(char* name) {
    refConst *node;
    node = headNode; 
    while (node != NULL){
        if (!strcmp(name, node->name))
            return node;
        node = node->next;
    }
    return NULL;
}

/*
**********************************************************************************************************************************
 YOU CAN MAKE CHANGES/ADD AUXILLIARY FUNCTIONS BELOW THIS LINE
**********************************************************************************************************************************
*/

void FindAndChangeNode(Node* node)
{
    refConst * nodeFound = NULL;
    NodeList* arguments = NULL;

    /* Check if it is a Variable and change for the constant value in case it is registered */
    if(node != NULL && node->exprCode == VARIABLE)
    {
        nodeFound = LookupConstList(node->name);
        if(nodeFound != NULL)
        {
            //Change the value directly:
            node->value = nodeFound->val;
            node->exprCode = CONSTANT;
            gChanged = true;
        }
    }
    /* Check if it is an Operation and not a function and then try to perform the same change 
    with their left and right node since it is divided*/
    else if (node != NULL && node->exprCode == OPERATION && node->opCode != FUNCTIONCALL)
    {
        //Find children in operation if need to change:
        FindAndChangeNode(node->left);
        FindAndChangeNode(node->right);

    }
    /* Check if it is a function so we can change the value for their arguments (loop all arguments) */
    else if(node != NULL && node->right == NULL && node->opCode == FUNCTIONCALL)
    {
        /* CONST PROP FOR FUNC CALLS */
        arguments = node->arguments;
        while(arguments != NULL)
        {
          FindAndChangeNode(arguments->node);
          arguments = arguments->next;
        }
    }
}

void registerVariable(Node* node)
{
    if(node != NULL && 
        node->right != NULL &&
        node->type == STATEMENT && 
        node->right->exprCode == CONSTANT && 
        node->stmtCode == ASSIGN)
    {
        //Found node to register:
        UpdateConstList(node->name, node->right->value);
    }
}

/*
************************************************************************************
  THIS FUNCTION IS MEANT TO UPDATE THE CONSTANT LIST WITH THE ASSOCIATED VARIABLE
  AND CONSTANT VALUE WHEN ONE IS SEEN. IT SHOULD ALSO PROPOGATE THE CONSTANTS WHEN 
  WHEN APPLICABLE. YOU CAN ADD A NEW FUNCTION IF YOU WISH TO MODULARIZE BETTER.
*************************************************************************************
*/
void TrackConst(NodeList* statements) {
       Node* node;
       while (statements != NULL) 
       {
	       node = statements->node;
            /*
             ****************************************
                     TODO : YOUR CODE HERE
             ****************************************
            */

            //First we need to register the different variables:
            registerVariable(node);


            //Now process all of the ones needed to substitude

            switch(statements->node->stmtCode)
            {
            case ASSIGN:
                FindAndChangeNode(node->right);
                break;
            case RETURN:
                //Check return statement:
                FindAndChangeNode(node->left);
                break;
            default:
                break;
            }

            statements = statements->next;
        }
}


bool ConstProp(NodeList* funcdecls) {
    gChanged = false;
    while (funcdecls != NULL){
            /*
             ****************************************
                     TODO : YOUR CODE HERE
             ****************************************
             */
        if(funcdecls->node != NULL && funcdecls->node->type == FUNCTIONDECL)
        {
            NodeList* statements = funcdecls->node->statements;
            TrackConst(statements);
        }
        FreeConstList();//Free the list we created.
        funcdecls = funcdecls->next;
    }
    return gChanged;
}

/*
**********************************************************************************************************************************
 YOU CAN MAKE CHANGES/ADD AUXILLIARY FUNCTIONS ABOVE THIS LINE
**********************************************************************************************************************************
*/
