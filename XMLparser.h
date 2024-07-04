#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <stdlib.h>
#include <stdio.h>

#ifndef TRUE
    #define TRUE 1
#endif
#ifndef FALSE
    #define FALSE 0
#endif


struct _XMLDocument 
{
    XMLNode* root;
};
typedef struct _XMLDocument XMLDocument;

struct _XMLNode
{
    char * word;
    char * type;
    int degree;
    _XMLNode* prev;
    _XMLNode* next;

};
typedef struct _XMLNode XMLNode;

struct _XMLNodeList
{
    int heap_size;
    int size;
    struct _XMLNode** data;
};
typedef struct _XMLNodeList XMLNodeList;


XMLNode* XMLNode_new(XMLNode* parent);
void XMLNode_free(XMLNode* node);
void XMLNodeList_init(XMLNodeList* list);
void XMLNodeList_add(XMLNodeList* list, struct _XMLNode* node);
void XMLNodeList_free(XMLNodeList* list);

int loadXMLDocument(XMLDocument* doc, const char* path);
int starts_with(const char *str, const char ch );

int loadXMLDocument(XMLDocument* doc, const char* path) {
    FILE* file = fopen(path, "r");
    if (file == NULL) {
        printf("Error opening file\n");
        return 1;
    }

    char c;
    char buffer[256];
    int d = 0;
    int tagBool = FALSE;
    int tagContent = FALSE;
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
                    printf("Tag node: %s\n", buffer);
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