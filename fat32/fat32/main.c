#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
struct VolumeLabel11{
    uint32_t Bytes4;
    uint32_t Bytes8;
    uint16_t Bytes10;
    uint8_t Bytes11;
};
// DBR的一些属性 分区引导扇区
struct DosBootRecord
{
    uint32_t BootJmp;   // 0跳转指令
    uint32_t OemName[2];  // 3文件系统标志和版本号
    uint16_t BytesPerSector;    // 11每扇区字节数
    uint8_t SectorPerCluster;   // 13每簇扇区数
    uint16_t ReservedSectorCount;   // 14保留扇区数
    uint8_t FATsCount;  // 16FAT表个数
    uint16_t RootDirCount16;  // 17根目录中目录个数
    uint16_t TotalSectorCount16;  // 19总扇区数
    uint8_t MediaType;  // 21存储介质
    uint16_t SectorPerFAT16;     // 22一个FAT表所占的扇区数
    uint16_t SectorPerTrack;    // 24
    uint16_t HeadSideCount;     // 26
    uint32_t HiddenSectorCount;     // 28
    uint32_t SectorCount;   // 32
    uint32_t SectorPerFAT32;    // 36
    uint16_t Flag32;    // 40
    uint16_t FATVersion;    // 42
    uint32_t RootFirstCluster;  // 44
    uint16_t FSInfoSector;  // 48
    uint16_t BackupBSSector;    // 50
    uint32_t FATExtend[3];  // 52
    uint8_t DriveNumber1;   // 64
    uint8_t DriveNumber2;   // 65
    uint8_t ExtendBootFlag; // 66
    uint32_t VolumeAlignment;   // 67
    struct VolumeLabel11 VolumeLabel;   // 71
    uint32_t FSFormat[2];  // 82
    uint16_t SignatureFlag;     // 510
} DBR;

struct FSINFO {
    uint32_t ExtendBootFlag;    // 512
    uint32_t FSINFOSignature;   // 996
    uint32_t NextAvailableCluster;  // 1004
    uint16_t Flag55AA;      // 1022
    
} FSINFO;



// FAT32的一些参数 文件分配表
struct FileAllocationTable32
{
    uint32_t FAT1Offset;    // FAT1 偏移量
    uint32_t FAT2Offset;    // FAT2 偏移量
    uint32_t RootDirectoryOffset;   // 根目录偏移量
    uint32_t BytesPerCluster;   // 每簇字节数
    uint32_t DirItemPerCluster;     // 每簇目录项
} FAT32;

// 存储目录项的结构体
typedef struct DirectoryInfo
{
    uint8_t LongDirOrder;   // 长目录项顺序
    uint8_t *Name;
    uint8_t Attribute;
    uint32_t Cluster;
    uint32_t Size;
} DirectoryInfo;

void printDBR()
{
    printf("\nDBR Attributes info:\n");
    printf("-----------------------------------------------------------\n");
    printf("DosBootRecordAttribute   Value\n");
    printf("-----------------------------------------------------------\n");
    printf(" BootJmp                 %d\n",DBR.BootJmp);
    printf(" OemName                 %d%d\n",DBR.OemName[0], DBR.OemName[1]);
    printf(" BytesPerSector          %d\n",DBR.BytesPerSector);
    printf(" SectorPerCluster        %d\n",DBR.SectorPerCluster);
    printf(" ReservedSectorCount     %d\n",DBR.ReservedSectorCount);
    printf(" FATsCount               %d\n",DBR.FATsCount);
    printf(" RootDirCount16          %d\n",DBR.RootDirCount16);
    printf(" TotalSectorCount16      %d\n",DBR.TotalSectorCount16);
    printf(" MediaType               %d\n",DBR.MediaType);
    printf(" SectorPerFAT16          %d\n",DBR.SectorPerFAT16);
    printf(" SectorPerTrack          %d\n",DBR.SectorPerTrack);
    printf(" HeadSideCount           %d\n",DBR.HeadSideCount);
    printf(" HiddenSectorCount       %d\n",DBR.HiddenSectorCount);
    printf(" SectorCount             %d\n",DBR.SectorCount);
    printf(" SectorPerFAT32          %d\n",DBR.SectorPerFAT32);
    printf(" Flag32                  %d\n",DBR.Flag32);
    printf(" FATVersion              %d\n",DBR.FATVersion);
    printf(" RootFirstCluster        %d\n",DBR.RootFirstCluster);
    printf(" FSInfoSector            %d\n",DBR.FSInfoSector);
    printf(" BackupBSSector          %d\n",DBR.BackupBSSector);
    printf(" FATExtend               %d%d%d\n",DBR.FATExtend[0],DBR.FATExtend[1],DBR.FATExtend[2]);
    printf(" DriveNumber1            %d\n",DBR.DriveNumber1);
    printf(" DriveNumber2            %d\n",DBR.DriveNumber2);
    printf(" ExtendBootFlag          %d\n",DBR.ExtendBootFlag);
    printf(" VolumeAlignment         %d\n",DBR.VolumeAlignment);
    printf(" VolumeLabel             %d%d%d%d\n",DBR.VolumeLabel.Bytes4, DBR.VolumeLabel.Bytes8,  DBR.VolumeLabel.Bytes10, DBR.VolumeLabel.Bytes11);
    printf(" FSFormat                %d%d\n",DBR.FSFormat[0], DBR.FSFormat[1]);
    printf(" SignatureFlag           %d\n",DBR.SignatureFlag);
    printf("-----------------------------------------------------------\n");
    
}
void printFSINFO()
{
    printf("\nFSINFO Attributes info:\n");
    printf("-----------------------------------------------------------\n");
    printf(" FSINFO Attribute        Value\n");
    printf("----------------------- -----------------------------------\n");
    printf(" ExtendBootFlag          %d\n",FSINFO.ExtendBootFlag);
    printf(" FSINFOSignature         %d\n",FSINFO.FSINFOSignature);
    printf(" NextAvailableCluster    %d\n",FSINFO.NextAvailableCluster);
    printf(" Flag55AA                %d\n",FSINFO.Flag55AA);
    printf("-----------------------------------------------------------\n");
}

// 从文件中读取一个字节
uint8_t getByteFromFile(FILE *input, uint32_t offset, uint32_t whence)
{
    uint8_t n;
    fseek(input, offset, whence);
    n = fgetc(input);
    return n;
}

// 从文件中读取一个字
uint16_t getWordFromFile(FILE *input, uint32_t offset, uint32_t whence)
{
    uint16_t n;
    fseek(input, offset, whence);
    n = fgetc(input);
    n += fgetc(input) << 8;
    return n;
}

// 从文件中读取一个双字
uint32_t getDoubleWordFromFile(FILE *input, uint32_t offset, uint32_t whence)
{
    uint32_t n;
    fseek(input, offset, whence);
    n = fgetc(input);
    n += fgetc(input) << 8;
    n += fgetc(input) << 16;
    n += fgetc(input) << 24;
    return n;
}


// 从字符串中读取一个字节
uint8_t getByte(uint8_t *buffer, uint32_t offset)
{
    uint8_t n;
    buffer += offset;
    n = buffer[0];
    return n;
}

// 从字符串中读取一个字
uint16_t getWord(uint8_t *buffer, uint32_t offset)
{
    uint16_t n;
    buffer += offset;
    n = buffer[0];
    n += buffer[1] << 8;
    return n;
}

// 从字符串中读取一个双字
uint32_t getDoubleWord(uint8_t *buffer, uint32_t offset)
{
    uint32_t n;
    buffer += offset;
    n = buffer[0];
    n += buffer[1] << 8;
    n += buffer[2] << 16;
    n += buffer[3] << 24;
    return n;
}

// 初始化DBR的相关属性
void initDBR(FILE *input, struct DosBootRecord *dbr)
{
    dbr->BootJmp = getDoubleWordFromFile(input, 0, SEEK_SET);
    dbr->OemName[0] = getDoubleWordFromFile(input, 3, SEEK_SET);
    dbr->OemName[1] = getDoubleWordFromFile(input, 7, SEEK_SET);
    dbr->BytesPerSector = getWordFromFile(input, 11, SEEK_SET);     // SEEK_SET 文件开头
    dbr->SectorPerCluster = getByteFromFile(input, 13, SEEK_SET);
    dbr->ReservedSectorCount = getWordFromFile(input, 14, SEEK_SET);
    dbr->FATsCount = getByteFromFile(input, 16, SEEK_SET);
    dbr->RootDirCount16 = getWordFromFile(input, 17, SEEK_SET);
    dbr->TotalSectorCount16 = getDoubleWordFromFile(input, 19, SEEK_SET);
    dbr->MediaType = getByteFromFile(input, 21, SEEK_SET);
    dbr->SectorPerFAT16 = getWordFromFile(input, 22, SEEK_SET);
    dbr->SectorPerTrack = getWordFromFile(input, 24, SEEK_SET);
    dbr->HeadSideCount = getWordFromFile(input, 26, SEEK_SET);
    dbr->HiddenSectorCount = getDoubleWordFromFile(input, 28, SEEK_SET);
    dbr->SectorCount = getWordFromFile(input, 32, SEEK_SET);
    dbr->SectorPerFAT32 = getDoubleWordFromFile(input, 36, SEEK_SET);
    dbr->Flag32 = getWordFromFile(input, 40, SEEK_SET);
    dbr->FATVersion = getWordFromFile(input, 42, SEEK_SET);
    dbr->RootFirstCluster = getDoubleWordFromFile(input, 44, SEEK_SET);
    dbr->FSInfoSector = getWordFromFile(input, 48, SEEK_SET);
    dbr->BackupBSSector = getWordFromFile(input, 50, SEEK_SET);
    dbr->FATExtend[0] = getDoubleWordFromFile(input, 52, SEEK_SET);
    dbr->FATExtend[1] = getDoubleWordFromFile(input, 56, SEEK_SET);
    dbr->FATExtend[2] = getDoubleWordFromFile(input, 60, SEEK_SET);
    dbr->DriveNumber1 = getByteFromFile(input, 64, SEEK_SET);
    dbr->DriveNumber2 = getByteFromFile(input, 65, SEEK_SET);
    dbr->ExtendBootFlag = getByteFromFile(input, 66, SEEK_SET);
    dbr->VolumeAlignment = getDoubleWordFromFile(input, 67, SEEK_SET);
    dbr->VolumeLabel.Bytes4 = getDoubleWordFromFile(input, 71, SEEK_SET);
    dbr->VolumeLabel.Bytes8 = getDoubleWordFromFile(input, 75, SEEK_SET);
    dbr->VolumeLabel.Bytes10 = getWordFromFile(input, 79, SEEK_SET);
    dbr->VolumeLabel.Bytes11 = getByteFromFile(input, 81, SEEK_SET);
    dbr->FSFormat[0] = getDoubleWordFromFile(input, 82, SEEK_SET);
    dbr->FSFormat[1] = getDoubleWordFromFile(input, 86, SEEK_SET);
    dbr->SignatureFlag = getWordFromFile(input, 510, SEEK_SET);
}

void initFSINFO(FILE *input, struct FSINFO *fsinfo)
{
    fsinfo->ExtendBootFlag = getDoubleWordFromFile(input, 512, SEEK_SET);
    fsinfo->FSINFOSignature = getDoubleWordFromFile(input, 996, SEEK_SET);
    fsinfo->NextAvailableCluster = getDoubleWordFromFile(input, 1004, SEEK_SET);
    fsinfo->Flag55AA = getWordFromFile(input, 1022, SEEK_SET);
}

// 初始化FAT32的相关参数
void initFAT32(struct FileAllocationTable32 *fat32, struct DosBootRecord *dbr)
{
    fat32->FAT1Offset = dbr->BytesPerSector * dbr->ReservedSectorCount;
    fat32->FAT2Offset = fat32->FAT1Offset + dbr->BytesPerSector * dbr->SectorPerFAT32;
    fat32->RootDirectoryOffset = fat32->FAT2Offset + dbr->BytesPerSector * dbr->SectorPerFAT32;
    fat32->BytesPerCluster = dbr->SectorPerCluster * dbr->BytesPerSector;
    fat32->DirItemPerCluster = fat32->BytesPerCluster / 0x20;
}

// 从文件中读取一个簇，返回一个指向buffer的指针
uint8_t *getCluster(FILE *input, uint32_t number)
{
    uint32_t offset;
    uint8_t *buffer;
    offset = FAT32.RootDirectoryOffset + FAT32.BytesPerCluster * (number - 2);
    buffer = (uint8_t *) calloc(FAT32.BytesPerCluster, 1);
    fseek(input, offset, SEEK_SET);
    fread(buffer, 1, FAT32.BytesPerCluster, input);
    return buffer;
}

// 十六进制输出buffer
void printBuffer(uint8_t *buffer, uint32_t size)
{
    for (int i = 0; i < size; i++)
    {
        printf("%02X ", buffer[i]);
        if (i % 4 == 3)
        {
            printf(" ");
        }
        if (i % 16 == 15)
        {
            printf("\n");
        }
    }
}

// 从buffer中读取一个目录项
DirectoryInfo *getDirectoryItem(uint8_t *buffer, uint32_t offset)
{
    uint8_t *directoryItem, attribute, *name;
    directoryItem = buffer + offset;
    attribute = directoryItem[0xB];
    DirectoryInfo *directoryInfo = calloc(1, sizeof(DirectoryInfo));
    // 判断目录项种类
    if (attribute == 0xF)
    {
        // 长目录项
        directoryInfo->LongDirOrder = directoryItem[0x0];
        name = calloc(14, sizeof(uint8_t));
        int count = 0;
        for (count = 0; count < 5; count++)
        {
            name[count] = directoryItem[1 + count * 2];
        }
        for (count = 5; count < 11; count++)
        {
            name[count] = directoryItem[4 + count * 2];
        }
        name[11] = directoryItem[28];
        name[12] = directoryItem[30];
        directoryInfo->Name = name;
        directoryInfo->Attribute = attribute;
        directoryInfo->Cluster = 0;
        directoryInfo->Size = 0;
    } else
    {
        // 短目录项
        directoryInfo->LongDirOrder = 0;
        directoryInfo->Attribute = attribute;
        directoryInfo->Size = getDoubleWord(directoryItem, 0x1C);
        uint32_t temp = getWord(directoryItem, 0x14) << 16;
        temp += getWord(directoryItem, 0x1A);
        directoryInfo->Cluster = temp;
        name = calloc(12, sizeof(uint8_t));
        for (int count = 0; count < 11; count++)
        {
            name[count] = directoryItem[count];
        }
        directoryInfo->Name = name;
    }
    return directoryInfo;
}

// 返回FAT32表中的下一簇簇号
uint32_t getNextCluster(FILE *input, uint32_t cluster)
{
    return getDoubleWordFromFile(input, FAT32.FAT1Offset + cluster * 4, SEEK_SET);
}

// 输入第一簇的簇号，返回该目录的目录项结构体指针数组
DirectoryInfo **getRawDirectory(FILE *input, uint32_t cluster)
{
    uint8_t isEnd = 0;
    uint8_t *buffer = getCluster(input, cluster);
    uint32_t size = 0;
    uint32_t nextCluster;
    DirectoryInfo **directoryList = calloc(FAT32.DirItemPerCluster, sizeof(DirectoryInfo *));
    DirectoryInfo *temp;
    while (1)
    {
        for (int count = 0; count < FAT32.DirItemPerCluster; count++)
        {
            // 获取一个目录项结构体
            temp = getDirectoryItem(buffer, count * 0x20);
            if (temp->LongDirOrder == 0 && temp->Name[0] == 0)
            {
                // 遍历到达结束
                isEnd = 1;
                break;
            } else if (temp->Name[0] == 0xe5 || temp->LongDirOrder == 0xe5)
            {
                // 该条目录已被删除
                continue;
            } else
            {
                // 合法的目录项
                directoryList[size++] = temp;
            }
        }
        if (isEnd)
        {
            free(buffer);
            break;
        }
        // 检索下一簇
        nextCluster = getNextCluster(input, cluster);
        if (nextCluster >= 0xf8)
        {
            // 到达结束
            free(buffer);
            break;
        } else if (nextCluster == 0xf7)
        {
            // 坏簇
            free(buffer);
            break;
        } else if (nextCluster >= 0xf0)
        {
            // 系统保留
            free(buffer);
            break;
        } else if (nextCluster >= 0x02)
        {
            // 已分配
            free(buffer);
            cluster = nextCluster;
            buffer = getCluster(input, cluster);
            directoryList = realloc(directoryList, (size + FAT32.DirItemPerCluster) * sizeof(DirectoryInfo *));
        } else
        {
            // 错误
        }
    }
    directoryList = realloc(directoryList, (size + 1) * sizeof(DirectoryInfo *));
    directoryList[size] = NULL;
    return directoryList;
}

// 显示目录的深度
void showDepth(uint32_t depth)
{
    for (int count = 0; count < depth; count++)
    {
        printf("    ");
    }
    printf("├── ");
}

// 递归遍历目录并输出
void walkDirectory(FILE *input, uint32_t cluster, uint32_t depth)
{
    // 获取目录项列表
    DirectoryInfo **directoryList = getRawDirectory(input, cluster);
    DirectoryInfo *directoryItem;
    uint32_t count = 0;
    uint8_t attribute, isLongName, *tempName;
    isLongName = 0;
    // 若不是根目录则跳过前两个目录项
    if (directoryList[0]->Name[0] == '.' && directoryList[0]->Name[1] == ' ')
    {
        count = 2;
    }
    // 遍历目录项
    while (directoryList[count] != NULL)
    {
        directoryItem = directoryList[count++];
        attribute = directoryItem->Attribute;
        if (attribute == 0xf)
        {
            // 长目录项
            if (isLongName)
            {
                // 非首个长目录项，将两次名称合并
                uint32_t length = strlen(tempName) + strlen(directoryItem->Name) + 1;
                uint32_t stringSize = 0;
                uint8_t *newName = calloc(length, sizeof(uint8_t));
                uint8_t *tempPointer = directoryItem->Name;
                while (*tempPointer != 0)
                {
                    newName[stringSize++] = *tempPointer++;
                }
                tempPointer = tempName;
                while (*tempPointer != 0)
                {
                    newName[stringSize++] = *tempPointer++;
                }
                free(tempName);
                tempName = newName;
            } else
            {
                // 首个长目录项
                isLongName = 1;
                uint32_t length = strlen(directoryItem->Name) + 1;
                uint8_t *newName = calloc(length, sizeof(uint8_t));
                strcpy(newName, directoryItem->Name);
                tempName = newName;
            }

        } else if (attribute & 0x8)
        {
            // 卷标
            printf("Volume ID: %s\n", directoryItem->Name);
            isLongName = 0;
        } else if (attribute & 0x10)
        {
            // 子目录
            showDepth(depth);
            if (isLongName)
            {
                printf("\033[;34m%s\033[0m\n", tempName);
                isLongName = 0;
            } else
            {
                printf("\033[;34m%s\033[0m\n", directoryItem->Name);
            }
            walkDirectory(input, directoryItem->Cluster, depth + 1);
        } else if (attribute & 0x20)
        {
            // 文件
            showDepth(depth);
            if (isLongName)
            {
                printf("%s", tempName);
                isLongName = 0;
            } else
            {
                for (int i = 0; i < 8; i++)
                {
                    if (directoryItem->Name[i] == ' ')
                    {
                        break;
                    }
                    putchar(directoryItem->Name[i]);
                }
                putchar('.');
                for (int i = 8; i < 11; i++)
                {
                    if (directoryItem->Name[i] == ' ')
                    {
                        break;
                    }
                    putchar(directoryItem->Name[i]);
                }
            }
            printf("\033[;32m  %d Bytes\033[0m\n", directoryItem->Size);
        }
    }
}



int main()
{
    FILE *input;
    input = fopen("/home/ruby/Documents/fat32/fat32d.img", "rb");
    initDBR(input, &DBR);
    initFSINFO(input, &FSINFO);
    initFAT32(&FAT32, &DBR);
    // 递归遍历根目录
    walkDirectory(input, 0x2, 0);
    printDBR();
    printFSINFO();
    return 0;
}
