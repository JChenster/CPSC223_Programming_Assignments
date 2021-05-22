typedef struct hashmap * CompressionDict;
typedef struct elt **  FlatCompressionDict;

CompressionDict compressionDictCreate();

void compressionDictDestroy(CompressionDict c);

void compressionDictUpdate(CompressionDict c, int * bigram, int old_occurences);

void compressionDictPrint(CompressionDict c);
void flatCompressionDictPrint(CompressionDict c, FlatCompressionDict f);

void compressionDictUpdateIter(CompressionDict c, const int * input);

struct elt ** compressionDictAssign(CompressionDict c);

void compressInput(CompressionDict c, const int * input);

