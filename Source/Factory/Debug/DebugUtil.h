#pragma once

#define DebugLog(message) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Emerald, TEXT(message));}
#define DebugLogError(message) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT(message));}