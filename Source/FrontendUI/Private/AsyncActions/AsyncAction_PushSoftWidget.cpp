// Sebastian Lara. All rights reserved


#include "AsyncActions/AsyncAction_PushSoftWidget.h"

UAsyncAction_PushSoftWidget* UAsyncAction_PushSoftWidget::PushSoftWidget(const UObject* WorldContextObject,
                                                                         APlayerController* OwningPlayerController,
                                                                         TSoftClassPtr<UWidget_ActivatableBase>
                                                                         InSoftWidgetClass,
                                                                         UPARAM(
	                                                                         meta = (Categories = "Frontend.WidgetStack"
	                                                                         ))
                                                                         FGameplayTag InWidgetStackTag,
                                                                         bool bFocusOnNewlyPushedWidget)
{
	checkf(!InSoftWidgetClass.IsNull(), TEXT("PushSoftWidgetToStack was passed a null soft widget class"));

	if (GEngine)
	{
		if (UWorld* const World = GEngine->GetWorldFromContextObject(WorldContextObject,
		                                                             EGetWorldErrorMode::LogAndReturnNull))
		{
			UAsyncAction_PushSoftWidget* Node = NewObject<UAsyncAction_PushSoftWidget>();
			Node->RegisterWithGameInstance(World);
			return Node;
		}
	}
	return nullptr;
}
