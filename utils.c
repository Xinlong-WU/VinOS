#include "utils.h"
#include "types.h"
string itoaa(int num,string arr){
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
    return arr;
}
