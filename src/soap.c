#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "soap.h"
#include "tools.h"
#include "sha1.h"

static bool PasswordDigest(const char *nonce, const char *created,
			   const char *clearpwd, char digest[30])
{
	size_t lbin_nonce = Base64decode_len(nonce);
	size_t lcreated = strlen(created);
	size_t lclearpwd = strlen(clearpwd);

	size_t lbuf = lbin_nonce + lcreated + lclearpwd;
	if (lbuf > 1024)
		return false;

	char *bin = (char*)alloca(lbuf);
	size_t bin_len = Base64decode(bin, nonce);
	strcpy(bin + bin_len, created);
	bin_len += lcreated;
	strcpy(bin + bin_len, clearpwd);
	bin_len += lclearpwd;

	char hash[20];
	SHA1(hash, bin, bin_len);
	Base64encode(digest, hash, sizeof(hash));
	return true;
}
