#ifndef DATATYPES_H
#define DATATYPES_H
/* datatypes.h 
 * The list ADT is needed to support the equilibrium test driver.
 * 
 * We need to set up two definitions to use the list ADT defined in list.c and
 * list.h
 *
 * data_t: The type of data that we want to store in the list
 *
 * comp_proc(A, B): the function to compare two data_t structures 
 */

/* include the definitions of the data we want to store in the list */

/* the list ADT works on data of this type */
typedef int data_t;

/* the comparison procedure is found in lab4.c */
#define comp_proc(x, y) (int_compare(x, y))

#endif
/* commands specified to vim. ts: tabstop, sts: soft tabstop sw: shiftwidth */
/* vi:set ts=8 sts=4 sw=4 et: */
