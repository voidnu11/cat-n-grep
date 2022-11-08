#include "mycat.h"

int main(int argc, char **argv) {
  vars v = {0, 1, 0, 0};
  set_opt_flags(argc, argv, &v);
  for (int idx = optind; idx < argc; idx++) {
    FILE *f = NULL;
    if ((f = fopen(argv[idx], "r")) == NULL)
      no_file_or_dir(argv[idx]);
    else
      file_processing(f, &v);
  }
  return 0;
}

void set_opt_flags(int argc, char **argv, vars *v) {
  int opt;
  while ((opt = getopt_long(argc, argv, OPTS, long_opts, NULL)) != -1) {
    switch (opt) {
    case 'b':
      v->opt_flags |= F_b;
      break;
    case 'e':
      v->opt_flags |= F_e | F_v;
      break;
    case 'n':
      v->opt_flags |= F_n;
      break;
    case 's':
      v->opt_flags |= F_s;
      break;
    case 't':
      v->opt_flags |= F_t | F_v;
      break;
    case 'v':
      v->opt_flags |= F_v;
      break;
    case 'E':
      v->opt_flags |= F_e;
      break;
    case 'T':
      v->opt_flags |= F_t;
      break;
    default:
      illegal_opt();
      break;
    }
  }
  if (v->opt_flags & F_b && v->opt_flags & F_n)
    v->opt_flags ^= F_n;
}

void file_processing(FILE *f, vars *v) {
  int c;
  while ((c = fgetc(f)) != EOF) {
    if (v->line_c == 0) {
      if (c == '\n' && v->opt_flags & F_s) {
        if (v->skip)
          continue;
        else
          v->skip = 1;
      }
      if ((v->opt_flags & F_b && c != '\n') || v->opt_flags & F_n)
        print_line_number(&v->line);
    }
    v->line_c++;
    if (c == '\n') {
      if (v->line_c > 1)
        v->skip = 0;
      if (v->opt_flags & F_e)
        fputc('$', stdout);
      v->line_c = 0;
    }
    if (v->opt_flags & F_v)
      do_v_flag(&c, v);
    else if (v->opt_flags & F_t && c == 9)
      print_ctrl_char(c);
    else
      fputc(c, stdout);
  }
  fclose(f);
}

void do_v_flag(int *c, vars *v) {
  if (*c >= 128) {
    *c -= 128;
    fprintf(stdout, "M-");
    if (*c < 32)
      print_ctrl_char(*c);
  } else if (*c < 32 &&
             ((*c != 9 && *c != 10) || (v->opt_flags & F_t && *c == 9))) {
    print_ctrl_char(*c);
  } else {
    fputc(*c, stdout);
  }
}
