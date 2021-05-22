#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <assert.h>

#include "decrypt.h"
#include "getInputs.h"

struct input{
    char * username;
    uint64_t password;
    struct input * next;
};

// Read encrypted passwords and store values in Inputs
EncryptInput encryptInputCreate(){
    EncryptInput inputs = malloc(sizeof(struct input *));
    * inputs = malloc(sizeof(struct input));
    struct input * cur = * inputs;
    cur->username = NULL;
    cur->next = NULL;

    char * username;
    char * password;
    while((username = getUsername()) != 0){
        password = getLine();

        // There are currently no entries
        if(cur->username == NULL){
            char * endptr;
            cur->password = strtoull(password, &endptr, 16); 
            cur->username = malloc(sizeof(char) * (strlen(username) + 1));
            strcpy(cur->username, username);
        }
        else{
            struct input * new = malloc(sizeof(struct input));
            char * endptr;
            new->password = strtoull(password, &endptr, 16); 
            new->username = malloc(sizeof(char) * (strlen(username) + 1));
            strcpy(new->username, username);
            new->next = NULL;
            cur->next = new;
            cur = new;
        }

        free(username);
        free(password);
        //printf("%s:%" PRIx64 "\n", cur->username, cur->password);
    }
    return inputs;
}

void encryptInputDestroy(EncryptInput inputs){
    struct input * next;
    for(struct input * i = * inputs; i != NULL; i = next){
        next = i->next;
        if (i->username != NULL){
            free(i->username);    
        }
        free(i);
    }
    free(inputs);
}

struct hashmap{
    size_t num_loc;
    size_t num_stored;
    struct elt ** table;
};

struct elt{
    char * username;
    uint64_t hashed_password;
    char * cracked_password;
    struct elt * next;
};


/* Next three functions are modified from dictIntString.c from 03-22-21 lecture
*/
#define INTIAL_SIZE (32)
static Hashmap hashmapCreateInternal(size_t intial_size){
    Hashmap h = malloc(sizeof(struct hashmap));
    h->num_loc = intial_size;
    h->num_stored = 0;
    h->table = calloc(intial_size, sizeof(struct elt *));
    return h;
}

Hashmap hashmapCreate(){
    return hashmapCreateInternal(INTIAL_SIZE);
}

void hashmapDestroy(Hashmap h){
    struct elt * next;
    for (size_t i = 0; i < h->num_loc; i++){
        for (struct elt * e = h->table[i]; e != 0; e = next){
            if(e->cracked_password != NULL){
                free(e->cracked_password);
            }
            next = e->next;
            free(e);
        }
    }
    free(h->table);
    free(h);
}
/*
End
*/

Hashmap hashmapAddInputs(Hashmap h, EncryptInput inputs, uint64_t salt_hash){
    for (struct input * cur = * inputs; cur != NULL && cur->username != NULL; cur = cur->next){
        // Input passwords are already hashed
        size_t i = cur->password % h->num_loc;

        // Check for existing instance of username ie. duplicate
        int is_duplicate = 0;
        for(struct elt * e = h->table[i]; e != 0; e = e->next){
            if(!strcmp(e->username, cur->username)){
                is_duplicate = 1;
            }
        }

        // Insert input if not duplicate
        if(!is_duplicate){
            struct elt * new = malloc(sizeof(struct elt));
            new->username = cur->username;
            new->cracked_password = NULL;
            new->hashed_password = cur->password;
            new->next = h->table[i];
            h->table[i] = new;
            h->num_stored++;

            // Enlarge hashmap table if too many stored inputs
            if(h->num_stored >= h->num_loc){
                Hashmap new_h = hashmapAddInputs(hashmapCreateInternal(h->num_loc * 2), inputs, salt_hash);
                hashmapDestroy(h);
                return new_h;
            }
        }
    }
    return h;
}

// Takes passwords from the dict and if it finds a match in the hash map, it updates elt accordingly
void crackPassword(Hashmap h, char * password, uint64_t salt_hash){
    uint64_t hashed_password = passwordHash(salt_hash, password);
    size_t i = hashed_password % h->num_loc;
    for (struct elt * e = h->table[i]; e != 0; e = e->next){
        if(e->hashed_password == hashed_password){
            e->cracked_password = malloc(sizeof(char) * (strlen(password) + 1));
            strcpy(e->cracked_password, password);
        }
    }
}

void matchCrackedPasswords(Hashmap h, EncryptInput inputs){
    for (struct input * cur = * inputs; cur != NULL && cur->username != NULL; cur = cur->next){
        size_t i = cur->password % h->num_loc;
        for(struct elt * e = h->table[i]; e != 0; e = e->next){
            if(strcmp(e->username, cur->username) == 0){
                if(e->cracked_password == NULL){
                    printf("%s:%" PRIx64 "\n", cur->username, cur->password);
                }
                else{
                    printf("%s:%s\n", cur->username, e->cracked_password);
                }
            }
        }
    }
}
