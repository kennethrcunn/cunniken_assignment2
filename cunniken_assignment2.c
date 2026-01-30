// Program Name: Movies
// Author: Kenneth Cunningham
// This program will 
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
  do {                                                                            //
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


  
  



  
}
