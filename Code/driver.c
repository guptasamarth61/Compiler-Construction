/***************************************
                |GROUP-09|
  Aditya Upadhyay      -   2017A7PS0083P
  Harpider Jot Singh   -   2017A7PS0057P
  Jaladi Lakshmi Teja  -   2017A7PS0068P
  Vishal Mittal        -   2017A7PS0080P
  Yash Vijay           -   2017A7PS0072P
*****************************************/

#include "driver.h"
#include "ast.h"
#include "lexer.h"
#include "parser.h"
#include "treeADT.h"
#include "semantic_analyzer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/**
 * @brief Prints menu options
 *
 */
void print_menu() {
  printf("\t\t MENU\n");
  printf("-----------------------------------------------------------------\n");
  printf("1. Remove comments from the source code\n");
  printf("2. Print tokens generated by lexer\n");
  printf("3. Print parse tree for the program\n");
  printf("4. Print time taken for tokenizing and parsing\n");
  printf("5. Print first sets\n");
  printf("6. Print follow sets\n");
  printf("7. Construct Symbol table\n");
  printf(" Press any other to exit \n");
  printf("-----------------------------------------------------------------\n");
  printf("Enter your choice:  ");
}

/**
 * @brief Read tokens from the file and make a mapping array which maps
 * enumerated tokens to strings
 *
 */
void populate_terminal_string() {

  FILE *file = fopen("tokens.txt", "r");
  fseek(file, 0, SEEK_END);
  int length = ftell(file);
  fseek(file, 0, SEEK_SET);

  char *t_file = malloc(sizeof(char) * (length + 1));
  if (t_file == NULL) {
    perror("terminal_string filling failed\n");
    exit(1);
  }

  fread(t_file, sizeof(char), length, file);
  t_file[length] = '\0';
  fclose(file);

  char *tk_read = NULL;
  int i;
  tk_read = strtok(t_file, ", \n");

  for (i = 0; tk_read != NULL; i++) {
    strcpy(terminal_string[i], tk_read);
    tk_read = strtok(NULL, ", \n");
  }

  free(t_file);
}

/**
 * @brief inserts all non-terminal strings in an array, which is used for
 * mapping enumerated non-terminals to corresponding string values
 *
 */
void populate_non_terminal_string() {
  FILE *file = fopen("non_terminals.txt", "r");

  fseek(file, 0, SEEK_END);
  int length = ftell(file);
  fseek(file, 0, SEEK_SET);

  char *nt_file = malloc(sizeof(char) * (length + 1));

  if (nt_file == NULL) {
    perror(ANSI_COLOR_RED "Parser init failed \n" ANSI_COLOR_RESET);
    exit(1);
  }

  fread(nt_file, sizeof(char), length, file);
  nt_file[length] = '\0';
  fclose(file);

  char *nt_read = NULL;
  int i;
  nt_read = strtok(nt_file, ", \n");

  for (i = 0; nt_read != NULL; i++) {
    strcpy(non_terminal_string[i], nt_read);
    nt_read = strtok(NULL, ", \n");
  }
  free(nt_file);
}

int main(int argc, char *argv[]) {

  if (argc < 3) {
    printf(ANSI_COLOR_RED
           "\n\nSyntax to run program: " ANSI_COLOR_RESET ANSI_COLOR_YELLOW
           "make file=<test_case_file> "
           "parse_tree_file=<output_file>\n\n" ANSI_COLOR_RESET);
    exit(1);
  }

  setvbuf(stdout, NULL, _IONBF, 0);

  populate_non_terminal_string();
  populate_terminal_string();

  int choice;
  char source_file[100];
  strcpy(source_file, argv[1]);
  strcpy(parse_tree_file, argv[2]);

  FILE *source = fopen(source_file, "r");
  if (source == NULL) {
    printf("Error opening file\n");
  }

  while (true) {
    print_menu();
    scanf("%d", &choice);
    printf("\n");

    switch (choice) {
    case 1: {
      lexer_init(source);
      char no_comment_file[100];
      printf("Enter name of the output file which will have the source code "
             "without comments\n");
      scanf("%s", no_comment_file);
      remove_comments(source, no_comment_file);
    } break;

    case 2: {
      lexer_init(source);
      tokenize_source_file(source);
    } break;

    case 3: {
      lexer_init(source);
      parser_init();

      FILE *fptr = fopen("grammar.txt", "r");
      if (fptr == NULL) {
        perror("fopen");
      }
      grammar_fill(fptr);

      populate_first_sets();

      populate_follow_sets();

      create_parse_table();
      // print_parse_table();

      tree_node *ptr = parse_input_source_code(source);

      if (ptr == NULL) {
        printf("Empty parse tree\n");
      }

      parse_tree_file_ptr = fopen(parse_tree_file, "w");
      // print_parse_tree(ptr);

      tree_node *ast_tree = construct_ast(ptr);
      // printf("\n*****AST******\n");
      // print_ast(ast_tree);
      print_parse_tree_for_tool(ast_tree);

      fclose(parse_tree_file_ptr);
      free_grammar();
      fclose(fptr);

    } break;
    case 4: {
      clock_t start_time, end_time;

      double total_CPU_time, total_CPU_time_in_seconds;

      start_time = clock();

      lexer_init(source);
      parser_init();

      FILE *fptr = fopen("grammar.txt", "r");
      if (fptr == NULL) {
        perror("fopen");
      }
      grammar_fill(fptr);

      populate_first_sets();

      populate_follow_sets();

      create_parse_table();

      tree_node *ptr = parse_input_source_code(source);

      fclose(fptr);

      end_time = clock();

      total_CPU_time = (double)(end_time - start_time);

      total_CPU_time_in_seconds = total_CPU_time / CLOCKS_PER_SEC;

      printf("Total CPU TIME taken (in microseconds):" ANSI_COLOR_GREEN
             " %lf usecs" ANSI_COLOR_RESET
             "\nTotal CPU time (in seconds)\t      : " ANSI_COLOR_GREEN
             "%lf secs\n" ANSI_COLOR_RESET,
             total_CPU_time, total_CPU_time_in_seconds);

    } break;
    case 5: {
      lexer_init(source);
      parser_init();

      FILE *fptr = fopen("grammar.txt", "r");
      if (fptr == NULL) {
        perror("fopen");
      }
      grammar_fill(fptr);

      populate_first_sets();

      print_first_sets();

      fclose(fptr);
    } break;
    case 6: {
      lexer_init(source);
      parser_init();

      FILE *fptr = fopen("grammar.txt", "r");
      if (fptr == NULL) {
        perror("fopen");
      }
      grammar_fill(fptr);

      populate_first_sets();

      populate_follow_sets();
      print_follow_sets();

      fclose(fptr);
      break;
    }
    case 7: {
      lexer_init(source);
      parser_init();

      FILE *fptr = fopen("grammar.txt", "r");
      if (fptr == NULL) {
        perror("fopen");
      }
      grammar_fill(fptr);

      populate_first_sets();

      populate_follow_sets();

      create_parse_table();
      print_parse_table();

      tree_node *ptr = parse_input_source_code(source);

      if (ptr == NULL) {
        printf("Empty parse tree\n");
      }

      tree_node *ast_tree = construct_ast(ptr);
      construct_symtable(ast_tree);
      parse_tree_file_ptr = fopen(parse_tree_file, "w");
      // print_parse_tree_for_tool(ptr);
      fclose(parse_tree_file_ptr);

      free_grammar();
      fclose(fptr);

    } break;
    default: {
      exit(0);
    } break;
    } // end of switch
  }
  fclose(source);
} // end of main
