#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
enum {
    TK_NUM = 256,
    TK_IDENT,
    TK_EOF,
};
typedef struct {
    int token_type;
    int val;//TK_NUMのときの値
    char *input;//入力時の文字列
} Token;
enum {
    ND_NUM = 256,
    ND_IDENT,
};
typedef struct Node {
    int ty;
    struct Node *left_hand_side;
    struct Node *right_hand_side;
    int val;
    char name;
} Node;
Token tokens[100];
Node *code[100];
void tokenize(char *p);
void gen(Node *node);
Node *new_node(int op, Node *lhs, Node *rhs);
Node *new_node_num(int val);
Node *expr();
Node *mul();
Node *term();
void error(int i);
void gem_lval(Node *node);