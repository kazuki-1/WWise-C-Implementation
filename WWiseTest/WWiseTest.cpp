// WWiseTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <AK/SoundEngine/Common/AkSoundEngine.h>
#include <AK/SoundEngine/Common/AkModule.h>
#include <AK/SoundEngine/Common/AkMemoryMgr.h>
#include <AK/SoundEngine/Common/IAkStreamMgr.h>
#include <AK/Tools/Common/AkPlatformFuncs.h>
#include <AK/SpatialAudio/Common/AkSpatialAudio.h>
#include <AK/SoundEngine/Common/AkStreamMgrModule.h>
#include <AK/MusicEngine/Common/AkMusicEngine.h>
#include <AK/Comm/AkCommunication.h>
#include <iostream>
#include <assert.h>
#include "../WWise/SoundBank/Wwise_IDs.h"
#include "../WWise Dependencies/Common/AkDefaultIOHookBlocking.h"
#include "../WWise Dependencies/Common/AkFileHelpers.h"
#include "../WWise Dependencies/Common/AkFilePackageLowLevelIOBlocking.h"
#include <filesystem>


CAkFilePackageLowLevelIOBlocking package;


int main()
{
    std::cout << "Hello World!\n";

    AKRESULT  result = AK_Fail;

    // WWiseMemoryManager initialization
    AkMemSettings  memorySettings;
    AK::MemoryMgr::GetDefaultSettings(memorySettings);
    result = AK::MemoryMgr::Init(&memorySettings);
    assert(result == AK_Success);

    // WWiseStreamManager Initialization
    AkStreamMgrSettings streamMgrSettings;
    AK::StreamMgr::GetDefaultSettings(streamMgrSettings);
    assert(AK::StreamMgr::Create(streamMgrSettings) != nullptr);
    AK::StreamMgr::SetCurrentLanguage(AKTEXT("English (US)"));




    AkDeviceSettings deviceSettings;
    AK::StreamMgr::GetDefaultDeviceSettings(deviceSettings);
    result = package.Init(deviceSettings);



    package.SetBasePath(L"../WWise/SoundBank/Windows");
    AK::StreamMgr::SetFileLocationResolver(&package);

    // WWiseSoundEngine Initialization

    AkInitSettings initSettings;
    AkPlatformInitSettings platformSettings;
    AK::SoundEngine::GetDefaultInitSettings(initSettings);
    AK::SoundEngine::GetDefaultPlatformInitSettings(platformSettings);
    result = AK::SoundEngine::Init(&initSettings, &platformSettings);
    assert(result == AK_Success);

    // WWiseMusicEngine Initialization

    AkMusicSettings musicSettings;
    AK::MusicEngine::GetDefaultInitSettings(musicSettings);
    result = AK::MusicEngine::Init(&musicSettings);
    assert(result == AK_Success);

    // WWiseSpatialAudio Initialization

    AkSpatialAudioInitSettings spatialAudioSettings;    // Automatically filled when constructed
    result = AK::SpatialAudio::Init(spatialAudioSettings);
    assert(result == AK_Success);

    // WWiseCommunication intialization

    //AkCommSettings commSettings;
    //AK::Comm::GetDefaultInitSettings(commSettings);
    //assert(AK::Comm::Init(commSettings) == AK_Success);


    std::filesystem::path path{ "Init.bnk" };

    AkBankID bankID;

    result = AK::SoundEngine::LoadBank(path.c_str(), bankID);
    assert(result == AK_Success);




    path.replace_filename("Test.bnk");

    result = AK::SoundEngine::LoadBank(path.c_str(), bankID);
    assert(result == AK_Success);



    AkUniqueID ids[] = { AK::EVENTS::PLAYMUSIC, AK::EVENTS::WALKONGRAVEL };
    AkInt32 event_count = 2;

    AkGameObjectID listener_id{ 100 };


    AK::SoundEngine::RegisterGameObj(listener_id);
    AK::SoundEngine::SetDefaultListeners(&listener_id, 1);


    AK::SoundEngine::PostEvent(AK::EVENTS::WALKONGRAVEL, listener_id);
    AK::SoundEngine::RenderAudio();
    system("pause");

    AK::SoundEngine::UnregisterAllGameObj();
    AK::SoundEngine::ClearPreparedEvents();
    AK::SoundEngine::ClearBanks();
    AK::MusicEngine::Term();
    AK::SoundEngine::Term();
    package.Term();
    if (AK::IAkStreamMgr::Get())
        AK::IAkStreamMgr::Get()->Destroy();
    AK::MemoryMgr::Term();

    return 0;






}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
