#include <iostream>
#include <string.h>
#include <cassert>

typedef struct __SPI_HandleTypeDef
{
  uint8_t                    *pTxBuffPtr;
  uint16_t                   TxXferSize;
} SPI_HandleTypeDef;


typedef struct
{
    float Vos1,vslope1 ;   // calibration values for the ADC channels, slope in deg C/volt
    float Vos2,vslope2 ;    //ch1,2
    float Vos3,vslope3 ;    //ch3,4
    float Vos4,vslope4 ;
    float Pos5,vslope5 ;  // offset in dBm at 0V
    float Pos6,vslope6 ;    //ch1,2
    float Pos7,vslope7 ;    //ch3,4
    float Pos8,vslope8 ;
    float Vos9,vslope9 ;    //rad,12v
    float Vos10,vslope10;
    float Vos11,vslope11;   //5v,curr
    float Vos12,vslope12;
    float Vos16,vslope16;
    float Vos17,vslope17;
    char EID[64];          // 60 character equipment id string
    char CALID[64];        // 60 character calibration id string

} cal_table_type;

typedef struct
{
    float Vos0,vslope0 ;   // calibration values for the ADC channels, slope in deg C/volt
    float Vos1,vslope1 ;   // calibration values for the ADC channels, slope in deg C/volt
    float Vos2,vslope2 ;    //ch1,2
    float Vos3,vslope3 ;    //ch3,4
    float Vos4,vslope4 ;
    float Pos5,vslope5 ;  // offset in dBm at 0V
    float Pos6,vslope6 ;    //ch1,2
    float Pos7,vslope7 ;    //ch3,4
    float Pos8,vslope8 ;
    float Vos9,vslope9 ;    //rad,12v
    float Vos10,vslope10;
    float Vos11,vslope11;   //5v,curr
    float Vos12,vslope12;
    float Vos16,vslope16;
    float Vos17,vslope17;
    char EID[64];          // 60 character equipment id string
    char CALID[64];        // 60 character calibration id string

} cal_table_type_alt;

typedef uint8_t att_table_type[5][2];



void *eeprom_data;


void eeprom_write_byte(SPI_HandleTypeDef* handle,uint8_t value,uint16_t address)
{

    memcpy((uint8_t*)eeprom_data + address, &value, sizeof(uint8_t));
    printf("\t\tWriting %1X to address %2X",value,address);
    printf("\n");

}

void eeprom_write_page(SPI_HandleTypeDef* handle,uint8_t* ptr,uint8_t pageNo)
{
    uint8_t offset = pageNo*16;

    memcpy((uint8_t*)eeprom_data + offset, ptr, 16);
    printf("\t\tWriting page %d\n",pageNo);
}


void eeprom_write_block(SPI_HandleTypeDef* handle,uint16_t address,uint8_t* pmemstart,uint16_t size)
{

    int i = 0;
    uint16_t preByteOffset,postByteOffset,nextAddress,pages,byteCounter;
    byteCounter = 0;

    // Determine how many bytes before the first full page must be written
    preByteOffset = 16-(address % 16);

    // If "preByteOffset" is 16 then it's a full page, no bytes needed
    if(preByteOffset == 16)
        preByteOffset = 0;

    // "preByteOffset" calculation calculates up to end of page, if given "size" means data doesn't reach
    // the end of the page then clip "preByteOffset" to "size".
    if(preByteOffset > size)
        preByteOffset = size;

    // Next available address after this block has been written
    nextAddress = address + size;

    // Calculate how many pages and postBytes are needed
    if(size >= 16) {
        postByteOffset = nextAddress % 16;
        pages = (size - (preByteOffset + postByteOffset)) / 16;
    }
    else {
        // We have no pages to write but preByteOffset isn't all the data
        if(preByteOffset < size)
            postByteOffset = nextAddress % 16;
        else
            postByteOffset = 0;
        pages = 0;
    }

    // Write any bytes before the first full page
    for(i = 0; i < preByteOffset; i++){
        eeprom_write_byte(handle,*(pmemstart + byteCounter),address);
        address++;
        byteCounter++;
    }

    // Write any full pages
    for(i = 0; i < pages; i++){
        eeprom_write_page(handle,pmemstart+(byteCounter),address/16);
        address+=16;
        byteCounter+=16;
    }

    // Write any bytes after the last full page
    for(i = 0; i < postByteOffset; i++){
        eeprom_write_byte(handle,*(pmemstart + byteCounter),address);
        address++;
        byteCounter++;
    }

}


int main() {
    SPI_HandleTypeDef handle;

    cal_table_type cal_table, cal_table_read;
    att_table_type att_table, att_table_read;
    cal_table_type_alt cal_table_alt, cal_table_alt_read;

    // Block of memory to simulate eeprom
    eeprom_data = (void*)malloc(sizeof(uint8_t) * 512);

    float *fp;
    int i;
    uint8_t *bytePointer1;
    uint8_t *bytePointer2;

    // Fill up cal_table
    fp = (float*)&cal_table;
    for(i = 0; i < 28; i++){
        *fp = i+0.1;
        fp ++;
    }
    strcpy(cal_table.EID,"!aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa!\r\n");
    strcpy(cal_table.CALID,"!bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb!\r\n");

    // Fill up cal_table_alt (cal_table + and extra 2 floats)
    fp = (float*)&cal_table_alt;
    for(i = 0; i < 30; i++){
        *fp = i+0.1;
        fp ++;
    }
    strcpy(cal_table_alt.EID,"!aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa!\r\n");
    strcpy(cal_table_alt.CALID,"!bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb!\r\n");

    att_table[0][0]=1;
    att_table[0][1]=2;
    att_table[1][0]=3;
    att_table[1][1]=4;
    att_table[2][0]=5;
    att_table[2][1]=6;
    att_table[3][0]=7;
    att_table[3][1]=8;
    att_table[4][0]=9;
    att_table[4][1]=10;


    /*
     * Test 1:
     * Basic test as the method will be used on the hardware. This is just page writes
     */
    printf("Running test 1:\n");
    printf("\tRunning eeprom_write_block() for cal_table\n");
    eeprom_write_block(&handle,0x00,(uint8_t*)&cal_table, sizeof(cal_table));
    printf("\tRunning eeprom_write_block() for att_table\n");
    eeprom_write_block(&handle,0xF0,(uint8_t*)&att_table, sizeof(att_table));

    memcpy((uint8_t*)&cal_table_read, (uint8_t*)eeprom_data+0x00, sizeof(cal_table_read));
    memcpy((uint8_t*)&att_table_read, (uint8_t*)eeprom_data+0xF0, sizeof(att_table_read));

    bytePointer1 = (uint8_t *)&cal_table;
    bytePointer2 = (uint8_t *)&cal_table_read;
    for(i = 0; i < sizeof(cal_table); i++){
        assert(("cal_table == cal_table_read",bytePointer1[i]==bytePointer2[i]));
    }


    bytePointer1 = (uint8_t *)&att_table;
    bytePointer2 = (uint8_t *)&att_table_read;
    for(i = 0; i < sizeof(att_table); i++){
        assert(("att_table == att_table_read",bytePointer1[i]==bytePointer2[i]));
    }
    printf("Test 1 passed \n");

    /*
     * Test 2:
     * Address is not at start of page. This will force the use of both page and byte writes
     */
    printf("Running test 2:\n");
    printf("\tRunning eeprom_write_block() for cal_table\n");
    eeprom_write_block(&handle,0x01,(uint8_t*)&cal_table, sizeof(cal_table));
    printf("\tRunning eeprom_write_block() for att_table\n");
    eeprom_write_block(&handle,0xF1,(uint8_t*)&att_table, sizeof(att_table));

    memcpy((uint8_t*)&cal_table_read, (uint8_t*)eeprom_data+0x01, sizeof(cal_table_read));
    memcpy((uint8_t*)&att_table_read, (uint8_t*)eeprom_data+0xF1, sizeof(att_table_read));

    bytePointer1 = (uint8_t *)&cal_table;
    bytePointer2 = (uint8_t *)&cal_table_read;
    for(i = 0; i < sizeof(cal_table); i++){
        assert(("cal_table == cal_table_read",bytePointer1[i]==bytePointer2[i]));
    }


    bytePointer1 = (uint8_t *)&att_table;
    bytePointer2 = (uint8_t *)&att_table_read;
    for(i = 0; i < sizeof(att_table); i++){
        assert(("att_table == att_table_read",bytePointer1[i]==bytePointer2[i]));
    }
    printf("Test 2 passed \n");

    /*
     * Test 3:
     * Use alternate cal_table with extra members
     */
    printf("Running test 3:\n");
    printf("\tRunning eeprom_write_block() for cal_table_alt\n");
    eeprom_write_block(&handle,0x00,(uint8_t*)&cal_table_alt, sizeof(cal_table_alt));
    printf("\tRunning eeprom_write_block() for att_table\n");
    eeprom_write_block(&handle,0xF4,(uint8_t*)&att_table, sizeof(att_table));

    memcpy((uint8_t*)&cal_table_alt_read, (uint8_t*)eeprom_data+0x00, sizeof(cal_table_alt_read));
    memcpy((uint8_t*)&att_table_read, (uint8_t*)eeprom_data+0xF4, sizeof(att_table_read));

    bytePointer1 = (uint8_t *)&cal_table_alt;
    bytePointer2 = (uint8_t *)&cal_table_alt_read;
    for(i = 0; i < sizeof(cal_table); i++){
        assert(("cal_table_alt == cal_table_alt_read",bytePointer1[i]==bytePointer2[i]));
    }


    bytePointer1 = (uint8_t *)&att_table;
    bytePointer2 = (uint8_t *)&att_table_read;
    for(i = 0; i < sizeof(att_table); i++){
        assert(("att_table == att_table_read",bytePointer1[i]==bytePointer2[i]));
    }
    printf("Test 3 passed \n");

    /*
     * Test 4:
     * Use alternate cal_table with extra members and shifted data
     */
    printf("Running test 3:\n");
    printf("\tRunning eeprom_write_block() for cal_table_alt\n");
    eeprom_write_block(&handle,0x02,(uint8_t*)&cal_table_alt, sizeof(cal_table_alt));
    printf("\tRunning eeprom_write_block() for att_table\n");
    eeprom_write_block(&handle,0xF6,(uint8_t*)&att_table, sizeof(att_table));

    memcpy((uint8_t*)&cal_table_alt_read, (uint8_t*)eeprom_data+0x02, sizeof(cal_table_alt_read));
    memcpy((uint8_t*)&att_table_read, (uint8_t*)eeprom_data+0xF6, sizeof(att_table_read));

    bytePointer1 = (uint8_t *)&cal_table_alt;
    bytePointer2 = (uint8_t *)&cal_table_alt_read;
    for(i = 0; i < sizeof(cal_table); i++){
        assert(("cal_table_alt == cal_table_alt_read",bytePointer1[i]==bytePointer2[i]));
    }


    bytePointer1 = (uint8_t *)&att_table;
    bytePointer2 = (uint8_t *)&att_table_read;
    for(i = 0; i < sizeof(att_table); i++){
        assert(("att_table == att_table_read",bytePointer1[i]==bytePointer2[i]));
    }
    printf("Test 4 passed \n");


    std::cout << "All tests passed" << std::endl;
    return 0;
}