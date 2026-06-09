// Sebastian Lara. All rights reserved


#include "Subsystems/FrontendUISubsystem.h"
#include "Widgets/Widget_PrimaryLayout.h"
#include "Engine/AssetManager.h"
#include "Widgets/Widget_ActivatableBase.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "FrontendDebugHelper.h"
#include "FrontendFunctionLibrary.h"
#include "FrontendGameplayTags.h"
#include "Widgets/Widget_ConfirmScreen.h"


UFrontendUISubsystem* UFrontendUISubsystem::Get(const UObject* WorldContextObject)
{
	if (GEngine)
	{
		const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
		return UGameInstance::GetSubsystem<UFrontendUISubsystem>(World->GetGameInstance());
	}
	return nullptr;
}

bool UFrontendUISubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (!CastChecked<UGameInstance>(Outer)->IsDedicatedServerInstance())
	{
		TArray<UClass*> FoundClasses;
		GetDerivedClasses(GetClass(), FoundClasses);
		return FoundClasses.IsEmpty();
	}
	return false;
}

void UFrontendUISubsystem::RegisterCreatedPrimaryLayoutWidget(UWidget_PrimaryLayout* InCreatedWidget)
{
	check(InCreatedWidget);

	CreatedPrimaryLayout = InCreatedWidget;
	Debug::Print(TEXT("Primary layout widget stored"));
}

void UFrontendUISubsystem::PushSoftWidgetToStackAsync(const FGameplayTag& InWidgetStackTag,
	TSoftClassPtr<UWidget_ActivatableBase> InSoftWidgetClass,
	TFunction<void(EAsyncPushWidgetState, UWidget_ActivatableBase*)> AsyncPushStateCallback)
{
	check(!InSoftWidgetClass.IsNull());
	UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(InSoftWidgetClass.ToSoftObjectPath(),
		
	FStreamableDelegate::CreateLambda([InSoftWidgetClass, this, InWidgetStackTag, AsyncPushStateCallback]()
	{
		UClass* LoadedWidgetClass = InSoftWidgetClass.Get();
		check(LoadedWidgetClass && CreatedPrimaryLayout);
		
		UCommonActivatableWidgetContainerBase* FoundWidgetStack = CreatedPrimaryLayout->FindWidgetStackByTag(InWidgetStackTag);
		UWidget_ActivatableBase* CreatedWidget = FoundWidgetStack->AddWidget<UWidget_ActivatableBase>(
			LoadedWidgetClass,[AsyncPushStateCallback](UWidget_ActivatableBase& CreatedWidgetInstance)
		{
			AsyncPushStateCallback(EAsyncPushWidgetState::OnCreatedBeforePush, &CreatedWidgetInstance);
		});
	AsyncPushStateCallback(EAsyncPushWidgetState::AfterPush, CreatedWidget);
	})
	);
}

void UFrontendUISubsystem::PushConfirmScreenToModalStackAsync(EConfirmScreenType InScreenType,
	const FText& InScreenTitle, const FText& InScreenMessage,
	TFunction<void(EConfirmScreenButtonType)> ButtonClickedCallback)
{
	UConfirmScreenInfoObject* CreatedInfoObject = nullptr;
	switch (InScreenType)
	{
	case EConfirmScreenType::ECST_Ok:
		CreatedInfoObject = UConfirmScreenInfoObject::CreateOkScreen(InScreenTitle,InScreenMessage);
		break;
	case EConfirmScreenType::ECST_OkCancel:
		CreatedInfoObject = UConfirmScreenInfoObject::CreateOkCancelScreen(InScreenTitle,InScreenMessage);
		break;
	case EConfirmScreenType::ECST_YesNo:
		CreatedInfoObject = UConfirmScreenInfoObject::CreateYesNoScreen(InScreenTitle,InScreenMessage);
		break;
	case EConfirmScreenType::ECST_Unknown:
		break;
	}
	check(CreatedInfoObject);
	
	PushSoftWidgetToStackAsync(FrontendGameplayTags::Frontend_WidgetStack_Modal,
		UFrontendFunctionLibrary::GetFrontendSoftWidgetClassByTag(FrontendGameplayTags::Frontend_Widget_ConfirmScreen),
		
		[CreatedInfoObject, ButtonClickedCallback](EAsyncPushWidgetState InPushWidgetState, UWidget_ActivatableBase* PushWidget)
		{
			if (InPushWidgetState == EAsyncPushWidgetState::OnCreatedBeforePush)
			{
				UWidget_ConfirmScreen* CreatedConfirmScreen = CastChecked<UWidget_ConfirmScreen>(PushWidget);
				CreatedConfirmScreen->InitConfirmScreen(CreatedInfoObject, ButtonClickedCallback);
			}
		}
		);
}
