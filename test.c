#include <assert.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>

#include "base45.h"

void check(char * in, char * out) {
	char enc[1024];
	unsigned char dec[1024];
	size_t elen = sizeof(enc), dlen = sizeof(dec);
	bzero(enc,1024);
	bzero(dec,1024);
	
	assert(0 == base45_encode(enc, &elen, (const unsigned char*) in, strlen(in)));
	assert(elen == strlen(out));
	assert(0 == strcmp(enc,out));

	assert(0 == base45_decode(dec, &dlen, out, strlen(out)));
	assert(dlen == strlen(in));
	assert(0 == bcmp(dec,in,dlen));

	printf("base64(\"%s\") -> \"%s\"\n", (char*)dec, enc);
};

int main(int argc, char **argv) {
#ifdef SWEDEN
 check("Hello!!","%69 VD92EX0");
 check("base-45","UJCLQE7W581");
 check("ietf!","QED8WEX0");
 check("COVID-19","-M8*+A%R81A6");
 check("2021 Digital Green Certificates for travel",
	"NF6OF6P34SED-EDAECS34ZKE1$CO345$CBWER.CGPC7WE.OEX.CBJEKWEKEC: C");
#else
 check("hello world","K3*J+EGLBVAYYB36");
 check("foo bar","%4VVO:F$X5");
 check("ietf!","19N6HOO$");
 check("Hello!!","Q-*2/.MZ B");
 check("COVID-19","37J$4 QAWL0E");

 check("2021 Digital Green Certificates for travel",
	"HREOZC%EXPZQX3G6VGTB%7+Q2BISAC87Y5A QC8774F7A2NY6G+MIJ$YUSQ3P");
#endif

};
