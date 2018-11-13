#include"9cc.h"

void tokenize(char *p) {
    int i = 0;
    while(*p) {
        if(isspace(*p))
        {
            p++;
            continue;
        }
        if(*p == '+' || *p == '-' || *p == '*' || *p == '/' || *p == '(' || *p == ')' || *p == '=')
        {
            tokens[i].token_type = *p;
            tokens[i].input = p;
            i++;
            p++;
            continue;
        }
        if(isdigit(*p))
        {
            tokens[i].token_type = TK_NUM;
            tokens[i].input = p;
            tokens[i].val = strtol(p,&p,10);
            i++;
            continue;
        }
        if ('a' <= *p && *p <= 'z') {
            tokens[i].token_type = TK_IDENT;
            tokens[i].input = p;
            i++;
            p++;
            continue;
        }

        fprintf(stderr, "トークないずできなかった: %s\n", p);
        exit(1);
    }
    tokens[i].token_type = TK_EOF;
    tokens[i].input = p;
}
