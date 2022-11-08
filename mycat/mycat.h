#ifndef SRC_CAT_MYCAT_H_
#define SRC_CAT_MYCAT_H_

#include <getopt.h>
#include <stdio.h>

#define OPTS "benstvET"

#define print_ctrl_char(x) fprintf(stdout, "^%c", x + 64)

#define illegal_opt() printf("usage: mycat [-%s] [file...]\n", OPTS)

#define no_file_or_dir(x) printf("mycat: %s: No such file or directory\n", x)

typedef unsigned char uchar;
typedef unsigned long ulong;

typedef struct var {
  char opt_flags;
  int line, skip, line_c;
} vars;

void set_opt_flags(int argc, char **argv, vars *v);
void file_processing(FILE *f, vars *v);
void do_v_flag(int *c, vars *v);
void print_line_number(int *n) { fprintf(stdout, "%6d\t", (*n)++); }

static struct option long_opts[] = {{"number-nonblank", no_argument, 0, 'b'},
                                    {"number", no_argument, 0, 'n'},
                                    {"squeeze-blank", no_argument, 0, 's'}};

enum Opt_flags {
  F_b = 1 << 0, // --number-nonblank
  F_e = 1 << 1, // -e -E
  F_n = 1 << 2, // --number
  F_s = 1 << 3, // --squeeze-blank
  F_t = 1 << 4, // -t -T
  F_v = 1 << 5, // -E -T
};

#endif // SRC_CAT_MYCAT_H_
