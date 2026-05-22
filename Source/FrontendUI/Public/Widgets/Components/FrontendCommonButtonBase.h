// Sebastian Lara. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "FrontendCommonButtonBase.generated.h"

class UCommonTextBlock;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class FRONTENDUI_API UFrontendCommonButtonBase : public UCommonButtonBase
{
	GENERATED_BODY()
protected:
	// ~Begin UUserWidget interface.
	virtual void NativePreConstruct() override;
	// ~End UUserWidget interface.
	
	// ~Begin UCommonButtonBase interface.
	virtual void NativeOnCurrentTextStyleChanged() override;
	// ~End UCommonButtonBase interface.

public:
    	UFUNCTION(BlueprintCallable)
    	void SetButtonText(FText InText);
private:
	
	// Bound widgets
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UCommonTextBlock> CommonTextBlock_ButtonText;
	
	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Frontend Button", meta = (AllowPrivateAccess = true))
	FText ButtonDisplayText = FText::FromString(TEXT("Default Text"));
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Frontend Button", meta = (AllowPrivateAccess = true))
	bool bUserUpperCaseForButtonText = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Frontend Button", meta = (AllowPrivateAccess = true))
	FText ButtonDescriptionText;
};
