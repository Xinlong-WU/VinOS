#include "utils.h"
#include "types.h"
#include "timer.h"

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

char timeStr[9] = "00:00:00";

char* timefmt(struct time T){
    char * str = "12:00:00";
    itoa(T.hour,str);
    if(T.hour)
        if(T.hour<10)
            timeStr[1]=str[0];
        else{
            timeStr[0] = str[0];
            timeStr[1] = str[1];
        }

    itoa(T.minit,str);
    if(T.minit)
        if(T.minit>10){
            timeStr[3] = str[0];
            timeStr[4] = str[1];
        }
        else
            timeStr[4]=str[0];

    itoa(T.second,str);
    if(T.second)
        if(T.second>10){
            timeStr[6] = str[0];
            timeStr[7] = str[1];
        }
        else
            timeStr[7]=str[0];
    
    return timeStr;
}

struct time now(){
    const uint32_t timestamp = _tick;
    struct time T;
    T.hour = (timestamp / (60 * 60)) % 24;
    T.minit = ((timestamp % (60 * 60)) / 60) % 60;
    T.second = timestamp % 60;
    return T;
}
