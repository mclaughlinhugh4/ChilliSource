//
//  ConeParticleEmitter.cpp
//  Chilli Source
//  Created by Scott Downie on 02/11/2012.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2012 Tag Games Limited
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

#include <ChilliSource/Rendering/Particles/Emitters/ConeParticleEmitter.h>
#include <ChilliSource/Core/String/StringParser.h>
#include <ChilliSource/Core/Math/MathUtils.h>

namespace ChilliSource
{
    namespace Rendering
    {
        //-------------------------------------------------------------
        /// Constructor
        ///
        /// @param Param Dictionary
        //-------------------------------------------------------------
        ConeParticleEmitter::ConeParticleEmitter(const Core::ParamDictionary& inParams, const MaterialCSPtr &inpMaterial, ParticleComponent* inpComponent)
        :ParticleEmitter(inParams, inpMaterial, inpComponent)
        {
            std::string strTemp;
            
            f32 fRadius = 45.0f;
            //Radius
            if(inParams.TryGetValue("Radius", strTemp))
            {
                fRadius =Core::MathUtils::Clamp( Core::ParseF32(strTemp), 1.0f, 89.0f);
            }
            
            mfBoxArea = sinf(Core::MathUtils::DegToRad(fRadius));
            mfBoxHeight = cosf(Core::MathUtils::DegToRad(fRadius));
            
        }
        //-----------------------------------------------------
        /// Create
        ///
        /// Create a new emitter from the given parameters
        ///
        /// @param Param dictionary
        /// @param Material
        /// @param Owning system
        /// @return Ownership of point emitter
        //-----------------------------------------------------
        ParticleEmitterUPtr ConeParticleEmitter::Create(const Core::ParamDictionary& inParams, const MaterialCSPtr &inpMaterial, ParticleComponent* inpComponent)
        {
            return ParticleEmitterUPtr(new ConeParticleEmitter(inParams, inpMaterial, inpComponent));
        }
		//-----------------------------------------------------
		//-----------------------------------------------------
		void ConeParticleEmitter::Emit(Particle& in_particle)
        {
            f32 fBoxAreaSq = mfBoxArea * mfBoxArea;
            Core::Vector3 vDir;
            do
            {
                vDir.x = Core::MathUtils::RandomInRange(-mfBoxArea, mfBoxArea);
                vDir.z = Core::MathUtils::RandomInRange(-mfBoxArea, mfBoxArea);
            }while(fBoxAreaSq < (vDir.x *vDir.x + vDir.z *vDir.z));
            
            vDir.y = mfBoxHeight;
            in_particle.m_velocity = vDir;
			in_particle.m_velocity.Normalise();
            in_particle.m_velocity *= mfInitialVelocity;
        }
        
    }
}