#pragma once

UENUM(BlueprintType)
enum class EConfirmScreenType : uint8
{
	ECST_Ok				UMETA(DisplayName="OK"),
	ECST_YesNo			UMETA(DisplayName="Yes No"),
	ECST_OkCancel		UMETA(DisplayName="Ok Cancel"),
	ECST_Unknown		UMETA(Hidden),
};

UENUM(BlueprintType)
enum class EConfirmScreenButtonType : uint8
{
	ECSBT_Confirmed,
	ECSBT_Cancel,
	ECSBT_Closed,
	ECSBT_Unknown UMETA(Hidden)
};