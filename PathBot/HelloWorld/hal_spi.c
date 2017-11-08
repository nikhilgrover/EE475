
/*
    by: b35970@freescale.com
*/
#include "hal_spi.h"
#include "kinetis.h"


#define SPI_MCR_HALT_MASK                        0x1u
#define SPI_MCR_CLR_RXF_MASK                     0x400u
#define SPI_MCR_CLR_TXF_MASK                     0x800u
#define SPI_MCR_DIS_RXF_MASK                     0x1000u
#define SPI_MCR_DIS_TXF_MASK                     0x2000u
#define SPI_MCR_MDIS_MASK                        0x4000u
#define SPI_MCR_MSTR_MASK                        0x80000000u

#define SPI_CTAR_FMSZ_MASK                       0x78000000u
#define SPI_SR_TCF_MASK                          0x80000000u
#define SPI_PUSHR_EOQ_MASK                       0x8000000u
#define SPI_PUSHR_CONT_MASK                      0x80000000u
#define SIM_SCGC3_SPI2_MASK                      0x1000u


#ifdef  __cplusplus
extern "C"
{
#endif

static void init_clock(void)
{
    SIM_SCGC3 |= SIM_SCGC3_SPI2_MASK;
}
static void init_io(void)
{
    PORTD_PCR11 &= ~PORT_PCR_MUX_MASK;
    PORTD_PCR11 |= PORT_PCR_MUX(2);
    PORTD_PCR12 &= ~PORT_PCR_MUX_MASK;
    PORTD_PCR12 |= PORT_PCR_MUX(2);
    PORTD_PCR13 &= ~PORT_PCR_MUX_MASK;
    PORTD_PCR13 |= PORT_PCR_MUX(2);
    PORTD_PCR14 &= ~PORT_PCR_MUX_MASK;
    PORTD_PCR14 |= PORT_PCR_MUX(2);
}
static void init_set_master(void)
{
    SPI2_MCR |= SPI_MCR_MSTR_MASK;
}
static void init_fifo(void)
{
    SPI2_MCR &= ~SPI_MCR_MDIS_MASK;

    SPI2_MCR |= SPI_MCR_DIS_RXF_MASK |
                SPI_MCR_DIS_TXF_MASK |
                SPI_MCR_CLR_RXF_MASK |
                SPI_MCR_CLR_TXF_MASK;

}
static void init_inactive_cs(void)
{
    SPI2_MCR |= SPI_MCR_PCSIS(1<<0);
}
static void init_inactive_clock(void)
{
    // must be low
}
static void init_frame_size(void)
{
    SPI2_CTAR0 &= ~SPI_CTAR_FMSZ_MASK;
    SPI2_CTAR0 |= SPI_CTAR_FMSZ(7);
}
static void init_clock_phase(void)
{
    // defalut = capture data on rising edge
}
static void init_baudrate(void)
{
    // default = sys clock/2/2 = 48/4 = 12M
}
static void init_msb_first(void)
{
    // default is msb first
}
void hal_spi_transfe_start(void)
{
    SPI2_MCR &= ~SPI_MCR_HALT_MASK;
}
void hal_spi_transfe_stop(void)
{
    SPI2_MCR |= SPI_MCR_HALT_MASK;
}
char hal_spi_transfer_one_byte(char v, char end)
{
    if(end)
        SPI2_PUSHR = //SPI_PUSHR_CONT_MASK |
                     SPI_PUSHR_EOQ_MASK  |
                     SPI_PUSHR_PCS(1<<0) |
                     (v);
    else
        SPI2_PUSHR = SPI_PUSHR_CONT_MASK |
                     SPI_PUSHR_PCS(1<<0) |
                     (v);

    while((SPI2_SR & SPI_SR_TCF_MASK)==0)
        ;
    SPI2_SR |= SPI_SR_TCF_MASK;
    return SPI2_POPR&0xff;
}
void hal_spi_init(void)
{
    init_clock();
    init_io();
    init_set_master();
    init_fifo();
    init_inactive_cs();
    init_inactive_clock();
    init_frame_size();
    init_clock_phase();
    init_msb_first();
    init_baudrate();
}
#ifdef  __cplusplus
}
#endif