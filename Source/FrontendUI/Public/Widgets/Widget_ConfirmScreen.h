// Sebastian Lara. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Widget_ActivatableBase.h"
#include "FrontendTypes/FrontendEnumTypes.h"
#include "Widget_ConfirmScreen.generated.h"

class UDynamicEntryBox;
class UCommonTextBlock;

USTRUCT(BlueprintType)
struct FConfirmScreenButtonInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EConfirmScreenButtonType ConfirmScreenButtonType= EConfirmScreenButtonType::ECSBT_Unknown;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ButtonTextToDisplay;
};

UCLASS()
class FRONTENDUI_API UConfirmScreenInfoObject : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(Transient)
	FText ScreenTitle;
	
	UPROPERTY(Transient)
	FText ScreenMessage;
	
	UPROPERTY(Transient)
	TArray<FConfirmScreenButtonInfo> AvailableScreenButtons;
	
	static UConfirmScreenInfoObject* CreateOkScreen(const FText& InScreenTitle, const FText& InScreenMessage);
	static UConfirmScreenInfoObject* CreateYesNoScreen(const FText& InScreenTitle, const FText& InScreenMessage);
	static UConfirmScreenInfoObject* CreateOkCancelScreen(const FText& InScreenTitle, const FText& InScreenMessage);
	
	static UConfirmScreenInfoObject* CreateConfirmScreenInfoObject(const FText& InScreenTitle, const FText& InScreenMessage, const int32 NumButtons);
};

/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class FRONTENDUI_API UWidget_ConfirmScreen : public UWidget_ActivatableBase
{
	GENERATED_BODY()
	
public:
	// Gets called outside the class when this widget is constructed, and before it is pushed to the modal stack.
	void InitConfirmScreen(UConfirmScreenInfoObject* InScreenInfoObject,  TFunction<void(EConfirmScreenButtonType)> ClickedButtonCallback);
	
private:
	UPROPERTY(meta= (BindWidget))
	TObjectPtr<UCommonTextBlock> CommonTextBlock_Title;
	
	UPROPERTY(meta= (BindWidget))
	TObjectPtr<UCommonTextBlock> CommonTextBlock_Message;
	
	UPROPERTY(meta= (BindWidget))
	TObjectPtr<UDynamicEntryBox> DynamicEntryBox_Buttons;
};
