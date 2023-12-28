//-----------------------------------------------------------------------------
// a3.c
//
// This program is a Scrabble game in witch two players can play at the same time
// The player with the heigher score winns
// The game starts usualy with a predefinde word 
// 
// Group: Marcus_M_21
//
// Author: 11819675
//-----------------------------------------------------------------------------
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "framework.h"

typedef struct _Tiles_
{
    int numbers_;
    char letters_;
}Tiles;

typedef struct _Score_
{
    int score_;
    // speilernummer
}Score;

#define MAGIC_NUMBER "Scrabble"
const char* ALPHABEAT[26] = {"A","B","C","D","E","F","G","H","I","J","K","L",
"M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"};


int readLine(int argc, char** argv);

//void display_playground(int playfield_dimension, char** playing_field[][]);

int switch_player(int player_turn);
bool isWordValid(int number_of_tiles, char* word, Tiles* list_of_tiles, int field_dimension, char row, char column, char orientation );
void printPlayingField(int score_1, int score_2, int field_dimension, char** playing_field, int number_of_tiles, Tiles* list_of_tiles);
char** playingField(int feeld_dimensions);
int calculateWordPoints(char* word, Tiles* tile, int number_of_tiles);
void printHelp();
bool is_horizontal(char orientation);
int getLetterPoints(char letter, Tiles* tile, int number_of_tiles);


//-----------------------------------------------------------------------------
///
/// This is the main function. Here is the mein logic of the Scrabble game
/// 
///
/// @param valid_config Theis parameter checks for the MAGICNUMBER
/// @param config_file In thei Parameter is stored the config file line by line
/// @param player1_score Score for player 1
/// @param player2_score Score for player 2    
/// @param player_turn Determines wich player turn it is
/// @param list_of_tiles These are the the tiles thet are on the top
/// @param tile This is one tile of the list of tiles
/// @param playfield_dimension This parameter is the height and width of the playing field
/// @param playing_field This parameter is the playing field, it is stored on the heap
/// @param command In this parameter is store the comman from the player with fgets
///
///
///
//
int main(int argc, char** argv)
{
  char valid_config[100];
  FILE* config_file;
  int player1_score = 0;
  int player2_score = 0;
  int player_turn = 0;
  Tiles list_of_tiles[50];
  Tiles tile;
  int playfield_dimension;
  char** playing_field;
  char command[100];

  // Check number of arguments
  if (argc != 2)
  {
    printf("Usage: ./a3 configfile\n");
    exit(1);
  }

  // config file
  config_file = fopen(argv[1], "r");
  if(!config_file)
  {
    printf("Error: Cannot open file: %s\n", argv[1]);
    exit(2);
  }

  // Check magic work
  fgets(valid_config, 100, config_file);
  if(strncmp(MAGIC_NUMBER, valid_config, 8) != 0 || strlen(valid_config) != 9)
  {
    printf("Error: Invalid file: %s\n", argv[1]);
    exit(3);
  }

  fgets(valid_config, 100, config_file);
  playfield_dimension = strlen(valid_config) - 1;
  // here is the playing field copied from config.txt
  playing_field = playingField(playfield_dimension);
  for (int row = 0; row < playfield_dimension; row++)
  {
    for (int column = 0; column < playfield_dimension; column++)
    {
      char tile = valid_config[column];
      playing_field[row][column] = tile;
    }
    fgets(valid_config, 100, config_file);
  }
  printf("\n");

  // copying the score from config file
  player_turn = (int)valid_config[0] - 48;
  fgets(valid_config, 100, config_file);
  player1_score = (int)strtok(valid_config, "\n")[0] - 48;
  fgets(valid_config, 100, config_file);
  player2_score = (int)strtok(valid_config, "\n")[0] - 48;
  fgets(valid_config, 100, config_file);


  int count = 0;
  char delimiter[] = {" "};
  char *element = strtok(valid_config, delimiter);
  int number_of_tiles = 0;
  // list_of_tiles[0] = list_of_tiles[number_of_tiles];
  while(element != NULL) 
  {
    tile.numbers_ = (int)element[1]-48;
    tile.letters_ = element[0];
    list_of_tiles[count] = tile;
    number_of_tiles++;
    // print the usabe tiles
    printf("%c%d", toupper(tile.letters_), tile.numbers_);
    element = strtok(NULL, delimiter);
    if (element != NULL){
      printf(", ");
    }
    if (count == 8)
    {
      printf("\n");
    }
    count++;
  }


  printf("\n");
  // print for the score of the players
  printf("  P1:    %d Points\n", player1_score);
  printf("  P2:    %d Points\n", player2_score);

  // Print first row
  printf(" |");
  for (int i = 0; i < playfield_dimension; i++)
  {
    const char* letter = ALPHABEAT[i];
    printf("%s", letter);
  }
  printf("\n");

  // Print '-' row
  for (int i = 0; i < playfield_dimension + 2; i++)
  {
    printf("-");
  }
  printf("\n");

  // Print board
  for (int i = 0; i < playfield_dimension; i++)
  {
    const char* letter = ALPHABEAT[i];
    printf("%s|", letter);
    for (int y = 0; y < playfield_dimension; y++)
    {
      printf("%c", playing_field[i][y]);
    }
    printf("\n");
  }
   printf("\n");

  
  while(true)
  {
    printf("Player %d > ", player_turn);
    if (fgets(command, 50, stdin) == NULL)
    {
      break;
    }
    int buffer = playfield_dimension;
    Input *input = calloc(buffer, sizeof(*input));
    parseCommand(command, input);

    if (input->is_error_ == true){
      printf("Error: Insert parameters not valid!\n");
      free(input);
      continue;
    }

    char row = input->row_;
    char column = input->column_;
    char orientation = input->orientation_;
    int row_index = (int)row - 97;
    int column_index = (int)column - 97;
    // here is each comand handeld
    if (input->command_ == HELP)
    {
      printHelp();
    }
    else if (input->command_ == QUIT){
      if (feof(stdin))
      {
        free(input);
        break;
      }
      free(input);
      break;
    }
    else if (input->command_ == INSERT){
      char* word = input->word_;  
      //                                                  
      if (isWordValid(number_of_tiles, word, list_of_tiles, playfield_dimension, row, column, orientation))
      {
        int field_dimension_size = 0;
        // increas score 
        if (is_horizontal(orientation))
        {
          field_dimension_size = playfield_dimension - (column - 97);
        } else
        {
          field_dimension_size = playfield_dimension - (row - 97);
        }

        char insertions_space[30];
        memset(insertions_space, 0, 30);
        // check if word can be put on this location
        for (int i = 0; i < field_dimension_size; i++)
        {
          if(is_horizontal(orientation))
          {
            insertions_space[i] = playing_field[row_index][column_index + i];
          }
        }
        // search if one of the elements of the word are allready on the playing field
        int score_word = 0;
        for(size_t i = 0; i < strlen(word); i++)
        {
          if(insertions_space[i] == ' ')
          {
            score_word += getLetterPoints(word[i], list_of_tiles, number_of_tiles);
          } 
          else if (toupper(insertions_space[i]) != toupper(word[i]) || insertions_space[i] != ' ')
          {
            score_word = 0;
            //printf("Error: Impossible move!\n");
            printf("\n");
            break; 
          }
        }
        // changin player turn
        if (player_turn == 1)
        {
          player1_score += score_word;
        }
        else 
        {
          player2_score += score_word;
        }
        
        player_turn = switch_player(player_turn);
      }

      printPlayingField(player1_score, player2_score, playfield_dimension, playing_field, number_of_tiles, list_of_tiles);
      
      free(word); 
    }
    else if (input->command_ == SAVE){
      //TODO Save to File
    }

    free(input);
  };
  
  for (int i = 0; i < playfield_dimension; i++)
  {
    free(playing_field[i]);
  }
  free(playing_field);
  fclose(config_file);
  return 0;
}

//-----------------------------------------------------------------------------
///
/// This function checks if a word iss allowd in the playfield 
/// It schecks for the length, if it has only characters and no numbers in it,
/// and checks if the word uses only letters out of list_of_tiles
/// 
///
/// @param number_of_tiles is the amount of tiles in the list of Tiles
/// @param word here is stored the for validity
/// @param list_of_tiles is a array where the tiles are stored 
/// @param field_dimensions height and width of the playing field
///
///
/// @return bool if word is valid or not
//
bool isWordValid(int number_of_tiles, char* word, Tiles* list_of_tiles, int field_dimension, char row, char column, char orientation)
{
  // anfangspunkt vom word + word length </> field dimension
  // check  if the word firs in the playing field
  if (is_horizontal(orientation))
  {
    if ((row - 97) + (int)strlen(word) > field_dimension)
    {
      printf("Error: Impossible move!\n");
      return false;
    }
  } 
  else
  {
    if ((column - 97) + (int)strlen(word) > field_dimension)
    {
      printf("Error: Impossible move!\n");
      return false;
    }
  }

  // check if the word is [A_Z] 
  for (size_t position_counter = 0; position_counter < strlen(word); position_counter++)
  {
    if (!(isalpha(word[position_counter])))
    {
      printf("Error: Insert parameters not valid!\n");
      return false;
    }
  }

  // check if the letters of the word are also in the list of tiles
  char accepted_caracters[number_of_tiles];
  for (int i = 0; i < number_of_tiles; i++)
  {
    accepted_caracters[i] = list_of_tiles[i].letters_;
  }
  //printf("%s", accepted_caracters);

  for (size_t i = 0; i < strlen(word); i++)
  {
    if (strrchr(accepted_caracters, word[i]) == NULL)
    {
      printf("%s\n", word);
      return false; 
    }
  }

  return true;  
}

//-----------------------------------------------------------------------------
///
/// This checks if the orientation is Horizontal or vertical
///
/// @param orientation can be horizontal or vertical
///
///
/// @return true for Horizontal and false for Vertical
//
bool is_horizontal(char orientation) {
  if (orientation == HORIZONTAL) {
    return true;
  } else {
    return false;
  }
}

//-----------------------------------------------------------------------------
///
/// This function is for switching the turn of the player
///
/// @param player_turn is a parameter for the tur of the player
/// 
///
/// @return 2 if player_turn is 1
//
int switch_player(int player_turn){
  if (player_turn == 1)
  {
    return 2;
  }
  else 
  {
    return 1;
  }
}

//-----------------------------------------------------------------------------
///
/// This function prinsts the playing fiel every time it updates
///
/// @param score_1 is the parameter for the score for player 1
/// @param score_2 is the parameter for the score for player 2
/// @param field_dimensiont height and width of the playing field
/// @param playing_field This parameter is the playing field, it is stored on the heap
///
///
//
void printPlayingField(int score_1, int score_2, int field_dimension, char** playing_field, int number_of_tiles, Tiles* list_of_tiles)
{
  printf("  P1:    %d Points\n", score_1);
  printf("  P2:    %d Points\n", score_2);

  printf(" |");
  for (int i = 0; i < field_dimension; i++)
  {
    const char* letter = ALPHABEAT[i];
    printf("%s", letter);
  }
  printf("\n");

  for (int i = 0; i < field_dimension + 2; i++)
  {
    printf("-");
  }
  printf("\n");

  for (int i = 0; i < field_dimension; i++)
  {
    const char* letter = ALPHABEAT[i];
    printf("%s|", letter);
    for (int y = 0; y < field_dimension; y++)
    {
      printf("%c", playing_field[i][y]);
    }
    printf("\n");
  }
}

//-----------------------------------------------------------------------------
///
/// This function is getting the wheit of the tile wit .number
///
/// @param letter This are the letter part of a Tile
/// @param tile This is one tile of the list of tiles
/// @param number_of_tiles This is the amount of tiles in the list of Tiles
///
/// @return the wheiht of the tile.
//
int getLetterPoints(char letter, Tiles* tile, int number_of_tiles)
{
  for (int element_of_list = 0; element_of_list < number_of_tiles; element_of_list++)
  {
    if (tolower(letter) == tolower(tile[element_of_list].letters_))
    {
      return tile[element_of_list].numbers_;
    }
  } 

  return 0;
}


//-----------------------------------------------------------------------------
///
/// This function calculates the points of the word 
/// 
///
/// @param word This is the word for witch the points are calculated
/// @param tile This is one tile of the list of tiles
/// @param number_of_tiles is the amount of tiles in the list of Tiles
///        
///
/// @return the points of the word
//
int calculateWordPoints(char* word, Tiles* tile, int number_of_tiles)
{
  int current_word_points = 0;

  for ( size_t letter_of_word = 0; letter_of_word < strlen(word); letter_of_word++)
  {
    for (int letter_of_list = 0; letter_of_list < number_of_tiles; letter_of_list++)
    {
      if (tolower(word[letter_of_word]) == tolower(tile[letter_of_list].letters_))
      {
        current_word_points += tile[letter_of_list].numbers_;
      
      }
    }
  } 

  return current_word_points;   
}

//-----------------------------------------------------------------------------
///
/// This Functon only prints the help Command
/// 
//
void printHelp()
{
  printf("Commands:\n");
  printf(" - insert <ROW> <COLUMN> <H/V> <WORD>\n");
  printf("    <H/V> stands for H: horizontal, V: vertical.\n");
  printf("\n");
  printf(" - help\n");
  printf("    Prints this help text.\n");
  printf("\n");
  printf(" - quit\n");
  printf("    Terminates the game.\n");
  printf("\n");
  printf(" - save\n");
  printf("    Saves the game to the current config file.\n");
  printf("\n");
  printf(" - load <CONFIGFILE>\n");
  printf("    load config file and start game.\n");
}


//-----------------------------------------------------------------------------
///
/// This Function is storring the playing Field on the heap
///
/// @param feeld_dimension This is the height and width of the playing Field
///
/// @return playing_field
//
char** playingField(int feeld_dimensions)
{
  // QUELLE: https://www.geeksforgeeks.org/dynamically-allocate-2d-array-c/
  // here we are using a Array of pointers with the help of a doublle pointer.
  // at the double pointer memory is alocated dinamicly on the Heap.
  // whit this we can acces every part of the Playingfield.

  char** playing_field = (char**)malloc(feeld_dimensions * sizeof(char*));
  for (int direction = 0; direction < feeld_dimensions; direction++)
  {
    playing_field[direction] = (char*)malloc(feeld_dimensions * sizeof(char));
  }
  return playing_field;
}
