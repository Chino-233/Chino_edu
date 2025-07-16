int bitXor(int x,int y){
    return ~(x&~y) & ~(~x&y);
  }
  
  int tmin(void){
    return 1<<31;
  }
  
  int isTmax(int x){
    return !((x+1)^(~x)) & !!(x+1);
  }
  
  int allOddBits(int x){
    int mask = 0xAA;
    mask = mask | (mask << 8);
    mask = mask | (mask << 16);
    return !((x & mask) ^ mask);
  }
  
  int negate(int x){
    return ~x+1;
  }
  
  int isAsciiDigit(int x){
    int lowerBound = x + (~0x30 + 1);
    int upperBound = 0x39 + (~x + 1);
    return !(lowerBound >> 31) & !(upperBound >> 31);
  }
  
  int conditional(int x,int y,int z){
    int mask = ~(!x) + 1;
    return (mask & y) | (~mask & z);
  }
  
  int isLessOrEqual(int x,int y){
    int signX = x >> 31;
    int signY = y >> 31;
    return (signX & !signY) | (!(signX ^ signY) & ((x + ~y) >> 31 & 1)) | !(x ^ y);
  }
  
  int logicalNeg(int x){
    return ((x|(~x+1))>>31)+1;
  }
  
  int howManyBits(int x){
    int sign = x >> 31;
    x = (sign & ~x) | (~sign & x);
    int bit16 = !!(x >> 16) << 4;
    x = x >> bit16;
    int bit8 = !!(x >> 8) << 3;
    x = x >> bit8;
    int bit4 = !!(x >> 4) << 2;
    x = x >> bit4;
    int bit2 = !!(x >> 2) << 1;
    x = x >> bit2;
    int bit1 = !!(x >> 1);
    x = x >> bit1;
    return bit16 + bit8 + bit4 + bit2 + bit1 + 1;
  }
  
  unsigned float_twice(unsigned uf){
    unsigned sign = uf & 0x80000000;
    unsigned exp = (uf & 0x7F800000) >> 23;
    unsigned frac = uf & 0x007FFFFF;
    
    if (exp == 0xFF) {
        return uf;
    }
    
    if (exp == 0) {
        frac <<= 1;
        return sign | frac;
    }
    
    exp++;
    
    if (exp == 0xFF) {
        return sign | 0x7F800000;
    }
    
    return sign | (exp << 23) | frac;
  }
  
  unsigned float_i2f(int x){
    unsigned sign = 0;
    if (x < 0) {
        sign = 0x80000000;
        x = -x;
    }
    
    if (x == 0) {
        return sign;
    }
    
    int exp = 31;
    while ((x & (1 << exp)) == 0) {
        exp--;
    }
    
    int shift = exp - 23;
    unsigned frac;
    
    if (shift > 0) {
        frac = (x >> (shift)) & 0x007FFFFF;
    } else {
        frac = (x << (-shift)) & 0x007FFFFF;
    }
    
    exp += 127;
    if (frac & (1 << 23)) {
        frac >>= 1;
        exp++;
    }
    
    if (exp >= 255) {
        return sign | 0x7F800000;
    }
    
    return sign | (exp << 23) | frac;
  }
  
  int float_f2i(unsigned uf){
    unsigned sign = uf & 0x80000000;
    unsigned exp = (uf & 0x7F800000) >> 23;
    unsigned frac = uf & 0x007FFFFF;
    
    if (exp == 0xFF) {
        return 0x80000000u;
    }
    
    if (exp < 127) {
        return 0;
    }
    
    int e = exp - 127;
    if (e >= 31) {
        return 0x80000000u;
    }
    
    frac |= 0x00800000;
    if (e > 23) {
        frac <<= (e - 23);
    } else {
        frac >>= (23 - e);
    }
    
    if (sign) {
        return ~frac + 1;
    } else {
        return frac;
    }
  }