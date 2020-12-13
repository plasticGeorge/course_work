#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

//������� ��� ��������� ������������� � ������������ �������� �� ���� ���������
#define MIN(a, b)(a > b ? b : a)
#define MAX(a, b)(a > b ? a : b)

/*------------------------------------------------------------------
��������������� ���������
------------------------------------------------------------------*/

//��������� ������, � ������� �������������� �����. �������� �������� ����� ������
typedef struct string{
    char* str;
    int size;
}string;

//��������� ��������� ����������. �������� �������� ������� ������� ������� ����������
typedef struct coincidence{
    char* str;
    int pos;
}coin;

/*------------------------------------------------------------------
��������������� �������
------------------------------------------------------------------*/

//������� ������ ������� �� ��������� ���������� ��������
int* prefshift_func(string* sub){
    int* prefshift = (int*)calloc(sub->size, sizeof(int));
    prefshift[0] = 0;
    for(int i = 0, j = 1; j < sub->size;)
        if(sub->str[i] == sub->str[j]){
            prefshift[j] = i + 1;
            i++; j++;
        }
        else if(i == 0){
            prefshift[j] = 0;
            j++;
        }
        else
            i = prefshift[i - 1];
    return prefshift;
}

//�������-������� - ������� ������ ��������� ����������� �������� ������ ����� ������ � �������� ���� ������.
int* suffixes_func(string* sub){
    int* suffixes = (int*)calloc(sub->size, sizeof(int));
    suffixes[sub->size - 1] = sub->size;
    for (int i = sub->size - 2, j = sub->size - 1, k; i >= 0; i--)
        if (i > j && suffixes[i + sub->size - 1 - k] < i - j)
            suffixes[i] = suffixes[i + sub->size - 1 - k];
        else {
            if (i < j)
                j = i;
            k = i;
        while (j >= 0 && sub->str[j] == sub->str[j + sub->size - 1 - k])
            j--;
        suffixes[i] = k - j;
        }
    return suffixes;
}

//������� ������ ������� �� ��������� �������� ��������
int* suffshift_func(string* sub){
    int* suffshift = (int*)calloc(sub->size, sizeof(int));
    int* suffixes = suffixes_func(sub);
    for (int i = 0; i < sub->size; i++)
        suffshift[i] = sub->size;
    int j = 0;
    for (int i = sub->size - 1; i >= 0; i--)
        if (suffixes[i] == i + 1)
            for (; j < sub->size - 1 - i; j++)
                if (suffshift[j] == sub->size)
                    suffshift[j] = sub->size - 1 - i;
    for (int i = 0; i < sub->size - 1; i++)
        suffshift[sub->size - 1 - suffixes[i]] = sub->size - 1 - i;
    free(suffixes);
    return suffshift;
}

//������� ������ ������� �� ��������� ����-������� ��� ��������� ������ � ����
int* stop_symbol_func_bm(string* sub){
    int* stop_symbol = (int*)calloc(256, sizeof(int));
    for(int i = 0; i < 256; i++){stop_symbol[i] = -1;}
    for(int i = 0; i < sub->size - 1; i++)
        stop_symbol[(int)sub->str[i]] = i;
    return stop_symbol;
}

//������� ������ ������� �� ��������� ����-������� ��� ��������� ������ � ���� � ��������
int* stop_symbol_func_bmh(string* sub){
    int* stop_symbol = (int*)calloc(256, sizeof(int));
    for(int i = 0; i < 256; i++){stop_symbol[i] = sub->size;}
    for(int i = 0; i < sub->size - 1; i++)
        stop_symbol[(int)sub->str[i]] = sub->size - 1 - i;
    return stop_symbol;
}

//������� ������ ������� �� ��������� ����-������� ��� ��������� ���-�������
int** stop_symbol_func_cht(string* sub, int alph_len){
    int** stop_symbol = (int**)calloc(alph_len, sizeof(int*));
    for(int i = 0; i < alph_len; i++)
        stop_symbol[i] = (int*)calloc(alph_len, sizeof(int));
    for (int i = 0; i < alph_len; ++i)
        for (int j = 0; j < alph_len; ++j)
            stop_symbol[i][j] = sub->size;
    for (int i = 0; i < alph_len; ++i)
        stop_symbol[i][(int)sub->str[0]] = sub->size - 1;
    for (int i = 1; i < sub->size - 1; ++i)
        stop_symbol[(int)sub->str[i - 1]][(int)sub->str[i]] = sub->size - 1 - i;
    return stop_symbol;
}

/*------------------------------------------------------------------
�������� �������
------------------------------------------------------------------*/

//�������� ����� � ������� � ������
coin* KMPSearch(string* str, string* sub){
    int* prefshift = prefshift_func(sub);
    for(int i = 0, j = 0; i < str->size;)
        if(str->str[i] == sub->str[j]){
            i++; j++;
            if(j == sub->size){
                coin* res = (coin*)malloc(sizeof(coin));
                res->pos = i - sub->size;
                int m = sub->size;
                if(res->pos < 10)
                    m += res->pos;
                else
                    m += 10;
                if((res->pos + 10) >= str->size)
                    m += str->size - res->pos - sub->size;
                else
                    m += 10;
                res->str = (char*)malloc(m + 1);
                for(int k = 0, l =(res->pos < 10?0:res->pos - 10); k < m; k++, l++)
                    res->str[k] = str->str[l];
                res->str[m] = '\0';
                return res;
            }
        }
        else if(j == 0)
            i++;
        else
            j = prefshift[j - 1];
    free(prefshift);
    return NULL;
}

//�������� ������ � ����
coin* BMSearch(string* str, string* sub){
    int* stop_symbol = stop_symbol_func_bm(sub);
    int* suffshift = suffshift_func(sub);
    for(int i = sub->size - 1, j = sub->size - 1; i < str->size;)
        if(str->str[i] == sub->str[j]){
            i--; j--;
            if(j == -1){
                coin* res = (coin*)malloc(sizeof(coin));
                res->pos = i + 1;
                int m = sub->size;
                if(res->pos < 10)
                    m += res->pos;
                else
                    m += 10;
                if((res->pos + 10) >= str->size)
                    m += str->size - res->pos - sub->size;
                else
                    m += 10;
                res->str = (char*)malloc(m + 1);
                for(int k = 0, l =(res->pos < 10?0:res->pos - 10); k < m; k++, l++)
                    res->str[k] = str->str[l];
                res->str[m] = '\0';
                return res;
            }
        }
        else if(j < sub->size - 1){
            i += MAX(sub->size - 1 - j + suffshift[j], sub->size - 1 - stop_symbol[(int)str->str[i]]);
            j = sub->size - 1;
        }
        else
            i += MAX(suffshift[j], j - stop_symbol[(int)str->str[i]]);
    free(stop_symbol);
    free(suffshift);
    return NULL;
}

//�������� ������ � ���� � ��������
coin* BMHSearch(string* str, string* sub){
    int* stop_symbol = stop_symbol_func_bmh(sub);
    for(int i = sub->size - 1, j = sub->size - 1; i < str->size;)
        if(str->str[i] == sub->str[j]){
            i--; j--;
            if(j == -1){
                coin* res = (coin*)malloc(sizeof(coin));
                res->pos = i + 1;
                int m = sub->size;
                if(res->pos < 10)
                    m += res->pos;
                else
                    m += 10;
                if((res->pos + 10) >= str->size)
                    m += str->size - res->pos - sub->size;
                else
                    m += 10;
                res->str = (char*)malloc(m + 1);
                for(int k = 0, l =(res->pos < 10?0:res->pos - 10); k < m; k++, l++)
                    res->str[k] = str->str[l];
                res->str[m] = '\0';
                return res;
            }
        }
        else if(j < sub->size - 1){
            i += sub->size - 1 - j + stop_symbol[(int)sub->str[sub->size - 1]];
            j = sub->size - 1;
        }
        else
            i += stop_symbol[(int)str->str[i]];
    free(stop_symbol);
    return NULL;
}

//�������� ��� � �������
coin* CHTSearch(string* str, string* sub){
    int** stop_symbol = stop_symbol_func_cht(sub, 256);
    int* suffshift = suffshift_func(sub);
    for(int i = sub->size - 1, j = sub->size - 1; i < str->size;)
        if(str->str[i] == sub->str[j]){
            i--; j--;
            if(j == -1){
                coin* res = (coin*)malloc(sizeof(coin));
                res->pos = i + 1;
                int m = sub->size;
                if(res->pos < 10)
                    m += res->pos;
                else
                    m += 10;
                if((res->pos + 10) >= str->size)
                    m += str->size - res->pos - sub->size;
                else
                    m += 10;
                res->str = (char*)malloc(m + 1);
                for(int k = 0, l =(res->pos < 10?0:res->pos - 10); k < m; k++, l++)
                    res->str[k] = str->str[l];
                res->str[m] = '\0';
                return res;
            }
        }
        else if(j < sub->size - 1){
            i += MAX(sub->size - 1 - j + suffshift[j], sub->size - 1 - j + stop_symbol[(int)str->str[i + sub->size - 2 - j]][(int)str->str[i + sub->size - 1 - j]]);
            j = sub->size - 1;
        }
        else
            i += MAX(suffshift[j], stop_symbol[(int)str->str[i - 1]][(int)str->str[i]]);
    for(int i = 0; i < 256; i++)
        free(stop_symbol[i]);
    free(stop_symbol);
    free(suffshift);
    return NULL;
}

/*------------------------------------------------------------------
������� �����-������
------------------------------------------------------------------*/

//������� ������, �� ������� �������������� �����
string* create_str(){
    setlocale(LC_ALL, "Rus");
    char* temp = (char*)malloc(4093);
    gets(temp);
    int n;
    for(n = 0; temp[n] != '\0'; n++){}
    if(n > 4093){
        printf("\n������ �������� ������ ��������� ���������� ��������(4093). ���������� ���������.\n");
        exit(-1);
    }
    string* res = (string*)malloc(sizeof(string));
    res->size = n;
    res->str = (char*)malloc(res->size + 1);
    for(int i = 0; i < res->size + 1; i++)
        res->str[i] = temp[i];
    free(temp);
    return res;
}

//������� ������� ���������
string* create_sub(){
    setlocale(LC_ALL, "Rus");
    char* temp = (char*)malloc(4093);
    gets(temp);
    int n;
    for(n = 0; temp[n] != '\0'; n++){}
    if(n > 4093){
        printf("\n������ ������� ������ ��������� ���������� ��������(4093). ���������� ���������.\n");
        exit(-1);
    }
    string* res = (string*)malloc(sizeof(string));
    res->size = n;
    res->str = (char*)malloc(res->size + 1);
    for(int i = 0; i < res->size + 1; i++)
        res->str[i] = temp[i];
    free(temp);
    return res;
}

//������� ������� � �������� ����������
void show_find(coin* find){
    setlocale(LC_ALL, "Rus");
    if (find)
        printf("\n���������� ����������� � ������� %u �������� ������.\n�������� ������ � ��������� ����������(������ �������� = ������ ��������� + 10 �������� ����� � 10 �������� ������, ���� ��� ��������, ����� ����������� ��������� ���������� �������� �� 10 � ��� �������): %s\n", find->pos, find->str);
    else
        printf("\n��������� �� �������!\n");
}

/*------------------------------------------------------------------
�������� �������
------------------------------------------------------------------*/

int main(){
    setlocale(LC_ALL, "Rus");
    coin* find;
    short choice;
    printf("\n������� �������� ������(����������� �� ������ - 4093 ��������): ");
    string* str = create_str();
    printf("\n������� ������� ������(����������� �� ������ - 4093 ��������): ");
    string* sub = create_sub();
    printf("\n�������� ���� �� ���������� ������ ���������:\n");
    printf("1. �������� ����� - ������� - ������\n");
    printf("2. �������� ������ - ����\n");
    printf("3. �������� ������ - ���� - ��������(���������� �������� ������ - ����)\n");
    printf("4. �������� ��� � �������(�������� ������ - ���� ��� �������� ���������)\n");
    printf("5. ����� �� ���������\n");
    printf("\n����: ");
    scanf("%hd", &choice);
    switch(choice){
        case 1:
            find = KMPSearch(str, sub);
            show_find(find);
            break;
        case 2:
            find = BMSearch(str, sub);
            show_find(find);
            break;
        case 3:
            find = BMHSearch(str, sub);
            show_find(find);
            break;
        case 4:
            find = CHTSearch(str, sub);
            show_find(find);
            break;
        case 5:
            return 0;
            break;
        default:
            printf("\n������� ������������ ��������. ���������� ���������.\n");
            exit(-1);
    }
    return 0;
}
