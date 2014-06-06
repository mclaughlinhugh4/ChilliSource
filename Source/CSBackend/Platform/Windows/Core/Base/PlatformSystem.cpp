//
//  PlatformSystem.cpp
//  Chilli Source
//  Created by Scott Downie on 24/11/2010.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2010 Tag Games Limited
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

#ifdef CS_TARGETPLATFORM_WINDOWS

#include <CSBackend/Platform/Windows/Core/Base/PlatformSystem.h>

#include <CSBackend/Rendering/OpenGL/Shader/GLSLShaderProvider.h>
#include <CSBackend/Rendering/OpenGL/Texture/TextureUnitSystem.h>
#include <ChilliSource/Core/Base/AppConfig.h>
#include <ChilliSource/Core/Base/Application.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

//As the opengl classes need to include glfw.h, they need to be included after windows.h to avoid macro redefinitions.
#include <CSBackend/Platform/Windows/GLFW/Base/GLFWManager.h>

namespace ChilliSource 
{
	namespace Windows
	{
		namespace
		{
			//This is global as LARGE_INTEGER is defined in windows.h. Including windows.h in 
			//PlatformSystem.h will cause compiler errors in FileSystem.h
			LARGE_INTEGER g_frequency;
		}

		CS_DEFINE_NAMEDTYPE(PlatformSystem);
		//-----------------------------------------
		//-----------------------------------------
		PlatformSystem::PlatformSystem() 
			: m_isSuspended(false), m_isFocused(false), m_appStartTime(0), m_appPreviousTime(0.0)
		{
			QueryPerformanceFrequency(&g_frequency);

			GLFWManager::Create();
			GLFWManager::Get()->Init(960, 640);

			//Register callbacks
			GLFWManager::Get()->SetWindowFocusDelegate((GLFWwindowfocusfun)&PlatformSystem::OnWindowFocusChanged);
			GLFWManager::Get()->SetWindowCloseDelegate((GLFWwindowclosefun)&PlatformSystem::OnWindowClosed);

			//Create the GL context
			GLenum GlewError = glewInit();
			if (GLEW_OK != GlewError)
			{
				//Problem: glewInit failed, something is seriously wrong.
				CS_LOG_FATAL("Glew Error On Init: " + std::string((const char*)glewGetErrorString(GlewError)));
			}
		}
		//--------------------------------------------------
		//--------------------------------------------------
		bool PlatformSystem::IsA(Core::InterfaceIDType in_interfaceId) const
		{
			return (Core::PlatformSystem::InterfaceID == in_interfaceId || PlatformSystem::InterfaceID == in_interfaceId);
		}
		//-------------------------------------------------
		//-------------------------------------------------
		void PlatformSystem::CreateDefaultSystems(Core::Application* in_application)
		{
			in_application->CreateSystem<OpenGL::GLSLShaderProvider>();
			in_application->CreateSystem<OpenGL::TextureUnitSystem>();
		}
		//-----------------------------------------
		//-----------------------------------------
		void PlatformSystem::OnResume()
		{
			m_isSuspended = false;
		}
		//-----------------------------------------
		//-----------------------------------------
		void PlatformSystem::OnForeground()
		{
			m_isFocused = true;
		}
		//-----------------------------------------
		//-----------------------------------------
		void PlatformSystem::Run()
		{
			GLFWManager::Get()->SetWindowTitle(Core::Application::Get()->GetAppConfig()->GetDisplayableName());

			Core::Application::Get()->Resume();
			Core::Application::Get()->Foreground();

			m_appStartTime = (u64)GLFWManager::Get()->GetTime();

			while (GLFWManager::Get() && GLFWManager::Get()->IsWindowAlive() == true)
			{
				if (!m_isSuspended)
				{
					GLFWManager::Get()->PollEvents();
				}
				if (!m_isSuspended)
				{
					f64 appCurrentTime = GLFWManager::Get()->GetTime();

					f32 deltaTime = (f32)(appCurrentTime - m_appPreviousTime);
					u64 uddwAppRunningTime = ((u64)m_appPreviousTime - m_appStartTime);
					m_appPreviousTime = appCurrentTime;

					Core::Application::Get()->Update(deltaTime, uddwAppRunningTime);
					Core::Application::Get()->Render();
				}
			}
		}
		//-----------------------------------------
		//-----------------------------------------
		void PlatformSystem::OnBackground()
		{
			m_isFocused = false;
		}
		//-----------------------------------------
		//-----------------------------------------
		void PlatformSystem::OnSuspend()
		{
			m_isSuspended = true;
		}
		//-------------------------------------------------
		//-------------------------------------------------
		void PlatformSystem::SetPreferredFPS(u32 in_fps)
		{
			GLFWManager::Get()->SetPreferredFPS(in_fps);
		}
		//--------------------------------------------
		//--------------------------------------------
		void PlatformSystem::Quit()
		{
			if(m_isFocused == true)
			{
				Core::Application::Get()->Background();
			}
			if(m_isSuspended == false)
			{
				Core::Application::Get()->Suspend();
			}

			Core::Application::Get()->Destroy();
		}
		//-------------------------------------------------
		//-------------------------------------------------
		std::string PlatformSystem::GetAppVersion() const
		{
			return ""; 
		}
		//--------------------------------------------------
		//--------------------------------------------------
		u64 PlatformSystem::GetSystemTimeMS() const
		{
			LARGE_INTEGER currentTime;
            QueryPerformanceCounter(&currentTime);
			return (u64)((currentTime.QuadPart) * 1000.0 / g_frequency.QuadPart);
		}
		//---GLFW Delegates
		//-------------------------------------------------
		//-------------------------------------------------
		void PlatformSystem::OnWindowClosed(GLFWwindow* in_window)
		{
			Core::Application::Get()->Quit();
		}
		//-------------------------------------------------
		//-------------------------------------------------
		void PlatformSystem::OnWindowFocusChanged(GLFWwindow* in_window, s32 in_isFocused)
		{
			if (in_isFocused)
			{
				Core::Application::Get()->Foreground();
			}
			else
			{
				Core::Application::Get()->Background();
			}
		}
		//-----------------------------------------
		//-----------------------------------------
		PlatformSystem::~PlatformSystem()
		{
			GLFWManager::Destroy();
		}
	}
}

#endif
