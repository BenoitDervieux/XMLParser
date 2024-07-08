#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../XMLparser.h"



int replaceXMLtext(const char * file_name, const char * insertion, XMLNode * node);
int main() {
    
    XMLDocument doc;
    XMLNodeList list;
    if(loadXMLDocument(&doc, "sup.xml", &list) == 1) {
        XMLNodeList_print(&list);
        XMLNode* to_replace = getNodeContent(&list, "wifi-name");
        if (to_replace != NULL)
            printf("Test printing the node: %s\n", to_replace->word);
        replaceXMLtext("sup.xml", "V&B", to_replace);
        reloadXMLDocument(&doc, "sup.xml", &list);
        to_replace = getNodeContent(&list, "wifi-pwd");
        replaceXMLtext("sup.xml", "123456", to_replace);

        printf("\n");
        printf("Document Loaded\n");
        
    }
    return 0;
 
}

int replaceXMLtext(const char * file_name, const char * insertion, XMLNode * node) {

    // Open the document
    FILE* file = fopen(file_name, "r");
    if (file == NULL) {
        printf("Error opening file\n");
        return 0;
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

    // Get the length of the insertion
    size_t length = strlen(insertion);

    // [TODO] --> Make the insertion operation here (So take out a word and replace it for instance)
    // Maybe in the future we'll need a XMLNode to access directly the start and the end
    int start = node->start;
    int end = node->end - 1;
    printf("On a le start et et le end: %d et %d\n", start, end);
    for (int i = start; i < end; i++) {
        printf("%c", buffer[i]);
    }
    printf("\n");

    // Here what we will do is to take the size of the buffer, substract the (end - start)
    // and then add the length of the word. --> this will give the new size of the buffer
    int final_size = size - (end - start) + length + 1;
    char * buffer_fsize = (char*) malloc(sizeof(char) * final_size);

    // Implement the first loop up to the index 
    for (int i = 0; i < start; i++) {
        buffer_fsize[i] = buffer[i];
    }
    // Implement the second loop for the replacing word
    // from the start index to the length of the new word
    for (int i = start; i < start + length; i++) {
        buffer_fsize[i] = insertion[i-start];
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

    FILE* file_to_write_to = fopen(file_name, "w");
    if (file_to_write_to == NULL) {
        printf("Error opening file\n");
        return 0;
    }
    for (int i = 0; i < final_size - 1; i++) {
        fputc(buffer_fsize[i], file_to_write_to);
    }
    fclose(file_to_write_to);
    return 1;
}