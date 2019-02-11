

시리얼 번호를 만들기 위해 가장 기본적으로 필요한 데이터가 제품 번호와 사용자ID일 것입니다. 이 2개의 데이터를 가지고 간단하게 시리얼 번호를 만드는 프로그램과 주어진 시리얼 번호에서 제품 번호와 사용자ID를 추출하는 프로그램을 만들어보겠습니다.

다음은 시리얼 번호를 생성하는 프로그램입니다.

```
#include <stdio.h>
#include <string.h>


#define ID_LEN 8
#define PRODUCT_LEN 1


int make_serial(char *serial, int serial_len,
                char *id, int id_len,
                char *product, int product_len)
{
    int i;
    
    if (id_len + product_len > serial_len)
    {
        return 1;
    }
    else
    {
        int slen;
        
        strncpy(serial, product, product_len);
        strncat(serial, id, id_len);

        slen = strlen(serial);
        for (i = 0; i < slen; i++)
        {
            serial[i] += 0x10;
        }
    }

    return 0;
}

int main(void)
{
    char id[32];
    char product[32];
    char serial[64] = {0,};
    
    printf("Input Product Number[1 or 2]:");
    fgets(product, 32, stdin);
    product[strlen(product) - 1] = '\0';

    printf("Input User ID[8-digits]:");
    fgets(id, 32, stdin);
    id[strlen(id) - 1] = '\0';

    make_serial(serial, 64,
                id, 32,
                product, 32);
    printf("Serial=%s\n", serial);
    
    return 0;
}
```



간단하게 만들어보기위해서 터미널에서 사용자 입력을 받도록 만들었습니다. 데이터를 파일에서 읽어도되고 설치프로그램에서 사용하는 GUI형태의 입력창을 사용해도 상관없습니다. 제가 보여드리고싶은 것은 사용자로부터 데이터를 받고 받은 데이터를 어떤 알고리즘을 이용해 가공해서 그 결과로 시리얼 번호가 생성된다는 처리 과정입니다.

이 처리 과정을 들여다보면 변할 수 있는 것을 찾을 수 있습니다. 현재 사용하는 데이터는 제품 종류와 사용자ID입니다만 시리얼 번호 생성에 필요한 데이터가 늘어날 수도 있습니다. 예를 들어 사용기간, 테스트버전과 상용버전의 구분, 특정 기능의 허용 여부 등 얼마든지 달라질 수 있습니다. 그리고 제품 종류가 현재는 2개이므로 사용자 입력에서 1개의 문자만을 받았지만 제품 갯수가 늘어날 수도 있습니다. 사용자ID도 한명의 사용자ID만 받을 수도 있지만 여러명이 동시에 사용할 수도 있습니다.

데이터 입력에 관한 사항들이 외부적인 변화 요인이라면 내부적으로는 시리얼 생성 알고리즘이 바뀔 수 있습니다. 예제에서는 숫자를 문자로 변환하는 아주 단순한 변환 알고리즘을 사용했습니다. 상용 제품에서는 암호화 알고리즘을 사용하는 경우가 일반적입니다. 시리얼 번호의 표기 방식도 문자의 조합에서 숫자와 문자의 조합으로 바뀔 수도 있고 특수문자도 필요해질 수 있습니다. 암호화 알고리즘을 사용했다면 문자열로 입력된 데이터들이 이진 데이터로 변환될 것입니다. 이진 데이터 그대로 16진수나 10진수로 표현해서 사용할 수도 있고 BASE64등의 인코딩 방식을 사용할 수도 있습니다.

다음은 시리얼 번호에서 제품 번호와 사용자ID를 추출하는 프로그램입니다.

```
#include <stdio.h>
#include <string.h>


#define ID_LEN 8
#define PRODUCT_LEN 1


int analyze_serial(char *serial, int serial_len,
                   char *id, int id_len,
                   char *product, int product_len)
{
    int i;

    if (id_len + product_len != serial_len)
    {
        return 1;
    }
    else
    {
        for (i = 0; i < serial_len; i++)
        {
            serial[i] -= 0x10;
        }

        strncpy(product, serial, product_len);
        product[product_len] = '\0';
        
        strncpy(id, serial+product_len, id_len);
        id[id_len] = '\0';
    }

    return 0;
}

int main(void)
{
    char id[32];
    char product[32];
    char serial[64] = {0,};

    printf("Input Serial Number[9-digits]:");
    fgets(serial, 64, stdin);
    serial[strlen(serial) - 1] = '\0';
    
    analyze_serial(serial, strlen(serial),
                   id, ID_LEN,
                   product, PRODUCT_LEN);
    
    printf("id=%s product=%s\n", id, product);
    return 0;
}

```


시리얼 번호에서 데이터를 추출하려면 각각의 데이터의 크기를 알아야합니다. 예제와같이 9자리의 시리얼 번호에서 처음 1자리는 제품 번호이고 그 다음의 8자리가 사용자ID임을 미리 정해야합니다. 시리얼 번호를 경우에 따라 복호화하거나해서 원본 데이터로 변환한 후 각 데이터로 분리합니다. 예제 프로그램에서는 각 자리의 문자를 숫자로 변환하고 각 데이터의 자리수에 맞게 분리했습니다.

시리얼 번호를 해독하는 이 프로그램도 시리얼 번호를 생성하는 프로그램과 마찬가지로 제품 번호나 사용자ID같은 데이터의 자리수나 데이터의 종류가 바뀌면 프로그램 전체가 바뀔 수밖에 없습니다. 아무리 각 데이터를 추출하는 함수들을 잘 모듈화해서 작성했다 하더라도 데이터의 종류가 늘어나면 함수들이 추가되야하고 시리얼 번호의 자리수도 바뀝니다.

예제 프로그램이 너무 간단해서 변화에 대처할 수 없다고 생각되시면 나름대로 좀더 변화에 유연한 프로그램을 생각해보시기 바랍니다. 과연 프로그램이 얼마나 유연해야 이런 변화들에 대처할 수 있을지, 내가 지금까지 개발해오던 방식대로 한다면 이런 변화들에 대처할 수 있을런지 되돌아볼 필요가 있습니다. 
