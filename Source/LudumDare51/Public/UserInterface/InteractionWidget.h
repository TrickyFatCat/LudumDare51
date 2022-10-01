// Copyright (c) 2022. Made by Title Goose Team during LudumDare 51. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "UserInterface/BaseUserWidget.h"
#include "InteractionWidget.generated.h"

class UImage;
class UTextBlock;
class UInteractionQueueComponent;

/**
 * 
 */
UCLASS()
class LUDUMDARE51_API UInteractionWidget : public UBaseUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

protected:
	UPROPERTY(meta=(BindWidget))
	UImage* Image_KeyHint = nullptr;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* Text_InteractionMessage = nullptr;

private:
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	APawn* PlayerPawn = nullptr;
	
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	UInteractionQueueComponent* InteractionComponent = nullptr;

};
