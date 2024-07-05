#ifndef XMLPARSER_H
#define XMLPARSER_H

// Disclaimer: I took a lot of inspiration from this person: https://github.com/jonahisadev/littlexml/blob/master/lxml.h
// I tried to follow his youtube tutorial first: 

#include <stdlib.h>
#include <stdio.h>

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
    int* degree;
    struct _XMLNode* prev;
    struct _XMLNode* next;
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
int loadXMLDocument(XMLDocument* doc, const char* path);
XMLNode* XMLNode_new(XMLNode* parent);
void XMLNode_free(XMLNode* node);
void XMLNodeList_init(XMLNodeList* list);
void XMLNodeList_add(XMLNodeList* list, struct _XMLNode* node);
void XMLNodeList_free(XMLNodeList* list);
void XMLNodeList_print(XMLNodeList* list);


int loadXMLDocument(XMLDocument* doc, const char* path) {
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

    //
    // Variables
    //
    char c;
    char buffer[256];
    int d = 0;
    int tagBool = FALSE;
    int tagContent = FALSE;

    // Node variables
    char* word;
    char* type;
    int* degree;
    struct _XMLNode* prev;


    doc->root = XMLNode_new(NULL);
    XMLNode* current_node = doc->root;
    XMLNodeList node_list = {0, 0, NULL};
    XMLNodeList_init(&node_list);

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
                    char worde[d+1];
                    for (int i = 0; i < d + 1; i++) {
                        worde[i] = buffer[i];
                    }
                    word = worde;
                    printf("Tag node pointer: %s\n", word);
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
                printf("Tag text: %s\n", buffer);
                for (int i = 0; i < d + 1; i++)
                    buffer[i] = 0;
                d=0;
            }
            tagContent = FALSE;
            tagBool = TRUE;
        }
    
    }
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
    node->word = NULL;
    node->type = NULL;
    node->degree = 0;
    node->prev = parent;
    node->next = NULL;
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
    list->data[list->size++] = node;
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
    if (node->degree)
        free(node->degree);
    // Do not know if this is smart to delete the previous, we'll se later
    //if (node->prev)
    //    free(node->prev);
    if (node->next)
        free(node->next);
    free(node);
}
