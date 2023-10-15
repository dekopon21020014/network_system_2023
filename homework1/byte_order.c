#include <stdio.h>
/* #include <arpa/inet.h> */

union u {
    unsigned int ipaddr;
    unsigned char doted_ipaddr[4];
};

int main(void) {
	union u u1;
    u1.ipaddr = 0xC0A80a01; // 192.168.10.1
    
    if(u1.doted_ipaddr[0] == 0xC0) {
        printf("your environment is Big Endian\n");
    } else {
        printf("your environment is Little Endian\n");
    }
    
    /*
    どうせリトルエンディアンだろうという確認
    printf("decimal: %d\n", u1.ipaddr);
    printf("hex: %x\n", u1.ipaddr);
    printf("doted: %d.%d.%d.%d\n", u1.doted_ipaddr[0],
                                   u1.doted_ipaddr[1],
           						   u1.doted_ipaddr[2],
           						   u1.doted_ipaddr[3]);
    */

    /*
    試しにビッグエンディアンにしたらどうなるか確認
    union u u2;
    u2.ipaddr = htonl(0xC0A80a01); // 192.168.10.1
    printf("decimal: %d\n", u2.ipaddr);
    printf("hex: %x\n", u2.ipaddr);
    printf("doted: %d.%d.%d.%d\n", u2.doted_ipaddr[0],
                                   u2.doted_ipaddr[1],
           						   u2.doted_ipaddr[2],
           						   u2.doted_ipaddr[3]);
    */
}
