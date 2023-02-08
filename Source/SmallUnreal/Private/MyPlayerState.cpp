// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerState.h"

float AMyPlayerState::ChangeCredit(float Delta)
{
	CreditAmount += Delta;

	return CreditAmount;
}

float AMyPlayerState::GetCreditAmount() const
{
	return CreditAmount;
}
