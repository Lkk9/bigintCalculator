#ifndef NOTATION_H
#define NOTATION_H

typedef enum { REVPOL, INFIX } notation_t;

int notation_get(notation_t *notation, char *str);

#endif
