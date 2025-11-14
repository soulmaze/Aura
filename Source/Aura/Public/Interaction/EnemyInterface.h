// Copyright Soulmaze Compony

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EnemyInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEnemyInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API IEnemyInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	//making pure function there will be no need for definition here so we can define it in the class that use this abstract class
	//pure virtual fuction make the class abstract
	virtual void HighlightActor() = 0;
	virtual void UnhighlightActor() = 0;
};
