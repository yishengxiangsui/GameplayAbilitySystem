// This Project Made By Yuan Xiaodong.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AuraUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, Category="AuraUserWidget")
	TObjectPtr<UObject> WidgetController;
public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();
};
