#include <stdlib.h>
#include <stdio.h>
#include "XMLparser.h"

int main() {

    XMLDocument doc;
    XMLNodeList list;
    if(loadXMLDocument(&doc, "test.xml", &list) == 1) {
        XMLNodeList_print(&list);
        printf("Document Loaded\n");
        
    }
    return 0;
}