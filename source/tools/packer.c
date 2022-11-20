/*////////////////////////////////////////////////////////////////////////////*/

#define NK_PACK_IMPLEMENTATION
#define NK_FILESYS_IMPLEMENTATION

#define NK_STATIC

#include <stdio.h>

#include <nk_pack.h>

int main(int argc, char** argv)
{
    printf("packing game assets into npak... ");
    nkBool res = nk_pack("binary/win32/assets.npak", "binary/win32/assets");
    printf("%s!\n", res ? "successful" : "failure");
    return 0;
}

/*////////////////////////////////////////////////////////////////////////////*/
