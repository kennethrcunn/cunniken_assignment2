// Program Name: Movies
// Author: Kenneth Cunningham
// This program will implement movie management functionality.
// Used to learn C concepts such as pointers, structures, and linked lists

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define max_langs 5      // Max number of languages a movie can have  
#define min_yr 1900      // Earliest year considered
#define max_yr 2021      // Earliest year considered

/*Stores movies data*/
struct movie {
  char *title;
  int year;
  char *languages[max_langs];
  int  langCount;
  double rating;
  struct movie *next;      // Pointer to next movie in list    
};

/*Function prototypes*/
struct movie *parseMovie(char *line);                        // Parses one line of the file into a movie structure                     
void freeMovies(struct movie *head);                         // Frees up allocated memory
void showMenu(void);                                         // Displays menu choices
void moviesByYear(struct movie *head, int year);             // Prints movies from given year
void highestRatedByYear(struct movie *head);                 // Prints highest rated movie per year
void moviesByLanguage(struct movie *head, char *language);   // Prints movies by language


int main(int argc, char *argv[]) {                         // Ensures a filename was given
  if (argc < 2) {                                          
    printf("You must provide a file name\n");
    return 1;                                              // Exits program with error
  }

  FILE *file = fopen(argv[1], "r");                        // Opens file
  if (!file) {
    perror("File failed to open");
    return 1;                                              // Exits program with error
  }

  char buffer[1024];                                      // Buffer to store each line from file
  struct movie *head = NULL;                              // Head of linked list
  struct movie *tail = NULL;                              // Tail of linked list
  int count = 0;                                          // Initializes count of processed movies


  /*Reads movies from file*/
  fgets(buffer, sizeof(buffer), file);                    // Skips header line

  while (fgets(buffer, sizeof(buffer), file)) {           // Reads each line from file
    struct movie *m = parseMovie(buffer);                 // Converts line to movioe struct
    if (!m) continue;                                     // If parsing fails, skip
    
    if (!head)                                            // If list is empty, initialize head and tail
      head = tail = m;
    else {
      tail -> next = m;    // Link new movie to end of list and updates tail
      tail = m;
    }
    count++;               // Increments movie count
  }
  fclose(file);            // Close file 
  printf("Processed file %s and parsed data for %d movies\n\n", argv[1], count);

  int choice;
  do {                                                                            // Prompts menu options
    showMenu();
    printf("\nEnter a choice from 1 to 4: ");
    scanf("%d", &choice);

    if (choice == 1) {                                                            // Displays movies from given year
      int year;
      printf("Enter the year for which you want to see movies: ");
      scanf("%d", &year);
      moviesByYear(head, year);                                                  
    }
    else if (choice == 2) {                                                       // Displays highest rated movie per year                                                    
      highestRatedByYear(head);
    }
    else if (choice == 3) {                                                       // Displays movies from chosen language
      char lang[20];
      printf("Enter the language for which you want to see movies: ");
      scanf("%19s", lang);
      moviesByLanguage(head, lang);
    }
    else if (choice != 4) {                                                      // Displays error message
      printf("You entered an invalid choice. Try again. \n");
    }
    
  } while (choice != 4);    // Loops until user chooses Exit                                                       

  freeMovies(head);         // Frees up memory
  return 0;                 // Program ends withour error
}


/*Helper functions*/

/*Displays the program menu options*/
void showMenu(void) {
  printf("\n1. Show movies released in the specified year\n");
  printf("2. Show highest rated movie for each year\n");
  printf("3. Show the title and year of release of all movies in a specific language\n");
  printf("4. Exit from the program\n"); 
}

/*Parses a CSV line into a movie structure*/
struct movie *parseMovie(char *line) {
  struct movie *m = malloc(sizeof(struct movie));                                 // Allocates memory
  if (!m) return NULL;
  m -> next = NULL; // Initializes next pointer

  char *saveptr;
  char *token = strtok_r(line, ",", &saveptr);                                    // Extracts title
  if (!token) return NULL;

  m -> title = strdup(token);                                                     // Stores title
  m -> year = atoi(strtok_r(NULL, ",", &saveptr));                                // Converts year to integer

  char *langs = strtok_r(NULL, ",", &saveptr);                                    // Extracts language field
  langs++;
  langs[strlen(langs) - 1] = 0;

  m -> langCount = 0;
  char *langToken;
  char *langSave;
  langToken = strtok_r(langs, ";", &langSave);

  /*Tokenize languages*/
  while (langToken && m -> langCount < max_langs) {
    m -> languages[m -> langCount++] = strdup(langToken);
    langToken = strtok_r(NULL, ";", &langSave);
  }
  m -> rating = strtof(strtok_r(NULL, ",", &saveptr), NULL);                       // Converts rating string to float
    return m;
}

/*Prints all movies released in a given year*/
void moviesByYear(struct movie *head, int year) {
  int found = 0;
  while (head) {
    if (head -> year == year) {
      printf("%s\n", head -> title);
      found = 1;
    }
    head = head -> next;
  }
  if (!found)
    printf("No data about movies released in the year %d\n", year);
}

/*Prints the highest rated movie for each year*/
void highestRatedByYear(struct movie *head) {
  struct movie *best[max_yr - min_yr + 1] = {NULL};

  while (head) {
    int idx = head -> year - min_yr;
    if (idx >= 0 && idx <= max_yr - min_yr) { 
      if (!best[idx] || head -> rating > best[idx] -> rating) 
        best[idx] = head;
    }
    head = head -> next;
  }
  for (int i = 0; i <= max_yr - min_yr; i++) {
    if (best[i])
      printf("%d %.1f %s\n", i + min_yr, best[i] -> rating, best[i] -> title);
  }
}

/*Prints movies available in a chosen language*/
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

/*Frees up all allocated memory for movie list*/
void freeMovies(struct movie *head) {
  while (head) {
    struct movie *tmp = head;
    free(head -> title);                                                                      // Free title string                 
    for (int i = 0; i < head ->langCount; i++)
      free(head -> languages[i]);                                                             // Free language strings  
    head = head -> next;
    free(tmp);                                                                                // Free movie struct  
  }
}
