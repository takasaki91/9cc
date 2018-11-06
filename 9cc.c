#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>

enum {
    TK_NUM = 256,
    TK_EOF,
};

typedef struct {
    int token_type;
    int val;//TK_NUMのときの値
    char *input;//入力時の文字列
} Token;

Token tokens[100];

void tokenize(char *p) {
    int i = 0;
    while(*p) {
        if(isspace(*p))
        {
            p++;
            continue;
        }
        if(*p == '+' || *p == '-')
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

        fprintf(stderr, "トークないずできなかった: %s\n", p);
        exit(1);
    }
    tokens[i].token_type = TK_EOF;
    tokens[i].input = p;
}

void error(int i) {

    fprintf(stderr, "予期せぬトークンです: %s\n", tokens[i].input);
    fprintf(stderr, "予期せぬトークンタイプです: %d \n", tokens[i].token_type);
    exit(1);
}
int main(int argc, char **argv) {
    if(argc != 2) {
        fprintf(stderr, "引数の個数が正しくありません\n");
        return 1;
    }

    tokenize(argv[1]);

    printf(".intel_syntax noprefix\n");
    printf(".global main\n");
    printf("main:\n");
    
    if(tokens[0].token_type != TK_NUM) {
        error(0);
    }
    printf("  mov rax, %d\n", tokens[0].val);

    int i = 1;
    while (tokens[i].token_type != TK_EOF) {
        if(tokens[i].token_type == '+'){
            i++;
            if(tokens[i].token_type != TK_NUM)
            {
                error(i);
            }
            printf("  add rax, %d\n", tokens[i].val);
            i++;
            continue;
        }
        if(tokens[i].token_type == '-'){
            i++;
            if(tokens[i].token_type != TK_NUM)
            {
                error(i);
            }
            printf("  sub rax, %d\n", tokens[i].val);
            i++;
            continue;
        }
        error(i);
    }
    printf("  ret\n");
    return 0;
}