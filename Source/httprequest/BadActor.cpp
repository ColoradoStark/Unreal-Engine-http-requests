// Fill out your copyright notice in the Description page of Project Settings.

#include "BadActor.h"


// Sets default values
ABadActor::ABadActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABadActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABadActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Uncomment this next line if you want to see a memory leak that will take your computer down fast.
	//FString * MyString = new FString[100000];
	
}

