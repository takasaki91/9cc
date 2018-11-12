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

enum {
    ND_NUM = 256,
};

typedef struct Node {
    int ty;
    struct Node *left_hand_side;
    struct Node *right_hand_side;
    int val;
} Node;

Token tokens[100];

int pos = 0;

void tokenize(char *p);
void gen(Node *node);
Node *new_node(int op, Node *lhs, Node *rhs);
Node *new_node_num(int val);
Node *expr();
Node *mul();
Node *term();
void error(int i);

void tokenize(char *p) {
    int i = 0;
    while(*p) {
        if(isspace(*p))
        {
            p++;
            continue;
        }
        if(*p == '+' || *p == '-' || *p == '*' || *p == '/' || *p == '(' || *p == ')')
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

Node *new_node(int op, Node *lhs, Node *rhs) {
    Node *node = malloc(sizeof(Node));
    node->ty = op;
    node->left_hand_side = lhs;
    node->right_hand_side = rhs;
    return node;
}

Node *new_node_num(int val) {
    Node *node = malloc(sizeof(Node));
    node->ty = ND_NUM;
    node->val = val;
    return node;
}

Node *term() {
    if (tokens[pos].token_type == TK_NUM) {
        return new_node_num(tokens[pos++].val);
    }
    if(tokens[pos].token_type == '(') {
        pos++;
        Node *node = expr();
        if (tokens[pos].token_type != ')') {
            fprintf(stderr, "開きカッコに対応する閉じカッコがありませんん: %s",
                tokens[pos].input);
        }
        pos++;
        return node;
    }
    fprintf(stderr, "数値でも開きカッコでもないトークンです: %s",tokens[pos].input);
}
Node *mul() {
    Node *lhs = term();
    if(tokens[pos].token_type == TK_EOF) {
        return lhs;
    }
    if (tokens[pos].token_type == '*') {
        pos++;
        return new_node('*', lhs, mul());
    }
    if (tokens[pos].token_type == '/') {
        pos++;
        return new_node('/', lhs, mul());
    }
    return lhs;
    //fprintf(stderr, "*\\ 想定してないトークンです: %s\n",tokens[pos].input);
}
Node *expr() {
    Node *lhs = mul();
    if(tokens[pos].token_type == TK_EOF)
        return lhs;
    if (tokens[pos].token_type == '+') {
        pos++;
        return new_node('+', lhs, expr());
    }
    if (tokens[pos].token_type == '-') {
        pos++;
        return new_node('-', lhs, expr());
    }
    return lhs;
    fprintf(stderr, "+-想定してないトークンです: %s\n",tokens[pos].input);
}

void gen(Node *node) {
    if(node->ty == ND_NUM) {
        printf("  push %d\n", node->val);
        return;
    }

    gen(node->left_hand_side);
    gen(node->right_hand_side);

    printf("  pop rdi\n");
    printf("  pop rax\n");
    
    switch(node->ty) {
        case '+':
            printf("  add rax, rdi\n");
            break;
        case '-':
            printf("  sub rax, rdi\n");
            break;
        case '*':
            printf("  mul rdi\n");
            break;
        case '/':
            printf("  mov rdx, 0\n");
            printf("  div rdi\n");
            break;
    }
    printf("  push rax\n");
}
int main(int argc, char **argv) {
    if(argc != 2) {
        fprintf(stderr, "引数の個数が正しくありません\n");
        return 1;
    }

    tokenize(argv[1]);
    Node* node = expr();


    printf(".intel_syntax noprefix\n");
    printf(".global main\n");
    printf("main:\n");
    
    gen(node);

    printf("  pop rax\n");
    printf("  ret\n");
    return 0;
}