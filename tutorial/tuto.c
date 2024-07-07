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
    const char* word = "Hello";
    // Get the length of the word
    size_t length = strlen(word);

    printf("%s\n", buffer);
    printf("%s\n", word);
    printf("Length word: %ld\n", length);
    // [TODO] --> Make the insertion operation here (So take out a word and replace it for instance)
    // Maybe in the future we'll need a XMLNode to access directly the start and the end

    // Initialize a new variable for the total length
    int total = size + length + 1;
    printf("Length total : %d\n", total);
    // Initialize a new buffer where we will add the word
    char * buffer2 = (char*) malloc(sizeof(char) * total);

    for (int i = 0; i < size; i++) {
        buffer2[i] = buffer[i];
    }
    int index= 0;
    for (int i = size; i < total; i++) {
        buffer2[i] = word[index++];
    }
    buffer2[total] = '\0';
    printf("%s\n", buffer2);
    
    fclose(file);

    FILE* file_to_write_to = fopen("original.txt", "w");
    if (file_to_write_to == NULL) {
        printf("Error opening file\n");
        return 1;
    }
    for (int i = 0; i < total - 1; i++) {
        fputc(buffer2[i], file_to_write_to);
    }
    fclose(file_to_write_to);
    return 0;
}