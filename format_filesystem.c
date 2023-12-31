#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//variables that hold our block size and number of blocks we will have in our volume.
#define BLOCK_SIZE 512
#define NUM_BLOCKS 1024

//volume control block (VCB) structure.
typedef struct {
    int totalBlocks;
    int freeBlocks;
} VCB;

//block structure.
typedef struct {
} Block;

//free space management structure (with bitmap).
typedef struct {
    unsigned char bitmap[NUM_BLOCKS / 8]; // 1 bit per block
} FreeSpace;

//root directory structure.
typedef struct {
    char name[32];
    int inode; 
} DirectoryEntry;

//function that will format our file system volume.
void formatVolume() {
    FILE *disk = fopen("myfs.img", "wb");
    if (disk == NULL) {
        perror("Unable to open disk image");
        exit(1);
    }

    //volume control block (VCB) initialization.
    VCB vcb;
    vcb.totalBlocks = NUM_BLOCKS;
    vcb.freeBlocks = NUM_BLOCKS; 
    fwrite(&vcb, sizeof(VCB), 1, disk);

    //free space management initialization.
    FreeSpace freeSpace;
    memset(freeSpace.bitmap, 0xFF, sizeof(freeSpace.bitmap)); //setting all bits to 1: free.
    fwrite(&freeSpace, sizeof(FreeSpace), 1, disk);

    //root directory with "." and ".." initialization.
    DirectoryEntry rootDir[2];
    strcpy(rootDir[0].name, ".");
    rootDir[0].inode = 0; 
    strcpy(rootDir[1].name, "..");
    rootDir[1].inode = 0; 
    fwrite(rootDir, sizeof(DirectoryEntry), 2, disk);

    fclose(disk);
}

int main() {
    formatVolume();
    return 0;
}
