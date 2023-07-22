#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
using namespace std;


/*{ Sample program
  in TINY language
  compute factorial
}

read x; {input an integer}
if 0<x then {compute only if x>=1}
  fact:=1;
  repeat
    fact := fact * x;
    x:=x-1
  until x=0;
  write fact {output factorial}
end
*/

// sequence of statements separated by ;
// no procedures - no declarations
// all variables are integers
// variables are declared simply by assigning values to them :=
// variable names can include only alphabetic charachters(a:z or A:Z) and underscores
// if-statement: if (boolean) then [else] end
// repeat-statement: repeat until (boolean)
// boolean only in if and repeat conditions < = and two mathematical expressions
// math expressions integers only, + - * / ^
// I/O read write
// Comments {}


////////////////////////////////////////////////////////////////////////////////////
// Scanner /////////////////////////////////////////////////////////////////////////

#define MAX_TOKEN_LEN 40

enum TokenType{
                IF, THEN, ELSE, END, REPEAT, UNTIL, READ, WRITE,
                ASSIGN, EQUAL, LESS_THAN,
                PLUS, MINUS, TIMES, DIVIDE, POWER,
                SEMI_COLON,
                LEFT_PAREN, RIGHT_PAREN,
                LEFT_BRACE, RIGHT_BRACE,
                ID, NUM,
                ENDFILE, ERROR
              };

// Used for debugging only /////////////////////////////////////////////////////////
const char* TokenTypeStr[]=
            {
                "If", "Then", "Else", "End", "Repeat", "Until", "Read", "Write",
                "Assign", "Equal", "LessThan",
                "Plus", "Minus", "Times", "Divide", "Power",
                "SemiColon",
                "LeftParen", "RightParen",
                "LeftBrace", "RightBrace",
                "ID", "Num",
                "EndFile", "Error"
            };


void Copy(char* a, const char* b, int n=0)
{
    if(n>0) {strncpy(a, b, n); a[n]=0;}
    else strcpy(a, b);
}

struct Token
{
    TokenType type;
    char str[MAX_TOKEN_LEN+1];

    Token(){str[0]=0; type=ERROR;}
    Token(TokenType _type, const char* _str) {type=_type; Copy(str, _str);}
};

//a struct that contains the attributes needed to print in every line of the output file.
struct Entry
{
    int line_num;
    string token_name;
    string token_type;

    Entry(int n, string name, string type){line_num = n; token_name = name; token_type = type;}

};

//The function that prints into the output file.
void Write(Entry e)
{
  ofstream file("output.txt",ios::app);

  file << "[" << e.line_num << "]" << e.token_name << "(" << e.token_type << ")" << "\n";

  file.close();
}

const Token reserved_words[]=
{
    Token(IF, "if"),
    Token(THEN, "then"),
    Token(ELSE, "else"),
    Token(END, "end"),
    Token(REPEAT, "repeat"),
    Token(UNTIL, "until"),
    Token(READ, "read"),
    Token(WRITE, "write")
};
const int num_reserved_words=sizeof(reserved_words)/sizeof(reserved_words[0]);

// if there is tokens like < <=, sort them such that sub-tokens come last: <= <
// the closing comment should come immediately after opening comment
const Token symbolic_tokens[]=
{
    Token(ASSIGN, ":="),
    Token(EQUAL, "="),
    Token(LESS_THAN, "<"),
    Token(PLUS, "+"),
    Token(MINUS, "-"),
    Token(TIMES, "*"),
    Token(DIVIDE, "/"),
    Token(POWER, "^"),
    Token(SEMI_COLON, ";"),
    Token(LEFT_PAREN, "("),
    Token(RIGHT_PAREN, ")"),
    Token(LEFT_BRACE, "{"),
    Token(RIGHT_BRACE, "}")
};
const int num_symbolic_tokens=sizeof(symbolic_tokens)/sizeof(symbolic_tokens[0]);

inline bool IsDigit(char ch){return (ch>='0' && ch<='9');}
inline bool IsLetter(char ch){return ((ch>='a' && ch<='z') || (ch>='A' && ch<='Z'));}
inline bool IsLetterOrUnderscore(char ch){return (IsLetter(ch) || ch=='_');}

//A function that it is used to clear the contents of the output file.
void clear()
{
    ofstream file("output.txt");
    file<<"";
    file.close();
}

int main()
{
    ifstream file("input.txt");
    string line; // The variable which will store the lines of the input file.
    bool comment = false; // A variable that tells us when a comment starts and ends.
    int line_num = 1; // A variable that increments each time we read from a new line from the input file.
    string word = ""; // A variable used when dealing with IDs or Nums.

    clear(); // clearing the content of the output file from the previous run.

    //Reading the contents of the input file.
    while (!file.eof())
    {
        getline(file,line);

        for (int i = 0; i < line.size(); i++)
        {
            if (line[i] == ' ' || (comment && line[i] != '}')) // Case in which we are inside a comment or the current character is a blank space.
            {
                continue;
            }

            else if (line[i] == '{')
            {
                comment = true;
                Entry e(line_num,"{","LeftBrace");
                Write(e);
            }

            else if (line[i] == '}')
            {
                comment = false;
                Entry e(line_num,"}","RightBrace");
                Write(e);
            }

            else if (IsLetterOrUnderscore(line[i])) // Case of IDs and reserved words.
            {
                word += line[i];
                if (!IsLetterOrUnderscore(line[i+1])) // Checking which token word is.
                {
                    if(word == "if")
                    {
                        Entry e(line_num,"if","If");
                        Write(e);
                    }


                    else if (word == "then")
                    {
                        Entry e(line_num,"then","Then");
                        Write(e);
                    }

                    else if (word == "else")
                    {
                        Entry e(line_num,"else","Else");
                        Write(e);
                    }

                    else if (word == "end")
                    {
                        Entry e(line_num,"end","End");
                        Write(e);
                    }

                    else if (word == "repeat")
                    {
                        Entry e(line_num,"repeat","Repeat");
                        Write(e);
                    }

                    else if (word == "until")
                    {
                        Entry e(line_num,"until","Until");
                        Write(e);
                    }

                    else if (word == "read")
                    {
                        Entry e(line_num,"read","Read");
                        Write(e);
                    }

                    else if (word == "write")
                    {
                        Entry e(line_num,"write","Write");
                        Write(e);
                    }

                    else
                    {
                        Entry e(line_num,word,"ID");
                        Write(e);
                    }
                    word = "";

                }
            }

            else if (IsDigit(line[i])) // Case of Nums.
            {
                word += line[i];
                if (!IsDigit(line[i+1]))
                {
                    Entry e(line_num,word,"Num");
                    Write(e);
                    word = "";
                }
            }

            // Cases of Symbols.
            else if (line[i] == ':' && line[i+1] == '=')
            {
                i++;
                Entry e(line_num,":=","Assign");
                Write(e);
            }

            else if (line[i] == '=')
            {
                Entry e(line_num,"=","Equal");
                Write(e);
            }

            else if (line[i] == '<')
            {
                Entry e(line_num,"<","LessThan");
                Write(e);
            }

            else if (line[i] == '+')
            {
                Entry e(line_num,"+","Plus");
                Write(e);
            }

            else if (line[i] == '-')
            {
                Entry e(line_num,"-","Minus");
                Write(e);
            }

            else if (line[i] == '*')
            {
                Entry e(line_num,"*","Times");
                Write(e);
            }

            else if (line[i] == '/')
            {
                Entry e(line_num,"/","Divide");
                Write(e);
            }

            else if (line[i] == '^')
            {
                Entry e(line_num,"^","Power");
                Write(e);
            }

            else if (line[i] == ';')
            {
                Entry e(line_num,";","SemiColon");
                Write(e);
            }

            else if (line[i] == '(')
            {
                Entry e(line_num,"(","LeftParen");
                Write(e);
            }

            else if (line[i] == ')')
            {
                Entry e(line_num,")","RightParen");
                Write(e);
            }

            else
            {
                word += line[i];
                Entry e(line_num,word,"Error");
                Write(e);
                word = "";
            }
        }

    line_num++;

    }

    return 0;
}
