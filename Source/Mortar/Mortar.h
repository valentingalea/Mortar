#pragma once

#include "CoreMinimal.h"

#define make_sure(cond) \
	if (!(ensure(cond))) return

DECLARE_LOG_CATEGORY_EXTERN(LogMortar, Warning, All);

void MortarScreenLog(const FString&);

template <typename FmtType, typename... Types>
inline void MortarLog(FmtType && fmt, Types &&... args)
{
	UE_LOG(LogMortar, Warning, fmt, args...);
	MortarScreenLog(FString::Printf(fmt, Forward<Types>(args)...));
}

template <typename FmtType, typename... Types>
inline void MortarErr(FmtType && fmt, Types &&... args)
{
	UE_LOG(LogMortar, Error, fmt, args...);
	MortarScreenLog(FString::Printf(fmt, Forward<Types>(args)...));
}