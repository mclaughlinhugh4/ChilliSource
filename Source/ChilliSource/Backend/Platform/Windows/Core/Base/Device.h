//
//  Device.h
//  Chilli Source
//  Created by Ian Copland on 24/04/2014.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2014 Tag Games Limited
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

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Backend/Platform/Windows/ForwardDeclarations.h>
#include <ChilliSource/Core/Base/Device.h>

namespace ChilliSource
{
    namespace Windows
    {
        //---------------------------------------------------------
        /// The Windows backend for the device system. This provides
        /// information on the specific Windows machine.
        ///
        /// @author I Copland
        //---------------------------------------------------------
        class Device final : public Core::Device
        {
        public:
            CS_DECLARE_NAMEDTYPE(Device);
            //-------------------------------------------------------
			/// Queries whether or not this system implements the
            /// interface with the given Id.
			///
            /// @author I Copland
            ///
			/// @param The interface Id.
			/// @param Whether system is of given type.
			//-------------------------------------------------------
			bool IsA(Core::InterfaceIDType in_interfaceId) const override;
            //---------------------------------------------------
            /// @author I Copland
            ///
            /// @return The device model name.
            //---------------------------------------------------
            const std::string& GetModel() override;
            //---------------------------------------------------
            /// @author I Copland
            ///
            /// @return The name of the device model type.
            //---------------------------------------------------
            const std::string& GetModelType() override;
            //---------------------------------------------------
            /// @author I Copland
            ///
            /// @return The name of the device manufacturer.
            //---------------------------------------------------
            const std::string& GetManufacturer() override;
            //---------------------------------------------------
            /// @author I Copland
            ///
            /// @return The locale code registered with the
            /// device.
            //---------------------------------------------------
            Core::Locale& GetLocale() override;
            //---------------------------------------------------
            /// @author I Copland
            ///
            /// @return The language the device is set to.
            //---------------------------------------------------
            Core::Locale& GetLanguage() override;
            //---------------------------------------------------
            /// @author I Copland
            ///
            /// @return The version of the operating system.
            //---------------------------------------------------
            const std::string& GetOSVersion() override;
            //---------------------------------------------------
            /// @author I Copland
            ///
            /// @return An identifier that can be used to uniquely
            /// identify the device.
            //---------------------------------------------------
            const std::string& GetUDID() override;
			//---------------------------------------------------
			/// @author I Copland
			///
			/// @return The number of CPU cores available on the
            /// device.
			//--------------------------------------------------
			u32 GetNumberOfCPUCores() override;
        private:
            friend Core::DeviceUPtr Core::Device::Create();
            //----------------------------------------------------
			/// Constructor. Declared private to force the use of
            /// the factory method.
            ///
            /// @author I Copland
			//----------------------------------------------------
			Device();
            
            std::string m_model;
            std::string m_modelType;
            std::string m_manufacturer;
            std::string m_udid;
            Core::Locale m_locale;
            Core::Locale m_language;
            std::string m_osVersion;
			u32 m_numCPUCores;
        };
    }
}