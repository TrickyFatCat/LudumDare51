// Copyright (c) 2022. Made by Title Goose Team during LudumDare 51. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Objects/KeyType.h"
#include "KeyringWidget.generated.h"

class UHorizontalBox;
class UKeyRingComponent;

/**
 * 
 */
UCLASS()
class LUDUMDARE51_API UKeyringWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativePreConstruct() override;
	virtual void NativeOnInitialized() override;

protected:
	UPROPERTY(meta=(BindWidget))
	UHorizontalBox* HBox_Keys = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UUserWidget> KeyIconWidget = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TSet<TSubclassOf<UKeyType>> KeysSet;

	UFUNCTION(BlueprintCallable)
	UUserWidget* AddKeyIcon(TSubclassOf<UKeyType> KeyType, const bool bAddSpacer);

	UPROPERTY(BlueprintReadOnly)
	TMap<TSubclassOf<UKeyType>, UUserWidget*> KeyIcons;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	UKeyRingComponent* KeyRingComponent;

	void CreateKeyIcons();
};
