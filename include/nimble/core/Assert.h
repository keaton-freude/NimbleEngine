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

