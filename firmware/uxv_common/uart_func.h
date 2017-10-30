#ifndef _FW_UART_FUNC_H
#define _FW_UART_FUNC_H




#ifdef __cplusplus
extern "C" {
#endif
extern    uint32_t      uart3_rxcount;

#if (HAL_USE_UART == TRUE)
extern    UARTConfig    uart3_cfg;
#endif

#ifdef __cplusplus
}
#endif



#endif /* _FW_UART_FUNC_H */
