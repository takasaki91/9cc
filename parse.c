#include"9cc.h"
static int pos= 0;
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
    //fprintf(stderr, "数値でも開きカッコでもないトークンです: %s",tokens[pos].input);
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