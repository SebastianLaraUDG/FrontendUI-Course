// Sebastian Lara. All rights reserved


#include "Widgets/Components/FrontendCommonButtonBase.h"
#include "CommonTextBlock.h"

void UFrontendCommonButtonBase::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	SetButtonText(ButtonDisplayText);
}

void UFrontendCommonButtonBase::SetButtonText(FText InText)
{
	if (CommonTextBlock_ButtonText)
	{
		CommonTextBlock_ButtonText->SetText(bUserUpperCaseForButtonText ? InText.ToUpper() : InText);
	}
}
