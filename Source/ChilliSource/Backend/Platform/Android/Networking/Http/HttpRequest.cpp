//
//  HttpRequest.cpp
//  Chilli Source
//
//  Created by Ian Copland on 08/11/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/Backend/Platform/Android/Networking/Http/HttpRequest.h>

#include <ChilliSource/Backend/Platform/Android/Networking/Http/HttpRequestJavaInterface.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>

namespace ChilliSource
{
	namespace Android
	{
		//------------------------------------------------------------------
		//------------------------------------------------------------------
		HttpRequest::HttpRequest(const Desc& in_requestDesc, u32 in_bufferFlushSize, const Delegate& in_delegate)
		: m_desc(in_requestDesc), m_completionDelegate(in_delegate), m_isRequestComplete(false), m_isPollingComplete(false), m_shouldKillThread(false),
		  m_responseCode(0), m_requestResult(Result::k_failed)
		{
			//Begin the read loop as a threaded task
			Core::TaskScheduler::ScheduleTask(Core::Task<>(this, &HttpRequest::PerformRequest));
		}
		//------------------------------------------------------------------
		//------------------------------------------------------------------
		void HttpRequest::Update()
		{
			//Check if the data has finished streaming and invoke the completion delegate on the main thread
			if(m_isPollingComplete == true)
			{
                m_isRequestComplete = true;

                if(m_completionDelegate != nullptr)
                {
                    if(m_requestResult != Result::k_cancelled)
                    {
                    	m_completionDelegate(this, m_requestResult);
                    }

                    m_completionDelegate = nullptr;
                }
			}
		}
		//------------------------------------------------------------------
		//------------------------------------------------------------------
		void HttpRequest::PerformRequest()
		{
			HttpRequestType type = HttpRequestType::k_get;
			if (m_desc.m_type == Networking::HttpRequest::Type::k_post)
			{
				type = HttpRequestType::k_post;
			}

			//This is a blocking call
			HttpRequestResultCode requestResultCode = HttpRequestJavaInterface::MakeHttpRequest(m_desc.m_url, type, m_desc.m_headers, m_desc.m_body, m_responseData, m_desc.m_redirectionUrl, (s32&)m_responseCode);

			if (m_shouldKillThread == false)
			{
				switch (requestResultCode)
				{
				case HttpRequestResultCode::k_success:
					m_requestResult = Result::k_completed;
					break;
				case HttpRequestResultCode::k_couldNotConnect:
				case HttpRequestResultCode::k_couldNotMakeRequest:
					m_requestResult = Result::k_failed;
					break;
				case HttpRequestResultCode::k_timeout:
					m_requestResult = Result::k_timeout;
					break;
				default:
					break;
				}
			}

			m_isPollingComplete = true;
		}
		//----------------------------------------------------------------------------------------
		//----------------------------------------------------------------------------------------
		void HttpRequest::Cancel()
		{
            m_shouldKillThread = true;
            m_requestResult = Result::k_cancelled;
		}
		//----------------------------------------------------------------------------------------
		//----------------------------------------------------------------------------------------
		bool HttpRequest::HasCompleted() const
		{
			return m_isRequestComplete;
		}
		//----------------------------------------------------------------------------------------
		//----------------------------------------------------------------------------------------
		const HttpRequest::Desc& HttpRequest::GetDescription() const
		{
			return m_desc;
		}
		//----------------------------------------------------------------------------------------
		//----------------------------------------------------------------------------------------
		const std::string& HttpRequest::GetResponse() const
		{
			return m_responseData;
		}
		//----------------------------------------------------------------------------------------
		//----------------------------------------------------------------------------------------
		u32 HttpRequest::GetResponseCode() const
		{
			return m_responseCode;
		}
        //----------------------------------------------------------------------------------------
		//----------------------------------------------------------------------------------------
		u32 HttpRequest::GetBytesRead() const
		{
			//TODO: Implement this.
			return 0;
		}
	}
}
