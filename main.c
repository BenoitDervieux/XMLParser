#include <stdlib.h>
#include <stdio.h>
#include "XMLparser.h"

int main() {

    XMLDocument doc;
    XMLNodeList list;
    if(loadXMLDocument(&doc, "test.xml", &list) == 1) {
        XMLNodeList_print(&list);
        // This can spot things we want
        // char* node = "test2";
        // printf("Test search content:\n      Content: %s, Type: %s, Degree: %d, Start: %ld, End: %ld\n", 
        //         XMLNode_getWord(&list, node), XMLNode_getType(&list, node), XMLNode_getDegree(&list, node)
        //         , XMLNode_getStart(&list, node), XMLNode_getEnd(&list, node)); 
        printf("\n");
        // printf("Test search node: %s\n", XMLNode_getWord(&list, "Wesh alors"));
        printf("Document Loaded\n");
        
    }
    return 0;
}
