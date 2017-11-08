#include <stdint.h>

#ifdef  __cplusplus
extern "C"
{
#endif

uint32_t hal_spi_dev_flash_get_id(void);
void hal_spi_dev_flash_erase_block(uint32_t addr);
void hal_spi_dev_flash_erase_dev(void);
void hal_spi_dev_flash_program_byte(uint32_t addr, char b);
void hal_spi_dev_flash_program_page(uint32_t addr, char *buf);
char hal_spi_dev_flash_read_byte(uint32_t addr);
void hal_spi_dev_flash_read_page(uint32_t addr, char *buf);
uint32_t hal_spi_dev_flash_get_page_size(void);
uint32_t hal_spi_dev_flash_get_block_size(void);
uint32_t hal_spi_dev_flash_get_dev_size(void);


void hal_spi_dev_flash_get_id_test(void);
void hal_spi_dev_flash_test(void);

#ifdef  __cplusplus
}
#endif