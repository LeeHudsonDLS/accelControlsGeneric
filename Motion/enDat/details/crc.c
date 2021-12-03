unsigned int MakeCrcNorm(unsigned int param8,unsigned int param16)
{
    unsigned int ff[5]; // State of the 5 flip-flops
    unsigned int code[24]; // Data-bit array
    unsigned int ex; // Auxiliary variable
    unsigned int crc = 0; // Determined CRC code
    signed int i; // Controlled variable for looping

    for(i = 0; i < 5; i++) // set all flip-flops to 1
        ff[i] = 1;

    for(i = 0; i < 8; i++) // read 8 bit parameter into code array
    { // and convert the bit sequence
        code[i] = (param8 & 0x0080) ? 1 : 0;
        param8 <<= 1;
    }

    for(i = 8; i < 24; i++) // read 16 bit parameter into code array
    { // and convert the bit sequence
        code[i] = (param16 & 0x8000) ? 1 : 0;
        param16 <<= 1;
    }

    for(i = 0; i < 24; i++) // calculate CRC, analog to
    { // described generator hardware
        ex = ff[4] ^ code[i];
        ff[4] = ff[3];
        ff[3] = ff[2] ^ ex;
        ff[2] = ff[1];
        ff[1] = ff[0] ^ ex;
        ff[0] = ex;
    }

    for(i = 4; i >= 0; i--) // save CRC in variable
    {
        ff[i] = ff[i] ? 0 : 1; // invert bits
        crc <<= 1;
        crc |= ff[i];
    }
    return crc;
}


void main(){
    unsigned int param8,param16,crcResult;
    
    param8 = 13;
    param16 = 32793;

    crcResult = MakeCrcNorm(param8,param16);
    printf("Result = %d\n",crcResult);

    return 0;



}