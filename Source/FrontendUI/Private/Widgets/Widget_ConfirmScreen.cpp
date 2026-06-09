// Sebastian Lara. All rights reserved


#include "Widgets/Widget_ConfirmScreen.h"
#include "CommonTextBlock.h"
#include "ICommonInputModule.h"
#include "Components/DynamicEntryBox.h"
#include "Widgets/Components/FrontendCommonButtonBase.h"

UConfirmScreenInfoObject* UConfirmScreenInfoObject::CreateOkScreen(const FText& InScreenTitle,
                                                                   const FText& InScreenMessage)
{
	UConfirmScreenInfoObject* InfoObject = CreateConfirmScreenInfoObject(InScreenTitle,InScreenMessage, 1);
	
	FConfirmScreenButtonInfo OkButtonInfo;
	OkButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::ECSBT_Closed;
	OkButtonInfo.ButtonTextToDisplay = FText::FromString("Ok");
	
	InfoObject->AvailableScreenButtons.Add(OkButtonInfo);
	
	return InfoObject;
}

UConfirmScreenInfoObject* UConfirmScreenInfoObject::CreateYesNoScreen(const FText& InScreenTitle,
	const FText& InScreenMessage)
{
	UConfirmScreenInfoObject* InfoObject = CreateConfirmScreenInfoObject(InScreenTitle,InScreenMessage, 2);
	
	FConfirmScreenButtonInfo YesButtonInfo;
	YesButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::ECSBT_Confirmed;
	YesButtonInfo.ButtonTextToDisplay = FText::FromString("Yes");
	
	FConfirmScreenButtonInfo NoButtonInfo;
	NoButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::ECSBT_Cancel;
	NoButtonInfo.ButtonTextToDisplay = FText::FromString("No");
	
	InfoObject->AvailableScreenButtons.Add(YesButtonInfo);
	InfoObject->AvailableScreenButtons.Add(NoButtonInfo);
	
	return InfoObject;
}

UConfirmScreenInfoObject* UConfirmScreenInfoObject::CreateOkCancelScreen(const FText& InScreenTitle,
	const FText& InScreenMessage)
{
	UConfirmScreenInfoObject* InfoObject = CreateConfirmScreenInfoObject(InScreenTitle,InScreenMessage, 2);
	
	FConfirmScreenButtonInfo OkButtonInfo;
	OkButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::ECSBT_Confirmed;
	OkButtonInfo.ButtonTextToDisplay = FText::FromString("Ok");
	
	FConfirmScreenButtonInfo CancelButtonInfo;
	CancelButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::ECSBT_Cancel;
	CancelButtonInfo.ButtonTextToDisplay = FText::FromString("Cancel");
	
	InfoObject->AvailableScreenButtons.Add(OkButtonInfo);
	InfoObject->AvailableScreenButtons.Add(CancelButtonInfo);
	
	return InfoObject;
}

UConfirmScreenInfoObject* UConfirmScreenInfoObject::CreateConfirmScreenInfoObject(const FText& InScreenTitle,
	const FText& InScreenMessage, const int32 NumButtons)
{
	UConfirmScreenInfoObject* InfoObject = NewObject<UConfirmScreenInfoObject>();
	InfoObject->ScreenTitle = InScreenTitle;
	InfoObject->ScreenMessage = InScreenMessage;
	InfoObject->AvailableScreenButtons.Reserve(NumButtons); // My optimization since only one element will be stored in this option.
	return InfoObject;
}

void UWidget_ConfirmScreen::InitConfirmScreen(UConfirmScreenInfoObject* InScreenInfoObject,
	TFunction<void(EConfirmScreenButtonType)> ClickedButtonCallback)
{
	check(InScreenInfoObject && CommonTextBlock_Title && CommonTextBlock_Message && DynamicEntryBox_Buttons);
	
	CommonTextBlock_Title->SetText(InScreenInfoObject->ScreenTitle);
	CommonTextBlock_Message->SetText(InScreenInfoObject->ScreenMessage);

	// Check if the entry box has previously created buttons.
	if (DynamicEntryBox_Buttons->GetNumEntries() != 0)
	{
		// Clear previous buttons.
		// The widget type for the entry box is specified in the child widget blueprint.
		DynamicEntryBox_Buttons->Reset<UFrontendCommonButtonBase>(
			[](UFrontendCommonButtonBase& ExistingButton) -> void
			{
				ExistingButton.OnClicked().Clear();
			}
		);
	}
	
	// Confirm screen should always contain buttons.
	check(!InScreenInfoObject->AvailableScreenButtons.IsEmpty());
	
	FDataTableRowHandle InputActionRowHandle;
	
	for (const FConfirmScreenButtonInfo& AvailableButtonInfo : InScreenInfoObject->AvailableScreenButtons)
	{
		switch (AvailableButtonInfo.ConfirmScreenButtonType)
		{
		case EConfirmScreenButtonType::ECSBT_Confirmed:
			InputActionRowHandle = ICommonInputModule::GetSettings().GetDefaultClickAction();
			break;
		case EConfirmScreenButtonType::ECSBT_Cancel:
			InputActionRowHandle = ICommonInputModule::GetSettings().GetDefaultBackAction();
			break;
		case EConfirmScreenButtonType::ECSBT_Closed:
			InputActionRowHandle = ICommonInputModule::GetSettings().GetDefaultBackAction();
			break;
		case EConfirmScreenButtonType::ECSBT_Unknown:
			break;
		}
		
		UFrontendCommonButtonBase* AddedButton = DynamicEntryBox_Buttons->CreateEntry<UFrontendCommonButtonBase>();
		AddedButton->SetButtonText(AvailableButtonInfo.ButtonTextToDisplay);
		AddedButton->SetTriggeredInputAction(InputActionRowHandle);
		//AddedButton->SetTriggeringEnhancedInputAction() // TODO: test this.
		AddedButton->OnClicked().AddLambda(
			[this, ClickedButtonCallback, AvailableButtonInfo]()
			{
				ClickedButtonCallback(AvailableButtonInfo.ConfirmScreenButtonType);
				DeactivateWidget();
			}
		);
	}

	if (DynamicEntryBox_Buttons->GetNumEntries() != 0)
	{
		// Set focus on the last focus. So if there are two buttons, one is yes, one is no.
		// Our gamepad will focus on the No button.
		DynamicEntryBox_Buttons->GetAllEntries().Last()->SetFocus();
	}
}
