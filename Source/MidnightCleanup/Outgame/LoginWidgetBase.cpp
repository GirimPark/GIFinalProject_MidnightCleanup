// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginWidgetBase.h"

#include "ActivePlayer.h"
#include "GRNetworkSubsystem.h"
#include "LogicHandler.h"
#include "MidnightCleanup/GI.h"
#include "MainWidgetBase.h"
#include "OutGamePC.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"

void ULoginWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	LoginButton = Cast<UButton>(GetWidgetFromName(TEXT("Button_Login")));
	SigninIDText = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("Text_SigninID")));
	SigninPWText = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("Text_SigninPW")));
	LoginFailAlertText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_LoginFailed")));

	JoinButton = Cast<UButton>(GetWidgetFromName(TEXT("Button_PopupSignup")));
	SignupIDText = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("Text_SignupID")));
	SignupPWText = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("Text_SignupPW")));
	SignupPWCheckText = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("Text_SignupPWCheck")));
	JoinIDResText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_JoinIDRes")));
	JoinPWResText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_JoinPWRes")));

	LoginButton->OnClicked.AddDynamic(this, &ULoginWidgetBase::OnLoginButtonClicked);
	JoinButton->OnClicked.AddDynamic(this, &ULoginWidgetBase::OnSignupButtonClicked);
	SigninIDText->OnTextCommitted.AddDynamic(this, &ULoginWidgetBase::OnTextCommitted);
	SigninPWText->OnTextCommitted.AddDynamic(this, &ULoginWidgetBase::OnTextCommitted);

	GetGameInstance()->GetSubsystem<UGRNetworkSubsystem>()->GetLogicHandler()->OnLoginRespond.AddDynamic(this, &ULoginWidgetBase::OnLoginRespond);
}

void ULoginWidgetBase::OnLoginRespond(int32 response)
{
	switch (response)
	{
	case ELoginResponse::LOGIN_SUCCESS:
		{
		SetVisibility(ESlateVisibility::Collapsed);

		AOutGamePC* OutGamePC = Cast<AOutGamePC>(GetOwningPlayer());
		OutGamePC->MainWidget->SetVisibility(ESlateVisibility::Visible);

		FString NickName = GetGameInstance()->GetSubsystem<UGRNetworkSubsystem>()->GetLogicHandler()->GetActivePlayerInfoRef()->Username;
		Cast<UGI>(GetGameInstance())->SetUserID(NickName);
		break;
		}
	case ELoginResponse::WRONG_ID:
		{
		LoginFailAlertText->SetVisibility(ESlateVisibility::Visible);
		LoginFailAlertText->SetText(FText::FromString(TEXT("존재하지 않는 계정입니다.")));
		break;
		}
	case ELoginResponse::WRONG_PW:
		{
		LoginFailAlertText->SetVisibility(ESlateVisibility::Visible);
		LoginFailAlertText->SetText(FText::FromString(TEXT("잘못된 비밀번호입니다.")));
		break;
		}
	case ELoginResponse::ALREADY_LOGIN:
		{
		LoginFailAlertText->SetVisibility(ESlateVisibility::Visible);
		LoginFailAlertText->SetText(FText::FromString(TEXT("접속 중인 계정입니다.")));
		break;
		}
	case ELoginResponse::SERVER_FAILED:
		{
		LoginFailAlertText->SetVisibility(ESlateVisibility::Visible);
		LoginFailAlertText->SetText(FText::FromString(TEXT("다시 시도해주세요.")));
		break;
		}
	}
}

void ULoginWidgetBase::OnLoginActionTriggered()
{
	if(GetVisibility() == ESlateVisibility::Visible)
	{
		LoginButton->OnClicked.Broadcast();
	}
}

void ULoginWidgetBase::OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if(CommitMethod == ETextCommit::Type::OnEnter)
	{
		LoginButton->OnClicked.Broadcast();
		LoginButton->OnPressed.Broadcast();
	}
}

void ULoginWidgetBase::OnLoginButtonClicked()
{
	LoginFailAlertText->SetVisibility(ESlateVisibility::Collapsed);

	if(SigninIDText->GetText().IsEmpty() || SigninPWText->GetText().IsEmpty())
	{
		LoginFailAlertText->SetVisibility(ESlateVisibility::Visible);
		LoginFailAlertText->SetText(FText::FromString(TEXT("아이디, 비밀번호를 모두 입력해주세요")));
		return;
	}

	GetGameInstance()->GetSubsystem<UGRNetworkSubsystem>()->ConnectToServer(IpAddress, 5001);
	GetGameInstance()->GetSubsystem<UGRNetworkSubsystem>()->GetLogicHandler()->RequestLogin(SigninIDText->GetText().ToString(), SigninPWText->GetText().ToString());
}

void ULoginWidgetBase::OnSignupButtonClicked()
{
	JoinIDResText->SetVisibility(ESlateVisibility::Collapsed);
	JoinPWResText->SetVisibility(ESlateVisibility::Collapsed);

	if(SignupIDText->GetText().IsEmpty() || SignupPWText->GetText().IsEmpty() || SignupPWCheckText->GetText().IsEmpty())
	{
		JoinPWResText->SetVisibility(ESlateVisibility::Visible);
		JoinPWResText->SetText(FText::FromString(TEXT("아이디, 비밀번호를 모두 입력하세요.")));
		return;
	}

	if(!SignupPWText->GetText().EqualTo(SignupPWCheckText->GetText()))
	{
		JoinPWResText->SetVisibility(ESlateVisibility::Visible);
		JoinPWResText->SetText(FText::FromString(TEXT("비밀번호가 일치하지 않습니다.")));
		return;
	}

	FRegexPattern Pattern(TEXT("^[A-Za-z0-9]+$"));
	FRegexMatcher Matcher(Pattern, SignupIDText->GetText().ToString());
	if(!Matcher.FindNext())
	{
		JoinIDResText->SetVisibility(ESlateVisibility::Visible);
		JoinIDResText->SetText(FText::FromString(TEXT("아이디는 대문자, 소문자 영문 및 숫자만 가능합니다.")));
		return;
	}

	GetGameInstance()->GetSubsystem<UGRNetworkSubsystem>()->ConnectToServer(IpAddress, 5001);
	GetGameInstance()->GetSubsystem<UGRNetworkSubsystem>()->GetLogicHandler()->RequestJoin(SignupIDText->GetText().ToString(), SignupPWText->GetText().ToString());
}

void ULoginWidgetBase::ResetLoginPopup()
{
	SigninIDText->SetText(FText::FromString(TEXT("")));
	SigninPWText->SetText(FText::FromString(TEXT("")));
	LoginFailAlertText->SetVisibility(ESlateVisibility::Collapsed);
}

void ULoginWidgetBase::ResetJoinPopup()
{
	SignupIDText->SetText(FText::FromString(TEXT("")));
	SignupPWText->SetText(FText::FromString(TEXT("")));
	SignupPWCheckText->SetText(FText::FromString(TEXT("")));
	JoinIDResText->SetVisibility(ESlateVisibility::Collapsed);
	JoinPWResText->SetVisibility(ESlateVisibility::Collapsed);
}