//
// Created by Matem on 6/23/2020.
//

#pragma once

#include <cstdlib>
#include <spdlog/spdlog.h>

/**
	Useful assert macros to be used liberally. Most will disappear under debug builds
 */


#define DIE_VAR_NULL(nullVar) do {		\
spdlog::error("{} was null!", #nullVar); \
std::abort(); 							\
} 										\
while(0); 								\

#define ASSERT_NOT_NULL(var)		\
	do {             				\
		if(!var) {					\
			DIE_VAR_NULL(#var)		\
		}							\
	} while(0);

#define ASSERT_NE(expr, val) do { \
    if (expr == val) {          \
        spdlog::error("{} is false", #expr); \
		std::abort();\
		}                      \
	} while(0);

#define ASSERT_TRUE(expr, message) do { \
    if (!expr) {            \
			spdlog::error(message);                   \
			std::abort();\
		}                              \
	} while(0);

template <typename T, typename U, typename ...Args>
inline void INTERNAL_DIE(T cond, U str, const char* file, int line, Args... args) {
	printf("\x1b[31m");
	spdlog::error("ASSERT at {}:{}\n\x1b[36mExpr: {}\x1b[31m", file, line, cond);
	spdlog::error(str, args...);
	printf("\x1b[0m");
	std::abort();
}

template <typename T, typename ...Args>
inline void DIE(T str, Args... args) {
	spdlog::error("ASSERT at {}:{}", __FILE__, __LINE__);
	spdlog::error(str, args...);
	std::abort();
}

#ifndef NDEBUG
#define ASSERT(cond, format, ...) do { \
    	cond ? (void)sizeof(cond) : INTERNAL_DIE(#cond, format, __FILE__, __LINE__, ## __VA_ARGS__); \
		(void)0;\
	} while (0);
#else
#define ASSERT(cond, format, ...) do { \
		(void)sizeof(cond);	\
	} while (0);
#endif