#include "utils.h"
#include "types.h"
string itoa(int num,string arr){
    int length = 0;
    int tmp = num;
    while(tmp > 0){
        tmp /= 10;
        length++;
    }
    for(int i = 1;i<=length;i++){
        int mod = 1;
        for(int j = 0;j<i;j++)
            mod*=10;

        tmp = (num % mod)/(mod/10);
        switch(tmp){
            case 0:
                arr[length-i]='0';
                break;
            case 1:
                arr[length-i]='1';
                break;
            case 2:
                arr[length-i]='2';
                break;
            case 3:
                arr[length-i]='3';
                break;
            case 4:
                arr[length-i]='4';
                break;
            case 5:
                arr[length-i]='5';
                break;
            case 6:
                arr[length-i]='6';
                break;
            case 7:
                arr[length-i]='7';
                break;
            case 8:
                arr[length-i]='8';
                break;
            case 9:
                arr[length-i]='9';
                break;
        }
    }
    arr[length]='\0';
    return arr;
}

int strcpy(string str1,string str2){
    if(str1 == NULL || str2 == NULL)
        return 0;
    string p1 = str1;
    string p2 = str2;
    int res = 0;
    do{
        res = *p1 - *p2;
        *p1+=1;
        *p2+=1;
    }while(res ==0 && *(p1-1) != '\0' && *(p2-1) != '\0');

    if(res == 0){
        if(*(p1-1) == '\0')
            res -= 1;
        if(*(p2-1) == '\0')
            res += 1;
    }

    return res;
}
