#include <ctype.h>
#include <gmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StackEntry {
  enum { OP, NUM } kind;
  union {
    mpz_t number;
    char op;
  } value;
};

struct Expr {
  struct StackEntry *entries;
  unsigned size;
};

struct Expr parse(char *input) {
  unsigned len = strlen(input);
  struct Expr e = {
      .entries = calloc(128, sizeof(struct StackEntry)),
      .size = 0,
  };

  char buf[256] = {};
  int buf_idx = 0;

  for (int i = 0; i <= len; i++) {
    char c = input[i];
    if (isspace(c) || c == '\0') {
      if (buf_idx > 0) {
        buf[buf_idx] = '\0';
        e.entries[e.size].kind = NUM;
        mpz_init_set_str(e.entries[e.size].value.number, buf, 10);
        e.size++;
        buf_idx = 0;
      }
      continue;
    }
    if (isdigit(c)) {
      buf[buf_idx++] = c;
      continue;
    }
    if (c == '+' || c == '-' || c == '*' || c == '/') {
      e.entries[e.size].kind = OP;
      e.entries[e.size].value.op = c;
      e.size++;
    }
  }

  return e;
}

int main() {
  char input[1024];
  fgets(input, sizeof(input), stdin);

  struct Expr expr = parse(input);

  mpz_t stack[128];
  int sp = 0;

  for (unsigned i = 0; i < expr.size; i++) {
    if (expr.entries[i].kind == NUM) {
      mpz_init_set(stack[sp], expr.entries[i].value.number);
      sp++;
    } else {
      mpz_t res;
      mpz_init(res);
      if (expr.entries[i].value.op == '+')
        mpz_add(res, stack[sp - 2], stack[sp - 1]);
      else if (expr.entries[i].value.op == '-')
        mpz_sub(res, stack[sp - 2], stack[sp - 1]);
      else if (expr.entries[i].value.op == '*')
        mpz_mul(res, stack[sp - 2], stack[sp - 1]);
      else if (expr.entries[i].value.op == '/')
        mpz_div(res, stack[sp - 2], stack[sp - 1]);
      sp -= 2;
      mpz_set(stack[sp], res);
      sp++;
    }
  }

  gmp_printf("%Zd\n", stack[0]);

  for (unsigned i = 0; i < expr.size; i++) {
    if (expr.entries[i].kind == NUM) {
      mpz_clear(expr.entries[i].value.number);
    }
  }
  free(expr.entries);

  for (int i = 0; i < 128; i++) {
    mpz_clear(stack[i]);
  }

  return 0;
}
