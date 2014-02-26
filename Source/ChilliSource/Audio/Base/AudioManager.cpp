/*
 * File: AudioManager.cpp
 * Date: 16/11/2010 2010 
 * Description: 
 */

/*
 * Author: Scott Downie
 * Version: v 1.0
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#include <ChilliSource/Audio/Base/AudioManager.h>
#include <ChilliSource/Audio/Base/AudioLoader.h>

#include <ChilliSource/Core/Resource/ResourceProvider.h>

namespace ChilliSource
{
	namespace Audio
	{
		DEFINE_NAMED_INTERFACE(AudioManager);
		//----------------------------------------------------------------
		/// Is A
		///
		/// Query the interface type
		/// @param The interface to compare
		/// @return Whether the object implements that interface
		//----------------------------------------------------------------
		bool AudioManager::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == AudioManager::InterfaceID;
		}
		//----------------------------------------------------------------
		/// Get Resource Type
		///
		/// @return The type of resource this manager handles
		//----------------------------------------------------------------
		Core::InterfaceIDType AudioManager::GetResourceType() const
		{
			return AudioResource::InterfaceID;
		}
		//----------------------------------------------------------------
		/// Manages Resource Of Type
		///
		/// @return Whether this object manages the object of type
		//----------------------------------------------------------------
		bool AudioManager::ManagesResourceOfType(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == AudioResource::InterfaceID;
		}
		//----------------------------------------------------------------
		/// Manages Resource With Extension
		///
		/// @param Extension
		/// @return Whether this object manages object with extension
		//----------------------------------------------------------------
		bool AudioManager::ManagesResourceWithExtension(const std::string &instrExtension) const
		{
			for (u32 nProvider = 0; nProvider < mResourceProviders.size(); nProvider++) 
			{
				if(mResourceProviders[nProvider]->CanCreateResourceFromFileWithExtension(instrExtension))
				{
					return true;
				}
			}
			
			return false;
		}
		//----------------------------------------------------------------
		/// Get Provider Type
		///
		/// @return The type of resource it consumes from resource provider
		//----------------------------------------------------------------
		Core::InterfaceIDType AudioManager::GetProviderType() const
		{
			return Audio::AudioResource::InterfaceID;
		}
		//-----------------------------------------------------------------
		/// Get Resource From File
		///
		/// Generic call to get the managers resource
        /// @param The storage location to load from
		/// @param File path to resource
		/// @return Generic pointer to object type
		//-----------------------------------------------------------------
		Core::ResourcePtr AudioManager::GetResourceFromFile(Core::StorageLocation ineStorageLocation, const std::string &instrFilePath)
		{
			return GetSoundFromFile(ineStorageLocation, instrFilePath);
		}
		//----------------------------------------------------------------
		/// Stream Sound From File
		///
		/// Starts streaming audio from file
        ///
        /// @param The storage location to load from
        /// @param the file path
        /// @return The audio resource pointer
		//----------------------------------------------------------------
		AudioResourceSPtr AudioManager::StreamSoundFromFile(Core::StorageLocation ineStorageLocation, const std::string &inFilePath)
		{
			Core::ResourcePtr pResource = CreateAudioResource();
			
			for(u32 nProvider = 0; nProvider < mResourceProviders.size(); nProvider++) 
			{
				if(mResourceProviders[nProvider]->StreamResourceFromFile(ineStorageLocation, inFilePath, pResource)) 
				{
					//Add it to the cache
					CS_DEBUG_LOG("Streaming sound " + inFilePath);
					AudioResourceSPtr pAudio = std::static_pointer_cast<AudioResource>(pResource);
					pAudio->SetName(inFilePath);
					pAudio->SetOwningResourceManager(this);
					pAudio->SetLoaded(true);
					return pAudio;
				}
			}
		
			//Resource not found
			CS_ERROR_LOG("Cannot find resource for sound with path " + inFilePath);
			return AudioResourceSPtr();
		}
		//----------------------------------------------------------------
		/// Get Sound From File
		///
		/// Creates (lazily loads) a model instance from file. If the 
		/// model it already loaded it returns a handle to the model
        ///
        /// @param The storage location to load from
        /// @param the file path
        /// @return the audio resource pointer
		//----------------------------------------------------------------
		AudioResourceSPtr AudioManager::GetSoundFromFile(Core::StorageLocation ineStorageLocation, const std::string &inFilePath)
		{
			MapStringToSoundEffectPtrItr pExistingResource = mMapFileNamesToSoundEffect.find(inFilePath);
			
			if(pExistingResource == mMapFileNamesToSoundEffect.end()) 
			{
				Core::ResourcePtr pResource = CreateAudioResource();
				
				for(u32 nProvider = 0; nProvider < mResourceProviders.size(); nProvider++) 
				{
					if(mResourceProviders[nProvider]->CreateResourceFromFile(ineStorageLocation, inFilePath, pResource)) 
					{
						//Add it to the cache
						CS_DEBUG_LOG("Loading sound " + inFilePath);
						
						AudioResourceSPtr pAudio = std::static_pointer_cast<AudioResource>(pResource);
						mMapFileNamesToSoundEffect.insert(std::make_pair(inFilePath, pAudio));
						pAudio->SetName(inFilePath);
						pAudio->SetOwningResourceManager(this);
						pAudio->SetLoaded(true);
						return pAudio;
					}
				}
			} 
			else 
			{
				return pExistingResource->second;
			}
			
			//Resource not found
			CS_ERROR_LOG("Cannot find resource for sound with path " + inFilePath);
			return AudioResourceSPtr();
		}
		//----------------------------------------------------------------
		/// Create Listener
		///
		/// @return Concrete audio listener
		//----------------------------------------------------------------
		AudioListenerSPtr AudioManager::CreateListener()
		{
			for(u32 nProvider = 0; nProvider < mResourceProviders.size(); nProvider++) 
			{
				AudioListenerSPtr pListener = static_cast<AudioLoader*>(mResourceProviders[nProvider])->CreateAudioListener();
				if(pListener) 
				{
					return pListener;
				}
			}
			
			return AudioListenerSPtr();
		}
		//-----------------------------------------------------------------
		/// Destroy Sound Effect
		///
		/// @param Handle to the sound you want to destroy
		//-----------------------------------------------------------------
		void AudioManager::Destroy(const Core::ResourcePtr& inpSoundEffect)
		{
			AudioResourceSPtr pSound = std::static_pointer_cast<AudioResource>(inpSoundEffect);
			for(MapStringToSoundEffectPtrItr it = mMapFileNamesToSoundEffect.begin(); it != mMapFileNamesToSoundEffect.end(); ++it)
			{
				if(it->second == pSound)
				{
					CS_DEBUG_LOG("Destroying sound effect " + pSound->GetName());
					mMapFileNamesToSoundEffect.erase(it);
					return;
				}
			}
			
			CS_ERROR_LOG("Destroying sound effect " + pSound->GetName());
		}
		//-----------------------------------------------------------------
		/// Destroy All Sound Effects
		///
		//-----------------------------------------------------------------
		void AudioManager::DestroyAll()
		{
			CS_DEBUG_LOG("Clearing sound effect cache");
			for(MapStringToSoundEffectPtrItr it = mMapFileNamesToSoundEffect.begin(); it != mMapFileNamesToSoundEffect.end(); ++it)
			{
				//If we are the only person using this then kill it dead
				if(it->second.use_count() == 1)
				{
					it = mMapFileNamesToSoundEffect.erase(it);
				}
			}
		}
	}
}
