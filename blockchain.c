#include "hash.h"


//initializes new Blockchain
struct Blockchain *initialize(){
    struct Blockchain *blockchain = (struct Blockchain *) malloc(sizeof(struct Blockchain));
    blockchain->head = NULL;
    blockchain->size = 0;
    return blockchain;
}

//adds block to blockchain
void add(struct Blockchain *blockchain, int data){

    //1. create new block containing data
    struct Block* block = (struct Block *)malloc(sizeof(struct Block));
    block->data = data;
    block->height = blockchain->size+1;
    if (blockchain->head == NULL){
        block->prevBlock = NULL;
        block->prevHash = NULL;
    } else {
        block->prevBlock = blockchain->head;
        block->prevHash = SSHA(toString(block->prevBlock), BLOCK_SIZE);
    }

    //2. adjust head of mylist
    blockchain->head = block;
    blockchain->size += 1;
}

//prints the Blockchain
void print(struct Blockchain* blockchain){
    struct Block *temp = blockchain->head;
    printf("Printing chain: \n");
    while (temp->prevBlock != NULL){
        printf("[%d: %d] ", temp->height, temp->data);
        temp = temp->prevBlock;
    }
    printf("\n");
}

//verifies that a blockchain has not been tampered with
void verify(struct Blockchain *blockchain) {
    struct Block *curr = blockchain->head;
    for (int j=1; j < blockchain->size; j++) {
        unsigned char *digest = SSHA(toString(curr->prevBlock), BLOCK_SIZE);
        for (int i=0; i<DIGEST_SIZE; i++){
            if (digest[i] != curr->prevHash[i]){
                printf("Verify: Blockchain not verified at index: %d\n", curr->prevBlock->height);

                return;
            }
        }
        curr = curr->prevBlock;
    }
    printf("Verify: Blockchain verified\n");
}

int delete_helper(struct Blockchain *blockchain, int data){
    struct Block *temp = blockchain->head;
    struct Block *prev = NULL;
    while (temp != NULL){
        if (temp->data == data){
            if (prev == NULL){
                blockchain->head = blockchain->head->prevBlock;
            }
            else {
                prev->prevBlock = temp->prevBlock;
            }
            free(temp);
            return 1;
        }
        prev = temp;
        temp = temp->prevBlock;
    }
    return 0;
}

void delete(struct Blockchain *blockchain, int height){
    struct Block *temp = blockchain->head;
    if (height > blockchain->size){
        printf("Not a valid index at: %d\n", height);
        return;
    }
    for (int i = 0; i <blockchain->size; i++){
        int height2 = temp->height;
        if (height2 == height){
            delete_helper(blockchain, temp->data);
            printf("Delete: Block at index: %d has been removed \n", height);
            return;
        }
        temp = temp->prevBlock;
    }
}

//alters block
void alter_block(struct Blockchain *blockchain, int index, int data){
    struct Block *temp = blockchain->head;
    while (temp->height != index){
        temp = temp->prevBlock;
    }
    temp->data = data;
    printf("alter_block: block %d is altered\n", index);
}

//alters 2 blocks
void alter_2_blocks(struct Blockchain *blockchain, int index, int data){
    struct Block *temp = blockchain->head;
    struct Block *next ;
    while (temp->height != index){
        next = temp;
        temp = temp->prevBlock;
    }
    temp->data = data;
    printf("alter_2_blocks: Data in block %d is altered\n", index);
    next->prevHash = SSHA(toString(temp), BLOCK_SIZE);
    printf("Prev hash in block %d is altered\n", index + 1);
}

//main method
int main(){
    struct Blockchain *blockchain = initialize();
    for (int i = 1; i<=20; i++){
        add(blockchain, 10*i);
    }
    print(blockchain);
    verify(blockchain);

    /* Uncomment one of the three functions at a time*/
//    delete(blockchain, 5);
//    alter_block(blockchain, 5, 5000);
//    alter_2_blocks(blockchain, 5, 5000);

    verify(blockchain);
    print(blockchain);
    return 0;
}
