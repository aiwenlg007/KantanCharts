// Copyright (C) 2015 Cameron Angus. All Rights Reserved.

#pragma once

#include "Interface.h"
#include "KantanCategoryDatasourceInterface.generated.h"


UINTERFACE(Blueprintable, MinimalAPI)
class UKantanCategoryDatasourceInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};


class KANTANCHARTSDATASOURCE_API IKantanCategoryDatasourceInterface
{
	GENERATED_IINTERFACE_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Category Datasource")
	int32 GetNumCategories() const;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Category Datasource")
	FName GetCategoryId(int32 CatIdx) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Category Datasource")
	FText GetCategoryName(int32 CatIdx) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Category Datasource")
	float GetCategoryValue(int32 CatIdx) const;
};


