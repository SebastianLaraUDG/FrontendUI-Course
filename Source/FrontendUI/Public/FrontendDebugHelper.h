#pragma once

namespace Debug
{
	static void Print(const FString& Message, int32 InKey = -1, const FColor& InColor = FColor::MakeRandomColor())
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 7.0f, InColor, Message);
			UE_LOG(LogTemp, Warning, TEXT("%s"), *Message);
		}
	}
}
