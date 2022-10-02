// "Copyright (c) 2022. Made by Title Goose Team during LudumDare 51. All rights reserved. "

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StickerNote.generated.h"

class UTextRenderComponent;

UCLASS()
class LUDUMDARE51_API AStickerNote : public AActor
{
	GENERATED_BODY()

public:
	AStickerNote();

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	UStaticMeshComponent* NoteMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Components)
	UTextRenderComponent* NoteTextRenderer = nullptr;

	UFUNCTION(BlueprintCallable, Category=NoteText)
	void SetNoteText(const FText& NewText);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=NoteText)
	FText NoteText;

public:
	virtual void Tick(float DeltaTime) override;
};
