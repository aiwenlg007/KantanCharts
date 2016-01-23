// Copyright (C) 2015 Cameron Angus. All Rights Reserved.

#pragma once

#include "KantanCartesianTypes.h"
#include "KantanSeriesStyle.generated.h"


UCLASS()
class KANTANCHARTSSLATE_API UKantanPointStyle : public UObject
{
	GENERATED_BODY()

public:
	/*
	Texture providing datapoint image in different sizes.
	*/
	UPROPERTY(EditAnywhere)
	UTexture2D* DataPointTexture;

	/*
	Pixel (x, y) offsets into the texture for each point size.
	The image sections should be 5x5, 7x7 and 9x9 respectively.
	*/
	UPROPERTY(EditAnywhere)
	FIntPoint PointSizeTextureOffsets[KantanDataPointSizeCount];

public:
	UKantanPointStyle() :
		DataPointTexture(nullptr)
		, PointSizeTextureOffsets()
	{}

	inline bool HasValidTexture() const
	{
		return DataPointTexture != nullptr;
	}
};

// Defines visual properties that can be configured per series
USTRUCT(BlueprintType)
struct FKantanSeriesStyle
{
	GENERATED_BODY()

	/*
	Unique ID for the series.
	*/
	UPROPERTY(EditAnywhere)
	FName StyleId;

	/*
	Point style asset.
	*/
	UPROPERTY(EditAnywhere)
	UKantanPointStyle* PointStyle;

	/*
	Color to use when drawing the series.
	*/
	UPROPERTY(EditAnywhere)
	FLinearColor Color;

	FKantanSeriesStyle() :
		StyleId(NAME_None)
		, PointStyle(nullptr)
		, Color(FLinearColor::White)
	{}

	inline bool HasValidPointStyle() const
	{
		return PointStyle != nullptr && PointStyle->HasValidTexture();
	}
};


