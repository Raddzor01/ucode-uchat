#include "../inc/libmx.h"

int mx_atoi(const char *str) {
	const char* word;
	long long var = 0;
	int number;
	while(mx_isspace(*str))
		str++;
	if (str[0] == '-')
	    number = -1;
	else if (str[0] != '-')
	    number = 1;
	if (!(mx_isdigit(str[0])))
	    if (str[0] != '-' && str[0] != '+')
		    return 0;
	if (str[0] == '-' || str[0] == '+')
		str++;
	word = str;
	while(mx_isdigit(*word)) {
		var *= 10;
		var += *word - '0';
		word++;
	}
	if (*word != '\0')
		return 0;
	return var * number;
}
