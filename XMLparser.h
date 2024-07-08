#ifndef XMLPARSER_H
#define XMLPARSER_H

// Disclaimer: I took a lot of inspiration from this person: https://github.com/jonahisadev/littlexml/blob/master/lxml.h
// I tried to follow his youtube tutorial first: https://www.youtube.com/watch?v=kPFYfTvMRs8&t=3898s

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef TRUE
    #define TRUE 1
#endif
#ifndef FALSE
    #define FALSE 0
#endif



struct _XMLNode
{
    char* word;
    char* type;
    int degree;
    long start;
    long end;

};
typedef struct _XMLNode XMLNode;

struct _XMLDocument 
{
    XMLNode* root;
};
typedef struct _XMLDocument XMLDocument;

struct _XMLNodeList
{
    int heap_size;
    int size;
    struct _XMLNode** data;
};
typedef struct _XMLNodeList XMLNodeList;

int starts_with(const char *str, const char ch );
int loadXMLDocument(XMLDocument* doc, const char* path, XMLNodeList* list);
XMLNode* XMLNode_init();
XMLNode* XMLNode_new(XMLNode* parent);
void XMLNode_free(XMLNode* node);
void XMLNodeList_init(XMLNodeList* list);
void XMLNodeList_add(XMLNodeList* list, struct _XMLNode* node);
void XMLNodeList_free(XMLNodeList* list);
void XMLNodeList_print(XMLNodeList* list);
char* XMLNode_getType(XMLNodeList* list,  char* node);
int XMLNode_getDegree(XMLNodeList* list,  char* node);
long int XMLNode_getStart(XMLNodeList* list,  char* node);
long int XMLNode_getEnd(XMLNodeList* list, char* node);
char* XMLNode_getNode(XMLNodeList* list,  char* text);
char* getLast(XMLNodeList* list);
XMLNode * getNodeContent(XMLNodeList * list, const char* node);
int reloadXMLDocument(XMLDocument* doc, const char* path, XMLNodeList* list);
int replaceXMLtext(const char * file_name, const char * insertion, XMLNode * node);


int loadXMLDocument(XMLDocument* doc, const char* path, XMLNodeList* list) {
    FILE* file = fopen(path, "r");
    if (file == NULL) {
        printf("Error opening file\n");
        return 1;
    }
    
    // The idea is to initialize a node, renew it every while
    // add every elements one after the other
    // and start again
    // So 1 - we intialize a node
    // 2 - we add its attribute when we have them
    // 3 - we add it into a wider linked list

    fseek(file, 0, SEEK_END);
    int size = ftell(file);
    fseek(file, 0, SEEK_SET);
    char* buffer = (char*) malloc(sizeof(char) * size + 1);
    //
    // Variables
    //
    char c;
    int d = 0;
    int tagBool = FALSE;
    int tagContent = FALSE;

    // Node variables
    char wordu[256];
    char type[3][20] = {
        "attribute",
        "key",
        "value"
    };
    int degree=0;
    struct _XMLNode* prev;


    doc->root = XMLNode_init();
    XMLNode* current_node = doc->root;
    XMLNodeList* node_list = list;
    XMLNodeList_init(node_list);

    while ((c = fgetc(file)) != EOF) {

        // long position;
        // position = ftell(file);
        // printf("\nCurrent file position: %ld, - Char read: %c", position, c);

        // Booleans to know if we are in a tag
        // and cut or not the content
        if(tagContent && c == '\n') {
            tagContent = FALSE;
        }
        // Don't know the whole structure of XML but here when it comes back to the line
        // we empty the buffer. Possibility to optimize here [TODO]
        if (c == '\n' && d > 0) {
            for (int i = 0; i < d + 1; i++)
                buffer[i] = 0;
            d=0;
        }

        //Recognize end of tag
        if (c == '>') {

            if (tagBool) {
                // This is where we catch the end of a tag, need to be carefull with the index of the 
                // tag. As we said, ftell starts with 1, not 0
                current_node->end = ftell(file);
                // Here we check if we are in the beginning of a tag
                // [TODO] --> To implement a way to define attributes here
                // Not implemented on purpose
                if (!starts_with(buffer, '/')) {
                    // Increase the degree (the depth of the tag)
                    degree++;
                    // Copy the content of the buffer inside the word
                    for (int i = 0; i < d + 1; i++) {
                        wordu[i] = buffer[i];
                    }
                    // Add the new node
                    // strdup --> function to create a kind of new pointer automatically
                    current_node->word = strdup(wordu);
                    current_node->type = strdup(type[1]);
                    current_node->degree = degree;
                    // Add it to the list
                    XMLNodeList_add(node_list, current_node);
                    // Here when we have the position we get the beginning of the index
                    // if (!strcmp(getLast(node_list),"test"))
                    //     printf("Position: %ld\n", ftell(file));
                }

                // Decrease the degree if we encounter a closing tag
                if (starts_with(buffer, '/')) {
                    degree--;
                }
                // Reset the buffer
                for (int i = 0; i < d + 1; i++)
                    buffer[i] = 0;
                d=0;
            }

            // Reset the booleans
            tagBool = FALSE;
            tagContent = TRUE;
            current_node->start = ftell(file);
        }

        // Copies tag in buffer
        if(tagBool && c!= '>' && c != '<') {
            buffer[d] = c;
            buffer[d+1] = '\0';
            d++;
        }

        // Copies content in buffer
        // Consider that the buffer is smaller than 256 char
        if(tagContent && c!= '>' && c != '<') {
            buffer[d] = c;
            buffer[d+1] = '\0';
            d++;
        }

        // Recognize beginning of tag
        if (c == '<') {
            // printf("Position beginning: %ld\n", ftell(file));
            // We get the start position of any node here, the position starts with 1
            // but in ftell terms it's the 0. However, we'll have to add 1
            if (tagContent)
                current_node->end = ftell(file);
            else 
                current_node->start = ftell(file);
            if(tagContent) {
                for (int i = 0; i < d + 1; i++) {
                    wordu[i] = buffer[i];
                }
                current_node->word = strdup(wordu);
                current_node->type = strdup(type[2]);
                current_node->degree = degree;
                // if (!strcmp(getLast(node_list),"test"))
                //     printf("Position: %ld\n", ftell(file));
                XMLNodeList_add(node_list, current_node);
                for (int i = 0; i < d + 1; i++)
                    buffer[i] = 0;
                d=0;
                
            }

            // Reset the booleans
            tagContent = FALSE;
            tagBool = TRUE;
        }
    
    }
    printf("\n");
    fclose(file);
    return 1;
}


// Check if the string starts with a specific character 
int starts_with(const char *str, const char ch ){
    return str[0] == ch;
}

// Instantiate a new node using a parent node
XMLNode* XMLNode_new(XMLNode* parent) {
    XMLNode* node = (XMLNode*)malloc(sizeof(XMLNode));;
    node->word = parent->word;
    node->type = parent->type;
    node->degree = parent->degree;
    node->start = parent->start;
    node->end = parent->end;
    return node;
}

// Free the node
void XMLNode_free(XMLNode* node) {
    if (node->word != NULL) {
        free(node->word);
    }
    if (node->type != NULL) {
        free(node->type);
    }
    node->degree = 0;
    node->start = 0;
    node->end = 0;
    // free(node);
};

// Initialize a new Node
XMLNode* XMLNode_init() {
    XMLNode* node = (XMLNode*)malloc(sizeof(XMLNode));;
    node->word = NULL;
    node->type = NULL;
    node->degree = 0;
    node->start = 0;
    node->end = 0;
    return node;
}

// Initialize the list of nodes
void XMLNodeList_init(XMLNodeList* list)
{
    list->heap_size = 1;
    list->size = 0;
    list->data = (XMLNode**) malloc(sizeof(XMLNode*) * list->heap_size);
}

// Add a new node
void XMLNodeList_add(XMLNodeList* list, XMLNode* node)
{
    while (list->size >= list->heap_size) {
        list->heap_size *= 2;
        list->data = (XMLNode**) realloc(list->data, sizeof(XMLNode*) * list->heap_size);
    }
    list->data[list->size++] = XMLNode_new(node);
}

// Print the list of nodes
void XMLNodeList_print(XMLNodeList* list) {
    for (int i = 0; i < list->size; i++) {
        printf("Word : %s, Type: %s, Degree: %d, Start: %ld, End: %ld\n", 
        list->data[i]->word, list->data[i]->type, list->data[i]->degree,
        list->data[i]->start, list->data[i]->end);
    }
}

// Free the list of Nodes
void XMLNodeList_free(XMLNodeList* list)
{
    if (list->data != NULL) {
        for (int i = 0; i < list->size; i++) {
            XMLNode_free(list->data[i]);
        }
        free(list->data);
    }
    list->data = NULL;
    list->heap_size = 0;
    list->size = 0;
    // free(list);
}

// Get the content the word of Node
// [TODO] --> Rename it because it feels like we get the content of the next node
// So it would be better with a "Get content node"
char* XMLNode_getWord(XMLNodeList* list,  char* node) {
    XMLNode* current;
    XMLNode* next;
    for (int i = 0; i < list->size; i++) {
        current = list->data[i];
        if (strcmp(current->word, node) == 0) {
            if (i+1 < list->size)
                return list->data[i+1]->word;
            return NULL;
        }
    }
    return NULL;
}

// Get the type of the node
char* XMLNode_getType(XMLNodeList* list,  char* node) {
    XMLNode* current;
    XMLNode* next;
    for (int i = 0; i < list->size; i++) {
        current = list->data[i];
        if (strcmp(current->word, node) == 0) {
            if (i+1 < list->size)
                return list->data[i+1]->type;
            return NULL;
        }
    }
    return NULL;
}

// Get the degree of the node
int XMLNode_getDegree(XMLNodeList* list,  char* node) {
    XMLNode* current;
    XMLNode* next;
    for (int i = 0; i < list->size; i++) {
        current = list->data[i];
        if (strcmp(current->word, node) == 0) {
            if (i+1 < list->size)
                return list->data[i+1]->degree;
            return -1;
        }
    }
    return -1;
}

// Get the start of the node
long int XMLNode_getStart(XMLNodeList* list,  char* node) {
    XMLNode* current;
    XMLNode* next;
    for (int i = 0; i < list->size; i++) {
        current = list->data[i];
        if (strcmp(current->word, node) == 0) {
            return list->data[i]->start;
        }
    }
    return -1;
}

// Get the end of the node
long int XMLNode_getEnd(XMLNodeList* list, char* node) {
    XMLNode* current;
    XMLNode* next;
    for (int i = 0; i < list->size; i++) {
        current = list->data[i];
        if (strcmp(current->word, node) == 0) {
            return list->data[i]->end;

        }
    }
    return -1;
}

// Get the previous node
// [TODO] --> Test this function because it seems as if it gives the normal amount of node
char* XMLNode_getNode(XMLNodeList* list,  char* text) {
    XMLNode* current;
    XMLNode* previous;
    for (int i = 0; i < list->size; i++) {
        current = list->data[i];
        if (strcmp(current->word, text) == 0) {
            if (i > 0)
                return list->data[i-1]->word;
            return NULL;
        }
    }
    return NULL;
}

// Get Last Node
char* getLast(XMLNodeList* list) {
    if (list->size != 0) {
        // printf("Get Last test %s\n", list->data[list->size-1]->word);
        return list->data[list->size-1]->word;
    }
    return NULL;
}

XMLNode * getNodeContent(XMLNodeList * list, const char* node) {
    XMLNode* current;
    XMLNode* next;
    for (int i = 0; i < list->size; i++) {
        current = list->data[i];
        if (strcmp(current->word, node) == 0) {
            if (i+1 < list->size)
                return list->data[i+1];
            return NULL;
        }
    }
    return NULL;
}

int reloadXMLDocument(XMLDocument* doc, const char* path, XMLNodeList* list) {
    XMLNodeList_free(list);
    XMLNodeList_init(list);
    return loadXMLDocument(doc, path, list);
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

#endif
