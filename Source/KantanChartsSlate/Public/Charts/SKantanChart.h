// Copyright (C) 2015 Cameron Angus. All Rights Reserved.

#pragma once

#include "SLeafWidget.h"
#include "KantanChartStyle.h"
#include "AxisUtility.h"
#include "CartesianAxisConfig.h"	// @TODO: Refactor so not including anything specific to cartesian charts


enum class EKantanChartLabelClass : uint8 {
	ChartTitle,
	AxisTitle,
	AxisMarkerLabel,	// For now, this can be category label as well as cartesian value label
};

/** A base class for chart widgets. */
class KANTANCHARTSSLATE_API SKantanChart :
	public SLeafWidget
	, public FGCObject
{
public:
	SLATE_BEGIN_ARGS(SKantanChart):
		_UpdateTickRate(0.0f)
		{}

		SLATE_ARGUMENT(FMargin, Margins)

		// @TODO: Should this be arg, attrib, or perhaps provided by the data source?
		SLATE_ARGUMENT(FText, ChartTitle)

		/** Rate at which this widget is ticked when sleeping in seconds */
		SLATE_ARGUMENT(float, UpdateTickRate)
	SLATE_END_ARGS()

	/**
	 * Construct the widget
	 * 
	 * @param InArgs   A declaration from which to construct the widget
	 */
	void Construct(const FArguments& InArgs);

	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyClippingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

	void SetMargins(FMargin const& InMargins);
	void SetChartTitle(FText const& InTitle);
	void SetChartTitlePadding(FMargin const& InPadding);
	void SetUpdateTickRate(float InRate);

	virtual const FKantanChartStyle* GetChartStyle() const = 0;

	static FSlateFontInfo GetLabelFont(const FKantanChartStyle* ChartStyle, EKantanChartLabelClass LabelClass);

protected:
	class EChartContentArea
	{
	public:
		// @TODO: Ideally generalize these to something like LeftBoxOuter, LeftBoxInner, Center...
		enum Type {
			XAxisBottomTitle,
			XAxisTopTitle,
			YAxisLeftTitle,
			YAxisRightTitle,
			XAxisBottom,
			XAxisTop,
			YAxisLeft,
			YAxisRight,
			Plot,

			Count,
		};
	};

	static const int32 ChartContentAreaCount = EChartContentArea::Count;

protected:
	// Implementation must return size of the given area along the required component.
	// KnownPlotSize will have at most 1 component calculated, which will be the other component (not ReqComp)
	virtual float GetChartAreaSize(EChartContentArea::Type Area, EAxis::Type ReqComp, FVector2D const& KnownPlotSize) const = 0;
	virtual int32 DrawChartArea(
		EChartContentArea::Type Area,
		const FPaintArgs& Args,
		const FGeometry& Geometry,
		const FGeometry& PlotSpaceGeometry,	// @TODO: So we always know the geometry for the plot itself, maybe better way to do this
		const FSlateRect& MyClippingRect,
		FSlateWindowElementList& OutDrawElements,
		int32 LayerId,
		const FWidgetStyle& InWidgetStyle,
		bool bParentEnabled
		) const = 0;
	virtual void OnActiveTick(double InCurrentTime, float InDeltaTime) = 0;

	// Geometry excluding margins
	FGeometry MakeInsetGeometry(FGeometry const& WidgetGeometry) const;
	// Determines the geometries for all internal chart areas
	void MakeAreaGeometries(FGeometry const& ContentGeometry, FGeometry(&Geoms)[ChartContentAreaCount]) const;

	inline bool HasChartTitle() const
	{
		return !ChartTitle.IsEmptyOrWhitespace();
	}

	// Determine the actual size of the title area
	FVector2D DetermineTitleAreaSize(FGeometry const& InsetGeometry, bool bIncludePadding) const;

	// Calculate size of axis title area
	FVector2D DetermineAxisTitleSize(FCartesianAxisConfig const& AxisCfg, EAxis::Type AxisOrientation) const;

	// Determines the maximum extents that could be required by axis value text, based on the used font and
	// maximum digits setting
	FVector2D DetermineAxisValueLabelMaxExtents(EAxis::Type Axis, int32 MaxLabelDigits) const;

	// Calculates the width necessary to draw an axis and its markers and labels
	float DetermineAxisRequiredWidth(
		EAxis::Type Axis,
		int32 MaxLabelDigits,
		float AxisMarkerLength,
		float AxisMarkerLabelGap
		) const;

	static bool WillGeometryGetClipped(FGeometry const& Geom, FSlateRect const& ClipRect);

	// Geometry for title
	FGeometry MakeTitleGeometry(FGeometry const& InsetGeometry) const;

	// Geometry excluding title
	FGeometry MakeContentGeometry(FGeometry const& InsetGeometry) const;

	int32 DrawChartBackground(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyClippingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FKantanChartStyle* Style, const FWidgetStyle& InWidgetStyle) const;
	int32 DrawChartTitle(const FPaintArgs& Args, const FGeometry& TitleGeometry, const FSlateRect& MyClippingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FKantanChartStyle* Style, const FWidgetStyle& InWidgetStyle) const;
	int32 DrawXAxisTitle(const FGeometry& Geometry, const FSlateRect& ClipRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, FText const& Title, FText const& Unit, AxisUtil::FAxisMarkerData const& MarkerData) const;
	int32 DrawYAxisTitle(const FGeometry& Geometry, const FSlateRect& ClipRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, FText const& Title, FText const& Unit, AxisUtil::FAxisMarkerData const& MarkerData) const;
	int32 DrawFixedAxis(const FGeometry& Geometry, const FSlateRect& ClipRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, EAxis::Type Axis, AxisUtil::FAxisTransform const& ValueToChartAxisTransform, EChartAxisPosition AxisPosition, AxisUtil::FAxisMarkerData const& MarkerData, bool bDrawMarkers, bool bDrawLabels, float AxisMarkerLength, float AxisMarkerLabelGap) const;

private:
	/** Controls the speed at which the widget is ticked when in slate sleep mode */
	void SetActiveTimerTickRate(float TickRate);

	/** Widgets active tick */
	EActiveTimerReturnType ActiveTick(double InCurrentTime, float InDeltaTime);

protected:
	FMargin Margins;
	FText ChartTitle;
	FMargin TitlePadding;

	/** Reference to the widgets current active timer */
	TWeakPtr< FActiveTimerHandle > ActiveTimerHandle;

	/** Rate at which the widget gets updates from its datasource */
	float UpdateTickRate;
};

