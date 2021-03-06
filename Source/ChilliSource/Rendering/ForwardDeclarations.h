//
//  ForwardDeclarations.h
//  Chilli Source
//  Created by Ian Copland on 26/02/2014.
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

#ifndef _CHILLISOURCE_RENDERING_FORWARDDECLARATIONS_H_
#define _CHILLISOURCE_RENDERING_FORWARDDECLARATIONS_H_

#include <ChilliSource/Core/Base/StandardMacros.h>

#include <memory>

namespace ChilliSource
{
	namespace Rendering
	{
        //------------------------------------------------------------
        /// Base
        //------------------------------------------------------------
        CS_FORWARDDECLARE_CLASS(CanvasRenderer);
        CS_FORWARDDECLARE_CLASS(ICullingPredicate);
        CS_FORWARDDECLARE_CLASS(ViewportCullPredicate);
        CS_FORWARDDECLARE_CLASS(FrustumCullPredicate);
        CS_FORWARDDECLARE_CLASS(MeshBatch);
        CS_FORWARDDECLARE_CLASS(MeshBuffer);
        CS_FORWARDDECLARE_CLASS(RenderCapabilities);
        CS_FORWARDDECLARE_CLASS(RenderComponent);
        CS_FORWARDDECLARE_CLASS(RenderComponentFactory);
        CS_FORWARDDECLARE_CLASS(Renderer);
        CS_FORWARDDECLARE_CLASS(RendererSortPredicate);
        CS_FORWARDDECLARE_CLASS(NullSortPredicate);
        CS_FORWARDDECLARE_CLASS(BackToFrontSortPredicate);
        CS_FORWARDDECLARE_CLASS(MaterialSortPredicate);
        CS_FORWARDDECLARE_CLASS(RenderSystem);
        CS_FORWARDDECLARE_CLASS(RenderTarget);
        CS_FORWARDDECLARE_CLASS(VertexDeclaration);
        enum class ShaderPass;
        enum class BlendMode;
        enum class CullFace;
        enum class DepthTestComparison;
        enum class SurfaceFormat;
        //------------------------------------------------------------
        /// Camera
        //------------------------------------------------------------
        CS_FORWARDDECLARE_CLASS(CameraComponent);
        CS_FORWARDDECLARE_CLASS(OrthographicCameraComponent);
        CS_FORWARDDECLARE_CLASS(PerspectiveCameraComponent);
        //------------------------------------------------------------
        /// Font
        //------------------------------------------------------------
        CS_FORWARDDECLARE_CLASS(Font);
        CS_FORWARDDECLARE_CLASS(FontProvider);
        //------------------------------------------------------------
        /// Lighting
        //------------------------------------------------------------
        CS_FORWARDDECLARE_CLASS(AmbientLightComponent);
        CS_FORWARDDECLARE_CLASS(DirectionalLightComponent);
        CS_FORWARDDECLARE_CLASS(LightComponent);
        CS_FORWARDDECLARE_CLASS(PointLightComponent);
        //------------------------------------------------------------
        /// Material
        //------------------------------------------------------------
        CS_FORWARDDECLARE_CLASS(Material);
        CS_FORWARDDECLARE_CLASS(MaterialFactory);
        CS_FORWARDDECLARE_CLASS(MaterialProvider);
        //------------------------------------------------------------
        /// Model
        //------------------------------------------------------------
        CS_FORWARDDECLARE_CLASS(AnimatedMeshComponent);
        CS_FORWARDDECLARE_CLASS(Mesh);
        CS_FORWARDDECLARE_STRUCT(MeshFeatureDescriptor);
        CS_FORWARDDECLARE_STRUCT(SubMeshDescriptor);
        CS_FORWARDDECLARE_STRUCT(MeshDescriptor);
        CS_FORWARDDECLARE_STRUCT(SkeletonDescriptor);
        CS_FORWARDDECLARE_CLASS(CSAnimProvider);
        CS_FORWARDDECLARE_CLASS(CSModelProvider);
        CS_FORWARDDECLARE_CLASS(Skeleton);
        CS_FORWARDDECLARE_CLASS(SkinnedAnimation);
        CS_FORWARDDECLARE_STRUCT(SkeletonNode);
        CS_FORWARDDECLARE_CLASS(SkinnedAnimationGroup);
        CS_FORWARDDECLARE_CLASS(StaticMeshComponent);
        CS_FORWARDDECLARE_CLASS(SubMesh);
        //------------------------------------------------------------
        /// Particles
        //------------------------------------------------------------
        CS_FORWARDDECLARE_STRUCT(Particle);
        CS_FORWARDDECLARE_CLASS(ParticleEffect);
        CS_FORWARDDECLARE_CLASS(CSParticleEffectProvider);
        CS_FORWARDDECLARE_CLASS(ParticleComponent);
        CS_FORWARDDECLARE_CLASS(ParticleSystem);
        CS_FORWARDDECLARE_CLASS(ColourChangerParticleAffector);
        CS_FORWARDDECLARE_CLASS(LinearForceParticleAffector);
        CS_FORWARDDECLARE_CLASS(ParticleAffector);
        CS_FORWARDDECLARE_CLASS(ParticleAffectorFactory);
        CS_FORWARDDECLARE_CLASS(ScalerParticleAffector);
        CS_FORWARDDECLARE_CLASS(SpinnerParticleAffector);
        CS_FORWARDDECLARE_CLASS(ConeParticleEmitter);
        CS_FORWARDDECLARE_CLASS(ParticleEmitter);
        CS_FORWARDDECLARE_CLASS(ParticleEmitterFactory);
        CS_FORWARDDECLARE_CLASS(PointParticleEmitter);
        CS_FORWARDDECLARE_CLASS(RingParticleEmitter);
        //------------------------------------------------------------
        /// Shader
        //------------------------------------------------------------
        CS_FORWARDDECLARE_CLASS(Shader);
        //------------------------------------------------------------
        /// Sprite
        //------------------------------------------------------------
        CS_FORWARDDECLARE_CLASS(DynamicSpriteBatch);
        CS_FORWARDDECLARE_CLASS(SpriteBatch);
        CS_FORWARDDECLARE_CLASS(SpriteComponent);
        //------------------------------------------------------------
        /// Texture
        //------------------------------------------------------------
        CS_FORWARDDECLARE_CLASS(Cubemap);
        CS_FORWARDDECLARE_CLASS(Texture);
        CS_FORWARDDECLARE_CLASS(TextureAtlas);
        CS_FORWARDDECLARE_CLASS(TextureAtlasProvider);
        CS_FORWARDDECLARE_CLASS(TextureProvider);
        CS_FORWARDDECLARE_CLASS(CubemapProvider);
        CS_FORWARDDECLARE_CLASS(UVs);
	}
}

#endif
