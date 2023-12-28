//-----------------------------------------------------------------------------
// a2.c
//
// This program is reading in multiple sentences as a text dinamicly and let's 
// you chous between 6 operationson the sentences. Befor chosing your operations 
// you can split the text into sentences snd ork with the sentences individually.
//
// Group: Markus_M_21
//
// Author: 11819675
//-----------------------------------------------------------------------------
//

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define BUFFER_SIZE 100

int gettingStringLength(char *string_content);
char* insertingText(char* next_text);
char* stichingSentences(char* string_one, char* string_two, char* connection_word);
void switchingPlaceOfSentence(char** sentences);
void statistics(int sentence_count, char* next_text);
char checkForNewLine(char* next_text);

//-----------------------------------------------------------------------------
///
/// This is the main function. It is the turning table for all operations.
///
/// @param text the text parameter is a char* the inputed text is stored here.
/// @param sentences thte sentences parameter is a char** where the text is stored 
///        in individual sentences.
///
//
int main()
{
  printf("Bitte Text eingeben:\n");
  char* text = NULL;
  text = insertingText(text);
  if (text == NULL)
  {
    printf("Zu wenig Speicher vorhanden!");
    return 3;

  }

  // in sentences it is stored the splitted string.
  char** sentences = NULL;
  // sentence_count counts the number of splited sentences.
  int sentence_count = 0;
  // current_sentence_length holds the legth of the given sentence.
  int current_sentence_length = 0;

  // theis loop iterates trough all characters of the text string.
  for (int i = 0; i < gettingStringLength(text); i++)
  {
    // checks the string text for teh end of sentence
    if ((text[i] == '.') || (text[i] == '!') || (text[i] == '?'))
    {
      // makes place for every sentence
      sentence_count++;
      sentences = realloc(sentences, sentence_count * sizeof(char*));
      sentences[sentence_count - 1] = calloc(current_sentence_length + 2, sizeof(char));
      // at this possition in every sentence I check for space at the begining of the sentence
      sentences[sentence_count - 1][0] = ' ';
      int j = 0;
      while (text[i-current_sentence_length + j] == ' ')
      {
        j++;
      }
      for (int jj = 1; j < current_sentence_length + 1; j++, jj++)
      {
        sentences [sentence_count - 1][j] = text[i-current_sentence_length + j];
      }

      sentences [sentence_count - 1][current_sentence_length] = text[i];
      current_sentence_length = 0;
      // this i++ is for ensuring s clean format
      i++;
    }
    else
    {
      current_sentence_length++;
    }

  }

  if (text[0] == '\n')
  {
    free(text);
    printf("Kein Satz gefunden!\n");
    return 1;
  }

  char choice = 0;
  int sentence_one = 0;
  int sentence_two = 0;
  char* buffer = NULL;
  

  while (choice != 'e')
  {
    if(choice != '\n')
    {
      printf("\nWählen Sie eine Option:\n"
             "  t: Text-Statistik\n"
             "  s: Zwei Sätze vertauschen\n"
             "  c: Zwei Sätze zusammenketten\n"
             "  p: Text mit Zeilennummer ausgeben\n"
             "  o: Text ohne Zeilennummer ausgeben\n"
             "  e: Programm beenden\n\n");
    }

    scanf("%c", &choice);
    
  /*   while (choice != 't' || choice != 's' || choice != 'c' || choice != 'p' || choice != 'o')
    {
      scanf("%c", &choice);
      printf("Ihre Wahl: ");
    }
     */
    // this switch statemment is udes for choseing the user input and displaying the desired functionality
    switch (choice)
    {
      case 't':
        printf("Ihre Wahl: ");
        printf("Text Statistik:\n");
        statistics(sentence_count, text);
        break;
      case 's':
        printf("Ihre Wahl: ");
        switchingPlaceOfSentence(sentences);
        break;
      case 'c':
        printf("Ihre Wahl: ");
        char* conection_word = "und ";
        printf("Nummer des ersten Satzes: ");
        scanf("%d", &sentence_one);
        if(sentence_one > sentence_count)
        {
          printf("Nummer des ersten Satzes: ");
          scanf("%d", &sentence_one);
        }
        printf("Nummer des zweiten Satzes: ");
        scanf("%d", &sentence_two);
        if (sentence_two > sentence_count)
        {
          printf("Nummer des zweiten Satzes: ");
          scanf("%d", &sentence_two);
        }
        buffer = stichingSentences(sentences[sentence_one - 1], sentences[sentence_two - 1], conection_word);

        free(sentences[sentence_two - 1]);
        sentences[sentence_one - 1] = buffer;
        sentences[sentence_two - 1] = NULL;
        break;
      case 'p':
        printf("Ihre Wahl:  ");
        printf("Nr. | Satz\n");
        printf("-----+----------------------------------------\n");
        int new_counter = 0;
        for (int number_of_sentences = 0; number_of_sentences < sentence_count; number_of_sentences++)
        {
          if (sentences[number_of_sentences] == NULL)
          {
            continue;
          }
          new_counter++;
          if (sentences[sentence_two - 1] == NULL)
          {
            printf("%4d | %s\n",new_counter , sentences[number_of_sentences]);
          }
          else
          {
            printf("%4d | %s\n",number_of_sentences + 1, sentences[number_of_sentences]);
          }
        }
        break;
      case 'o':
        printf("Ihre Wahl: ");
        printf("Text:\n");
        for (int number_of_sentences = 0; number_of_sentences < sentence_count; number_of_sentences++)
        {
          if (sentences[number_of_sentences] == NULL)
          {
            continue;
          }
          printf("%s ", sentences[number_of_sentences]);
        }
        printf("\n");
        break;
      case 'e':
        printf("Ihre Wahl: ");
        printf("Programmende!\n");
        break;
      default:
        break;
    }
  }

  // this for loof is freeing all sentences
  for (int i = 0; i < sentence_count; i++)
  {
    free(*(sentences + i));
  }
  free(sentences);
  free(text);

  return 0;
}

//-----------------------------------------------------------------------------
///
/// Ths is the function whete the string is filld dinamicly in.
///
/// @param counter counts the number of how many times I increas the BUFFER_SIZE
///
/// @return cahr* text
//
char* insertingText( char* text)
{
  int counter = 0;
  text = calloc(BUFFER_SIZE, sizeof(char));
  if(text == NULL)
  {
    printf("Zu wenig Speicher vorhanden!");
  }
  while (fgets(text + (BUFFER_SIZE * counter ) -counter, BUFFER_SIZE, stdin))
  {
    if(text[gettingStringLength(text) - 1] == '\n')
    {
      break;
    }
    counter++;
    text = realloc(text, (counter + 1) * BUFFER_SIZE);
    
  }
  return text;
}

//-----------------------------------------------------------------------------
///
/// This function is taking 2 int as input for 2 sentences and switches the 
/// place of the 2 sentences.
///
/// @param switched_sentence_one The the number for the firs sentence.
/// @param switched_sentence_two The the number for the second sentence.
/// @param temp Is used as a temparary parameter for switching teh sentences.
///
//
void switchingPlaceOfSentence(char** sentences)
{
  int switched_sentence_one = 0;
  int switched_sentence_two = 0;
  printf("Nummer des ersten Satzes: ");
  scanf("%d", &switched_sentence_one);
  printf("Nummer des zweiten Satzes: ");
  scanf("%d", &switched_sentence_two);

  char* temp = sentences[switched_sentence_one - 1];
  sentences[switched_sentence_one - 1] = sentences[switched_sentence_two - 1];
  sentences[switched_sentence_two - 1] = temp;

}

//-----------------------------------------------------------------------------
///
/// This function is used for counting the number of eklements of a given string.
///
/// @param contents_counter This parameter holds the number of characters in teh string.
///
/// @return contents_counter;
//
int gettingStringLength(char *string_content)
{
  unsigned int contents_counter = 0;
  while (*string_content != '\0')
  {
    contents_counter++;
    string_content++;
  }
  return contents_counter;
}

//-----------------------------------------------------------------------------
///
/// This function is used for figuering out how many sentences, characters, letters,
/// (kaptal and non kapital leters) are stored int the string.
///
/// @param characters are all characters counted
/// @param upper_case_letters are all upper Kase letter
/// @param lower_case_lettersare are all lower Kase letter
/// @param space_counter are all spaces
/// @param special_characters are all special characters (punctuation)
/// @param digit_counter are all numbers in the text
///
//
void statistics(int sentence_count, char* next_text)
{
  int characters = 0;
  int upper_case_counter = 0;
  int lower_case_counter = 0;
  //int space_counter = 0;
  int special_characters = 0;
  int punctuation_counter = 0;
  for (int i = 0; i < gettingStringLength(next_text); i++)
  {

    if (isalpha(next_text[i]))
    {
      characters++;
      if (isupper(next_text[i]))
      {
        upper_case_counter++;
      }
      else
      {
        lower_case_counter++;
      }
      if (next_text[i] == "." || next_text[i] == "!" || next_text[i] == "?")
      {
        punctuation_counter++;
      }
    }
    else
    {
      /* if (next_text[i] == "." || next_text[i] == "!" || next_text[i] == "?")
      {
        punctuation_counter++;
      } */
      
      special_characters++;
    }

    /* if (gettingStringLength(next_text) != '\n' || gettingStringLength(next_text) != '\0')
    {
    characters++;
    } */
    
/* 
input[i] == "." || input[i] == "!" || input[i] == "?"
    if (isupper(next_text[i]))
    {
      upper_case_counter++;
    }
    else
    {
      lower_case_counter++;
    }
    if (isspace(next_text[i]))
    {
      space_counter++;
    }
    if ((isalpha(next_text[i]) == 0 ) && (isspace(next_text[i]) == 0))
    {
      special_characters++;
    }
    if (isdigit(next_text[i]))
    {
      digit_counter++;
    }
     */
  }
  // printf("characrters %d\n", characters);
  // printf("")

  printf("%d Sätze, %d Zeichen\n", sentence_count, characters + punctuation_counter);
  printf("%d Buchstaben (%d Großbuchstaben, %d Kleinbuchstaben)\n", characters, upper_case_counter, lower_case_counter);
  /* 
  int letters = characters - space_counter - special_characters - digit_counter;
  int lower_casse_letters = lower_case_counter - space_counter - special_characters;

  printf("%d Sätze, %d Zeichen\n", sentence_count, characters - space_counter);
  printf("%d Buchstaben (%d Großbuchstaben, %d Kleinbuchstaben)\n",letters, upper_case_counter, lower_casse_letters);
 */}

//-----------------------------------------------------------------------------
///
/// This function is connecting any two sentences with a "und"
///
/// @param sentence_one Holds the number of characters of the first string
/// @param sentence_two Holds the number of characters of the second string
/// @param connection Holds the number of characters of theconection word.
/// @param length Is the length of the conected sentence with the cnnecting word
/// @param buffer This char* holds the conected sentences
///
/// @return buffer
//
char* stichingSentences(char* string_one, char* string_two, char* connection_word)
{
  int sentence_one = gettingStringLength(string_one);
  int sentence_two = gettingStringLength(string_two);
  int conection = gettingStringLength(connection_word);

  int length = sentence_one + conection + sentence_two + 1;

  char* buffer = calloc(length, sizeof(char));

  for (int i = 0; i < sentence_one; i++)
  {
    buffer[i] =  string_one[i];
  }
  buffer[sentence_one - 1] = ' ';
  for (int i = 0; i < conection; i++)
  {
    buffer[sentence_one + i] = connection_word[i];
  }
  for (int i = 0; i < sentence_two; i++)
  {
    buffer[sentence_one + conection + i] = string_two[i];
  }
  
  buffer[length - 1] = '\0';
  
  return buffer;
}

