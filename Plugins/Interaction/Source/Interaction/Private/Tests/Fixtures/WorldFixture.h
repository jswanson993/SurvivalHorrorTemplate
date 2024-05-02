#pragma once
#include "EngineUtils.h"

class FWorldFixture
{
TWeakObjectPtr<UWorld> WeakWorld;
	
public:
	explicit FWorldFixture(const FURL& URL = FURL());
	

	UWorld* GetWorld() const {return WeakWorld.Get();}
	~FWorldFixture();
};
