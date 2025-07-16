/* 显示浮点数的结构 */

#include <stdio.h>
#include <stdlib.h>
float strtof(const char *nptr, char **endptr);

#define FLOAT_SIZE 32
#define FRAC_SIZE 23
#define EXP_SIZE 8
#define BIAS ((1<<(EXP_SIZE-1))-1)
#define FRAC_MASK ((1<<FRAC_SIZE)-1)
#define EXP_MASK ((1<<EXP_SIZE)-1)

/* 浮点辅助函数 */
unsigned f2u(float f)
{
  union {
    unsigned u;
    float f;
  } v;
  v.u = 0;
  v.f = f;
  return v.u;
}

static float u2f(unsigned u)
{
  union {
    unsigned u;
    float f;
  } v;
  v.u = u;
  return v.f;
}

/* 获取指数 */
unsigned get_exp(unsigned uf)
{
  return (uf>>FRAC_SIZE) & EXP_MASK;
}

/* 获取小数部分 */
unsigned get_frac(unsigned uf)
{
  return uf & FRAC_MASK;
}

/* 获取符号 */
unsigned get_sign(unsigned uf)
{
  return (uf>>(FLOAT_SIZE-1)) & 0x1;
}

void show_float(unsigned uf)
{
  float f = u2f(uf);
  unsigned exp = get_exp(uf);
  unsigned frac = get_frac(uf);
  unsigned sign = get_sign(uf);

  printf("\n浮点值 %.10g\n", f);
  printf("位表示 0x%.8x, 符号 = %x, 指数 = 0x%.2x, 小数 = 0x%.6x\n",
     uf, sign, exp, frac);
  if (exp == EXP_MASK) {
    if (frac == 0) {
      printf("%c无穷大\n", sign ? '-' : '+');
    } else
      printf("非数值(NaN)\n");
  } else {
    int denorm = (exp == 0);
    int uexp = denorm ? 1-BIAS : exp - BIAS;
    int mantissa = denorm ? frac : frac + (1<<FRAC_SIZE);
    float fman = (float) mantissa / (float) (1<<FRAC_SIZE);
    printf("%s. %c%.10f X 2^(%d)\n",
       denorm ? "非规格化" : "规格化",
       sign ? '-' : '+',
       fman, uexp);
  }
}

/* 从字符串中提取十六进制/十进制/或浮点值 */
static int get_num_val(char *sval, unsigned *valp) {
  char *endp;
  /* 看看它是整数还是浮点数 */
  int ishex = 0;
  int isfloat = 0;
  int i;
  for (i = 0; sval[i]; i++) {
    switch (sval[i]) {
    case 'x':
    case 'X':
      ishex = 1;
      break;
    case 'e':
    case 'E':
      if (!ishex)
    isfloat = 1;
      break;
    case '.':
      isfloat = 1;
      break;
    default:
      break;
    }
  }
  if (isfloat) {
    float fval = strtof(sval, &endp);
    if (!*endp) {
      *valp = *(unsigned *) &fval;
      return 1;
    }
    return 0;
  } else {
    long long int llval = strtoll(sval, &endp, 0);
    long long int upperbits = llval >> 31;
    /* 负数将给出-1，正数将给出0或1 */
    if (valp && (upperbits == 0 || upperbits == -1 || upperbits == 1)) {
      *valp = (unsigned) llval;
      return 1;
    }
    return 0;
  }
}


void usage(char *fname) {
  printf("用法: %s val1 val2 ...\n", fname);
  printf("值可以作为十六进制格式或浮点数给出\n");
  exit(0);
}


int main(int argc, char *argv[])
{
  int i;
  unsigned uf;
  if (argc < 2)
    usage(argv[0]);
  for (i = 1; i < argc; i++) {
    char *sval = argv[i];
    if (get_num_val(sval, &uf)) {
      show_float(uf);
    } else {
      printf("无效的32位数字: '%s'\n", sval);
      usage(argv[0]);
    }
  }
  return 0;
}