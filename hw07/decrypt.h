typedef struct input ** EncryptInput;
EncryptInput encryptInputCreate();
void encryptInputDestroy(EncryptInput inputs);

typedef struct hashmap * Hashmap;
Hashmap hashmapCreate();
void hashmapDestroy(Hashmap h);
Hashmap hashmapAddInputs(Hashmap h, EncryptInput inputs, uint64_t salt_hash);

void crackPassword(Hashmap h, char * password, uint64_t salt_hash);
void matchCrackedPasswords(Hashmap h, EncryptInput inputs);
