#include "mygrep.h"

#include <stdlib.h>

int main(int argc, char **argv) {
  if (argc == 1) {
    illegal_opt();
    return 2;
  }
  vars v = {REG_NEWLINE | REG_NOSUB, 0, 0, NULL, 0, "", 0, 1, 1, 0};
  read_options(argc, argv, &v);
  for (int idx = optind; idx < argc; idx++)
    f_proc(argv[idx], do_search, &v);
  free_patterns(&v);
  return 0;
}

void f_proc(char *f_name, void(process)(FILE *, vars *), vars *v) {
  FILE *f = NULL;
  ushort i = 0;

  for (; (v->f_name[i] = *f_name); i++, f_name++) {
  }

  if ((f = fopen(v->f_name, "r")) == NULL) {
    if (~v->opt_flags & F_s || v->opt_flags & F_f)
      no_file_or_dir(v->f_name);
  } else {
    process(f, v);
    fclose(f);
  }
}

void do_search(FILE *f, vars *v) {
  v->lines = 1;
  v->matches = 0;
  f_read_by_line(f, print_result, v);
  if (v->opt_flags & F_c)
    print_count(v);
}

int print_result(char *line, vars *v) {
  regex_t regex;
  v->skip = 1;
  v->free = 1;
  for (int i = 0; i < v->patc; i++) {
    int reti = regcomp(&regex, v->patv[i], v->regx_flags);
    reti = regexec(&regex, line, 0, NULL, 0);
    if (!reti) {
      v->matches += 1;
      v->skip = ~v->opt_flags & F_v ? 0 : 1;
      v->free = 1;
      break;
    } else {
      regfree(&regex);
      if (v->opt_flags & F_v)
        v->skip = 0;
      v->free = 0;
    }
  }
  if (~v->opt_flags & F_l && ~v->opt_flags & F_c && !v->skip)
    print_str(line, &regex, v);
  if (v->free)
    regfree(&regex);
  if (!v->skip && v->opt_flags & F_l && ~v->opt_flags & F_c)
    printf("%s\n", v->f_name);
  v->lines += 1;
  return !v->skip && v->opt_flags & F_l ? 1 : 0;
}

void print_str(char *line, regex_t *regex, vars *v) {
  if (v->opt_flags & F_o) {
    regmatch_t matches[32];
    while (!regexec(regex, line, 32, matches, 0)) {
      if (~v->opt_flags & F_h && v->show_name)
        printf("%s:", v->f_name);
      if (v->opt_flags & F_n)
        printf("%zu:", v->lines);
      while (matches[0].rm_so < matches[0].rm_eo) {
        putchar(*(line + (matches[0].rm_so++)));
      }
      if (matches[0].rm_so != -1)
        putchar('\n');
      line += matches[0].rm_eo;
    }
  } else {
    if (~v->opt_flags & F_h && v->show_name)
      printf("%s:", v->f_name);
    if (v->opt_flags & F_n)
      printf("%zu:", v->lines);
    printf("%s\n", line);
  }
}

void print_count(vars *v) {
  if (~v->opt_flags & F_l) {
    if (~v->opt_flags & F_h && v->show_name)
      printf("%s:", v->f_name);
    printf("%zu\n",
           (v->opt_flags & F_v) ? v->lines - 1 - v->matches : v->matches);
  }
  if (v->opt_flags & F_l && !v->skip)
    printf("%s\n", v->f_name);
}

void f_read_by_line(FILE *f, int(fu)(char *, vars *), vars *v) {
  size_t line_c = 0;
  char *line = NULL, read = 1;
  while (read == 1) {
    int c = fgetc(f);
    if (line_c == 0) {
      line = (char *)malloc(1);
    } else {
      char *temp = (char *)realloc(line, line_c + 1);
      if (temp != NULL)
        line = temp;
      else
        break;
    }
    line[line_c++] = c;
    if (c == '\n' || c == EOF) {
      if (c == EOF && line_c == 1) {
        read = 0;
      } else {
        line[line_c - 1] = '\0';
        if (fu(line, v))
          read = 0;
      }
      line_c = 0;
      free(line);
    }
  }
}

void add_patterns(FILE *f, vars *v) { f_read_by_line(f, add_pattern, v); }

int add_pattern(char *pattern, vars *v) {
  int c = 0;
  v->patv = (char **)realloc(v->patv, (v->patc + 1) * sizeof(char *));
  v->patv[v->patc] = (char *)malloc(1);
  for (; *pattern; pattern++) {
    v->patv[v->patc] = (char *)realloc(v->patv[v->patc], c + 1);
    v->patv[v->patc][c++] = *pattern;
  }
  v->patv[v->patc] = (char *)realloc(v->patv[v->patc], c + 1);
  v->patv[v->patc][c] = '\0';
  v->patc++;
  return 0;
}

void free_patterns(vars *v) {
  for (ushort i = 0; i < v->patc; i++)
    if (v->patv[i])
      free(v->patv[i]);
  if (v->patv)
    free(v->patv);
}

void read_options(int argc, char **argv, vars *v) {
  int opt = 0;
  while ((opt = getopt_long(argc, argv, OPTS, NULL, NULL)) != -1) {
    switch (opt) {
    case 'v':
      v->opt_flags |= F_v;
      break;
    case 'c':
      v->opt_flags |= F_c;
      break;
    case 'l':
      v->opt_flags |= F_l;
      break;
    case 'n':
      v->opt_flags |= F_n;
      break;
    case 'h':
      v->opt_flags |= F_h;
      break;
    case 's':
      v->opt_flags |= F_s;
      break;
    case 'o':
      v->opt_flags |= F_o;
      break;
    case 'i':
      v->opt_flags |= F_i;
      v->regx_flags |= REG_ICASE;
      break;
    case 'e':
      v->opt_flags |= F_e;
      add_pattern(optarg, v);
      break;
    case 'f':
      v->opt_flags |= F_f;
      f_proc(optarg, add_patterns, v);
      break;
    default:
      free_patterns(v);
      illegal_opt();
      break;
    }
  }
  if (~v->opt_flags & F_e && ~v->opt_flags & F_f) {
    add_pattern(argv[optind], v);
    optind++;
  }
  if (v->patv == NULL)
    exit(2);
  if (v->opt_flags & F_o)
    v->regx_flags ^= REG_NOSUB;
  if (argc - optind > 1)
    v->show_name = 1;
}
