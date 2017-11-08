#include <stdint.h>

//#define BKPT  //printf("Hello!");

#ifdef  __cplusplus
extern "C"
{
#endif


extern void CaptureStack();

inline void PrintWord(uint32_t word);
inline void ConvertToHexChars(uint8_t h, char buffer[], uint64_t index);
void ScanForFlashCode();
extern void StackAccess(unsigned long *sptr);
static inline uint32_t* GetSP();

extern void SendData();

#ifdef  __cplusplus
}
#endif
