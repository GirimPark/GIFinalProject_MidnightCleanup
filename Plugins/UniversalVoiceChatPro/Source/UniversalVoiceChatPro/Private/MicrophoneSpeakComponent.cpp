// Universal Cross-Platform Voice Chat MeoPlay Copyright (C) 2021 MeoPlay <contact@meoplay.com> All Rights Reserved.


#include "MicrophoneSpeakComponent.h"
#include "AudioCaptureAndroid.h"
#include "PlayerVoiceChatActor.h"
#include "opus.h"
#include "Async/Async.h"
#include "Runtime/Launch/Resources/Version.h" 

#include "UniversalVoiceChatPro.h"

#define DEBUG_OPUS 0
#define ADD_ENTROPY_TO_PACKET 0
#define USE_UE4_MEM_ALLOC 1
#define MAX_OPUS_FRAMES_PER_PACKET 48
#define MAX_OPUS_FRAMES 6
#define MAX_OPUS_FRAME_SIZE MAX_OPUS_FRAMES * 320 
#define MAX_OPUS_UNCOMPRESSED_BUFFER_SIZE 48 * 1024
//  at 48 kHz the permitted values are 120 (2.5ms), 240 (5ms), 480 (10ms), 960 (20ms), 1920 (40ms), and 2880 (60ms)
//#define NUM_OPUS_FRAMES_PER_SEC 50
//#define NUM_OPUS_FRAMES_PER_SEC 200
#define OPUS_CHECK_CTL(Category, CTL) \
	if (ErrCode != OPUS_OK) \
	{ \
		UE_LOG(Category, Warning, TEXT("Failure to get CTL %s"), #CTL); \
	}

// Sets default values for this component's properties
UMicrophoneSpeakComponent::UMicrophoneSpeakComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bAutoActivate = true;
}

// Called when the game starts
void UMicrophoneSpeakComponent::BeginPlay()
{
	Super::BeginPlay();


	VoiceAudioComponent = NewObject<UAudioComponent>(((AActor*)GetOwner()), UAudioComponent::StaticClass());
	VoiceSoundWaveProcedural = NewObject<USoundWaveProcedural>();
	VoiceAudioComponent->RegisterComponent();
	VoiceAudioComponent->SetMobility(EComponentMobility::Movable);
	VoiceAudioComponent->AttachToComponent(((AActor*)GetOwner())->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	VoiceAudioComponent->SetRelativeLocation(FVector(0, 0, 0));

	VoiceAudioComponent->VolumeMultiplier = AudioComponentVolume;

	UE_LOG(LogVoiceChatPro, Display, TEXT("UMicrophoneSpeakComponent::BeginPlay done"));

}

void UMicrophoneSpeakComponent::OnUnregister() {
	UActorComponent::OnUnregister();

	if (isSpeakingLocalSetting) endSpeaking();

	
}

void UMicrophoneSpeakComponent::OnComponentDestroyed(bool bDestroyingHierarchy) {
	
}


bool UMicrophoneSpeakComponent::initAudioResources(int32 _voiceSampleRate, int32 _voiceNumChannels, int32 opusFramesPerSec) {

	if (wasInitAudioResources) {
		UE_LOG(LogVoiceChatPro, Display, TEXT("UMicrophoneSpeakComponent::initAudioResources wasInitAudioResources %d"), wasInitAudioResources);
#if PLATFORM_WINDOWS
		UE_LOG(LogVoiceChatPro, Display, TEXT("UMicrophoneSpeakComponent::initAudioResources recreate Windows capture"));
		PCVoiceCapture->Shutdown();
		PCVoiceCapture = FVoiceModule::Get().CreateVoiceCapture(APlayerVoiceChatActor::selectedAudioInputDevice, _voiceSampleRate, _voiceNumChannels);
#endif
		return false;
	}

	if (VoiceAudioComponent == NULL || VoiceSoundWaveProcedural == NULL || !VoiceAudioComponent->IsRegistered()) {
		UE_LOG(LogVoiceChatPro, Warning, TEXT("UMicrophoneSpeakComponent::initAudioResources can't init yet, objects not created in beginplay"));
		return false;
	}
	
	voiceNumChannels = _voiceNumChannels;
	voiceSampleRate = _voiceSampleRate;

	UE_LOG(LogVoiceChatPro, Display, TEXT("UMicrophoneSpeakComponent::initAudioResources _voiceSampleRate %d voiceSampleRate %d voiceNumChannels %d"), _voiceSampleRate, voiceSampleRate, voiceNumChannels);


	bool resencopusinit = OpusInit(voiceSampleRate, voiceNumChannels, opusFramesPerSec);
	bool resdecopusinit = OpusDecoderInit(voiceSampleRate, voiceNumChannels, opusFramesPerSec);
	UE_LOG(LogVoiceChatPro, Display, TEXT("initAudioResources opus resopusinit %d resdecopusinit %d"), resencopusinit, resdecopusinit);

	VoiceAudioComponent->bAutoActivate = true;
	VoiceAudioComponent->bAlwaysPlay = true;
	VoiceAudioComponent->PitchMultiplier = 1.0f;
	VoiceAudioComponent->bIsUISound = false;
	VoiceAudioComponent->AttenuationSettings = nullptr;
	VoiceAudioComponent->bOverrideAttenuation = false;
	VoiceAudioComponent->bAllowSpatialization = false;
	
#	if (ENGINE_MAJOR_VERSION == 4) && (ENGINE_MINOR_VERSION >=26)
	VoiceAudioComponent->bIsVirtualized = true;
	#endif
	
	VoiceSoundWaveProcedural->SetSampleRate(voiceSampleRate);
	VoiceSoundWaveProcedural->NumChannels = voiceNumChannels;
	VoiceSoundWaveProcedural->Duration = INDEFINITELY_LOOPING_DURATION;
	VoiceSoundWaveProcedural->SoundGroup = SOUNDGROUP_Voice;
	VoiceSoundWaveProcedural->bLooping = false;
	VoiceSoundWaveProcedural->bProcedural = true;
	VoiceSoundWaveProcedural->Pitch = 1.0f;
	VoiceSoundWaveProcedural->Volume = 1.0f;
	VoiceSoundWaveProcedural->AttenuationSettings = nullptr;
	VoiceSoundWaveProcedural->bDebug = true;
	VoiceSoundWaveProcedural->VirtualizationMode = EVirtualizationMode::PlayWhenSilent;

	/* set attenuation settings in case repnotify was called before beginplay */
	setAttenuationAssetPath(attenuationIsEnabled, pathToAttenuationAsset);
	setSourceChainEffectAssetPath(sourceChainEffectIsEnabled, pathToSourceChainEffect);

#if PLATFORM_WINDOWS
	FString commandMicNoiseGateThreshold = "voice.MicNoiseGateThreshold  0.01";
	FString commandSilenceDetectionThreshold = "voice.SilenceDetectionThreshold 0.01";

	APlayerController* PController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PController)
	{
		PController->ConsoleCommand(*commandMicNoiseGateThreshold, true);
		PController->ConsoleCommand(*commandSilenceDetectionThreshold, true);
	}

	
	PCVoiceCapture = FVoiceModule::Get().CreateVoiceCapture(APlayerVoiceChatActor::selectedAudioInputDevice, voiceSampleRate, voiceNumChannels);

	if (PCVoiceCapture.IsValid())
	{
		MaxRawCaptureDataSize = PCVoiceCapture->GetBufferSize();

		PCVoiceCaptureBuffer.Empty(MaxRawCaptureDataSize);
		PCVoiceCaptureBuffer.Reserve(MaxRawCaptureDataSize);

		PCVoiceCaptureBuffer.Empty(MaxRawCaptureDataSize);
		PCVoiceCaptureBufferAppended.Reserve(MaxRawCaptureDataSize);

		UE_LOG(LogVoiceChatPro, Display, TEXT("initAudioResources PCVoiceCapture.IsValid()  valid PCVoiceCapture->GetBufferSize() %d"), PCVoiceCapture->GetBufferSize());
	}
	else {
		UE_LOG(LogVoiceChatPro, Warning, TEXT("initAudioResources PCVoiceCapture.IsValid() not valid"));
	}

	UE_LOG(LogVoiceChatPro, Display, TEXT("initAudioResources opus resopusinit %d resdecopusinit %d"), resencopusinit, resdecopusinit);
#endif


	wasInitAudioResources = true;
	return true;
}


// Called every frame
void UMicrophoneSpeakComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);	
		
	//UE_LOG(LogVoiceChatPro, Warning, TEXT("VoiceAudioComponent->GetComponentLocation server ? %d %s %s "), GetWorld()->IsServer(), *(((AActor*)GetOwner())->GetActorLocation().ToString()), *(VoiceAudioComponent->GetComponentLocation().ToString()));

	if (VoiceAudioComponent == NULL || VoiceSoundWaveProcedural == NULL || !VoiceAudioComponent->IsRegistered()) return;
	
	// reset volume when silenced
	timerResetRuntimeVolume += DeltaTime;
	if (latestVolume > 0 && timerResetRuntimeVolume > delayResetRuntimeVolume) {
		latestVolume = 0.0f;
	}

	// only capture if this component is from local voice chat actor
	if (GetNetMode() != NM_DedicatedServer && UUniversalVoiceChat::GetMyPlayerVoiceActor() != NULL && UUniversalVoiceChat::GetMyPlayerVoiceActor() == (AActor*)GetOwner()){
	
#if PLATFORM_WINDOWS		
		if (PCVoiceCapture.IsValid() && PCVoiceCapture->IsCapturing()) {
			uint32 VoiceCaptureBytesAvailable = 0;
			EVoiceCaptureState::Type CaptureState = PCVoiceCapture->GetCaptureState(VoiceCaptureBytesAvailable);
			
			PCVoiceCaptureBuffer.Reset();

			if (CaptureState == EVoiceCaptureState::Ok && VoiceCaptureBytesAvailable > 0)
			{
				float VoiceCaptureTotalSquared = 0;

				PCVoiceCaptureBuffer.SetNumUninitialized(VoiceCaptureBytesAvailable);
				
				// get current voice data
				uint64 OutSampleCounter = 0;
				uint32 OutAvailableVoiceData = 0;
				PCVoiceCapture->GetVoiceData(PCVoiceCaptureBuffer.GetData(), VoiceCaptureBytesAvailable, OutAvailableVoiceData, OutSampleCounter);
				//UE_LOG(LogVoiceChatPro, Warning, TEXT("OutAvailableVoiceData %d OutSampleCounter %d"), OutAvailableVoiceData, OutSampleCounter);

				OnDataMicrophoneReceived.Broadcast(PCVoiceCaptureBuffer);

				PCVoiceCaptureBufferAppended.Append(PCVoiceCaptureBuffer);			
					
				int32 amountWillBeRemind = PCVoiceCaptureBufferAppended.Num() % encoderBytesPerFrame;
				int32 frameSentToOpus = PCVoiceCaptureBufferAppended.Num() / encoderBytesPerFrame;
				int32 startOffsetRemind = (PCVoiceCaptureBufferAppended.Num() / encoderBytesPerFrame) * encoderBytesPerFrame - 1;
					

				//UE_LOG(LogVoiceChatPro, Warning, TEXT("PCVoiceCaptureBuffer %d PCVoiceCaptureBufferAppended %d frameSentToOpus %d encoderBytesPerFrame %d"), 
				//	PCVoiceCaptureBuffer.Num(), PCVoiceCaptureBufferAppended.Num(), frameSentToOpus, encoderBytesPerFrame);

				// transmit local voice data to server	
				if (frameSentToOpus > 0) {
					int32 uint8sizeBufferPCMMic = frameSentToOpus * encoderBytesPerFrame;//PCVoiceCaptureBuffer.Num();
					uint8 *pointerBufferPCMMic = (uint8*)malloc(sizeof(uint8) * uint8sizeBufferPCMMic);

					//UE_LOG(LogVoiceChatPro, Warning, TEXT("prepare transmitDataToOtherAndLocal %d %d"), uint8sizeBufferPCMMic, PCVoiceCaptureBuffer.Num());

					memcpy(pointerBufferPCMMic, PCVoiceCaptureBufferAppended.GetData(), uint8sizeBufferPCMMic);					
					transmitDataToOtherAndLocal(pointerBufferPCMMic, uint8sizeBufferPCMMic);

					// remove transmited data
					PCVoiceCaptureBufferAppended.RemoveAt(0, uint8sizeBufferPCMMic, false);
				}
			}
		}
#endif
	}
}

bool UMicrophoneSpeakComponent::getWasInitAudioResources() {
	return wasInitAudioResources;
}

bool UMicrophoneSpeakComponent::isPlayingAudioVoice() {
	return VoiceAudioComponent !=NULL && VoiceAudioComponent->IsPlaying();
}

void UMicrophoneSpeakComponent::PlayAudioVoice() {
	
	if (VoiceAudioComponent == NULL || VoiceSoundWaveProcedural == NULL || !VoiceAudioComponent->IsRegistered() || VoiceAudioComponent->IsPlaying())
		return;

	UE_LOG(LogVoiceChatPro, Display, TEXT("PlayAudioVoice VoiceAudioComponent->Play()"));

	VoiceAudioComponent->SetSound(VoiceSoundWaveProcedural);
	VoiceAudioComponent->Play();
}


void UMicrophoneSpeakComponent::muteAudio(bool _isMuted) {
	isMutedLocalSetting = _isMuted;
	//UE_LOG(LogVoiceChatPro, Warning, TEXT("UMicrophoneSpeakComponent::muteAudio %d"), isMutedLocalSetting);
}

void UMicrophoneSpeakComponent::SetVoiceVolume(float volume) {
	UE_LOG(LogVoiceChatPro, Display, TEXT("UMicrophoneSpeakComponent::SetVoiceVolume volume %f"), volume);
	if (VoiceAudioComponent != NULL) {
		AudioComponentVolume = volume;
		VoiceAudioComponent->SetVolumeMultiplier(volume);
	}
}

void UMicrophoneSpeakComponent::setAttenuationAssetPath(bool enableAttenuation, FString _pathToAttenuationAsset) {

	UE_LOG(LogVoiceChatPro, Display, TEXT("UMicrophoneSpeakComponent::setAttenuationAssetPath %d %s override ? %d"), enableAttenuation, *_pathToAttenuationAsset, overrideLocallyAttenuation);

	if (!overrideLocallyAttenuation) {
		UE_LOG(LogVoiceChatPro, Display, TEXT("UMicrophoneSpeakComponent::setAttenuationAssetPath !overrideLocallyAttenuation set attenuationToLoad to %s"), *_pathToAttenuationAsset);
		pathToAttenuationAsset = _pathToAttenuationAsset;
	}
	else {
		UE_LOG(LogVoiceChatPro, Display, TEXT("UMicrophoneSpeakComponent::setAttenuationAssetPath overrideLocallyAttenuation "));
	}
	attenuationIsEnabled = enableAttenuation;

	FString attenuationToLoad = overrideLocallyAttenuation ? overrideLocallyAttenuationPath : pathToAttenuationAsset;

	UE_LOG(LogVoiceChatPro, Display, TEXT("UMicrophoneSpeakComponent::setAttenuationAssetPath attenuationToLoad %s"), *attenuationToLoad);

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("UMicrophoneSpeakComponent::setAttenuationAssetPath %d %s"), (int)enableAttenuation, *_pathToAttenuationAsset));

	USoundAttenuation *soundAttenuationAsset = NULL;
	
	if (!attenuationToLoad.IsEmpty()) {
		soundAttenuationAsset = LoadObject<USoundAttenuation>(NULL, *attenuationToLoad, NULL, LOAD_None, NULL);
		if (soundAttenuationAsset == NULL) {
			UE_LOG(LogVoiceChatPro, Warning, TEXT("UMicrophoneSpeakComponent::setAttenuationAssetPath error null %s"), *attenuationToLoad);
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("UMicrophoneSpeakComponent::setAttenuationAssetPath error null %s"), *GetName()));
		}
		else {
			UE_LOG(LogVoiceChatPro, Display, TEXT("UMicrophoneSpeakComponent::setAttenuationAssetPath loaded %s"), *attenuationToLoad);
		}
	}
	else {
		UE_LOG(LogVoiceChatPro, Warning, TEXT("UMicrophoneSpeakComponent::setAttenuationAssetPath attenuationToLoad.IsEmpty %s"), *attenuationToLoad);
	}

	if (VoiceAudioComponent != NULL && VoiceSoundWaveProcedural != NULL) {
		if (attenuationIsEnabled && soundAttenuationAsset) {
			if (!attenuationToLoad.IsEmpty()) {
				//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("UMicrophoneSpeakComponent::setAttenuationAssetPath enable attenuation %s"), *pathToAttenuationAsset));
				UE_LOG(LogVoiceChatPro, Display, TEXT("UMicrophoneSpeakComponent::setAttenuationAssetPath enable attenuation %s"), *attenuationToLoad);
			}
			VoiceAudioComponent->bOverrideAttenuation = false;
			VoiceAudioComponent->AttenuationSettings = soundAttenuationAsset;			
			VoiceAudioComponent->bAllowSpatialization = true;			
		}
		else {
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("UMicrophoneSpeakComponent::setAttenuationAssetPath disable attenuation")));

			UE_LOG(LogVoiceChatPro, Display, TEXT("UMicrophoneSpeakComponent::setAttenuationAssetPath disable attenuation attenuationIsEnabled %d soundAttenuationAsset %d "),
				attenuationIsEnabled, soundAttenuationAsset != nullptr);
			VoiceAudioComponent->bOverrideAttenuation = false;
			VoiceAudioComponent->AttenuationSettings = nullptr;
			VoiceAudioComponent->bAllowSpatialization = false;
		}
	}
	else{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("UMicrophoneSpeakComponent::setAttenuationAssetPath disable attenuation")));
		UE_LOG(LogVoiceChatPro, Display, TEXT("UMicrophoneSpeakComponent::setAttenuationAssetPath disable attenuation VoiceAudioComponent != NULL %d VoiceSoundWaveProcedural != NULL %d"),
			VoiceAudioComponent != nullptr, VoiceSoundWaveProcedural != nullptr);
	}

	if (VoiceAudioComponent != NULL) {
		VoiceAudioComponent->Stop();
		VoiceAudioComponent->Activate(true);
	}
}


void UMicrophoneSpeakComponent::setSourceChainEffectAssetPath(bool enableSourceChainEffect, FString _pathToSourceChainEffect) {
	UE_LOG(LogVoiceChatPro, Display, TEXT("APlayerVoiceChatActor::setSourceChainEffectAssetPath %d %s override ? %d"), enableSourceChainEffect, *_pathToSourceChainEffect, overrideLocallySourceEffect);

	if (!overrideLocallySourceEffect) {
		pathToSourceChainEffect = _pathToSourceChainEffect;
	}	
	sourceChainEffectIsEnabled = enableSourceChainEffect;

	FString sourceChainEffectToLoad = overrideLocallySourceEffect ? overrideLocallySourceEffectPath : pathToSourceChainEffect;

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("UMicrophoneSpeakComponent::setAttenuationAssetPath %d %s"), (int)enableAttenuation, *_pathToAttenuationAsset));

	USoundEffectSourcePresetChain* sourceChainEffectAsset = NULL;

	if (!sourceChainEffectToLoad.IsEmpty()) {
		sourceChainEffectAsset = LoadObject<USoundEffectSourcePresetChain>(NULL, *sourceChainEffectToLoad, NULL, LOAD_None, NULL);
		if (sourceChainEffectAsset == NULL) {
			UE_LOG(LogVoiceChatPro, Warning, TEXT("UMicrophoneSpeakComponent::setSourceChainEffectAssetPath error null %s"), *sourceChainEffectToLoad);
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("UMicrophoneSpeakComponent::setAttenuationAssetPath error null %s"), *GetName()));
		}
	}

	if (VoiceAudioComponent != NULL && VoiceSoundWaveProcedural != NULL) {
		if (sourceChainEffectIsEnabled && sourceChainEffectAsset) {
			if (!sourceChainEffectToLoad.IsEmpty()) {
				//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("UMicrophoneSpeakComponent::setAttenuationAssetPath enable attenuation %s"), *pathToAttenuationAsset));
				UE_LOG(LogVoiceChatPro, Display, TEXT("UMicrophoneSpeakComponent::setSourceChainEffectAssetPath enable  %s"), *sourceChainEffectToLoad);
			}
			VoiceAudioComponent->SetSourceEffectChain(sourceChainEffectAsset);
		}
		else {
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("UMicrophoneSpeakComponent::setAttenuationAssetPath disable attenuation")));
			UE_LOG(LogVoiceChatPro, Display, TEXT("UMicrophoneSpeakComponent::setSourceChainEffectAssetPath disable "));
			VoiceAudioComponent->SetSourceEffectChain(nullptr);
		}
	}
	else {
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("UMicrophoneSpeakComponent::setAttenuationAssetPath disable attenuation")));
		UE_LOG(LogVoiceChatPro, Display, TEXT("UMicrophoneSpeakComponent::setSourceChainEffectAssetPath disable attenuation"));
	}

	if (VoiceAudioComponent != NULL) {
		VoiceAudioComponent->Stop();
		VoiceAudioComponent->Activate(true);
	}
}

void UMicrophoneSpeakComponent::setOverrideLocallyAttenuationPath(bool enableAttenuation, bool overrideLocally, FString _pathToAttenuationAsset) {
	attenuationIsEnabled = enableAttenuation;
	overrideLocallyAttenuation = overrideLocally;
	overrideLocallyAttenuationPath = _pathToAttenuationAsset;
	// refresh attenuation
	setAttenuationAssetPath(attenuationIsEnabled, pathToAttenuationAsset);
}

void UMicrophoneSpeakComponent::setOverrideLocallySourceEffectPath(bool enableSourceEffect, bool overrideLocally, FString _pathToSourceEffectAsset) {
	sourceChainEffectIsEnabled = enableSourceEffect;
	overrideLocallySourceEffect = overrideLocally;
	overrideLocallySourceEffectPath = _pathToSourceEffectAsset;
	// refresh source effect
	setSourceChainEffectAssetPath(sourceChainEffectIsEnabled, pathToSourceChainEffect);
}

void UMicrophoneSpeakComponent::setLocallyMultiplierVolume(float multiplierVolume) {
	UE_LOG(LogVoiceChatPro, Display, TEXT("UMicrophoneSpeakComponent::setLocallyMultiplierVolume locallyMultiplierVolume base volume %f multiplier volume %f"),
		AudioComponentVolume, multiplierVolume);

	locallyMultiplierVolume = multiplierVolume;
	SetVoiceVolume(AudioComponentVolume);
}

float UMicrophoneSpeakComponent::getLocallyMultiplierVolume() {
	return locallyMultiplierVolume;
}

bool UMicrophoneSpeakComponent::startSpeaking(bool _shouldHearMyOwnVoice, bool _isGlobal, TArray<int32> _radioChannel, bool _useRange, float _maxRange) {
	shouldHearMyOwnVoiceLocalSetting = _shouldHearMyOwnVoice;
	isGlobalLocalSetting = _isGlobal;
	radioChannelLocalSetting.Empty();
	for (int i = 0; i < _radioChannel.Num(); i++) {
		radioChannelLocalSetting.Add(_radioChannel[i]);
	}	
	useRangeLocalSettings = _useRange;
	maxRangeLocalSettings = _maxRange;

	UE_LOG(LogVoiceChatPro, Display, TEXT("UMicrophoneSpeakComponent::startSpeaking"));

	if (VoiceAudioComponent == NULL || VoiceSoundWaveProcedural == NULL || !VoiceAudioComponent->IsRegistered()) {
		UE_LOG(LogVoiceChatPro, Warning, TEXT("UMicrophoneSpeakComponent::startSpeaking null objects"));
		return false;
	}
	if (isSpeakingLocalSetting) {
		UE_LOG(LogVoiceChatPro, Warning, TEXT("UMicrophoneSpeakComponent::startSpeaking already speaking"));
		return true;
	}
	if (!wasInitAudioResources) {
		bool resInit = initAudioResources(48000, 1, DEFAULT_NUM_OPUS_FRAMES_PER_SEC);
		if (!resInit) {
			UE_LOG(LogVoiceChatPro, Warning, TEXT("UMicrophoneSpeakComponent::startSpeaking wasn't init default"));
			return false;
		}
		UE_LOG(LogVoiceChatPro, Warning, TEXT("UMicrophoneSpeakComponent::startSpeaking inited default"));
	}
	PlayAudioVoice();

#if PLATFORM_ANDROID
	UE_LOG(LogVoiceChatPro, Display, TEXT("UMicrophoneSpeakComponent::startSpeaking PLATFORM_ANDROID MicrophoneStart "));
	bool res = UAudioCaptureAndroid::AndroidMicrophoneStart(this, voiceSampleRate);
	isSpeakingLocalSetting = res;
	return res;
#endif
#if PLATFORM_WINDOWS	
	UE_LOG(LogVoiceChatPro, Display, TEXT("UMicrophoneSpeakComponent::startSpeaking PLATFORM_WINDOWS Start "));
	if (PCVoiceCapture != NULL && PCVoiceCapture.IsValid()) {
		PCVoiceCapture->Start();
	}	
	isSpeakingLocalSetting = true;
	return true;
#endif
#if PLATFORM_IOS
	UE_LOG(LogVoiceChatPro, Display, TEXT("UMicrophoneSpeakComponent::startSpeaking PLATFORM_IOS requestRecordPermission "));

	bool couldOpen = false;
	bool resOpen = false;
	switch ([[AVAudioSession sharedInstance] recordPermission]) {
		case AVAudioSessionRecordPermissionGranted:
			// init comp ios voice
			iosCapture.callbackSpeakMicrophone = this;
			resOpen = iosCapture.iosOpenCaptureStream(voiceSampleRate, voiceNumChannels, encoderBytesPerFrame);
			UE_LOG(LogVoiceChatPro, Display, TEXT("iosCapture.OpenCaptureStream %d "), resOpen);
			resOpen = iosCapture.StartStream();
			UE_LOG(LogVoiceChatPro, Display, TEXT("iosCapture.StartStream %d "), resOpen);
			isSpeakingLocalSetting = true;
			couldOpen = true;
			break;
		case AVAudioSessionRecordPermissionDenied:
			couldOpen = false;
			break;
		case AVAudioSessionRecordPermissionUndetermined:
			couldOpen = false;
			break;
		default:
			couldOpen = false;
			break;
	}

	return couldOpen;
#endif
	
	return true;
}

void UMicrophoneSpeakComponent::endSpeaking() {
	if (!isSpeakingLocalSetting) return;
	UE_LOG(LogVoiceChatPro, Warning, TEXT("UMicrophoneSpeakComponent::stopSpeaking"));

#if PLATFORM_ANDROID
	UAudioCaptureAndroid::AndroidMicrophoneStop(this);
#endif
#if PLATFORM_IOS
	bool res = iosCapture.StopStream();
	UE_LOG(LogVoiceChatPro, Display, TEXT("iosCapture.StopStream %d "), res);
#endif
#if PLATFORM_WINDOWS	
	if (PCVoiceCapture != NULL && PCVoiceCapture.IsValid()) {
		PCVoiceCapture->Stop();
	}	
#endif	
	isSpeakingLocalSetting = false;
}
// data = opus encoded, samplerate = audiorecord sample rate, pcmsize = raw short array size from audio record
void UMicrophoneSpeakComponent::RPCClientTransmitVoiceData_Implementation(TArray<uint8> const &data, int32 sampleRate, int32 numchannels, int32 PCMSize, int32 opusFramePerSec, bool _isGlobal, TArray<int32> const& _radioChannel, bool _useRange, float _maxRange) {
	
	if (GetWorld() == NULL || GetOwner() == NULL) {
		UE_LOG(LogVoiceChatPro, Warning, TEXT("server cannot send RPC because getworld is NULL"));
		return;
	}


	if (((APlayerVoiceChatActor*)GetOwner())->ServerPerformAntiCheat) {
		if (_isGlobal) {
			if (((APlayerVoiceChatActor*)GetOwner())->AntiCheatAllowUseGlobal == false) {
				UE_LOG(LogVoiceChatPro, Warning, TEXT("SERVER : This client was not allowed to use global, make sure server allowed it by using ServerSetAllowUseProximity"));
				return;
			}
		}

		if (_useRange) {
			if (((APlayerVoiceChatActor*)GetOwner())->AntiCheatAllowUseProximity == false) {
				UE_LOG(LogVoiceChatPro, Warning, TEXT("SERVER : This client was not allowed to use range, make sure server allowed it by using ServerSetAllowUseProximity"));
				return;
			}
			else {
				if (UKismetMathLibrary::Abs(_maxRange - ((APlayerVoiceChatActor*)GetOwner())->AntiCheatMaxProximityRange) > 10) {
					UE_LOG(LogVoiceChatPro, Warning, TEXT("SERVER : This client didn't use same range as server,  make sure server and client use same range  (client start speaking and server ServerSetMaxProximityRange)"));
					return;
				}
			}
		}
	}
	
	


	//UE_LOG(LogVoiceChatPro, Warning, TEXT(" RPCClientTransmitVoiceData_Implementation %d %d isglobal %d _radioChannel %d _useRange %d _maxRange %f"), sampleRate, numchannels, _isGlobal, _radioChannel, _useRange, _maxRange);
	// if we are not using range, then it s a voice chat like counter strike
	if (!_useRange) {
		// this is global chat for everyone, disregarding teams/radio channels
		if (_isGlobal) {
			RPCServerBroadcastVoiceData(data, sampleRate, numchannels, PCMSize, opusFramePerSec);
		}
		else { // this is team/radio channel
			/* find out to whom to send the voice datas */
			TArray<AActor*> FoundActors;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerVoiceChatActor::StaticClass(), FoundActors);
			//UE_LOG(LogVoiceChatPro, Warning, TEXT(" RPCClientTransmitVoiceData_Implementation total actor found %d"), FoundActors.Num());
			for (int i = 0; i < FoundActors.Num(); i++) {
				for (int j = 0; j < _radioChannel.Num(); j++) {
					if (((APlayerVoiceChatActor*)FoundActors[i])->radioChannelSubscribed.Contains(_radioChannel[j])) {
						((APlayerVoiceChatActor*)FoundActors[i])->microphoneSpeakComponent->RPCReceiveVoiceFromServer(this, data, sampleRate, numchannels, PCMSize, opusFramePerSec);
					}
				}
			}
		}
	}
	else {
		
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerVoiceChatActor::StaticClass(), FoundActors);
		//UE_LOG(LogVoiceChatPro, Warning, TEXT(" RPCClientTransmitVoiceData_Implementation total actor found %d"), FoundActors.Num());
		for (int i = 0; i < FoundActors.Num(); i++) {		
			float dist = FVector::Dist(GetOwner()->GetActorLocation(), FoundActors[i]->GetActorLocation());
			if (dist < _maxRange) {
				if (_isGlobal) {
					((APlayerVoiceChatActor*)FoundActors[i])->microphoneSpeakComponent->RPCReceiveVoiceFromServer(this, data, sampleRate, numchannels, PCMSize, opusFramePerSec);
				}
				else {
					for (int j = 0; j < _radioChannel.Num(); j++) {
						if (((APlayerVoiceChatActor*)FoundActors[i])->radioChannelSubscribed.Contains(_radioChannel[j])) {
							((APlayerVoiceChatActor*)FoundActors[i])->microphoneSpeakComponent->RPCReceiveVoiceFromServer(this, data, sampleRate, numchannels, PCMSize, opusFramePerSec);
						}
					}
				}
			}

			//UE_LOG(LogVoiceChatPro, Warning, TEXT(" RPCClientTransmitVoiceData_Implementation dist %f"), dist);
		}
	}
	
}

// RPC sent to one client, given radio / teams
void UMicrophoneSpeakComponent::RPCReceiveVoiceFromServer_Implementation(UMicrophoneSpeakComponent *compToOutputVoice, TArray<uint8> const &dataEncoded, int32 sampleRate, int32 numchannels, int32 PCMSize, int32 opusFramePerSec) {
	
	//UE_LOG(LogVoiceChatPro, Warning, TEXT("RPCReceiveVoiceFromServer_Implementation sampleRate %d pcmsize %d encoded : %d"), sampleRate, PCMSize, dataEncoded.Num());
	if (compToOutputVoice != NULL) {
		//UE_LOG(LogVoiceChatPro, Warning, TEXT("RPCReceiveVoiceFromServer_Implementation sampleRate %d pcmsize %d encoded : %d"), sampleRate, PCMSize, dataEncoded.Num());
		if (UUniversalVoiceChat::GetMyPlayerVoiceActor() != NULL && UUniversalVoiceChat::GetMyPlayerVoiceActor()->microphoneSpeakComponent != compToOutputVoice) {
			compToOutputVoice->payloadReceivedVoiceData(dataEncoded, sampleRate, numchannels, PCMSize, opusFramePerSec);
		}
	}
	else {
		UE_LOG(LogVoiceChatPro, Warning, TEXT("RPCReceiveVoiceFromServer_Implementation compToOutputVoice not ok"));
	}
}

// RPC broadcast sent to clients, disregarding radio / teams ( global chat )
void UMicrophoneSpeakComponent::RPCServerBroadcastVoiceData_Implementation(TArray<uint8> const &dataEncoded, int32 sampleRate, int32 numchannels, int32 PCMSize, int32 opusFramePerSec){

	//if ( (GetWorld()->IsServer() == 1 && GetNetMode() != NM_DedicatedServer) || GetWorld()->IsServer() == 0)
	if ((GetNetMode() == NM_ListenServer) || (GetNetMode() == NM_Client))
	{
		//UE_LOG(LogVoiceChatPro, Warning, TEXT("RPCServerBroadcastVoiceData_Implementation"));
		if (UUniversalVoiceChat::GetMyPlayerVoiceActor() == NULL) return;
		if (VoiceAudioComponent == NULL || VoiceSoundWaveProcedural == NULL || !VoiceAudioComponent->IsRegistered()) return;
				
		
		if (!IsRunningDedicatedServer() && UUniversalVoiceChat::GetMyPlayerVoiceActor() != (AActor*)GetOwner()){
			/*UE_LOG(LogVoiceChatPro, Warning, TEXT("RPCServerBroadcastVoiceData_Implementation isserver %d %d %d GetOwner() %s UUniversalVoiceChat::GetMyPlayerVoiceActor() %s"), 
				GetWorld()->IsServer(), GetNetMode() != NM_DedicatedServer, GetNetMode(), *(((AActor*)GetOwner())->GetName()), *(UUniversalVoiceChat::GetMyPlayerVoiceActor()->GetName()) );
			UE_LOG(LogVoiceChatPro, Warning, TEXT("RPCServerBroadcastVoiceData_Implementation sampleRate %d pcmsize %d encoded : %d "), sampleRate, PCMSize, dataEncoded.Num());*/
			payloadReceivedVoiceData(dataEncoded, sampleRate, numchannels, PCMSize, opusFramePerSec);
		}
	}
}

// client play sound data received
void UMicrophoneSpeakComponent::payloadReceivedVoiceData(TArray<uint8> const &dataEncoded, int32 sampleRate, int32 numchannels, int32 PCMSize, int32 opusFramePerSec) {
	//UE_LOG(LogVoiceChatPro, Warning, TEXT("RPCServerBroadcastVoiceData_Implementation GetWorld()->IsServer() %d ROLE_Authority %d IsRunningDedicatedServer() %d"), GetWorld()->IsServer(), GetOwnerRole() == ROLE_Authority, IsRunningDedicatedServer());

		// check if was initialised, then play audio
	if (!wasInitAudioResources) {
		bool resInit = initAudioResources(sampleRate, numchannels, opusFramePerSec);
		if (!resInit) {
			UE_LOG(LogVoiceChatPro, Warning, TEXT("UMicrophoneSpeakComponent::payloadReceivedVoiceData wasn't init default"));
			return;
		}
	}
	PlayAudioVoice();

	// reserver opus decode buffer	
	bufferRPCdecodedData.Reset();
	
	// give a big enough data buffer for decoding
	if (bufferRPCdecodedData.GetAllocatedSize() < PCMSize * 20) {
		//UE_LOG(LogVoiceChatPro, Warning, TEXT("payloadReceivedVoiceData bufferRPCdecodedData  increase buffer size"));
		bufferRPCdecodedData.Reserve(PCMSize * 20);
	}

	// decode the opus data received
	uint32 outsize = PCMSize * 20;
	//UE_LOG(LogVoiceChatPro, Warning, TEXT("payloadReceivedVoiceData  before OpusDecode sampleRate %d pcmsize %d encoded : %d decoded %d"), sampleRate, PCMSize, dataEncoded.Num(), outsize);

	OpusDecode(dataEncoded.GetData(), dataEncoded.Num(), bufferRPCdecodedData.GetData(), outsize);
	
	//UE_LOG(LogVoiceChatPro, Warning, TEXT("payloadReceivedVoiceData  after OpusDecode sampleRate %d pcmsize %d encoded : %d decoded %d"), sampleRate, PCMSize, dataEncoded.Num(), outsize);
	

	//////////////////////////////////////////////////////
	// audio engineering, increase volume from microphone PCM data
	// just in case for volume gain, make sur it s modulo 0

	int32 sizeBufferVolumeGain = outsize;
	if (bufferRPCdecodedData.Num() % 2 == 1) {
		sizeBufferVolumeGain -= 1;
	}

	//UE_LOG(LogVoiceChatPro, Warning, TEXT("UMicrophoneSpeakComponent::payloadReceivedVoiceData bufferRPCdecodedData %d sizeBufferVolumeGain %d"),
	//	bufferRPCdecodedData.Num(), sizeBufferVolumeGain);

	float volumeGlobalChat = APlayerVoiceChatActor::voiceChatGlobalVolume;
	int16* pcm16DataVolume = (int16*)(bufferRPCdecodedData.GetData());
	
	if (volumeGlobalChat < 0.99f || volumeGlobalChat > 1.01f 
		|| locallyMultiplierVolume < 0.99f || locallyMultiplierVolume > 1.01f) {

		for (int i = 0; i < sizeBufferVolumeGain / 2; i++) {

			int32 valGain = (int32)(pcm16DataVolume[i] * volumeGlobalChat * locallyMultiplierVolume);
			bool thresholdReached = false;
			if (valGain < -32768) {
				valGain = -32767;
				thresholdReached = true;
			}
			if (valGain > 32767) {
				valGain = 32766;
				thresholdReached = true;
			}

			pcm16DataVolume[i] = (int16)valGain;

		}
		//UE_LOG(LogVoiceChatPro, Warning, TEXT("payloadReceivedVoiceData sizeBufferVolumeGain %d volumeGlobalChat %f locallyMultiplierVolume %f"), sizeBufferVolumeGain, volumeGlobalChat, locallyMultiplierVolume);
	}

	int32 sumPCMVolume = 0;
	for (int i = 0; i < sizeBufferVolumeGain / 2; i++) {
		sumPCMVolume += FMath::Abs(pcm16DataVolume[i]);
	}

	sumPCMVolume = (int32)(((float)sumPCMVolume) / (sizeBufferVolumeGain / 2));
	latestVolume = sumPCMVolume;
	timerResetRuntimeVolume = 0.0f;
	OnMicrophoneVolumeReceived.Broadcast(sumPCMVolume);
	///////////////////////
	
	// only play audio if not muted
	if (!APlayerVoiceChatActor::muteAll && !isMutedLocalSetting && VoiceSoundWaveProcedural != NULL) {
		VoiceSoundWaveProcedural->QueueAudio(bufferRPCdecodedData.GetData(), outsize);
	}
}


void UMicrophoneSpeakComponent::receiveDataMicrophone(TArray<int16> data, int32 readsize) {

	if (data.Num() == 0) return;
	
	//UE_LOG(LogVoiceChatPro, Warning, TEXT("micro c++ receiveDataMicrophone %d"), readsize);

	TArray<int16> VoiceCaptureBuffer;


	VoiceCaptureBuffer.Reset();
	VoiceCaptureBuffer.SetNumUninitialized(readsize);
	for (int32 i = 0; i < readsize; i++) {
		VoiceCaptureBuffer[i] = data[i];
	}
	
	

	//UE_LOG(LogVoiceChatPro, Warning, TEXT("micro c++ receiveDataMicrophone  QueueAudio r2 %d %d"), VoiceCaptureBuffer.Num(), data[0]);

	/* translating int16 buffer to uint8*/
	int32 uint8RawDataSize = VoiceCaptureBuffer.Num() * 2;
	uint8 *pointerBufferRawData = (uint8*)malloc(sizeof(uint8) * uint8RawDataSize);
	memcpy(pointerBufferRawData, VoiceCaptureBuffer.GetData(), uint8RawDataSize);

	/* delegate mic raw input*/
	TArray<uint8> delegateData;
	delegateData.Append(pointerBufferRawData, uint8RawDataSize);
	OnDataMicrophoneReceived.Broadcast(delegateData);

	// append data for opus encoder
	MobileVoiceCaptureBufferAppended.Append(pointerBufferRawData, uint8RawDataSize);

	int32 amountWillBeRemind = MobileVoiceCaptureBufferAppended.Num() % encoderBytesPerFrame;
	int32 frameSentToOpus = MobileVoiceCaptureBufferAppended.Num() / encoderBytesPerFrame;
	int32 startOffsetRemind = (MobileVoiceCaptureBufferAppended.Num() / encoderBytesPerFrame) * encoderBytesPerFrame - 1;

	if (frameSentToOpus > 0) {
		int32 uint8sizeBufferPCMMic = frameSentToOpus * encoderBytesPerFrame;//PCVoiceCaptureBuffer.Num();
		uint8 *pointerBufferPCMMic = (uint8*)malloc(sizeof(uint8) * uint8sizeBufferPCMMic);

		//UE_LOG(LogVoiceChatPro, Warning, TEXT("prepare transmitDataToOtherAndLocal %d %d"), uint8sizeBufferPCMMic, MobileVoiceCaptureBufferAppended.Num());

		memcpy(pointerBufferPCMMic, MobileVoiceCaptureBufferAppended.GetData(), uint8sizeBufferPCMMic);
		transmitDataToOtherAndLocal(pointerBufferPCMMic, uint8sizeBufferPCMMic);

		// remove transmited data
		MobileVoiceCaptureBufferAppended.RemoveAt(0, uint8sizeBufferPCMMic, false);
	}

	free(pointerBufferRawData);
}

/*** uint8 *pointerBufferPCMMic is malloced ****/
void UMicrophoneSpeakComponent::transmitDataToOtherAndLocal(uint8 *pointerBufferPCMMic, int32 uint8sizeBufferPCMMic) {

	uint8* encodedData = (uint8*)malloc(sizeof(uint8)* uint8sizeBufferPCMMic);
	uint32 encodedDataSize = uint8sizeBufferPCMMic;
	uint8* rawData = (uint8*)malloc(sizeof(uint8)* uint8sizeBufferPCMMic);

	memcpy(rawData, pointerBufferPCMMic, uint8sizeBufferPCMMic);

	//////////////////////////////////////////////////////
	// audio engineering, increase volume from microphone PCM data
	// just in case for volume gain, make sur it s modulo 0
	int32 sizeBufferVolumeGain = uint8sizeBufferPCMMic;
	if (uint8sizeBufferPCMMic % 2 == 1) {
		sizeBufferVolumeGain -= 1;
	}

	float volumeGain = APlayerVoiceChatActor::rawMicrophoneGain;
	int16* pcm16DataVolume = (int16*)(pointerBufferPCMMic);
	if (volumeGain < 0.99f || volumeGain > 1.01f) {
		for (int i = 0; i < sizeBufferVolumeGain / 2; i++) {

			int32 valGain = (int32)(pcm16DataVolume[i] * volumeGain);
			bool thresholdReached = false;
			if (valGain < -32768) {
				valGain = -32767;
				thresholdReached = true;
			}
			if (valGain > 32767) {
				valGain = 32766;
				thresholdReached = true;
			}

			pcm16DataVolume[i] = (int16)valGain;

			//UE_LOG(LogVoiceChatPro, Warning, TEXT("pcm16DataTestHear[i] %d valGain %d"), pcm16DataVolume[i], valGain);
		}
	}

	int32 sumPCMVolume = 0;
	for (int i = 0; i < sizeBufferVolumeGain / 2; i++) {
		sumPCMVolume += FMath::Abs(pcm16DataVolume[i]);
	}

	sumPCMVolume = (int32)(((float)sumPCMVolume) / (sizeBufferVolumeGain / 2));

	OnMicrophoneVolumeReceived.Broadcast(sumPCMVolume);
	latestVolume = sumPCMVolume;
	timerResetRuntimeVolume = 0.0f;
	//UE_LOG(LogVoiceChatPro, Warning, TEXT("sumPCMVolume mean %d"), sumPCMVolume);

	//////////////////////////////////////////////////////
	

	int32 dataRemainder = OpusEncode(rawData, uint8sizeBufferPCMMic, encodedData, encodedDataSize);
	//UE_LOG(LogVoiceChatPro, Warning, TEXT("receiveDataMicrophone end opus encode res dataRemainder : %d uncompressd %d compressed %d"), dataRemainder, uint8sizeBufferPCMMic, encodedDataSize);


	bufferEncodedOpus.Reset();
	for (uint32 i = 0; i < encodedDataSize; i++) {
		bufferEncodedOpus.Add(encodedData[i]);
	}

	free(rawData);
	free(encodedData);

	//UE_LOG(LogVoiceChatPro, Warning, TEXT("micro c++ receiveDataMicrophone compressedData before %d compressed %d decompressed %d "), VoiceCaptureBuffer.Num(), compressedData.Num(), deCompressedData.Num());

	if (encodedDataSize > 0 && sumPCMVolume > APlayerVoiceChatActor::thresholdSendData) {
		AsyncTask(ENamedThreads::GameThread, [this, pointerBufferPCMMic, uint8sizeBufferPCMMic]() {
			if (this != NULL && this->IsValidLowLevel() && IsValid(this)) {
				
				if (!APlayerVoiceChatActor::muteAll && shouldHearMyOwnVoiceLocalSetting && VoiceSoundWaveProcedural != NULL) {
					VoiceSoundWaveProcedural->QueueAudio(pointerBufferPCMMic, uint8sizeBufferPCMMic);
				}
				RPCClientTransmitVoiceData(bufferEncodedOpus, voiceSampleRate, voiceNumChannels, uint8sizeBufferPCMMic, NUM_OPUS_FRAMES_PER_SEC, isGlobalLocalSetting, radioChannelLocalSetting, useRangeLocalSettings, maxRangeLocalSettings);
			}
			free(pointerBufferPCMMic);
		});
	}
	else {
		if (sumPCMVolume <= APlayerVoiceChatActor::thresholdSendData) {
			if(uint8sizeBufferPCMMic) free(pointerBufferPCMMic);
		}
		else {
			UE_LOG(LogVoiceChatPro, Warning, TEXT("receiveDataMicrophone encoded data was 0, skip"));
		}		
	}
}

bool UMicrophoneSpeakComponent::OpusInit(int32 InSampleRate, int32 InNumChannels, int32 opusFramePerSec)
{
	UE_LOG(LogVoiceChatPro, Display, TEXT("opus EncoderVersion: %s"), ANSI_TO_TCHAR(opus_get_version_string()));

	UE_LOG(LogVoiceChatPro, Display, TEXT("OpusInit: %d %d"), InSampleRate, InNumChannels);

	if (InSampleRate != 8000 &&
		InSampleRate != 12000 &&
		InSampleRate != 16000 &&
		InSampleRate != 24000 &&
		InSampleRate != 48000)
	{
		UE_LOG(LogVoiceChatPro, Error, TEXT("opus Voice encoder doesn't support %d hz"), InSampleRate);
		return false;
	}

	if (opusFramePerSec != 400 &&
		opusFramePerSec != 200 &&
		opusFramePerSec != 100 &&
		opusFramePerSec != 50 &&
		opusFramePerSec != 25)
	{
		UE_LOG(LogVoiceChatPro, Error, TEXT("opus Voice encoder doesn't support opusFramePerSec %d per sec"), opusFramePerSec);
		return false;
	}

	if (InNumChannels < 1 || InNumChannels > 2)
	{
		UE_LOG(LogVoiceChatPro, Warning, TEXT("opus Voice encoder only supports 1 or 2 channels"));
		return false;
	}

	encoderSampleRate = InSampleRate;
	encoderNumChannels = InNumChannels;
	NUM_OPUS_FRAMES_PER_SEC = opusFramePerSec;

	// 20ms frame sizes are a good choice for most applications (1000ms / 20ms = 50)
	encoderFrameSize = encoderSampleRate / NUM_OPUS_FRAMES_PER_SEC;
	encoderBytesPerFrame = encoderFrameSize * encoderNumChannels * sizeof(opus_int16);

	//MaxFrameSize = FrameSize * MAX_OPUS_FRAMES;

	int32 EncError = 0;

	const int32 Application = OPUS_APPLICATION_VOIP;

#if USE_UE4_MEM_ALLOC
	const int32 EncSize = opus_encoder_get_size(encoderNumChannels);
	Encoder = (OpusEncoder*)FMemory::Malloc(EncSize);
	EncError = opus_encoder_init(Encoder, encoderSampleRate, encoderNumChannels, Application);
#else
	Encoder = opus_encoder_create(SampleRate, NumChannels, Application, &EncError);
#endif

	if (EncError == OPUS_OK)
	{
		// Turn on variable bit rate encoding
		const int32 UseVbr = 1;
		opus_encoder_ctl(Encoder, OPUS_SET_VBR(UseVbr));

		// Turn off constrained VBR
		const int32 UseCVbr = 0;
		opus_encoder_ctl(Encoder, OPUS_SET_VBR_CONSTRAINT(UseCVbr));

		// Complexity (1-10)
		const int32 Complexity = 1;
		opus_encoder_ctl(Encoder, OPUS_SET_COMPLEXITY(Complexity));

		// Forward error correction
		const int32 InbandFEC = 0;
		opus_encoder_ctl(Encoder, OPUS_SET_INBAND_FEC(InbandFEC));

#if DEBUG_OPUS
		//DebugEncoderInfo(Encoder);
#endif // DEBUG_OPUS
	}
	else
	{
		UE_LOG(LogVoiceChatPro, Warning, TEXT("opus Failed to init Opus Encoder: %s"), ANSI_TO_TCHAR(opus_strerror(EncError)));
	}

	UE_LOG(LogVoiceChatPro, Display, TEXT("OPUS_OK"));
	return EncError == OPUS_OK;
}


int32 UMicrophoneSpeakComponent::OpusEncode(const uint8* RawPCMData, uint32 RawDataSize, uint8* OutCompressedData, uint32& OutCompressedDataSize)
{
	check(Encoder);

	int32 HeaderSize = 0;
	const int32 BytesPerFrame = encoderBytesPerFrame;//encoderFrameSize * encoderNumChannels * sizeof(opus_int16);
	const int32 MaxFramesEncoded = MAX_OPUS_UNCOMPRESSED_BUFFER_SIZE / BytesPerFrame;

	// total bytes / bytes per frame
	const int32 NumFramesToEncode = FMath::Min((int32)RawDataSize / BytesPerFrame, MaxFramesEncoded);
	const int32 DataRemainder = RawDataSize % BytesPerFrame;
	const int32 RawDataStride = BytesPerFrame;

	//UE_LOG(LogVoiceChatPro, Warning, TEXT("opus encode MaxFramesEncoded %d NumFramesToEncode %d DataRemainder %d"), MaxFramesEncoded, NumFramesToEncode, DataRemainder);


	if (NumFramesToEncode == 0)
	{
		// We can avoid saving out an empty header if we know we're not going to send anything
		check(DataRemainder == RawDataSize);
		OutCompressedDataSize = 0;
		return DataRemainder;
	}

	// Store the number of frames to be encoded
	check(NumFramesToEncode < MAX_uint8);
	OutCompressedData[0] = (uint8)NumFramesToEncode;
	OutCompressedData[1] = encoderGeneration;
	HeaderSize += 2 * sizeof(uint8);

	// Store the offset to each encoded frame
	uint16* CompressedOffsets = (uint16*)(OutCompressedData + HeaderSize);
	const uint32 LengthOfCompressedOffsets = NumFramesToEncode * sizeof(uint16);
	HeaderSize += LengthOfCompressedOffsets;

	// Store the entropy to each encoded frame
#if ADD_ENTROPY_TO_PACKET
	uint32* EntropyOffsets = (uint32*)(OutCompressedData + HeaderSize);
	uint32 LengthOfEntropyOffsets = NumFramesToEncode * sizeof(uint32);
	HeaderSize += LengthOfEntropyOffsets;
#endif

	// Space available after overhead
	int32 AvailableBufferSize = OutCompressedDataSize - HeaderSize;

	// Start of the actual compressed data
	uint8* CompressedDataStart = OutCompressedData + HeaderSize;
	int32 CompressedBufferOffset = 0;
	for (int32 i = 0; i < NumFramesToEncode; i++)
	{
		
		int32 CompressedLength = opus_encode(Encoder, (const opus_int16*)(RawPCMData + (i * RawDataStride)), encoderFrameSize, CompressedDataStart + CompressedBufferOffset, AvailableBufferSize);
		if (CompressedLength < 0)
		{
			const char* ErrorStr = opus_strerror(CompressedLength);
			UE_LOG(LogVoiceChatPro, Warning, TEXT("opus Failed to encode: [%d] %s"), CompressedLength, ANSI_TO_TCHAR(ErrorStr));

			// Mark header as nothing encoded
			OutCompressedData[0] = 0;
			OutCompressedDataSize = 0;
			return 0;
		}
		else if (CompressedLength != 1)
		{
#if ADD_ENTROPY_TO_PACKET
			opus_encoder_ctl(Encoder, OPUS_GET_FINAL_RANGE(&Entropy[LastEntropyIdx]));

			UE_LOG(LogVoiceChatPro, VeryVerbose, TEXT("Entropy[%d]=%d"), i, Entropy[LastEntropyIdx]);
			EntropyOffsets[i] = Entropy[LastEntropyIdx];

			LastEntropyIdx = (LastEntropyIdx + 1) % NUM_ENTROPY_VALUES;
#endif

#if DEBUG_OPUS
			DebugFrameEncodeInfo(CompressedDataStart + CompressedBufferOffset, CompressedLength, SampleRate);
#endif // DEBUG_OPUS

			AvailableBufferSize -= CompressedLength;
			CompressedBufferOffset += CompressedLength;

			check(CompressedBufferOffset < MAX_uint16);
			CompressedOffsets[i] = (uint16)CompressedBufferOffset;
		}
		else
		{
			UE_LOG(LogVoiceChatPro, Warning, TEXT("Nothing to encode!"));
			CompressedOffsets[i] = 0;
#if ADD_ENTROPY_TO_PACKET
			EntropyOffsets[i] = 0;
#endif
		}
	}

	// End of buffer
	OutCompressedDataSize = HeaderSize + CompressedBufferOffset;

	UE_LOG(LogVoiceChatPro, VeryVerbose, TEXT("OpusEncode[%d]: RawSize: %d HeaderSize: %d CompressedSize: %d NumFramesEncoded: %d Remains: %d"), encoderGeneration, RawDataSize, HeaderSize, OutCompressedDataSize, NumFramesToEncode, DataRemainder);


	encoderGeneration = (encoderGeneration + 1) % MAX_uint8;
	return DataRemainder;
}



bool UMicrophoneSpeakComponent::OpusDecoderInit(int32 InSampleRate, int32 InNumChannels, int32 opusFramePerSec)
{
	UE_LOG(LogVoiceChatPro, Display, TEXT("DecoderVersion: %s"), ANSI_TO_TCHAR(opus_get_version_string()));

	if (InSampleRate != 8000 &&
		InSampleRate != 12000 &&
		InSampleRate != 16000 &&
		InSampleRate != 24000 &&
		InSampleRate != 48000)
	{
		UE_LOG(LogVoiceChatPro, Warning, TEXT("Voice decoder doesn't support %d hz"), InSampleRate);
		return false;
	}

	if (opusFramePerSec != 400 &&
		opusFramePerSec != 200 &&
		opusFramePerSec != 100 &&
		opusFramePerSec != 50 &&
		opusFramePerSec != 25)
	{
		UE_LOG(LogVoiceChatPro, Error, TEXT("opus Voice decoder doesn't support opusFramePerSec %d per sec"), opusFramePerSec);
		return false;
	}


	if (InNumChannels < 1 || InNumChannels > 2)
	{
		UE_LOG(LogVoiceChatPro, Warning, TEXT("Voice decoder only supports 1 or 2 channels"));
		return false;
	}

	decoderSampleRate = InSampleRate;
	decoderNumChannels = InNumChannels;
	NUM_OPUS_FRAMES_PER_SEC = opusFramePerSec;

	// 20ms frame sizes are a good choice for most applications (1000ms / 20ms = 50)
	decoderFrameSize = decoderSampleRate / NUM_OPUS_FRAMES_PER_SEC;

	int32 DecError = 0;

#if USE_UE4_MEM_ALLOC
	const int32 DecSize = opus_decoder_get_size(decoderNumChannels);
	Decoder = (OpusDecoder*)FMemory::Malloc(DecSize);
	DecError = opus_decoder_init(Decoder, decoderSampleRate, decoderNumChannels);
#else
	Decoder = opus_decoder_create(SampleRate, NumChannels, &DecError);
#endif
	if (DecError == OPUS_OK)
	{
#if DEBUG_OPUS
		DebugDecoderInfo(Decoder);
#endif // DEBUG_OPUS
	}
	else
	{
		UE_LOG(LogVoiceChatPro, Warning, TEXT("Failed to init Opus Decoder: %s"), ANSI_TO_TCHAR(opus_strerror(DecError)));
	}

	return DecError == OPUS_OK;
}



inline bool SanityCheckHeader(uint32 HeaderSize, uint32 CompressedDataSize, int32 NumFramesToDecode, const uint16* CompressedOffsets)
{
	bool bHeaderDataOk = (HeaderSize <= CompressedDataSize);
	if (bHeaderDataOk)
	{
		// Validate that the sum of the encoded data sizes fit under the given amount of compressed data
		uint16 LastCompressedOffset = 0;
		int32 TotalCompressedBufferSize = 0;
		for (int32 Idx = 0; Idx < NumFramesToDecode; Idx++)
		{
			// Offsets should be monotonically increasing (prevent later values intentionally reducing bad previous values)
			if (CompressedOffsets[Idx] >= LastCompressedOffset)
			{
				TotalCompressedBufferSize += (CompressedOffsets[Idx] - LastCompressedOffset);
				LastCompressedOffset = CompressedOffsets[Idx];
			}
			else
			{
				bHeaderDataOk = false;
				break;
			}
		}

		bHeaderDataOk = bHeaderDataOk && ((HeaderSize + TotalCompressedBufferSize) <= CompressedDataSize);
	}

	return bHeaderDataOk;
}

void UMicrophoneSpeakComponent::OpusDecode(const uint8* InCompressedData, uint32 CompressedDataSize, uint8* OutRawPCMData, uint32& OutRawDataSize)
{
	
	uint32 HeaderSize = (2 * sizeof(uint8));
	if (!InCompressedData || (CompressedDataSize < HeaderSize))
	{
		OutRawDataSize = 0;
		return;
	}

	const int32 BytesPerFrame = decoderFrameSize * decoderNumChannels * sizeof(opus_int16);
	const int32 MaxFramesEncoded = MAX_OPUS_UNCOMPRESSED_BUFFER_SIZE / BytesPerFrame;

	const int32 NumFramesToDecode = InCompressedData[0];
	const int32 PacketGeneration = InCompressedData[1];

	if (PacketGeneration != decoderLastGeneration + 1)
	{
		UE_LOG(LogVoiceChatPro, Verbose, TEXT("Packet generation skipped from %d to %d"), decoderLastGeneration, PacketGeneration);
	}

	if ((NumFramesToDecode > 0) && (NumFramesToDecode <= MaxFramesEncoded))
	{
		// Start of compressed data offsets
		const uint16* CompressedOffsets = (const uint16*)(InCompressedData + HeaderSize);
		uint32 LengthOfCompressedOffsets = NumFramesToDecode * sizeof(uint16);
		HeaderSize += LengthOfCompressedOffsets;

#if ADD_ENTROPY_TO_PACKET
		// Start of the entropy to each encoded frame
		const uint32* EntropyOffsets = (uint32*)(InCompressedData + HeaderSize);
		uint32 LengthOfEntropyOffsets = NumFramesToDecode * sizeof(uint32);
		HeaderSize += LengthOfEntropyOffsets;
#endif

		// At this point we have all our pointer fix up complete, but the data it references may be invalid in corrupt/spoofed packets
		// Sanity check the numbers to make sure everything works out
		if (SanityCheckHeader(HeaderSize, CompressedDataSize, NumFramesToDecode, CompressedOffsets))
		{
			// Start of compressed data
			const uint8* CompressedDataStart = (InCompressedData + HeaderSize);

			int32 CompressedBufferOffset = 0;
			int32 DecompressedBufferOffset = 0;
			uint16 LastCompressedOffset = 0;

			for (int32 i = 0; i < NumFramesToDecode; i++)
			{
				const int32 UncompressedBufferAvail = (OutRawDataSize - DecompressedBufferOffset);

				if (UncompressedBufferAvail >= (MAX_OPUS_FRAMES * BytesPerFrame))
				{
					if (CompressedOffsets[i] > 0)
					{
						const int32 CompressedBufferSize = (CompressedOffsets[i] - LastCompressedOffset);

						check(Decoder);
						const int32 NumDecompressedSamples = opus_decode(Decoder,
							CompressedDataStart + CompressedBufferOffset, CompressedBufferSize,
							(opus_int16*)(OutRawPCMData + DecompressedBufferOffset), MAX_OPUS_FRAME_SIZE, 0);

#if DEBUG_OPUS
						DebugFrameDecodeInfo(CompressedDataStart + CompressedBufferOffset, CompressedBufferSize, SampleRate);
#endif // DEBUG_OPUS

						if (NumDecompressedSamples < 0)
						{
							const char* ErrorStr = opus_strerror(NumDecompressedSamples);
							UE_LOG(LogVoiceChatPro, Warning, TEXT("Failed to decode: [%d] %s"), NumDecompressedSamples, ANSI_TO_TCHAR(ErrorStr));
						}
						else
						{
							if (NumDecompressedSamples != decoderFrameSize)
							{
								UE_LOG(LogVoiceChatPro, Warning, TEXT("Unexpected decode result NumSamplesDecoded %d != FrameSize %d"), NumDecompressedSamples, decoderFrameSize);
							}

#if ADD_ENTROPY_TO_PACKET
							opus_decoder_ctl(Decoder, OPUS_GET_FINAL_RANGE(&Entropy[LastEntropyIdx]));

							if (Entropy[LastEntropyIdx] != EntropyOffsets[i])
							{
								UE_LOG(LogVoiceDecode, Verbose, TEXT("Decoder Entropy[%d/%d] = %d expected %d"), i, NumFramesToDecode - 1, Entropy[LastEntropyIdx], EntropyOffsets[i]);
							}


							LastEntropyIdx = (LastEntropyIdx + 1) % NUM_ENTROPY_VALUES;
#endif

							// Advance within the decompressed output stream
							DecompressedBufferOffset += (NumDecompressedSamples * decoderNumChannels * sizeof(opus_int16));
						}

						// Advance within the compressed input stream
						CompressedBufferOffset += CompressedBufferSize;
						LastCompressedOffset = CompressedOffsets[i];
					}
					else
					{
						UE_LOG(LogVoiceChatPro, Verbose, TEXT("Decompression buffer skipped a frame"));
						// Nothing to advance within the compressed input stream
					}
				}
				else
				{
					UE_LOG(LogVoiceChatPro, Warning, TEXT("Decompression buffer too small to decode voice"));
					break;
				}
			}

			OutRawDataSize = DecompressedBufferOffset;
		}
		else
		{
			UE_LOG(LogVoiceChatPro, Warning, TEXT("Failed to decode: header corrupted"));
			OutRawDataSize = 0;
		}
	}
	else
	{
		UE_LOG(LogVoiceChatPro, Warning, TEXT("Failed to decode: buffer corrupted"));
		OutRawDataSize = 0;
	}

	UE_LOG(LogVoiceChatPro, VeryVerbose, TEXT("OpusDecode[%d]: RawSize: %d HeaderSize: %d CompressedSize: %d NumFramesDecoded: %d "), PacketGeneration, OutRawDataSize, HeaderSize, CompressedDataSize, NumFramesToDecode);

	decoderLastGeneration = PacketGeneration;
}
