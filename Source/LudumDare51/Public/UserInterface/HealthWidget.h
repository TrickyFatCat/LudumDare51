// Copyright (c) 2022. Made by Title Goose Team during LudumDare 51. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthWidget.generated.h"

class UHealthComponent;
class UHorizontalBox;

/**
 * 
 */
UCLASS()
class LUDUMDARE51_API UHealthWidget : public UUserWidget
{
	GENERATED_BODY()
	public:
	virtual void NativePreConstruct() override;
	virtual void NativeOnInitialized() override;

protected:
	UPROPERTY(meta=(BindWidget))
	UHorizontalBox* HBox_Health= nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UUserWidget> HealthIconWidget = nullptr;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	TArray<UUserWidget*> HealthIcons;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	UHealthComponent* HealthComponent;

	void CreateHealthIcons(const int32 Amount = 5);
};
