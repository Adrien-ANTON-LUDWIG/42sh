#ifndef PRINTER_H
#define PRINTER_H

#include "lexer.h"

void lexer_printer(struct lexer *lex);
void print_token(struct token *tk);

#endif /* PRINTER_H */