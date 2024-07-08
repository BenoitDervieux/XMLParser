#include <stdlib.h>
#include <stdio.h>
#include "XMLparser.h"

int main() {
    // Here we have a small 
    XMLDocument doc;
    XMLNodeList list;
    if(loadXMLDocument(&doc, "test.xml", &list) == 1) {
        XMLNodeList_print(&list);
        XMLNode* to_replace = getNodeContent(&list, "wifi-name");
        if (to_replace == NULL) {
            printf("No content\n");
            return 1;
        }
        replaceXMLtext("test.xml", "V&B", to_replace);
        reloadXMLDocument(&doc, "test.xml", &list);
        to_replace = getNodeContent(&list, "wifi-pwd");
        if (to_replace == NULL) {
            printf("No content\n");
            return 1;
        }
        replaceXMLtext("test.xml", "123456", to_replace);
        printf("\n");
        printf("Document Loaded\n");
        
    }
    return 0;
}
