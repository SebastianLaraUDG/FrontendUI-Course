// Sebastian Lara. All rights reserved

#include "FrontendUI/Public/Controllers/FrontendPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"

void AFrontendPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	TArray<AActor*> FoundCameras;
	// Get cameras
	UGameplayStatics::GetAllActorsOfClassWithTag(
		this,
		ACameraActor::StaticClass(),
		FName("Default"), FoundCameras);

	// Set the default camera as the view target
	if (!FoundCameras.IsEmpty())
	{
		SetViewTarget(FoundCameras[0]);
	}
}
