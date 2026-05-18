// Sebastian Lara. All rights reserved


#include "FrontendFunctionLibrary.h"
#include "FrontendSettings/FrontendDeveloperSettings.h"


TSoftClassPtr<UWidget_ActivatableBase> UFrontendFunctionLibrary::GetFrontendSoftWidgetClassByTag(
	FGameplayTag InWidgetTag)
{
	auto FrontendDeveloperSettings = GetDefault<UFrontendDeveloperSettings>();
	checkf(FrontendDeveloperSettings->FrontendWidgetMap.Contains(InWidgetTag),
	       TEXT("Could not find the corresponding widget under the tag %s"), *InWidgetTag.ToString())
	return  FrontendDeveloperSettings->FrontendWidgetMap.FindRef(InWidgetTag);
}
