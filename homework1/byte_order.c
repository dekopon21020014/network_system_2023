#include <stdio.h>
/*
#include <arpa/inet.h>
#include <math.h>
*/

#define IP_ADDRESS 0xC0A80A01 // 192.168.10.1

union u {
    unsigned int ipaddr;
    unsigned char doted_ipaddr[4];
};

int main(void) {
    union u u1; 
    u1.ipaddr = IP_ADDRESS; // 192.168.10.1
    printf("==================================================\n");
    if(u1.doted_ipaddr[0] == 0xC0) {
        printf("Byte order of your environment is Big Endian\n");
    } else if(u1.doted_ipaddr[0] == 0x01) {
        printf("Byte order of your environment is Little Endian\n");
    }
    printf("==================================================\n");

    /*
    // ここから下はただの確認
    // どうせリトルエンディアンだろうという確認
    printf("decimal: %d\n", u1.ipaddr);
    printf("hex: %x\n", u1.ipaddr);
    printf("doted: %d.%d.%d.%d\n", u1.doted_ipaddr[0],
                                   u1.doted_ipaddr[1],
           						   u1.doted_ipaddr[2],
           						   u1.doted_ipaddr[3]);


    // リトルエンデアンのintを1byteずつアクセスして取得する
    unsigned int sum1 = 0;
	int i;
    for(i = 0; i < sizeof(u1.doted_ipaddr); i++) {
        sum1 += (unsigned int)(u1.doted_ipaddr[i] * pow(256, i));
    }
    printf("sum1 = %x\n", sum1);                                   
    printf("==================================================\n");

    // 試しにビッグエンディアンにしたらどうなるか
    union u u2;
    u2.ipaddr = htonl(0xC0A80a01); // 192.168.10.1
    printf("decimal: %d\n", u2.ipaddr);
    printf("hex: %x\n", u2.ipaddr);
    printf("doted: %d.%d.%d.%d\n", u2.doted_ipaddr[0],
                                   u2.doted_ipaddr[1],
           						   u2.doted_ipaddr[2],
           						   u2.doted_ipaddr[3]);

    // ビッグエンディアンintを1byteずつアクセスして出力する
    unsigned int sum2 = 0;
    for(i = 0; i < sizeof(u2.doted_ipaddr); i++) {
        sum2 += (unsigned int)(u2.doted_ipaddr[i] * pow(256, 3-i));
    }
    printf("sum2 = %x\n", sum2);
    printf("==================================================\n");
    */
}
