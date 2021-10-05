#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG
#define NUM_ROUNDS 64
#define FILE_NAME "/root/credentials.txt"

typedef unsigned int uint32_t;
/* take 64 bits of data in v[0] and v[1] and 128 bits of key[0] - key[3] */
void encipher(unsigned int num_rounds, uint32_t v[2], uint32_t const key[4]) {
    unsigned int i;
    uint32_t v0=v[0], v1=v[1], sum=0, delta=0x9E3779B9;
    for (i=0; i < num_rounds; i++) {
        v0 += (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + key[sum & 3]);
        sum += delta;
        v1 += (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + key[(sum>>11) & 3]);
    }
    v[0]=v0; v[1]=v1;
}
 
void decipher(unsigned int num_rounds, uint32_t v[2], uint32_t const key[4]) {
    unsigned int i;
    uint32_t v0=v[0], v1=v[1], delta=0x9E3779B9, sum=delta*num_rounds;
    for (i=0; i < num_rounds; i++) {
        v1 -= (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + key[(sum>>11) & 3]);
        sum -= delta;
        v0 -= (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + key[sum & 3]);
    }
    v[0]=v0; v[1]=v1;
}

int main(int argc, char *argv[])
{
	unsigned int u[2], p[2];
	unsigned int k[4];
	FILE *f;
	char strcmd[1000];

	k[0]=0x3333; k[1]=k[2]=k[3]=0;
	if(argc == 3) {
		//u[0]=0x11111111; u[1]=0x22222222;
		// u[1]=909cfbff u[0]=84f50b7d p[1]=909cfbff p[0]=84f50b7d =>  u='admin' p='admin' 
		u[0] = u[1] = 0;
		strcpy((unsigned char *)u, argv[1]);
		p[0] = p[1] = 0;
		strcpy((unsigned char *)p, argv[2]);
		encipher(NUM_ROUNDS, u, k);
		encipher(NUM_ROUNDS, p, k);
                if((f = fopen(FILE_NAME, "wt")) == NULL) {
                        printf("error opening %s for writing", FILE_NAME);
                        return -1;
                }		
		fprintf(f, "%x %x %x %x\n", u[1], u[0], p[1], p[0]);
		fclose(f);
		printf("wrote file %s \n", FILE_NAME);
	} else if(argc == 2) {
		if((f = fopen(FILE_NAME, "rt")) == NULL) {
			printf("error opening %s for reading", FILE_NAME);
			return -1;
		}
		fscanf(f, "%x %x %x %x\n", &u[1], &u[0], &p[1], &p[0]);
		decipher(NUM_ROUNDS, u, k);
		decipher(NUM_ROUNDS, p, k);
#ifdef DEBUG
		printf("u='%s' p='%s' \n", (unsigned char*)u, (unsigned char*)p);
#endif
		sprintf(strcmd, "curl -v --cacert \"${CERTIFICATE_LOCATION}\" --user '%s:%s' \"${HAWKBIT_URL}/rest/v1/targets\" -i -X POST -H 'Content-Type: application/json;charset=UTF-8' -d \"[ {\\\"securityToken\\\" : \\\"${TARGET_TOKEN}\\\", \\\"controllerId\\\" : \\\"${TARGET_ID}\\\",\\\"name\\\" : \\\"${TARGET_ID}\\\", \\\"description\\\" : \\\"Controller ${TARGET_ID}\\\"} ]\"", (unsigned char*)u, (unsigned char*)p);
#ifdef DEBUG
		printf("strcmd=%s\n", strcmd);
#endif
		return system(strcmd);
	} else
		printf("encipher-decipher: no option\n");
	
	return 0;
}
