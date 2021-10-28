#ifndef _H_BASE45
#define _H_BASE45

/** int base45_encode(char * dstOrNull, size_t *dst_lenOrNull, const unsigned char * src, size_t src_len);
 
 params:
   dstOrNull		pointer to a dst_lenOrNull sized destination to write the encoded string to;
			or NULL to skip writing.
   dst_lenOrNull	pointer to the length of the dstOrNull buffer; will return actual value.
   src			source data
   src_len		source length

   If the encoded string is longer than *dst_lenOrNull the actual length will still be returned. This
   can be used in conjuction with a NULL destination to get the length of the buffer needed. (Though x3
   is always safe).

   The resulting buffer will be \0 terminated. This \0 does not count in the returned length.

   returns 0 on sucess.
*/


int base45_encode(char * dstOrNull, size_t *dst_lenOrNull, const unsigned char * src, size_t src_len);

/** int base45_decode(unsigned char * dstOrNull, size_t *dst_lenOrNull, const char * src, size_t src_len);

 params:
   dstOrNull		pointer to a dst_lenOrNull sized destination to write the decoded bnary data
			or NULL to skip writing.
   dst_lenOrNull	pointer to the length of the dstOrNull buffer; will return actual value.
   src			source data
   src_len		source length or 0 (in which case the terminating \0 of the string is used).

   If the encoded string is longer than *dst_lenOrNull the actual length will still be returned. This
   can also be used, in combination with a NULL destination, to get the size of the buffer needed.

   The decode is case-insensitive; and will give an error on unknown characters (including on \r 
   and \n). If dst_lenOrNull is null; dstOrNull, when non-null, is assumed to be of a suitable
   length. Having both null will result in an error (return value -2)

   The resulting buffer will not be \0 terminated.

   returns 0 on sucess. 
   returns -1 on illegal values or impossible lengths, -2 on illegal inputs.
*/

int base45_decode(unsigned char * dstOrNull, size_t *dst_lenOrNull, const char * src, size_t src_len);
#endif

