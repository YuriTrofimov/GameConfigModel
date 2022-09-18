// Copyright:       Copyright (C) 2022 Yuriy Trofimov
// Source Code:     https://github.com/YuriTrofimov/GameConfigModel

#include "LookupParameterWidget.h"
#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "Components/TextBlock.h"
#include "GameConfigModel/Parameters/ParameterLookup.h"

void ULookupParameterWidget::SetGameParameter(UGameParameter* InParameter)
{
	Super::SetGameParameter(InParameter);
	ViewModel = Cast<UParameterLookup>(InParameter);
	if (!ViewModel) return;

	ViewModel->OnSelectionChanged.AddDynamic(this, &ThisClass::OnSelectionChangedHandler);
	ViewModel->OnOptionsListChanged.AddDynamic(this, &ThisClass::OnOptionsListChangedHandler);
	if (ButtonNext)
	{
		ButtonNext->OnClicked.AddDynamic(this, &ThisClass::OnNextButtonClicked);
	}
	if (ButtonPrevious)
	{
		ButtonPrevious->OnClicked.AddDynamic(this, &ThisClass::OnPreviousButtonClicked);
	}
}

void ULookupParameterWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void ULookupParameterWidget::OnNextButtonClicked()
{
	if (!ViewModel) return;
	ViewModel->SelectNextOption();
}

void ULookupParameterWidget::OnPreviousButtonClicked()
{
	if (!ViewModel) return;
	ViewModel->SelectPreviousOption();
}

void ULookupParameterWidget::OnSelectionChangedHandler(FParameterLookupOption Option, int SelectedIndex, bool bCanBack, bool bCanForward)
{
	OnSelectionChanged(Option, SelectedIndex, bCanBack, bCanForward);
}

void ULookupParameterWidget::OnSelectionChanged_Implementation(FParameterLookupOption Option, int SelectedIndex, bool bCanBack, bool bCanForward)
{
	if (ValueCaption)
	{
		ValueCaption->SetText(Option.Caption);
	}
	if (ButtonNext)
	{
		ButtonNext->SetIsEnabled(bCanForward);
	}
	if (ButtonPrevious)
	{
		ButtonPrevious->SetIsEnabled(bCanBack);
	}
	if (OptionsCombo)
	{
		if (OptionsCombo->GetSelectedIndex() != SelectedIndex)
		{
			OptionsCombo->SetSelectedIndex(SelectedIndex);
		}
	}
}

void ULookupParameterWidget::OnOptionsListChangedHandler()
{
	OnOptionsListChanged();
}

void ULookupParameterWidget::OnOptionsListChanged_Implementation()
{
	if (!OptionsCombo || !ViewModel) return;
	OptionsCombo->ClearOptions();
	TArray<FText> Captions;
	ViewModel->GetOptionsCaptions(Captions);
	for (const auto& Caption : Captions)
	{
		OptionsCombo->AddOption(Caption.ToString());
	}
}
