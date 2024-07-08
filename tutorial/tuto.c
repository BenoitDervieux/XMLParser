#include <stdio.h>
#include <string.h>
#include <stdlib.h>



int main() {
    // Open the document
    FILE* file = fopen("original.txt", "r");
    if (file == NULL) {
        printf("Error opening file\n");
        return 1;
    }
    // Get to the end and get the size
    fseek(file, 0, SEEK_END);
    int size = ftell(file);
    fseek(file, 0, SEEK_SET);
    // Setup a first buffer
    char* buffer = (char*) malloc(sizeof(char) * size + 1);

    // Store the values in the buffer
    for (int i = 0; i < size; i++) {
        buffer[i] = fgetc(file);
    }
    buffer[size] = '\0';

    // Take the word
    const char* word = "Hellololo";
    // Get the length of the word
    size_t length = strlen(word);

    printf("%s\n", buffer);
    printf("%s\n", word);
    printf("Length word: %ld\n", length);

    // [TODO] --> Make the insertion operation here (So take out a word and replace it for instance)
    // Maybe in the future we'll need a XMLNode to access directly the start and the end
    int start = 5;
    int end = 10;
    for (int i = start; i < end; i++) {
        printf("%c", buffer[i]);
    }
    printf("\n");

    // Here what we will do is to take the size of the buffer, substract the (end - start)
    // and then add the length of the word. --> this will give the new size of the buffer
    // [PROBLEM] --> Now we need to think about the insertion but I believe this is
    // where the start "starts"

    int final_size = size - (end - start) + length + 1;
    char * buffer_fsize = (char*) malloc(sizeof(char) * final_size);

    // Here the loop will be more complicated
    // Implement the first loop up to the index 
    for (int i = 0; i < start; i++) {
        buffer_fsize[i] = buffer[i];
    }
    // Implement the second loop for the replacing word
    // from the start index to the length of the new word
    for (int i = start; i < start + length; i++) {
        buffer_fsize[i] = word[i-start];
    }
    // implement the rest of the document
    // from the end index to the end of the buffer 1
    for(int i = end; i < size; i++) {
        buffer_fsize[start + length + (i-end)] = buffer[i];
    }

    buffer_fsize[final_size] = '\0';
    printf("%s\n", buffer_fsize);
    printf("*************************\n");

    
    fclose(file);

    FILE* file_to_write_to = fopen("original.txt", "w");
    if (file_to_write_to == NULL) {
        printf("Error opening file\n");
        return 1;
    }
    for (int i = 0; i < final_size - 1; i++) {
        fputc(buffer_fsize[i], file_to_write_to);
    }
    fclose(file_to_write_to);
    return 0;
}