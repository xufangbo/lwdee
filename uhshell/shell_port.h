/**
 * @file shell_port.h
 * @author Letter (NevermindZZT@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2019-02-22
 * 
 * @copyright (c) 2019 Letter
 * 
 */

#ifndef __SHELL_PORT_H__
#define __SHELL_PORT_H__

#include "shell.h"

#ifdef __cplusplus
extern "C" {
#endif /**< defined __cplusplus */
extern Shell shell;

void userShellInit(void);

#ifdef __cplusplus
}
#endif /**< defined __cplusplus */
#endif
