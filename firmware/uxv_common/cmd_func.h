#ifndef _FW_CMD_FUNC_H
#define _FW_CMD_FUNC_H

#ifdef __cplusplus
extern "C" {
#endif

extern uint8_t  cmd_func_otp_get_lock(int index);
extern void     cmd_func_otp_set_lock0(void /* int index*/);

extern uint32_t cmd_func_otp_get_serial(void);
extern void     cmd_func_otp_set_serial(uint32_t serial);

extern uint32_t cmd_func_otp_get_device(void);
extern void     cmd_func_otp_set_device(uint32_t device);

extern uint32_t cmd_func_otp_get_id16(void);
extern void     cmd_func_otp_set_id16(uint32_t id16);

extern uint32_t cmd_func_otp_get_id8(void);
extern void     cmd_func_otp_set_id8(uint32_t id8);

extern uint8_t* cmd_func_otp_get_name(void);

//extern void     cmd_func_uid(uint32_t* uid0, uint32_t* uid1, uint32_t* uid2);
extern void cmd_func_uid(uint32_t* uid);

extern void     cmd_func_reset(void);
extern void     cmd_func_goto_exec(uint32_t base_address);
extern void     cmd_func_goto_sysboot(void);

#ifdef __cplusplus
}
#endif

#endif /* _FW_CMD_FUNC_H */
