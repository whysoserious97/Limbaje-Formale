#include <stdio.h>
char rule[5][5]={
    {'#','d','#','#','#'},
    {'#','#','a','#','d'},
    {'#','#','#','b','#'},
    {'a','c','#','#','#'},
    {'#','#','#','#','#'}
    };
    fin[]={'0','0','0','0','1'};
int check(char *arr,int state,int string_index,int size){
        for (int i =0; i<5;i++){
            if (arr[string_index]==rule[state][i])
            {
                if (string_index+1==size && fin[i]=='1')
                    return 1;
                else if (string_index+1==size)
                    return 0;
                else
                    return check(arr,i,string_index+1,size);
            }
        }
    return 0;
}
int main()
{
    char arr[]="dacbc";
    int size=sizeof(arr)/sizeof(arr[0])-1;
    printf("%d\n",check(arr,0,0,size));
    return 0;
}
