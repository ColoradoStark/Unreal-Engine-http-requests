	UFUNCTION(BlueprintCallable)
	void GetPlayerInfo(FRequest_GetPlayerInfo PlayerID);
	void GetPlayerInfoResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
