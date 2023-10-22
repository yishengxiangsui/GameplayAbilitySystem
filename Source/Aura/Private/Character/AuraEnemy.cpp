// This Project Made By Yuan Xiaodong.


#include "Character/AuraEnemy.h"

#include "Aura/Aura.h"


AAuraEnemy::AAuraEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void AAuraEnemy::HighlightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	if(Weapon!=nullptr)
	{
		Weapon->SetRenderCustomDepth(true);
		Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	}
}

void AAuraEnemy::UnHighlightActor()
{
	
	GetMesh()->SetRenderCustomDepth(false);
	if(Weapon!=nullptr)
	{
		Weapon->SetRenderCustomDepth(false);

	}
}
