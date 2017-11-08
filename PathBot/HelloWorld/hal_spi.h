// #ifndef _HAL_SPI_H
// #define _HAL_SPI_H
#ifdef  __cplusplus
extern "C"
{
#endif
void hal_spi_init(void);
void hal_spi_transfe_start(void);
void hal_spi_transfe_stop(void);
char   hal_spi_transfer_one_byte(char v, char end);

#ifdef  __cplusplus
}
#endif

//#endif