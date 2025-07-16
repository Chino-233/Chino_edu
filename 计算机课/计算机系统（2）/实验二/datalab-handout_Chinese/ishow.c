/* 显示定点数的值 */
#include <stdlib.h>
#include <stdio.h>

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
    return 0; /* 这里不应该有浮点数 */
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

void show_int(unsigned uf)
{
  printf("十六进制 = 0x%.8x,\t有符号 = %d,\t无符号 = %u\n",
     uf, (int) uf, uf);
}


void usage(char *fname) {
  printf("用法: %s val1 val2 ...\n", fname);
  printf("值可以以十六进制或十进制给出\n");
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
      show_int(uf);
    } else {
      printf("无法将 '%s' 转换为32位数字\n", sval);
    }
  }
  return 0;
}