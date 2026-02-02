// Program Name: Movies
// Author: Kenneth Cunningham
// This program will implement movie management functionality.
// Used to learn C concepts such as pointers, structures, and linked lists

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define max_langs 5 

/**/
struct movie {
  char *title;
  int year;
  char *languages[max_langs];
  int  langCount;
  double rating;
  struct movie *next;
};

/**/
struct movie *parseMovie(char *line);                      //                     
void freeMovies(struct movie *head);                       //
void showMenu(void);
void moviesByYear(struct movie *head, int year);
void highestRatedByYear(struct movie *head);
void moviesByLanguage(struct movie *head, char *language);


int main(int argc, char *argv[]) {
  if (argc < 2) {                                          //
    printf("You must provide a file name\n");
    return 1;
  }

  FILE *file = fopen(argv[1], "r");                        //
  if (!file) {
    perror("File failed to open");
    return 1;
  }

  char buffer[1024];                                      //
  struct movie *head = NULL;
  struct movie *tail = NULL;
  int count = 0;


  /*Reads movies from file*/
  fgets(buffer, sizeof(buffer), file);                    //Skips header

  while (fgets(buffer, sizeof(buffer), file)) {           //
    struct movie *m = parseMovie(buffer);
    if (!head)
      head = tail = m;
    else {
      tail -> next = m;
      tail = m;
    }
    count++;
  }

  fclose(file);
  printf("Processed file %s and parsed data for %d movies\n\n", argv[1], count);

  int choice;
  do {                                                                            //Prompt for menu options
    showMenu();
    printf("\nEnter a choice from 1 to 4: ");
    scanf("%d", &choice);

    if (choice == 1) {                                                            //  
      int year;
      printf("Enter the year for which you want to see movies: ");
      scanf("%d", &year);
      moviesByYear(head, year);
    }
    else if (choice == 2) {                                                       //
      highestRatedByYear(head);
    }
    else if (choice == 3) {                                                       //
      char lang[20];
      printf("Enter the language for which you want to see movies: ");
      scanf("%s", lang);
      moviesByLanguage(head, lang);
    }
    else if (choice !== 4) {                                                      //
      printf("You entered an invalid choice. Try again. \n");
    }
    
  } while (choice != 4);                                                          //

  freeMovies(head);
  return 0;
}



/**/
void showMenu(void) {
  printf("\n1. Show movies released in the specified year\n");
  printf("2. Show highest rated movie for each year\n");
  printf("3. Show the title and year of release of all movies in a specific language\n");
  printf("4. Exit from the program\n"); 
}

struct movie *parseMovie(char *line) {
  struct movie *m = malloc(sizeof(struct movie));
  m -> next = NULL;

  char *saveptr;
  char *token = strtok_r(line, ", ", &saveptr);

  m -> title = strdup(token);
  m -> year = atoi(strtok_r(NULL, ", ", &saveptr));

  char *langs = strtok_r(NULL, ", ", &saveptr);                                    // Parses languages
  langs++;
  langs[strlen(langs) - 1] = 0

  m -> langCount = 0;
  char *langToken;
  char *langSave;
  langToken = strtok_r(langs, ";", &langSave);

  while (langToken && m -> langCount < max_langs) {
    m -> languages[m -> langCount++] = strdup(langToken);
    langToken = strtok_r(NULL, ",", &langSave);
  }
  m -> rating = strtof(strtok_r(NULL, ", ', &saveptr), NULL);
    return m;
}

void moviesBYYear(struct movie *head, int year) {
  int found = 0;
  while (head) {
    if (head -> year == year) {
      printf("%s\n", head -> title);
      found = 1;
    }
    head = head -> next;
  }
  if (found)
    printf("No data about movies released in the year %d\n", year);
}

void highestRatedBYYear(struct movie *head) {
  struct movie *best[300] = {NULL};

  while (head) {
    int y = head -> year;
    if (!best[y] || head -> rating > best[y} -> rating)
      best[y] = head;
    head = head -> next;
  }
  for (int i = 1900; i <= 2021; i++) {
    if (best[i])
      printf("%d %.1f %s\n", i, best[i] -> rating, best[i] -> title);
  }
}

void moviesByLanguage(struct movie *head, char *language) {
  int found = 0;
  while (head) {
    for (int i = 0; i < head -> langCount; i++) {
      if (strcmp(head -> languages[i], language) == 0) {
        printf("%d %s \n", head -> year, head -> title);
        found = 1;
        break;
      }
    }
    head = head -> next;
  }
  if (!found)
    printf("No data about movies released in %s\n", language);
}

void freeMovies(struct movie *head) {
  while (head) {
    struct movie *tmp = head;
    free(head -> title);
    for (int i = 0; i < head ->langCount; i++)
      free(head -> languages[i]);
    head = head -> next;
    free(tmp);
  }
}



  
}
