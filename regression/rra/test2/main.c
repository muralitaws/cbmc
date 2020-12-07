#include <stdlib.h>
#include <assert.h>
#include<stdbool.h>

#define MAX_LEN 100

bool foo(char* a1, size_t a1_len, char* a2, size_t a2_len){
    bool res = true;
    if(a1_len == a2_len){
        for(size_t i; i < a1_len; i++){
            if(a2[i] != a1[i] + 1) res &= false;
        }
        return res;
    }
    else {
        return false;
    }

}

int bar(char * a1, size_t a1_len, char * a2, size_t a2_len) {
    size_t i;
    assert(a1_len == a2_len);
    __CPROVER_assume(i < a1_len);
    if(a2[i] == a1[i] + 1) return 1;
    else return 0; 
}

void main(){
    char * a1;
    char * a2;
    size_t a1_len;
    size_t a2_len;

    __CPROVER_assume(a1_len < MAX_LEN);
    __CPROVER_assume(a2_len < MAX_LEN);
    a1 = malloc(a1_len);
    a2 = malloc(a2_len);

    if(foo(a1, a1_len, a2, a2_len)) {
        assert(bar(a1, a1_len, a2, a2_len) > 0);
    }
    else assert(true);
    return;
}