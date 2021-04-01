#include <assert.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>

#include "base45.h"

void check(char * in, char * out) {
	char enc[0124];
	unsigned char dec[1024];
	size_t elen = sizeof(enc), dlen = sizeof(dec);
	
	assert(0 == base45_encode(enc, &elen, (const unsigned char*) in, strlen(in)));
	assert(elen == strlen(out));
	assert(0 == strcmp(enc,out));

        assert(0 == base45_decode(dec, &dlen, out, strlen(out)));
	assert(dlen == strlen(in));
	assert(0 == bcmp(dec,in,dlen));

	printf("base64(\"%s\") -> \"%s\"\n", (char*)dec, enc);
};

int main(int argc, char **argv) {
 check("Hello!!","%69 VD92EX0");
 check("base-45","UJCLQE7W581");
 check("ietf!","QED8WEX0");
};
