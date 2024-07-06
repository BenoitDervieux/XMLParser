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
char* XMLNode_getNode(XMLNodeList* list,  char* text);


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

        //long position;
        //position = ftell(file);
        //printf("\nCurrent file position: %ld\n", position);

        if(tagContent && c == '\n') {
            tagContent = FALSE;
        }
        if (c == '\n') {
            for (int i = 0; i < d + 1; i++)
                buffer[i] = 0;
            d=0;
        }

        //Recognize end of tag
        if (c == '>') {
            if (tagBool) {
                if (!starts_with(buffer, '/')) {
                    degree++;
                    for (int i = 0; i < d + 1; i++) {
                        wordu[i] = buffer[i];
                    }
                    current_node->word = strdup(wordu);
                    current_node->type = strdup(type[1]);
                    current_node->degree = degree;
                    XMLNodeList_add(node_list, current_node);
                }
                if (starts_with(buffer, '/')) {
                    degree--;
                }
                for (int i = 0; i < d + 1; i++)
                    buffer[i] = 0;
                d=0;
            }
            tagBool = FALSE;
            tagContent = TRUE;
        }

        // Copies tag in buffer
        if(tagBool && c!= '>' && c != '<') {
            buffer[d] = c;
            buffer[d+1] = '\0';
            d++;
        }

        if(tagContent && c!= '>' && c != '<') {
            buffer[d] = c;
            buffer[d+1] = '\0';
            d++;
        }

        // Recognize beginning of tag
        if (c == '<') {
            if(tagContent) {
                for (int i = 0; i < d + 1; i++) {
                    wordu[i] = buffer[i];
                }
                current_node->word = strdup(wordu);
                current_node->type = strdup(type[2]);
                current_node->degree = degree;
                XMLNodeList_add(node_list, current_node);
                for (int i = 0; i < d + 1; i++)
                    buffer[i] = 0;
                d=0;
            }
            tagContent = FALSE;
            tagBool = TRUE;
        }
    
    }
    // XMLNodeList_print(&node_list);
    printf("\n");

    fclose(file);
    return 1;
}

#endif

int starts_with(const char *str, const char ch ){
    return str[0] == ch;
}

XMLNode* XMLNode_new(XMLNode* parent) {
    XMLNode* node = malloc(sizeof(struct _XMLNode));
    node->word = parent->word;
    node->type = parent->type;
    node->degree = parent->degree;
    return node;
}
void XMLNode_free(XMLNode* node);

XMLNode* XMLNode_init() {
    XMLNode* node = malloc(sizeof(struct _XMLNode));
    node->word = NULL;
    node->type = NULL;
    node->degree = 0;
    return node;
}
void XMLNode_free(XMLNode* node);

void XMLNodeList_init(XMLNodeList* list)
{
    list->heap_size = 1;
    list->size = 0;
    list->data = (XMLNode**) malloc(sizeof(XMLNode*) * list->heap_size);
}

void XMLNodeList_add(XMLNodeList* list, XMLNode* node)
{
    while (list->size >= list->heap_size) {
        list->heap_size *= 2;
        list->data = (XMLNode**) realloc(list->data, sizeof(XMLNode*) * list->heap_size);
    }
    list->data[list->size++] = XMLNode_new(node);
}

void XMLNodeList_print(XMLNodeList* list) {
    for (int i = 0; i < list->size; i++) {
        printf("Word : %s, Type: %s, Degree: %d\n", list->data[i]->word, list->data[i]->type, list->data[i]->degree);
    }
}

void XMLNodeList_free(XMLNodeList* list)
{
    free(list);
}

void XMLNode_free(XMLNode* node)
{
    if (node->word)
        free(node->word);
    if (node->type)
        free(node->type);
    //if (node->degree)
    //    free(node->degree);
    // Do not know if this is smart to delete the previous, we'll se later
    //if (node->prev)
    //    free(node->prev);
    // if (node->next)
    //     free(node->next);
    free(node);
}

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
