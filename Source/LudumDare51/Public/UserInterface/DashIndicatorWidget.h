// Copyright (c) 2022. Made by Title Goose Team during LudumDare 51. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DashIndicatorWidget.generated.h"

class UImage;
class UProgressBar;
class UDashComponent;
/**
 * 
 */
UCLASS()
class LUDUMDARE51_API UDashIndicatorWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

protected:
	UPROPERTY(meta=(BindWidget))
	UImage* Image_DashIcon = nullptr;

	UPROPERTY(meta=(BindWidget))
	UProgressBar* ProgressBar_Cooldown = nullptr;

	UFUNCTION(BlueprintCallable, Category="Dash")
	float GetCooldownProgress() const;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	UDashComponent* DashComponent;
};
