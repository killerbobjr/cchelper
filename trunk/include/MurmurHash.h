#pragma once

namespace base {

#define HASH_SEED		(1689)
unsigned int MurmurHash2 ( const void * key, int len, unsigned int seed );
};