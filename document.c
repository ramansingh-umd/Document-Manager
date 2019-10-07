/*
Ramandeep Singh
115240766
rsingh12
*/

#include <stdio.h>
#include <string.h>
#include "document.h"

/* Creates the document */
int init_document(Document *doc, const char *name) {
  
  if (doc == NULL || name == NULL || strlen(name) > MAX_STR_SIZE) {
    return FAILURE;
  }

  doc->number_of_paragraphs = 0;
  strcpy(doc->name, name);

  return SUCCESS;
}

/* Resets the document back to the start of creation */
int reset_document(Document *doc) {

  if (doc == NULL) {
    return FAILURE;
  }

  doc->number_of_paragraphs = 0;

  return SUCCESS;
}

/* Prints out every paragraph with every line */
int print_document(Document *doc) {

  int curr_paragraph, curr_line, num_paragraphs, num_lines;

  if (doc == NULL) {
    return FAILURE;
  }
  
  printf("Document name: \"%s\"\n", doc->name);
  printf("Number of Paragraphs: %d\n", doc->number_of_paragraphs);

  num_paragraphs = doc->number_of_paragraphs;
  
  for (curr_paragraph = 0; curr_paragraph < num_paragraphs; curr_paragraph++) {

    num_lines = doc->paragraphs[curr_paragraph].number_of_lines;
    
    for (curr_line = 0; curr_line < num_lines; curr_line++) {
      
      printf("%s\n", doc->paragraphs[curr_paragraph].lines[curr_line]);
    }
    
    printf("\n");
  }

  return SUCCESS;
}

/* Adds a new paragraph */
int add_paragraph_after(Document *doc, int paragraph_number) {

  int i;
  
  if (doc == NULL || doc->number_of_paragraphs == MAX_PARAGRAPHS
      || paragraph_number > doc->number_of_paragraphs) {
    return FAILURE;
  }

  /* Shifts paragraphs if needed */
  for (i = doc->number_of_paragraphs; i > paragraph_number; i--) {
    doc->paragraphs[i] = doc->paragraphs[i-1];
  }

  doc->paragraphs[i].number_of_lines = 0;
  
  doc->number_of_paragraphs++;

  return SUCCESS;
}

/* Adds a new line to the designated paragraph */
int add_line_after(Document *doc, int paragraph_number,
		   int line_number, const char *new_line) {

  Paragraph *ptr;
  int i;
  
  if (doc == NULL || paragraph_number > doc->number_of_paragraphs ||
      doc->paragraphs[paragraph_number - 1].number_of_lines
      == MAX_PARAGRAPH_LINES ||
      line_number > doc->paragraphs[paragraph_number - 1].number_of_lines
      || new_line == NULL) {
    return FAILURE;
  }

  ptr = &(doc->paragraphs[paragraph_number - 1]);

  /* Shifts any lines if needed */
  for (i = ptr->number_of_lines; i > line_number; i--) {
    strcpy(ptr->lines[i], ptr->lines[i - 1]);
  }

  strcpy(ptr->lines[line_number], new_line);

  ptr->number_of_lines++;
  
  return SUCCESS;
}

/* Returns how many lines are in the designated paragraph */
int get_number_lines_paragraph(Document *doc, int paragraph_number,
			       int *number_of_lines) {

  Paragraph *ptr;
  
  if (doc == NULL || number_of_lines == NULL
      || paragraph_number > doc->number_of_paragraphs) {
    return FAILURE;
  }

  ptr = &(doc->paragraphs[paragraph_number - 1]);
  
  *number_of_lines = ptr->number_of_lines;
  
  return SUCCESS;
}

/* Adds a line to the end of the designated paragraph */
int append_line(Document *doc, int paragraph_number, const char *new_line) {

  Paragraph *ptr;
  
  if (doc == NULL || paragraph_number > doc->number_of_paragraphs
      || new_line == NULL) {
    return FAILURE;
  }

  ptr = &(doc->paragraphs[paragraph_number - 1]);
  
  if (ptr->number_of_lines == MAX_PARAGRAPH_LINES) {
    return FAILURE;
  }

  strcpy(ptr->lines[ptr->number_of_lines], new_line);

  ptr->number_of_lines++;
  
  return SUCCESS;
}

/* Removes the designated line from the paragraph */
int remove_line(Document *doc, int paragraph_number, int line_number) {

  int i, number_of_lines;
  Paragraph *ptr;
  
  if (doc == NULL || paragraph_number > doc->number_of_paragraphs
      || line_number > doc->paragraphs[paragraph_number - 1].number_of_lines) {
    return FAILURE;
  }

  ptr = &(doc->paragraphs[paragraph_number - 1]);
  number_of_lines = ptr->number_of_lines;

  /* Shifts lines if needed */
  for (i = line_number - 1; i < number_of_lines; i++) {
    strcpy(ptr->lines[i], ptr->lines[i + 1]);
  }

  ptr->number_of_lines--;
  
  return SUCCESS;
}

/* Loads into a new or existing document and adds paragraphs
to the beginning */
int load_document(Document *doc, char data[][MAX_STR_SIZE + 1],
		  int data_lines) {

  int i;
  int paragraph_number = 0;

  if (doc == NULL || data == NULL || data_lines == 0) {
    return FAILURE;
  }

  add_paragraph_after(doc, paragraph_number);

  /* Goes through the data array and adds paragraphs and lines */
  for (i = 0; i < data_lines; i++) {
    
    if (data[i][0] == '\0') {
      paragraph_number++;
      add_paragraph_after(doc, paragraph_number);
    }

    else {
      append_line(doc, paragraph_number + 1, data[i]);
    }
  }

  return SUCCESS;
}

/* Replaces all occurrences of target with replacement */
int replace_text(Document *doc, const char *target, const char *replacement) {

  int curr_paragraph, curr_line, curr_index,
    num_chars_to_copy, num_paragraphs, num_lines;
  char *curr_spot, *prev_spot;
  char updated_line[MAX_STR_SIZE + 1];
  
  if (doc == NULL || target == NULL || replacement == NULL) {
    return FAILURE;
  }

  num_paragraphs = doc->number_of_paragraphs;

  /* Go through each line in each paragraph */
  for (curr_paragraph = 0; curr_paragraph < num_paragraphs; curr_paragraph++) {

    num_lines = doc->paragraphs[curr_paragraph].number_of_lines;
    
    for (curr_line = 0; curr_line < num_lines; curr_line++) {

      curr_index = 0;
      curr_spot = doc->paragraphs[curr_paragraph].lines[curr_line];
      prev_spot = curr_spot;
      updated_line[0] = '\0';

      /* Keep going while instances of target are found */
      while ((curr_spot = strstr(curr_spot, target)) != NULL) {

	num_chars_to_copy = curr_spot - prev_spot;
	curr_index += num_chars_to_copy;

	*(prev_spot + num_chars_to_copy) = '\0';

	strcat(updated_line, prev_spot);
	strcat(updated_line, replacement);

	curr_index += strlen(replacement);
	
	strcat(updated_line, curr_spot + strlen(target));
	
	curr_spot = curr_spot + strlen(target);
	prev_spot = curr_spot;

	updated_line[curr_index] = '\0';
      }

      strcat(updated_line, prev_spot);

      strcpy(doc->paragraphs[curr_paragraph].lines[curr_line], updated_line);
    }
  }

  return SUCCESS;
}

/* Surrounds all instances of target with [ ] */
int highlight_text(Document *doc, const char *target) {

  char replacement[MAX_STR_SIZE + 1] = {0};
  
  if (doc == NULL || target == NULL) {
    return FAILURE;
  }
  
  strcat(replacement, HIGHLIGHT_START_STR);
  strcat(replacement, target);
  strcat(replacement, HIGHLIGHT_END_STR);
  
  replace_text(doc, target, replacement);

  return SUCCESS;
}

/* Completely remove all instances of target */
int remove_text(Document *doc, const char *target) {

  if (doc == NULL || target == NULL) {
    return FAILURE;
  }

  replace_text(doc, target, "");

  return SUCCESS;
}
