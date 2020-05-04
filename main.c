#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/stat.h>

/*
This program allows for many cipher applications. It allows the user to encrypt text for a classical rail fence cipher and both ecryption/decryption for a 2-level
rail fence cipher. There is a user-friendly menu system which allows the user to easily choose what they want the program to do. The only ambigous areas would be those
of reading the number of rails A and B (respectively) and when entering text for encryption or decryption.

When choosing the number of rails the user will be prompted: Please enter the number of rails for A and B:. The user should answer A followed by a SPACE and then B.
e.g Please enter the number of rails for A and B: 4 2
This will allow for the 2-level rail cipher to have 4 maximum rails and then 2 rails on the smaller peak.

The other ambiguos area is that of enterting text to encrypt/decrypt via the console. The user can enter as much text as they would like including new line characters,
once the user is satisfied with the amount of text entered they should terminate the message by following the end of it with a '~' character. This characer followed by
ENTER key will allow for the text to be used by the program.

This program also allows for file i/o. Suitable text files labelled 'message' and 'cipherText' respectively in the folder the main.c file is located in will allow for reading
and writing to both.

The only user input prompts are those to select which application the user wants to do and those to select the number of rails. These cannot be read from a file and need to
be typed into the console when prompted to by the program.

The program does not allow for multiple applications to be done in one execution. To perform another action the program must be exited and run again.
*/

void railFence(char *message, char *cipherText, int length, int A);

void railFence2(char *message, char *cipherText, int length, int A, int B, int dir);

/*
The int main() section is used to determine what the user wants to do. It determines the application needed and prompts the user to input all necessary data needed for
execution.

Common inputs for all programs are the number of rails. With the 2-level rail cipher needing 2 inputs for the number of rails (on how this is entered see block omment above).
There is also the case where text is to written into the console by the user should they choose to do so. These are the only user inputs apart from action selection that
are needed.

There are limitations to the length of strings which are entered which is 8193 characters. This is per the assesment guidelines. Also in the 2-level rail cipher selection
there is a need for A>B and B>0 (per the assesment guidelines as well). For file i/o the file needs to exist and have information to read from it for the program to allow
useful actions to be done with it. All counters and variables are mostly of the integer type. This is because you can't for example have 3.5 rails.

The basic flow of int main() is prescribed as follows:
 - Program selection (Encryption of classical cipher, encryption of 2-level cipher, decryption of 2-level cipher)
 - Input selection (Does the user want to write text into the console or read from a file)
 - Rail amount selection (Number of classical rails needed or rails for A and B needed
*/
int main(int argc,char **argv)
{
    printf("This program allows for cipher functions to be executed.\nPlease choose an option from the list below:\n\n");
    printf("1 - Encryption of a classic rail-fence cipher.\n\n"); //Allowing for a user friendly menu to choose what they want the program to do.
    printf("2 - Encryption of a 2-level rail-fence cipher.\n\n");
    printf("3 - Decryption of a 2-level rail-fence cipher.\n\n");
    printf("Please enter an integer corresponding to a program: ");

    FILE *messageFILE; //Declaring pointers to files and opening them for use later on in the program
    FILE *cipherTextFILE;
    messageFILE = fopen("message.txt", "r");
    cipherTextFILE = fopen("cipherText.txt", "r");

    int x; //Initialsing integer x to be used in menu interface choices.
    char mess[8193]; //Declaring a message string for the user to input into stdin
    char ciph[8193]; //Declaring a cipher string for the user to input into stdin
    char temp; //A temporary value such that when entering a string into stdin it skips the \n statement beforehand
    int A, B; //Declaring A and B for use to determine the amount of rails in the 2-level cipher
    int length; //Declaring a variable to use in determining a strings length
    int size; //Declaring a variable size that it uses to determine the size of files use
    char c; //Declaring a char c used as a position holder for writing a file into a string
    int i; //Declaring a variable i to be used as a counter in further applications

    scanf("%d", &x); //Reading user input on what program to execute

    switch(x) //Using the users input, it chooses which case to execute. Has a defualt for invalid input.
    {
        case 1: //Case for the user to encrypt a message based on the classic rail-fence cipher.
            printf("Please choose an option on how to read the text:\n\n"); //Another user friendly menu to choose how to read the text.
            printf("1 - Write text in the console.\n\n");
            printf("2 - Read text from a file.\n\n");
            printf("Please enter an integer corresponding to a program: ");
            scanf("%d", &x); //Reading user input on what the program should then execute

            if (x==1) //Case for reading text from stdin for use in the decryption algorithim
            {
                fclose(messageFILE); //Closes the cipherText file

                printf("Please enter text to encrypt (with a terminating '~' char): "); //Anoter user friendly text menu for the user to easily enter text
                scanf("%c", &temp); //Creating a temporary scan value such that the text is allowed to be read and not the \n character
                scanf("%[^~]", mess); //Reading the user input from the terminal and storing it in cipherText

                printf("\n");
                printf("Please enter the number of rails: ");
                scanf("%c", &temp); //Creating a temporary scan value such that the text is allowed to be read and not the \n character
                scanf("%d", &A);
                printf("\n");

                if (A < 2)
                {
                    printf("Invalid integer entered, please restart the program.\n");
                    return 0;
                }

                length=0; //Initialising length such that it doesn't start at a random integer
                while (mess[length] != '\0') //Finding the length of the cipherText string
                {
                    length++;
                }
                railFence(mess, ciph, length, A); //Passing information to the function such that it can decrypt the message
            }

            else if (x==2) //Case for reading text from cipherText[] for use in the decrytion algorithim
            {
                if (messageFILE == NULL) //Case for if the file does not exist, it promotes an error message
                {
                    printf("Error opening file.\n");
                    return 0;
                }
                else //Case for if there is a file and it will begin to write the contents to a string
                {
                    i=0;
                    while ((c = fgetc(messageFILE)) != EOF) //Checking if the postion is at the end of the fiel or not
                    {
                        mess[i] = c;
                        i++;
                    }
                    mess[i]= '\0'; //Making the last character of a string a terminating character
                }

                fseek(messageFILE, 0L, SEEK_END); //Placing a point at the end of the file cipherText
                size = ftell(messageFILE); //Getting the position of the point at the end of the file and hence, the size
                if (size < 4) //Case for if the file holds insufficient information, it then promotes an error message
                {
                    printf("Error detecting file content.\n");
                    return 0;
                }
                fclose(messageFILE); //Closes the cipherText file

                i=0;
                length=0;
                while (mess[i] != '\0') //Assigning a length value to the length varaible based on the size of the string
                {
                    mess[i++];
                    length++;
                }

                printf("Please enter the number of rails: ");
                scanf("%c", &temp); //Creating a temporary scan value such that the text is allowed to be read and not the \n character
                scanf("%d", &A);
                printf("\n");
                railFence(mess, ciph, length, A); //Passing information to the function such that it can decrypt the message
            }

            else //Case for the user iputting an invalid integer
            {
                printf("Invalid integer entered, please restart the program.\n");
            }

        break;

        case 2: //Case for the user to encrypt a message based on the 2-level rail fence cipher
            printf("Please choose an option on how to read the text:\n\n"); //Another user friendly menu to choose how to read the text.
            printf("1 - Write text in the console.\n\n");
            printf("2 - Read text from a file.\n\n");
            printf("Please enter an integer corresponding to a program: ");
            scanf("%d", &x); //Reading user input on what the program should then execute

            if (x==1) //Case for reading text from stdin for use in the decryption algorithim
            {
                fclose(messageFILE); //Closes the cipherText file

                printf("Please enter text to encrypt (with a terminating '~' char): "); //Anoter user friendly text menu for the user to easily enter text
                scanf("%c", &temp); //Creating a temporary scan value such that the text is allowed to be read and not the \n character
                scanf("%[^~]", mess); //Reading the user input from the terminal and storing it in cipherText

                printf("\n");
                printf("Please enter the number of rails for A and B: ");
                scanf("%c", &temp); //Creating a temporary scan value such that the text is allowed to be read and not the \n character
                scanf("%d %d", &A, &B);
                printf("\n");

                if (B>A || B<2) //Failsafe such that A is nto less than B and B is greater than 1
                {
                    printf("Invalid integer entered, please restart the program.\n");
                    return 0;
                }
                length=0; //Initialising length such that it doesn't start at a random integer
                while (mess[length] != '\0') //Finding the length of the cipherText string
                {
                    length++;
                }
                railFence2(mess, ciph, length, A, B, 1); //Passing information to the function such that it can decrypt the message
            }

            else if (x==2) //Case for reading text from cipherText[] for use in the decrytion algorithim
            {
                if (messageFILE == NULL) //Case for if the file does not exist, it promotes an error message
                {
                    printf("Error opening file.\n");
                    return 0;
                }
                else //Case for if there is a file and it will begin to write the contents to a string
                {
                    i=0;
                    while ((c = fgetc(messageFILE)) != EOF) //Checking if the postion is at the end of the fiel or not
                    {
                        mess[i] = c;
                        i++;
                    }
                    mess[i]= '\0'; //Making the last character of a string a terminating character
                }

                fseek(messageFILE, 0L, SEEK_END); //Placing a point at the end of the file cipherText
                size = ftell(messageFILE); //Getting the position of the point at the end of the file and hence, the size
                if (size < 4) //Case for if the file holds insufficient information, it then promotes an error message
                {
                    printf("Error detecting file content.\n");
                    return 0;
                }
                fclose(messageFILE); //Closes the cipherText file

                i=0;
                length=0;
                while (mess[i] != '\0') //Assigning a length value to the length varaible based on the size of the string
                {
                    mess[i++];
                    length++;
                }

                printf("Please enter the number of rails for A and B: ");
                scanf("%c", &temp); //Creating a temporary scan value such that the text is allowed to be read and not the \n character
                scanf("%d %d", &A, &B);
                printf("\n");
                if (B>A || B<2) //Failsafe such that A is not less than B and B is greater than 1
                {
                    printf("Invalid integer entered, please restart the program.\n");
                    return 0;
                }
                railFence2(mess, ciph, length, A, B, 1); //Passing information to the function such that it can decrypt the message
            }

            else //Case for the user iputting an invalid integer
            {
                printf("Invalid integer entered, please restart the program.\n");
            }
        break;

        case 3: //Case for the user to decrypt a message based on the 2-Level rail-fence cipher
            printf("Please choose an option on how to read the text:\n\n"); //Another user friendly menu to choose how to read the text.
            printf("1 - Write text in the console.\n\n");
            printf("2 - Read text from a file.\n\n");
            printf("Please enter an integer corresponding to a program: ");
            scanf("%d", &x); //Reading user input on what the program should then execute

            if (x==1) //Case for reading text from stdin for use in the decryption algorithim
            {
                fclose(cipherTextFILE); //Closes the cipherText file

                printf("Please enter text to decrypt (with a terminating '~' char): "); //Anoter user friendly text menu for the user to easily enter text
                scanf("%c", &temp); //Creating a temporary scan value such that the text is allowed to be read and not the \n character
                scanf("%[^~]", ciph); //Reading the user input from the terminal and storing it in cipherText

                printf("\n");
                printf("Please enter the number of rails for A and B: ");
                scanf("%c", &temp); //Creating a temporary scan value such that the text is allowed to be read and not the \n character
                scanf("%d %d", &A, &B);
                printf("\n");

                if (B>A || B<2) //Failsafe such that A is nto less than B and B is greater than 1
                {
                    printf("Invalid integer entered, please restart the program.\n");
                    return 0;
                }
                length=0; //Initialising length such that it doesn't start at a random integer
                while (ciph[length] != '\0') //Finding the length of the cipherText string
                {
                    length++;
                }
                railFence2(mess, ciph, length, A, B, 0); //Passing information to the function such that it can decrypt the message
            }

            else if (x==2) //Case for reading text from cipherText[] for use in the decrytion algorithim
            {
                if (cipherTextFILE == NULL) //Case for if the file does not exist, it promotes an error message
                {
                    printf("Error opening file.\n");
                    return 0;
                }
                else //Case for if there is a file and it will begin to write the contents to a string
                {
                    i=0;
                    while ((c = fgetc(cipherTextFILE)) != EOF) //Checking if the postion is at the end of the fiel or not
                    {
                        ciph[i] = c;
                        i++;
                    }
                    ciph[i]= '\0'; //Making the last character of a string a terminating character
                }

                fseek(cipherTextFILE, 0L, SEEK_END); //Placing a point at the end of the file cipherText
                size = ftell(cipherTextFILE); //Getting the position of the point at the end of the file and hence, the size
                if (size < 5) //Case for if the file holds insufficient information, it then promotes an error message
                {
                    printf("Error detecting file content.\n");
                    return 0;
                }
                fclose(cipherTextFILE); //Closes the cipherText file

                i=0;
                length=0;
                while (ciph[i] != '\0') //Assigning a length value to the length varaible based on the size of the string
                {
                    ciph[i++];
                    length++;
                }

                printf("Please enter the number of rails for A and B: ");
                scanf("%c", &temp); //Creating a temporary scan value such that the text is allowed to be read and not the \n character
                scanf("%d %d", &A, &B);
                printf("\n");
                if (B>A || B<2) //Failsafe such that A is not less than B and B is greater than 1
                {
                    printf("Invalid integer entered, please restart the program.\n");
                    return 0;
                }
                railFence2(mess, ciph, length, A, B, 0); //Passing information to the function such that it can decrypt the message
            }

            else //Case for the user iputting an invalid integer
            {
                printf("Invalid integer entered, please restart the program.\n");
            }
        break;

        default:
        printf("Invalid integer entered, please restart the program.\n");
        return 0;
    }
}

/*
This function allows for the classical rail cipher to be executed in encryption mode.

The function takes inputs passed to it via int main() such as the message to be encrypted, the cipherText it reads to, length of the string it is passed and number of rails.

The function does not return anything and the final ouput is printed via the function and written into a file via the function as well. So all outputs are sent inside the
function and nothing is passed back to int main().

Theoretically there are no limitations to string length and number of rails it can use, provided it passes the failsafes incorporated in the int main() section to get here.
However this works proved the user does not include the character '`' in their input. This is a character not used much and as so it was chosen. If the user inputs this
character (located aboved the TAB key traditionally) it will break.

Flow control goes as follows:
 - Creating a 2D array and setting all the characters to a '`' character
 - Filling in all the spaces where a letter should go with a '#' character
 - Filing in all the '#' characters with the message
 - Reading the 2D array for the encrypted text and writing to cipherText and printing it
*/
void railFence(char *message, char *cipherText, int length, int A)
{
    FILE *cipherTextFILE; //Declaring pointers to files and opening them for use later on in this function
    cipherTextFILE = fopen("cipherText.txt", "w"); //Opening the files in write-only mode for use in the function

    int RailFenceArray[A][length]; //Declaring a 2D array for use in encrypting and decrypting
    int i=0, j=0; //Declaring i and j to use them to specify certain characters in the 2D array
    int bound; //This is used as a state variable to determine what the rail fence is doing
    char mess[length]; //Creating string for message to use in the function further on
    char ciph[length]; //Creating string for cipherText to use in the fucntion further on
    int k; //Declaring a variable k for use as a counter in further applications
    int c; //Declaring a variable c as a placeholder for a char's ASCII value
    strcpy(mess, message); //Copying string from message to mess
    strcpy(ciph, cipherText); //Copying string from cipherText to ciph

     while (i < A) //Making all elements of the array a '`' char, but it is stored as its ASCII value
    {
        RailFenceArray[i][j] =96;
        j++;
        if (j == length) //Checking if the array has reached the end of the row, if so it goes to the next line
        {
            i++;
            j=0;
        }
    }
    i=0; //Resetting i and j to use in further applications
    j=0;
    bound=0; //Initialisng bound such that the cipher starts at the top left of the 2D Array


    while (j < length) //Making elements of the array where a letter would be a '#' char, but it is stored as its ASCII value
    {
        if (i == 0 && bound == 0) //Determining the state such that the cipher goes down the zig-zag
        {
            while (i < (A-1)) //Setting all elements that fall along the down zig to the '#' character as its ASCII value
            {
                RailFenceArray[i][j] = 35;
                i++;
                j++;
                if (j == length) //This is the first of many exit conditons statements in the defining the zig-zag section. It controls the array such that it doesn't extend further than allowed.
                {
                    break;
                }
            }
            bound = 1;
        }
        if (j == length)
        {
            break;
        }


        if (i == (A-1) && bound == 1) //Determining the state such that the cipher goes up the zig-zag
        {
            while (i > 0) //Setting all elements that fall along the up zag to the '#' character as its ASCII value
            {
                RailFenceArray[i][j] = 35;
                i--;
                j++;
                if (j == length)
                {
                    break;
                }
                bound = 0;
            }
        }
    }

    i=0; //Resetting i and j to use in further applications
    j=0;
    k=0; //Initialising k for use such that it doesn't start in the middle of the string

    while (j < length) //Filling in the 2D array with all the ASCII values of the letters in their corresponding spots
    {
        while (i < A) //Filling in the first column with the right characters
        {
            if (RailFenceArray[i][j] == 35)
            {
                c = mess[k];
                RailFenceArray[i][j] = c;
                k++;
            }
            i++;
            if (k == length) //Creating a failsafe such that the program will not allow the access to characters outside the string's length
            {
                break;
            }
        }
        i=0;
        j++; //Once column is complete this jumps to the next column
        if (k == length)
        {
            break;
        }
    }

    i=0; //Resetting i and j to use in further applications
    j=0;
    k=0; //Initialising k for use such that it doesn't start in the middle of the string

    while (i < A) //Reading the 2D array and filling in ciph[] with the encrypted message
    {
        while (j < length) //Reading the first row with the right characters
        {
            if (RailFenceArray[i][j] != 96) //Filling in ciphertext with the correct characters
            {
                ciph[k] = RailFenceArray[i][j];
                k++;
            }
            j++;
            if (k == length) //Creating a failsafe such that the program will not allow the access to characters outside the string's length
            {
                break;
            }
        }
        j=0;
        i++; //Once row is complete this jumps to the next row
        if (k == length)
        {
            break;
        }
    }
    printf("The encrypted message is: %s", ciph); //Prints the decrypted message to the terminal via stdout()
    fprintf(cipherTextFILE, "%s", ciph); //Places the mess[] string into an output file
    fclose(cipherTextFILE); //Closes the cipherText file with the encrypted message in it
}

/*
This function allows for the 2-level rail cipher to be executed in encryption or decryption mode.

The function takes inputs passed to it via int main() such as the message to be encrypted, the cipherText to be decrypted, length of the string it is passed,
number of rails for A & B and which mode it should run in (encryption or decryption).

The function does not return anything and the final ouput is printed via the function and written into a file via the function as well. So all outputs are sent inside the
function and nothing is passed back to int main().

Theoretically there are no limitations to string length and number of rails it can use, provided it passes the failsafes incorporated in the int main() section to get here.
However this works proved the user does not include the character '`' in their input. This is a character not used much and as so it was chosen. If the user inputs this
character (located aboved the TAB key traditionally) it will break.

Flow control goes as follows:
 - Creating a 2D array and setting all the characters to a '`' character
 - Filling in all the spaces where a letter should go with a '#' character
 - Choosing which mode to run in (encryption/decryption)
 - Filing in all the '#' characters with the message or cipherText
 - Reading the 2D array for the text and writing to a file and printing it
*/
void railFence2(char *message, char *cipherText, int length, int A, int B, int dir)
{
    FILE *messageFILE; //Declaring pointers to files and opening them for use later on in this function
    FILE *cipherTextFILE;

    int RailFenceArray[A][length]; //Declaring a 2D array for use in encrypting and decrypting
    int i=0, j=0; //Declaring i and j to use them to specify certain characters in the 2D array
    int bound; //This is used as a state variable to determine what the rail fence is doing
    char mess[length]; //Creating string for message to use in the function further on
    char ciph[length]; //Creating string for cipherText to use in the fucntion further on
    int k; //Declaring a variable k for use as a counter in further applications
    int c; //Declaring a variable c as a placeholder for a char's ASCII value
    strcpy(mess, message); //Copying string from message to mess
    strcpy(ciph, cipherText); //Copying string from cipherText to ciph

    while (i < A) //Making all elements of the array a '`' char, but it is stored as its ASCII value
    {
        RailFenceArray[i][j] = 96;
        j++;
        if (j == length) //Checking if the array has reached the end of the row, if so it goes to the next line
        {
            i++;
            j=0;
        }
    }
    i=0; //Resetting i and j to use in further applications
    j=0;
    bound=0; //Initialisng bound such that the cipher starts at the top left of the 2D Array


    while (j < length) //Making elements of the array where a letter would be a '#' char, but it is stored as its ASCII value
    {
        if (i == 0 && bound == 0) //Determining the state such that the cipher goes down the long zig-zag
        {
            while (i < (A-1)) //Setting all elements that fall along the large down zig to the '#' character as its ASCII value
            {
                RailFenceArray[i][j] = 35;
                i++;
                j++;
                if (j == length) //This is the first of many exit conditons statements in the defining the zig-zag section. It controls the array such that it doesn't extend further than allowed.
                {
                    break;
                }
            }
            bound = 1;
        }
        if (j == length)
        {
            break;
        }


        if (i == (A-1) && bound == 1) //Determining the state such that the cipher goes up the small zig-zag
        {
            while (i > (A-B)) //Setting all elements that fall along the small up zag to the '#' character as its ASCII value
            {
                RailFenceArray[i][j] = 35;
                i--;
                j++;
                if (j == length)
                {
                    break;
                }
            }
            bound = 2;
        }
        if (j == length)
        {
            break;
        }


        if (i == (A-B) && bound == 2) //Determining the state such that the cipher goes down the small zig-zag;
        {
            while (i < (A-1)) //Setting all elements that fall along the small down zig to the '#' character as its ASCII value
            {
                RailFenceArray[i][j] = 35;
                i++;
                j++;
                if (j == length)
                {
                    break;
                }
            }
            bound = 3;
        }
        if (j == length)
        {
            break;
        }


        if (i == (A-1) && bound == 3) //Determining the state such that the cipher goes up the large zig-zag
        {
            while (i > 0) //Setting all elements that fall along the large up zag to the '#' character as its ASCII value
            {
                RailFenceArray[i][j] = 35;
                i--;
                j++;
                if (j == length)
                {
                    break;
                }
                bound = 0;
            }
        }
    }
    i=0;
    j=0;

    switch (dir) //Reads state from input dir on whether to encrypt or decrypt a 2-level rail fence
    {
        case 0: //Case for the fucntion to decrypt a message from ciphertex[] and write to message[]
            messageFILE = fopen("message.txt", "w"); //Opening the files in write-only mode for use in the function

            i=0; //Resetting i and j to use in further applications
            j=0;
            k=0; //Initialising k for use such that it doesn't start in the middle of the string

            while (i < A) //Filling in the 2D array with all the ASCII values of the letters in their corresponding spots
            {
                while (j < length) //Filling in the first row with the right characters
                {
                    if (RailFenceArray[i][j] == 35)
                    {
                        c = ciph[k];
                        RailFenceArray[i][j] = c;
                        k++;
                    }
                    j++;
                    if (k == length) //Creating a failsafe such that the program will not allow the access to characters outside the string's length
                    {
                        break;
                    }
                }
                j=0;
                i++; //Once row is complete this jumps to the next row
                if (k == length)
                {
                    break;
                }
            }
            i=0; //Resetting i and j to use in further applications
            j=0;
            k=0; //Initialising k for use such that it doesn't start in the middle of the string

            while (j < length) //Reading the 2D array and filling in mess[] with the decrypted message
            {
                while (i < A) //Reading the first row with the right characters
                {
                    if (RailFenceArray[i][j] != 96) //Filling in message with the correct characters
                    {
                        mess[k] = RailFenceArray[i][j];
                        k++;
                    }
                    i++;
                    if (k == length) //Creating a failsafe such that the program will not allow the access to characters outside the string's length
                    {
                        break;
                    }
                }
                i=0;
                j++; //Once column is complete this jumps to the next column
                if (k == length)
                {
                    break;
                }
            }
            printf("The decrypted message is: %s", mess); //Prints the decrypted message to the terminal via stdout()
            fprintf(messageFILE, "%s", mess); //Places the mess[] string into an output file
            printf("\n");
            fclose(messageFILE); //Closes the text file with the decrypted message in it

        break;

        case 1: //Case for the fucntion to encrypt a message from message[] and write to cipherText[]
            cipherTextFILE = fopen("cipherText.txt", "w");

            i=0; //Resetting i and j to use in further applications
            j=0;
            k=0; //Initialising k for use such that it doesn't start in the middle of the string

            while (j < length) //Filling in the 2D array with all the ASCII values of the letters in their corresponding spots
            {
                while (i < A) //Filling in the first column with the right characters
                {
                    if (RailFenceArray[i][j] == 35)
                    {
                        c = mess[k];
                        RailFenceArray[i][j] = c;
                        k++;
                    }
                    i++;
                    if (k == length) //Creating a failsafe such that the program will not allow the access to characters outside the string's length
                    {
                        break;
                    }
                }
                i=0;
                j++; //Once column is complete this jumps to the next column
                if (k == length)
                {
                    break;
                }
            }

            i=0; //Resetting i and j to use in further applications
            j=0;
            k=0; //Initialising k for use such that it doesn't start in the middle of the string

            while (i < A) //Reading the 2D array and filling in ciph[] with the encrypted message
            {
                while (j < length) //Reading the first row with the right characters
                {
                    if (RailFenceArray[i][j] != 96) //Filling in ciphertext with the correct characters
                    {
                        ciph[k] = RailFenceArray[i][j];
                        k++;
                    }
                    j++;
                    if (k == length) //Creating a failsafe such that the program will not allow the access to characters outside the string's length
                    {
                        break;
                    }
                }
                j=0;
                i++; //Once row is complete this jumps to the next row
                if (k == length)
                {
                    break;
                }
            }
            printf("The encrypted message is: %s", ciph); //Prints the decrypted message to the terminal via stdout()
            fprintf(cipherTextFILE, "%s", ciph); //Places the mess[] string into an output file
            printf("\n");
            fclose(cipherTextFILE); //Closes the cipherText file with the encrypted message in it

        break;
    }
}
