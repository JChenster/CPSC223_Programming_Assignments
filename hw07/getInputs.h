void FNV1a(uint64_t *hash, const char *s);
uint64_t precomputeSaltHash(const char *salt);
uint64_t passwordHash(uint64_t saltHash, const char *password);

char * getLine();
char * getUsername();
char * fileGetLine(FILE * f);
