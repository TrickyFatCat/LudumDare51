// "Copyright (c) 2022. Made by Title Goose Team during LudumDare 51. All rights reserved. "


#include "GameplayObjects/StickerNote.h"

#include "Components/TextRenderComponent.h"


AStickerNote::AStickerNote()
{
	PrimaryActorTick.bCanEverTick = true;

	NoteMesh = CreateDefaultSubobject<UStaticMeshComponent>("NoteMesh");
	SetRootComponent(NoteMesh);

	NoteTextRenderer = CreateDefaultSubobject<UTextRenderComponent>("NoteTextRenderer");
	NoteTextRenderer->SetupAttachment(GetRootComponent());
}

void AStickerNote::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	NoteTextRenderer->SetText(NoteText);
}

void AStickerNote::BeginPlay()
{
	Super::BeginPlay();
}

void AStickerNote::SetNoteText(const FText& NewText)
{
	NoteText = NewText;
	NoteTextRenderer->SetText(NewText);
}

void AStickerNote::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

