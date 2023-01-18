#include "libmx.h"

void mx_printint(int n) {
	char buffer[12];
	int i = 0;
	if (n == 0)
		mx_printchar('0'); 
	if (n < 0) {
		mx_printchar('-');
		n *= -1;
	} 
	for (; n != 0; n /= 10) 
		buffer[i++] = 48 + n % 10;
	for (int j = i - 1; j >= 0; j--)
		mx_printchar(buffer[j]);
}
