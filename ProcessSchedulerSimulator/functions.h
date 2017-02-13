#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include "process.h"

//  A comparator for the Process class
//  This is implemented so that the Process class may work with the STL sort function
//  First Input: A constant pointer to a Process object to be tested for the
//  left hand side of the less comparator
//  Second Input: A constant pointer to a Process object to be tested for the 
//  right hand side of the less comparator
//  Returns true if the priority of the left hand side is less than the priority of the
//  right hand side
//  Returns false if the priority of the left hand side is greater than or equalt to the 
//  priority of the right hand side
struct APtrProcessComp {
  bool operator()(const Process *lhs, const Process *rhs) const {
    return lhs->priority() < rhs->priority();
  }
} ProcessCmp;

#endif // FUNCTIONS_H_