#include <stdio.h>
#include <gmp.h>

#define NUMBER_COUNT 100000

void PrintFactorization(mpz_t moduli, mpz_t prime, int numberNum)
{
  mpz_t quotient;
  mpz_init(quotient);
  mpz_div(quotient, moduli, prime);
  printf("Number #%d: ", numberNum);
  mpz_out_str(stdout, 10, moduli);
  printf(" = ");
  mpz_out_str(stdout, 10, prime);
  printf(" * ");
  mpz_out_str(stdout, 10, quotient);
  printf("\n");
}

void tryFactorizeWithDivisor(mpz_t *considered, mpz_t primeDivisor)
{
  for (int i = 0; i < NUMBER_COUNT; i++)
  {
    mpz_t reminder;
    mpz_init(reminder);
    mpz_mod(reminder, considered[i], primeDivisor);
    if (mpz_cmp_ui(reminder, 0) != 0)
    {
      continue;
    }
    else
    {
      PrintFactorization(considered[i], primeDivisor, i);
      break;
    }
  }
}

void tryFactorizeNoncoprimes(mpz_t *considered, mpz_t n, int numberNum)
{
  for (int i = 0; i < numberNum; i++)
  {
    mpz_t gcd;
    mpz_init(gcd);
    mpz_gcd(gcd, considered[i], n);
    if (mpz_cmp_ui(gcd, 1) > 0)
    {
      PrintFactorization(considered[i], gcd, i);
      PrintFactorization(n, gcd, numberNum);
      break;
    }
  }
}

void factorizeWithConsideredProductsGcd(mpz_t n, mpz_t gcdWithProduct, mpz_t *considered, int numberNum)
{
  mpz_t commonPrimeDiv, primeDivisor;
  mpz_init_set_ui(commonPrimeDiv, 1);
  mpz_init(primeDivisor);
  if (mpz_cmp(gcdWithProduct, n) < 0)
  {
    mpz_set(primeDivisor, gcdWithProduct);
    tryFactorizeWithDivisor(considered, primeDivisor);
    PrintFactorization(n, gcdWithProduct, numberNum);
    return;
  }
  else
  {
    tryFactorizeNoncoprimes(considered, n, numberNum);
  }
}

int main(int argc, char const *argv[])
{
  FILE *fptr;
  char buffer[1024];
  int numberNum = 0;

  mpz_t tmp, product, gcd;
  mpz_init(tmp);
  mpz_init(gcd);
  mpz_init(product);
  mpz_set_ui(product, 1);

  mpz_t considered[NUMBER_COUNT];

  fptr = fopen("file.txt", "r");
  while (fscanf(fptr, "%1023s", buffer) != EOF)
  {
    mpz_set_str(tmp, buffer, 16);
    mpz_init_set(considered[numberNum++], tmp);

    mpz_gcd(gcd, tmp, product);
    if (mpz_cmp_ui(gcd, 1) > 0)
    {
      factorizeWithConsideredProductsGcd(tmp, gcd, considered, numberNum);
      break;
    }
    else
    {
      mpz_mul(product, product, tmp);
    }
  }
  fclose(fptr);
  return 0;
}