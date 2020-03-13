#define LED LATDbits.LATD0

#define MAX_VOLTAGE_LEVEL 31

#define HEX_BUFFER_SIZE 2

#define BIT5 31
#define BIT8 255
#define BIT10 1023
#define BIT12 4095

#define MEM_SIZE 1024

#define ADC_ENABLE ADCON0bits.ADON
#define DAC_ENABLE VREFCON1bits.DACEN
#define CM1_ENABLE CM1CON0bits.C1ON

#define TMR0_ENABLE T0CONbits.TMR0ON 

#define AD_GODONE  ADCON0bits.GO_DONE
#define CM1_OUT CM1CON0bits.C1OUT

char *ConvertToHexString(unsigned char value){
    static char buffer[HEX_BUFFER_SIZE+1];
    unsigned char parse_value=0;
    for(unsigned char i=0;i<HEX_BUFFER_SIZE;i++){
        parse_value=(value>>(4*i))&0x0F;
        buffer[HEX_BUFFER_SIZE-(i+1)]=getHex(parse_value);
    }
    buffer[HEX_BUFFER_SIZE]='\0';
    return buffer;
}

char getHex(unsigned char value){
    char hex=0;
    if(value == 10)hex='a';
    if(value == 11)hex='b';
    if(value == 12)hex='c';
    if(value == 13)hex='d';
    if(value == 14)hex='e';
    if(value == 15)hex='f';
    if(value < 10){
        hex = (value +'0');
    }
    
    return hex;
}


void calcVoltage(unsigned int sample, unsigned int format){
    isNullFlag=false;
    
    float devider=(float)sample/format;
    float voltage=devider*3.3;
    VOLTAGE=(int)voltage;
    
    if(((int)((voltage-VOLTAGE)*10)) == 0){
        isNullFlag=true;
    }
    VOLTAGE_MILI=(int)((voltage-VOLTAGE)*100);
}

unsigned int getSample(){
    AD_GODONE=1;
    NOP();
    while(AD_GODONE);
    NOP();
    return (ADRESH<<8 | ADRESL);
}

void setSample(unsigned char value){
    if(value > MAX_VOLTAGE_LEVEL)value=MAX_VOLTAGE_LEVEL;
    VREFCON2=value;
}

unsigned int formatSample(unsigned int from,unsigned int to,unsigned int sample){
    unsigned int result=0x0000;
    float fromResult=(float)sample/from;
    float toResult=to*fromResult;
    
    result=(unsigned int)toResult;
    return result;
}

unsigned char roundSample(unsigned int sample, unsigned char format){
    sample>>=(format-8);
    if ( (sample | 0x01) > 0xFF )return 0xFF;
    else return (sample | 0x01);
}

bit isBufferFull(){
    if(indexer >= MEM_SIZE)return true;
    else return false;
}

void ShowProgress(unsigned char progress){
    LATD=0x00;
    LATD|=0x01;
    for(unsigned char i=0;i<progress;i++){
        LATD<<=1;
        LATD|=0x01;
    }
}