/*
 * File: AudioSystem.cpp
 * Date: 18/11/2010 2010 
 * Description: 
 */

/*
 * Author: Scott Downie
 * Version: v 1.0
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#include <ChilliSource/Audio/Base/AudioSystem.h>

namespace ChilliSource
{
	namespace Audio
	{
        DEFINE_NAMED_INTERFACE(AudioSystem);
		//-------------------------------------------------------
		/// Constructor
		///
		/// Default
		//-------------------------------------------------------
		AudioSystem::AudioSystem() : mfMasterEffectVolume(kfDefaultAudioVolume), mfMasterStreamVolume(kfDefaultAudioVolume),
		mfDopplerFactor(kfDefaultDoppler), mfRolloffFactor(kfDefaultRolloff), mfDistanceFactor(kfDefaultDistance), mpAudioComponentFactory(nullptr), mpAudioManager(nullptr)
		{
			//Set the overall system volume
			SetMasterEffectVolume(kfDefaultAudioVolume);
			SetMasterStreamVolume(kfDefaultAudioVolume);
		}
		//-------------------------------------------------------
		/// Is A
		///
		/// @return Whether this object is of given type
		//-------------------------------------------------------
		bool AudioSystem::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == Core::IUpdateable::InterfaceID || inInterfaceID == Core::IComponentProducer::InterfaceID || inInterfaceID == AudioSystem::InterfaceID;
		}
		//-------------------------------------------------------
		/// Set Master Effect Volume
		///
		/// Set the volume for the entire audio effect system
		/// @param Normalised volume level [0, 1]
		//-------------------------------------------------------
		void AudioSystem::SetMasterEffectVolume(f32 infVolume)
		{
			mfMasterEffectVolume = Core::CMathUtils::Clamp(infVolume, 0.0f, 1.0f);
			
			//Trigger the active sounds OnMasterVolumeChanged delegate
			mOnMasterEffectVolumeChangedEvent.Invoke();
		}
		//-------------------------------------------------------
		/// Set Master Stream Volume
		///
		/// Set the volume for the entire audio stream system
		/// @param Normalised volume level [0, 1]
		//-------------------------------------------------------
		void AudioSystem::SetMasterStreamVolume(f32 infVolume)
		{
			mfMasterStreamVolume = Core::CMathUtils::Clamp(infVolume, 0.0f, 1.0f);
			
			//Trigger the active sounds OnMasterVolumeChanged delegate
			mOnMasterStreamVolumeChangedEvent.Invoke();
		}
		//-------------------------------------------------------
		/// Get Master Effect Volume
		///
		/// Get the volume for the entire audio effect system
		/// @return Normalised volume level [0, 1]
		//-------------------------------------------------------
		f32 AudioSystem::GetMasterEffectVolume() const
		{
			return mfMasterEffectVolume;
		}
		//-------------------------------------------------------
		/// Get Master Stream Volume
		///
		/// Get the volume for the entire audio stream system
		/// @return Normalised volume level [0, 1]
		//-------------------------------------------------------
		f32 AudioSystem::GetMasterStreamVolume() const
		{
			return mfMasterStreamVolume;
		}
		//-------------------------------------------------------
		/// Get Audio Manager
		///
		/// @param Audio manager
		//-------------------------------------------------------
		const AudioManager& AudioSystem::GetAudioManager() const
		{
			return *mpAudioManager;
		}
		//-------------------------------------------------------
		/// Get Audio Manager Pointer
		///
		/// @param Pointer to audio manager
		//-------------------------------------------------------
		const AudioManager* AudioSystem::GetAudioManagerPtr() const
		{
			return mpAudioManager;
		}
		//----------------------------------------------------
		/// Get Number Of Component Factories
		///
		/// @return Number of factories in this system
		//----------------------------------------------------
		u32  AudioSystem::GetNumComponentFactories() const
		{
			return 1;
		}
		//-------------------------------------------------------
		/// Get Component Factory Pointer
		///
		/// @return Audio component factory pointer
		//-------------------------------------------------------
		Core::IComponentFactory* AudioSystem::GetComponentFactoryPtr(u32 inudwIndex)
		{
			return mpAudioComponentFactory;
		}
		//-------------------------------------------------------
		/// Get Component Factory
		///
		/// @return Audio component factory
		//-------------------------------------------------------
		Core::IComponentFactory& AudioSystem::GetComponentFactory(u32 inudwIndex)
		{
			return *mpAudioComponentFactory;
		}
		//-------------------------------------------------------
		/// Get Master Effect Volume Changed Event
		///
		/// Exposed event that allows sound resources to 
		/// listen for this event
		/// @return Audio event
		//-------------------------------------------------------
		Core::IEvent<AudioVolumeEventDelegate>& AudioSystem::GetMasterEffectVolumeChangedEvent()
		{
			return mOnMasterEffectVolumeChangedEvent;
		}
		//-------------------------------------------------------
		/// Get Master Stream Volume Changed Event
		///
		/// Exposed event that allows sound resources to 
		/// listen for this event
		/// @return Audio event
		//-------------------------------------------------------
		Core::IEvent<AudioVolumeEventDelegate>& AudioSystem::GetMasterStreamVolumeChangedEvent()
		{
			return mOnMasterStreamVolumeChangedEvent;
		}
		//-------------------------------------------------------
		/// Destructor
		///
		//-------------------------------------------------------
		AudioSystem::~AudioSystem()
		{
		}
	}
}