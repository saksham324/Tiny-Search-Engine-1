#include <stdio.h>

// converts a word to lowercase
void normalizeWord(char *s) {
    if (s != NULL) {
        int c = 0;
   
        while (s[c] != '\0') {

            // subtracting 32 from the ASCII value
            if (s[c] >= 'A' && s[c] <= 'Z') {
                s[c] = s[c] + 32;
            }
            c++;
        }
    }
}