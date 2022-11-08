#ifndef SRC_GREP_MYGREP_H_
#define SRC_GREP_MYGREP_H_

#define OPTS "ivclnhsf:e:o"

#include <getopt.h>
#include <regex.h>
#include <stdio.h>

#define illegal_opt()                                                          \
  printf("usage: mygrep [%s] [pattern] [file...]\n", OPTS);                  \
  exit(2)

#define no_file_or_dir(x) printf("mygrep: %s: No such file or directory\n", x)

typedef unsigned short ushort;
typedef struct variables {
  int regx_flags;   // regex flags
  ushort opt_flags; // options flags
  ushort patc;      // patterns count
  char **patv;      // patterns strings
  char show_name;
  char f_name[255];
  char skip;
  char free;
  size_t lines;
  size_t matches;
} vars;

void read_options(int argc, char **argv, vars *v);
void f_proc(char *f_name, void(process)(FILE *, vars *), vars *v);

void f_read_by_line(FILE *f, int(fu)(char *, vars *), vars *v);

void print_str(char *line, regex_t *regex, vars *v);
void print_count(vars *v);

void do_search(FILE *f, vars *v);
void add_patterns(FILE *f, vars *v);

int print_result(char *line, vars *v);
int add_pattern(char *pattern, vars *v);

void free_patterns(vars *v);

enum OPT_FLAGS {
  F_e = 1 << 0, // 1    //          1
  F_i = 1 << 1, // 2    //         10
  F_v = 1 << 2, // 4    //        100
  F_c = 1 << 3, // 8    //       1000
  F_l = 1 << 4, // 16   //      10000
  F_n = 1 << 5, // 32   //     100000
  F_h = 1 << 6, // 64   //    1000000
  F_s = 1 << 7, // 128  //   10000000
  F_o = 1 << 8, // 256  //  100000000
  F_f = 1 << 9, // 512  // 1000000000
};

#endif // SRC_GREP_MYGREP_H_
