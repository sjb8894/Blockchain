#include "hash.h"

//return 40 bits hash digest
unsigned char* SSHA(const unsigned char *msg, size_t size){
    unsigned char A, B, C, D, E;
    A = 11;
    B = 22;
    C = 33;
    D = 44;
    E = 55;

    for (int i = 0; i < size; i++){
        for (int round = 0; round < 12; round++) {
            unsigned char F = (B&C)^D;
            unsigned char old_A = A;
            A = F + E + (A >> 3) + msg[i] + round;
            E = D;
            D = C;
            C = B << 1;
            B = old_A;
        }
    }
    unsigned char* digest = (unsigned char* )malloc(DIGEST_SIZE*sizeof(unsigned char)) ;
    digest[0] = A;
    digest[1] = B;
    digest[2] = C;
    digest[3] = D;
    digest[4] = E;
    return digest;
}

void test_SSHA_string(){
    char msg[] = "Hello World!";
    unsigned char* digest = SSHA((unsigned char*)msg, strlen(msg));
    for (int i = 0; i < DIGEST_SIZE; i++){
        printf("%d ", digest[i]);
    }
    printf("\n");

    char msg2[] = "Hello World?";
    unsigned char* digest2 = SSHA((unsigned char*)msg2, strlen(msg2));
    for (int i = 0; i < DIGEST_SIZE; i++){
        printf("%d ", digest2[i]);
    }
    printf("\n");
}

void test_SSHA_block(){
    struct Block *block1 = (struct Block *)calloc(1, BLOCK_SIZE);
    block1->height = 1;
    block1->data = 10;
    block1->prevHash = NULL;
    block1->prevBlock = NULL;

    //calls tostring on created block which is passed into SSHA function and return val listed as digest
    unsigned char* digest = SSHA(toString(block1), BLOCK_SIZE); //how we hash a struct to block
    for (int i = 0; i<DIGEST_SIZE; i++){
        printf("%d ", digest[i]);
    }
    printf("\n");
}

unsigned char* toString(struct Block *block){
    //copy block to string
    unsigned char* string = (unsigned char *)malloc(BLOCK_SIZE);

    //copy memory
    memcpy(string, block, BLOCK_SIZE); //if block contains \0
    return string;
}

//compares the digest of two blocks
int match(unsigned char* msg1, unsigned char* msg2, int size){
    printf("\n");

    //initializes digests 1 and 2 to be compared from return value of SSHA function
    unsigned char* digest1 = SSHA((unsigned char*)msg1, size);
    unsigned char* digest2 = SSHA((unsigned char*)msg2, size);

    //iterate over two digests
    for (int i = 0; i < DIGEST_SIZE; i++){
        if (digest1[i] != digest2[i]){
            return 0;
        }
    }
    return 1;
}

//int main(){
//
//    //tests SSHA function and prints hash
//    unsigned char* digest_return = SSHA((unsigned char*)"Hello World!", DIGEST_SIZE);
//    for (int i = 0; i < DIGEST_SIZE; i++){
//        printf("%d ", digest_return[i]);
//    }
//
//    //tests match function and prints result based on output
//    int success_or_not = match((unsigned char*)"hello", (unsigned char*)"hello", DIGEST_SIZE);
//    if (success_or_not == 1){
//        printf("Two messages match \n");
//    } else {
//        printf("Two messages do not match \n");
//
//    }
//
//}



