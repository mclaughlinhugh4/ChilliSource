//
//  LocalNotificationSystem.h
//  Chilli Source
//  Created by Steven Hendrie on 13/12/2011.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2011 Tag Games Limited
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//

#ifdef CS_TARGETPLATFORM_ANDROID

#ifndef _CSBACKEND_PLATFORM_ANDROID_CORE_NOTIFICATION_LOCALNOTIFICATIONSYSTEM_H_
#define _CSBACKEND_PLATFORM_ANDROID_CORE_NOTIFICATION_LOCALNOTIFICATIONSYSTEM_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Notification/LocalNotificationSystem.h>
#include <CSBackend/Platform/Android/ForwardDeclarations.h>

namespace CSBackend
{
	namespace Android
	{
		//--------------------------------------------------------
		/// Android backend for the local notification system
		///
		/// @author Steven Hendrie
		//--------------------------------------------------------
		class LocalNotificationSystem : public CSCore::LocalNotificationSystem
		{
		public:
			CS_DECLARE_NAMEDTYPE(LocalNotificationSystem);
            //---------------------------------------------------
            /// @author Ian Copland
            ///
			/// @param The interface ID to check against.
            /// @return Whether this implements the passed in
			/// interface id.
            //--------------------------------------------------
            bool IsA(CSCore::InterfaceIDType in_interfaceId) const override;
			//--------------------------------------------------
			/// Enables and disables addition of local notifications.
			/// All existing notifications will be cancelled
			/// when this is disabled. This is enabled by default.
			///
			/// @author Ian Copland
			///
			/// @param Whether or not to enable the scheduling
			/// of app notifications.
			//---------------------------------------------------
			void SetEnabled(bool in_enabled) override;
			//---------------------------------------------------
			/// Schedules a Local Notifcation which should fire
			/// at the given time. A Local Notification uses
			/// the system specfic notification alarms and can
			/// be received even if it was scheduled during a
			/// previous instance of the application.
			///
			/// @author Ian Copland
			///
			/// @param The notification id
			/// @param The notification params.
			/// @param Time in seconds at which it should trigger.
			/// @param [Optional] The notification priority. Defaults
			/// to standard priority.
			//---------------------------------------------------
			void ScheduleNotificationForTime(CSCore::Notification::ID in_id, const CSCore::ParamDictionary& in_params, TimeIntervalSecs in_time, CSCore::Notification::Priority in_priority = CSCore::Notification::Priority::k_standard) override;
			//--------------------------------------------------------
			/// Builds a list of all notifications currently scheduled
			/// within the given time period.
			///
			/// @author Ian Copland
			///
			/// @param [Out] The list of notifications.
			/// @param [Optional] The start time.
			/// @param [Optional] The end time.
			//--------------------------------------------------------
			void GetScheduledNotifications(std::vector<CSCore::NotificationCSPtr>& out_notifications, TimeIntervalSecs in_time = 0, TimeIntervalSecs in_period = std::numeric_limits<TimeIntervalSecs>::max()) const override;
			//-------------------------------------------------------
			/// Prevent any notifications with given ID type from firing
			///
			/// @author Steven Hendrie
			///
			/// @param ID type
			//------------------------------------------------------
			void CancelByID(CSCore::Notification::ID in_id) override;
			//------------------------------------------------------
			/// @author Steven Hendrie
			///
			/// Terminate all currently scheduled notifications
			//-----------------------------------------------------
			void CancelAll() override;
            //--------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return An event that can be used to listen for
            /// new notifications being recieved.
            //---------------------------------------------------
            CSCore::IConnectableEvent<ReceivedDelegate>& GetReceivedEvent() override;
			//-----------------------------------------------------
			/// Called when game receives a local notification.
			///
			/// @author Steven Hendrie
            ///
            /// @param The notification ID
            /// @param The notification params.
            /// @param The notification priority.
			//----------------------------------------------------
			void OnNotificationReceived(CSCore::Notification::ID in_id, const CSCore::ParamDictionary& in_params, CSCore::Notification::Priority in_priority);
		private:
            friend CSCore::LocalNotificationSystemUPtr CSCore::LocalNotificationSystem::Create();
            //-------------------------------------------------------
            /// Private constructor to force use of factory method
            ///
            /// @author S Downie
            //-------------------------------------------------------
            LocalNotificationSystem();

		private:
            bool m_enabled;
            CSCore::Event<ReceivedDelegate> m_recievedEvent;
			LocalNotificationJavaInterfaceSPtr m_localNotificationJI;
		};
	}
}
#endif

#endif
