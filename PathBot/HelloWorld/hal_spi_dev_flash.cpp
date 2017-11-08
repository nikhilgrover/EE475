
/*
    by: b35970@freescale.com
*/

#include "hal_spi_dev_flash.h"

//#include "kinetis.h"
#include "hal_spi.h"

//#ifndef WProgram_h

//#endif



#define PAGE_SIZE 256

#define CMD_GET_ID      0x9f
#define CMD_BLOCK_ERASE 0x20
#define CMD_DEV_ERASE   0x60
#define CMD_PROGRAM     0x02
#define CMD_READ        0x0B
#define CMD_GET_SR      0x05
#define CMD_SET_SR      0x01
#define CMD_WRITE_EN    0x06
#define CMD_WRITE_DIS   0x04

#include "usb_serial.h"
// #ifdef  __cplusplus


// extern "C"
// {
// #endif

static char get_sr(void)
{
    char v;
    hal_spi_transfe_start();
    hal_spi_transfer_one_byte(CMD_GET_SR,0);
    v = hal_spi_transfer_one_byte(0x00,1);
    hal_spi_transfe_stop();
    Serial.print("v = ");
    Serial.println(v, HEX);
    return v;
}
static void write_enable(void)
{
    hal_spi_transfe_start();
    hal_spi_transfer_one_byte(CMD_WRITE_EN,1);
    hal_spi_transfe_stop();
}
static void wait(void)
{
    while(get_sr()&0x01 == 0x01)
        ;
}

static void global_unprotected(void)
{
    write_enable();
    hal_spi_transfe_start();
    hal_spi_transfer_one_byte(CMD_SET_SR,0);
    hal_spi_transfer_one_byte(0,1);
    hal_spi_transfe_stop();
    wait();
}
void hal_spi_dev_flash_get_id_test(void)
{
    char v;
    Serial.println("test - get - id, should get 0x1f, 0x45, 0x01.\n");
    hal_spi_transfe_start();
    v = hal_spi_transfer_one_byte(CMD_GET_ID,0);
    v = hal_spi_transfer_one_byte(0x00,0);// Serial.println("v=%x\n",v);
    v = hal_spi_transfer_one_byte(0x00,0);// Serial.println("v=%x\n",v);
    v = hal_spi_transfer_one_byte(0x00,0);// Serial.printlnntf("v=%x\n",v);
    v = hal_spi_transfer_one_byte(0x00,1);// Serial.println("v=%x\n",v);
    hal_spi_transfe_stop();
}
uint32_t hal_spi_dev_flash_get_id(void)
{
    uint32_t r = 0;
    Serial.println("test - get - id");
    hal_spi_transfe_start();
    hal_spi_transfer_one_byte(CMD_GET_ID,0);
    r |= hal_spi_transfer_one_byte(0x00,0)<<0;
    r |= hal_spi_transfer_one_byte(0x00,0)<<8;
    r |= hal_spi_transfer_one_byte(0x00,0)<<16;
    r |= hal_spi_transfer_one_byte(0x00,1)<<24;
    hal_spi_transfe_stop();
    return r;
}
void hal_spi_dev_flash_erase_block(uint32_t addr)
{
    write_enable();
    hal_spi_transfe_start();
    hal_spi_transfer_one_byte(CMD_BLOCK_ERASE,0);
    hal_spi_transfer_one_byte(addr>>16,0);
    hal_spi_transfer_one_byte(addr>>8, 0);
    hal_spi_transfer_one_byte(addr>>0, 1);
    hal_spi_transfe_stop();
    wait();
}
void hal_spi_dev_flash_erase_dev(void)
{
    write_enable();
    hal_spi_transfe_start();
    hal_spi_transfer_one_byte(CMD_DEV_ERASE,1);
    hal_spi_transfe_stop();
    wait();
}
void hal_spi_dev_flash_program_byte(uint32_t addr, char b)
{
    write_enable();
    hal_spi_transfe_start();
    hal_spi_transfer_one_byte(CMD_PROGRAM,0);
    hal_spi_transfer_one_byte(addr>>16,0);
    hal_spi_transfer_one_byte(addr>>8, 0);
    hal_spi_transfer_one_byte(addr>>0, 0);
    hal_spi_transfer_one_byte(b, 1);
    hal_spi_transfe_stop();

    wait();
}
void hal_spi_dev_flash_program_page(uint32_t addr, char *buf)
{
    int i;
    write_enable();
    hal_spi_transfe_start();
    hal_spi_transfer_one_byte(CMD_PROGRAM,0);
    hal_spi_transfer_one_byte(addr>>16,0);
    hal_spi_transfer_one_byte(addr>>8, 0);
    hal_spi_transfer_one_byte(addr>>0, 0);
    for(i=0;i<(PAGE_SIZE-1);i++)
        hal_spi_transfer_one_byte(buf[i],0);
    hal_spi_transfer_one_byte(buf[i],1);
    hal_spi_transfe_stop();
    wait();
}
char hal_spi_dev_flash_read_byte(uint32_t addr)
{
    char r;
    hal_spi_transfe_start();
    hal_spi_transfer_one_byte(CMD_READ,0);
    hal_spi_transfer_one_byte(addr>>16,0);
    hal_spi_transfer_one_byte(addr>>8, 0);
    hal_spi_transfer_one_byte(addr>>0, 0);
    hal_spi_transfer_one_byte(0x00,0);
    r = hal_spi_transfer_one_byte(0x00,1);
    hal_spi_transfe_stop();
    return r;
}
void hal_spi_dev_flash_read_page(uint32_t addr, char *buf)
{
    int i;
    hal_spi_transfe_start();
    hal_spi_transfer_one_byte(CMD_READ,0);
    hal_spi_transfer_one_byte(addr>>16,0);
    hal_spi_transfer_one_byte(addr>>8, 0);
    hal_spi_transfer_one_byte(addr>>0, 0);
    hal_spi_transfer_one_byte(0x00,0);
    for(i=0;i<(PAGE_SIZE-1);i++)
        buf[i] = hal_spi_transfer_one_byte(0x00,0);
    buf[i] = hal_spi_transfer_one_byte(0x00,1);
    hal_spi_transfe_stop();
}
uint32_t hal_spi_dev_flash_get_page_size(void)
{
    return 256;
}
uint32_t hal_spi_dev_flash_get_block_size(void)
{
    return 1024*4;
}
uint32_t hal_spi_dev_flash_get_dev_size(void)
{
    return 0x100000;
}
void hal_spi_dev_flash_test2(void)
{
    global_unprotected();
}

void print_buf(void *buf, uint32_t size)
{
    int i;
    char *p = (char*)buf;
    for(i=0;i<size;i++)
    {
        Serial.print("0x");
        Serial.print(p[i], HEX);
        //printf("0x%.2x,", p[i]);
        if(((i+1)&0x3) == 0)
            Serial.print("  ");
        if(((i+1)&0x7) == 0)
            Serial.print("\n");
    }
    Serial.println("");
    //printf("\n");
}
extern void hal_spi_dev_flash_test(void)
{
    Serial.println("hal_spi_dev_flash_test start!");
    static char buf[256];
    int i;

    hal_spi_dev_flash_get_id_test();

    Serial.println("unprotecting:...");
    global_unprotected();
    Serial.println("erasing:...");
    hal_spi_dev_flash_erase_block(0);
    Serial.println("reading page...");
    hal_spi_dev_flash_read_page(0,buf);
    print_buf(buf,PAGE_SIZE);

    Serial.println("programing a page...");
    for(i=0;i<256;i++)
        buf[i] = i+0x30;
    hal_spi_dev_flash_program_page(0,buf);

    Serial.println("clearing buffer...");
    for(i=0;i<256;i++)
        buf[i] = 0;

    Serial.println("reading page...");
    hal_spi_dev_flash_read_page(0,buf);
    print_buf(buf,PAGE_SIZE);
    Serial.println("demo end.");
}

// #ifdef  __cplusplus
// }
// #endif
