//
//  HttpRequestSystem.h
//  Chilli Source
//  Created by Ian Copland on 08/11/2011.
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

#ifndef _CSBACKEND_PLATFORM_ANDROID_HTTP_HTTPREQUESTSYSTEM_H_
#define _CSBACKEND_PLATFORM_ANDROID_HTTP_HTTPREQUESTSYSTEM_H_

#include <CSBackend/Platform/Android/ForwardDeclarations.h>
#include <ChilliSource/Networking/Http/HttpRequestSystem.h>

#include <vector>

namespace CSBackend
{
	namespace Android
	{	
		//--------------------------------------------------------------------------------------------------
		/// Android implementation of the http connection system. Reponsible for making http requests to remote
		/// urls and managing the lifetime of the requests and the connections. Uses the Java
		/// HttpUrlConnection library
		///
		/// @author Ian Copland
		//--------------------------------------------------------------------------------------------------
		class HttpRequestSystem final : public CSNetworking::HttpRequestSystem
		{
		public:

			CS_DECLARE_NAMEDTYPE(HttpRequestSystem);

			//--------------------------------------------------------------------------------------------------
			/// @author Ian Copland
			///
			/// @param Interface ID
            ///
			/// @return Whether object if of argument type
			//--------------------------------------------------------------------------------------------------
            bool IsA(CSCore::InterfaceIDType in_interfaceId) const override;
            //--------------------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param The number of seconds that will elapse before a request is deemed to have timed out
            /// on connection
            //--------------------------------------------------------------------------------------------------
            void SetConnectionTimeout(u32 in_timeoutSecs) override;
            //--------------------------------------------------------------------------------------------------
            /// Causes the system to issue a request with the given details.
            ///
            /// @author Ian Copland
            ///
            /// @param A descriptor detailing the request params
			/// @param A function to call when the request is completed. Note that the request can be completed with failure as well as success.
            ///
            /// @return A pointer to the request. The system owns this pointer. Returns nullptr if the request cannot be created.
            //--------------------------------------------------------------------------------------------------
            CSNetworking::HttpRequest* MakeRequest(const CSNetworking::HttpRequest::Desc& in_requestDesc, const CSNetworking::HttpRequest::Delegate& in_delegate) override;
			//--------------------------------------------------------------------------------------------------
            /// Equivalent to calling cancel on every incomplete request in progress.
            ///
            /// @author Ian Copland
            //--------------------------------------------------------------------------------------------------
            void CancelAllRequests() override;
            //--------------------------------------------------------------------------------------------------
            /// Checks if the device is internet ready
            ///
            /// @author Ian Copland
            ///
            /// @return Success if net available
            //--------------------------------------------------------------------------------------------------
            bool CheckReachability() const override;

		private:
            friend CSNetworking::HttpRequestSystemUPtr CSNetworking::HttpRequestSystem::Create();
            //--------------------------------------------------------------------------------------------------
            /// Private constructor to fore use of factory method
            ///
            /// @author Ian Copland
            //--------------------------------------------------------------------------------------------------
            HttpRequestSystem() = default;
			//--------------------------------------------------------------------------------------------------
            /// Poll the connection on active requests
            ///
            /// @author Ian Copland
			///
			/// @param Time since last update in seconds
			//--------------------------------------------------------------------------------------------------
            void OnUpdate(f32 in_timeSinceLastUpdate) override;
            //--------------------------------------------------------------------------------------------------
            /// Called when the system is destroyed. Cancels all pending requests
            ///
            /// @author Ian Copland
			//--------------------------------------------------------------------------------------------------
            void OnDestroy() override;

		private:

			std::vector<HttpRequest*> m_requests;
		};
	}

}

#endif

#endif
