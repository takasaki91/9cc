#include "9cc.h"
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

    if(strcmp(argv[1],"-test") == 0) {
        printf("test\n");
        runtest();
        return 0;
    }
    tokenize(argv[1]);
    code[0] = expr();


    printf(".intel_syntax noprefix\n");
    printf(".global main\n");
    printf("main:\n");
    
    printf("  push rbp\n");
    printf("  mov rbp, rsp\n");
    printf("  sub rsp, 208\n");
    for (int i = 0; code[i]; i++) {
        gen(code[i]);
        printf("  pop rax\n");
    }

    printf("  mov rsp, rbp\n");
    printf("  pop rbp\n");
    printf("  ret\n");
    return 0;
}