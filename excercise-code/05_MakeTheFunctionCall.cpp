AHttpService::AHttpService() { PrimaryActorTick.bCanEverTick = false; }
void AHttpService::BeginPlay() {
	Super::BeginPlay();
	Http = &FHttpModule::Get();

	//FRequest_Create PlayerCredentials;
	//PlayerCredentials.name = TEXT("Bilbo");
	//PlayerCredentials.ip = TEXT("200.100.50.25");
	//PlayerCredentials.hp = 100;
	//PlayerCredentials.mana = 100;

	//Create(PlayerCredentials);

	FRequest_GetPlayerInfo PlayerToGet;
	PlayerToGet.id = 1;
	GetPlayerInfo(PlayerToGet);
}
