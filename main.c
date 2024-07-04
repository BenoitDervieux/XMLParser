#include <stdlib.h>
#include <stdio.h>
#include "XMLparser.h"

int main() {

    XMLDocument doc;
    if(loadXMLDocument(&doc, "test.xml") == 1) {
        printf("Document Loaded");
        
    }
    return 0;
}