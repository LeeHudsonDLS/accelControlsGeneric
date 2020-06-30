#include <iostream>
#include <stdio.h>
#include <cstring>

int main(){

    uint16_t total = 0;
    uint16_t result = 0;
    char* message  = "~ 01 ED 1,\"SR20A-VA-IONP-03\" ";

    for(int i =0; i < strlen(message); i++){
        total+=message[i];
    }

    printf("Total is %d\n",total);

    result = total%256;
    printf("%02X\n",result);
    printf("~%s%02X\n",message,result);
}