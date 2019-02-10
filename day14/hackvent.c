#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include </usr/local/include/gmp.h>
 
int main(int argc, char *argv[])
{
    
    if (argc == 1) {
    	return 0;
    }
    
    mpz_t n, d, e, pt, ct;
 
    mpz_init(pt);
    mpz_init(ct);
    mpz_init_set_str(n, "F66EB887F2B8A620FD03C7D0633791CB4804739CE7FE001C81E6E02783737CA21DB2A0D8AF2D10B200006D10737A0872C667AD142F90407132EFABF8E5D6BD51", 16);
    mpz_init_set_str(e, "65537", 10);
    //mpz_init_set_str(d, "E3463615DB7B046B5CF7F79592D90172DA1D6D37426D9160D56B4AB846E12CA544C86BE53CEC00AE04AE43BA03E0ADEB24E06329E0E6F77F0187DCC2CFE2C049", 16);
 
    //const char *plaintext = "HV17-5BMu-mgD0-G7Su-EYsp-Mg0b";
    
    mpz_import(pt, strlen(argv[1]), 1, 1, 0, 0, argv[1]);
 
    if (mpz_cmp(pt, n) > 0)
        abort();
 
    mpz_powm(ct, pt, e, n);
    gmp_printf("Crypted: %ZX\n", ct);
    //gmp_printf("Crypted: %Zd\n", ct);
 
    //mpz_powm(pt, ct, d, n);
    //gmp_printf("Decoded:   %Zd\n", pt);
 
    //char buffer[64];
    //mpz_export(buffer, NULL, 1, 1, 0, 0, pt);
    //printf("As String: %s\n", buffer);
 
    //mpz_clears(pt, ct, n, e, d, NULL);
    mpz_clears(pt, ct, n, e, NULL);
    return 0;
}