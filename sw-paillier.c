#include <contiki.h>
#include <contiki-lib.h>
#include <contiki-net.h>


#include "sys/rtimer.h"
#include "pt.h"

//Relic Includes
#include "relic_err.h"
#include "relic_bn.h"
#include "relic_ec.h"
#include "relic_cp.h"
/*#include "relic.h"*/

#include <string.h>
#include <stdio.h>
#include <stdint.h>

#include <stdio.h> /* For printf() */
#include "flash-erase.h"
/*---------------------------------------------------------------------------*/
PROCESS(jan_test, "Hello jan process");
AUTOSTART_PROCESSES(&jan_test, &flash_erase_process);
/*---------------------------------------------------------------------------*/

void section(char* message){
    printf("\n\n\r");
    printf("==============\n\r %s \r\n==============\n\r", message);
}
static uint32_t plain_txt[RELIC_BN_BITS]     = { 0x11111111, 0x0000000, 0x22222222, 0x0000000, 0x33333333, 0x0000000, 0xf0000000, 0x0000000,
    0x55555555, 0x0000000, 0x66666666, 0x0000000, 0x77777777, 0x0000000, 0xf0000000, 0x0000000};

PROCESS_THREAD(jan_test, ev, data)
{
    PROCESS_BEGIN();
    printf("init:%i \n",core_init());
    printf("initialized\n");
    printf("DV_DIGS %d\n", DV_DIGS);

    int result;

    static rtimer_clock_t time;

    section("PAILLIER");

    bn_t a, b, c, d, n, l, s;
    uint8_t in[RELIC_BN_BITS / 8 + 1], out[RELIC_BN_BITS / 8 + 1];
    int in_len, out_len;

    bn_null(a);
    bn_null(b);
    bn_null(c);
    bn_null(d);
    bn_null(n);
    bn_null(l);
    bn_null(s);

    bn_new(a);
    bn_new(b);
    bn_new(c);
    bn_new(d);
    bn_new(n);
    bn_new(l);
    bn_new(s);


    section("KEY GENERATION");
    time = RTIMER_NOW();
    result = cp_phpe_gen(n, l, RELIC_BN_BITS / 2);
    time = RTIMER_NOW() - time;
    if (result != STS_OK){
        printf("Key generation failed!\n\r");
        return 1;
    }
    printf("paillier_gen(), %lu ms\n\r",
            (uint32_t)((uint64_t)time * 1000 / RTIMER_SECOND));

    in_len = bn_size_bin(n);
    out_len = RELIC_BN_BITS / 8 + 1;
    memset(in, 0, sizeof(in));
    /*rand_bytes(in + (in_len - 10), 10);*/
    memcpy(in, plain_txt, sizeof(in));

    section("ENCRYPT");
    time = RTIMER_NOW();
    cp_phpe_enc(out, &out_len, in, in_len, n);
    time = RTIMER_NOW() - time;
    if (result != STS_OK){
        printf("Encryption failed!\n\r");
        return 1;
    }
    printf("paillier_enc(), %lu ms\n\r",
            (uint32_t)((uint64_t)time * 1000 / RTIMER_SECOND));


    section("DECRYPT");
    time = RTIMER_NOW();
    cp_phpe_dec(out, in_len, out, out_len, n, l);
    time = RTIMER_NOW() - time;
    if (result != STS_OK){
        printf("Encryption failed!\n\r");
        return 1;
    }
    printf("paillier_enc(), %lu ms\n\r",
            (uint32_t)((uint64_t)time * 1000 / RTIMER_SECOND));

    section("CORRECTNESS");
    int equal = memcmp(in, out, in_len) == 0;
    printf("Encryption is correct: %i", equal);
    PROCESS_END();
}

