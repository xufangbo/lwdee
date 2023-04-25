#pragma once

#include "core/log.hpp"

#define leopard_trace(fromat, ...) logger(log_trace, __FUNCTION__, __FILE__, __LINE__, fromat, ##__VA_ARGS__)
#define leopard_debug(fromat, ...) logger(log_debug, __FUNCTION__, __FILE__, __LINE__, fromat, ##__VA_ARGS__)
#define leopard_info(fromat, ...) logger(log_info, __FUNCTION__, __FILE__, __LINE__, fromat, ##__VA_ARGS__)
#define leopard_warn(fromat, ...) logger(log_warn, __FUNCTION__, __FILE__, __LINE__, fromat, ##__VA_ARGS__)
#define leopard_error(fromat, ...) logger(log_error, __FUNCTION__, __FILE__, __LINE__, fromat, ##__VA_ARGS__)
#define leopard_fatal(fromat, ...) logger(log_fatal, __FUNCTION__, __FILE__, __LINE__, fromat, ##__VA_ARGS__)

// #define leopard_trace  //
// #define leopard_debug  //
// #define leopard_info   //
// #define leopard_warn   //
// #define leopard_error  //
// #define leopard_fatal  //
