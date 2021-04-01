#ifndef _H_BASE45
#define _H_BASE45

/** int base45_encode(char * dstOrNull, size_t *dst_lenOrNull, const unsigned char * src, size_t src_len);
 
 params:
   dstOrNull		pointer to a dst_lenOrNull sized destination to write the encoded string to;
			or NULL to skip writing.
   dst_lenOrNull	pointer to the length of the dstOrNull buffer; will return actual value.
   src			source data
   src_len		source length

   If the encoded string is longer than *dst_lenOrNull the actual length will still be returned.

   returns 0 on sucess.
*/


int base45_encode(char * dstOrNull, size_t *dst_lenOrNull, const unsigned char * src, size_t src_len);

/** int base45_decode(unsigned char * dstOrNull, size_t *dst_lenOrNull, const char * src, size_t src_len);

 params:
   dstOrNull		pointer to a dst_lenOrNull sized destination to write the decoded bnary data
			or NULL to skip writing.
   dst_lenOrNull	pointer to the length of the dstOrNull buffer; will return actual value.
   src			source data
   src_len		source length or null (in which case the terminating \0 of the string is used).

   If the encoded string is longer than *dst_lenOrNull the actual length will still be returned.

   returns 0 on sucess. 
   returns -1 on illegal values or impossible lengths.
*/

int base45_decode(unsigned char * dstOrNull, size_t *dst_lenOrNull, const char * src, size_t src_len);
#endif

