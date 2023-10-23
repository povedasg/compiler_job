#include "optimizer.h"
#include "constfolding.h"
#include "constprop.h"
#include "deadassign.h"

void Optimizer(NodeList *funcdecls) {
/*
*************************************
     TODO: YOUR CODE HERE
*************************************
*/
     bool madeChange = true;
     while(madeChange)
     {
          madeChange = false;
          madeChange |= ConstantFolding(funcdecls);
          madeChange |= ConstProp(funcdecls); 
          madeChange |= DeadAssign(funcdecls); 
     }
     /* No need to free funcdecls here, since it is done in main.c, in the caller */
}
