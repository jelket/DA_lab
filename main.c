#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define ASIZE  100 // INT_MAX
#define XSIZE  100  //INT_MAX

#define max(a,b) ((a) > (b) ? (a) : (b))

void preBmBc(const unsigned int *x, int m, int bmBc[]) {
    int i;

    for (i = 0; i < ASIZE; ++i)
        bmBc[i] = m;
    for (i = 0; i < m - 1; ++i){
        bmBc[x[i]] = m - i - 1;
    }
}


void suffixes(const unsigned int *x, int m, int *suff);

void suffixes(const unsigned int *x, int m, int *suff) {
    int g, i;
    int f = 0;

    suff[m - 1] = m;
    g = m - 1;
    for (i = m - 2; i >= 0; --i) {
        if (i > g && suff[i + m - 1 - f] < i - g)
            suff[i] = suff[i + m - 1 - f];
        else {
            if (i < g)
                g = i;
            f = i;
            while (g >= 0 && x[g] == x[g + m - 1 - f])
                --g;
            suff[i] = f - g;
        }
    }
}

void preBmGs(unsigned int *x, int m, int bmGs[], int suff[]) {
    int i, j;
    suffixes(x, m, suff);

    for (i = 0; i < m; ++i)
        bmGs[i] = m;
    j = 0;
    for (i = m - 1; i >= 0; --i)
        if (suff[i] == i + 1)
            for (; j < m - 1 - i; ++j)
                if (bmGs[j] == m)
                    bmGs[j] = m - 1 - i;
    for (i = 0; i <= m - 2; ++i)
        bmGs[m - 1 - suff[i]] = m - 1 - i;
}
void AG(unsigned int x[], int m, unsigned int y[], int n, int a[], int b) {
    int i, j, k, s, shift,
            bmGs[XSIZE], skip[XSIZE], suff[XSIZE], bmBc[ASIZE];

    preBmGs(x, m, bmGs, suff);
    preBmBc(x, m, bmBc);
    memset(skip, 0, m*sizeof(int));

    j = 0;
    while (j <= n - m) {
        i = m - 1;
        while (i >= 0) {
            k = skip[i];
            s = suff[i];

            if (k > 0)
                if (k > s) {
                    if (i + 1 == s)
                        i = (-1);
                    else
                        i -= s;
                    break;
                }
                else {
                    i -= k;
                    if (k < s)
                        break;
                }
            else {
                if (x[i] == y[i + j])
                    --i;
                else
                    break;
            }
        }

        if (i < 0) {
            int num = j;
            int string = 0;
            for(;num > a[string]; string++){
                num -= a[string];
            }
            printf("%d, %d\n", string+1  ,num+1);
            skip[m - 1] = m;
            shift = bmGs[0];
        }
        else {
            skip[m - 1] = m - 1 - i;
            shift = max(bmGs[i], bmBc[y[i + j]] - m + 1 + i);
        }
        j += shift;
        memcpy(skip, skip + shift, (m - shift)*sizeof(int));
        memset(skip + m - shift, 0, shift*sizeof(int));
    }
}

int main() {
    unsigned int pat[200];
    unsigned int text[10000];
    char lego[30];
    char c;
    int l = 0;
    int k = 0;
    int m = 0;
    int p = 0;
    int string = 0;
    int NumsInStr[100];

    while (true) {
        c = getchar();
        if (c == '\n' || c == EOF){
            lego[m] = 0;
            pat[l] = strtol(lego, (char **)NULL, 10);
            l++;
            memset(lego, 0, sizeof(lego));
            m = 0;
            break;
        } else if (c == ' '){
            lego[m] = 0;
            pat[l] = strtol(lego, (char **)NULL, 10);
            l++;
            m = 0;
            memset(lego, 0, sizeof(lego));
        } else if (c >= '0' && c <= '9') {
            lego[m] = c;
            m++;
        }

    }
    unsigned int kpat[l];

    while (true) {
        c = getchar();
        if (c == EOF){
            if (strtol(lego, (char **)NULL, 10) != 0) {
                lego[m] = 0;
                text[p] = strtol(lego, (char **) NULL, 10);
                //printf("%d\n", text[p]);
                p++;
                k++;
                memset(lego, 0, sizeof(lego));
                NumsInStr[string] = k;
            }
            break;
        } else if (c == '\n') {
            lego[m] = 0;
            if (strtol(lego, (char **)NULL, 10) != 0) {
                text[p] = strtol(lego, (char **) NULL, 10);
                //printf("number %d\n", text[p]);
                p++;
                k++;
                NumsInStr[string] = k;
                string++;
                k = 0;
            }
            m = 0;
            memset(lego, 0, sizeof(lego));
        } else if (c == ' '){
            lego[m] = 0;
            if (strtol(lego, (char **)NULL, 10) != 0) {
                text[p] = strtol(lego, (char **) NULL, 10);
                //printf("number %d\n", text[p]);
                p++;
                k++;
            }
            m = 0;
            memset(lego, 0, sizeof(lego));
        } else if (c >= '0' && c <= '9') {
            lego[m] = c;
            //printf("got simbol %c\n", lego[m]);
            m++;
        }

    }

    unsigned int ptext[p];

    for(int i = 0; i < l; i++ ){
        kpat[i] = pat[i];
    }

    for(int i = 0; i < p; i++ ){
        ptext[i] = text[i];
    }
    AG(kpat, l, ptext, p, NumsInStr, string);
}
