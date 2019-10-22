/**
 * @file   ApplicationNodeImplementation.h
 * @author Sebastian Maisch <sebastian.maisch@uni-ulm.de>
 * @date   2016.11.30
 *
 * @brief  Declaration of the application node implementation common for coordinator and worker nodes.
 */

#pragma once

#include "core/app/ApplicationNodeBase.h"

namespace viscom {

    class MeshRenderable;
    class AnimMeshRenderable;
    class AnimationState;

    struct syncedInfo {
        glm::vec3 color_[12];
        float brightness_;
        bool calibrateColor_ = true;
    };

    class ApplicationNodeImplementation : public ApplicationNodeBase
    {
    public:
        explicit ApplicationNodeImplementation(ApplicationNodeInternal* appNode);
        ApplicationNodeImplementation(const ApplicationNodeImplementation&) = delete;
        ApplicationNodeImplementation(ApplicationNodeImplementation&&) = delete;
        ApplicationNodeImplementation& operator=(const ApplicationNodeImplementation&) = delete;
        ApplicationNodeImplementation& operator=(ApplicationNodeImplementation&&) = delete;
        virtual ~ApplicationNodeImplementation() override;

        virtual void UpdateFrame(double currentTime, double elapsedTime) override;
        virtual void ClearBuffer(FrameBuffer& fbo) override;
        virtual void DrawFrame(FrameBuffer& fbo) override;

#ifdef VISCOM_USE_SGCT
        void UpdateSyncedInfo() override;
        void DecodeData() override;
#endif


    private:
        FullscreenQuad* colorQuad_;
        FullscreenQuad* textureQuad_;

        GLint colorUniformLoc_;
        GLint textureUniformLoc_;

        std::shared_ptr<Texture> tex_;

        std::vector<ovr::DeviceInfo> connectedDevices_;

    protected:
        /** Holds the synchronized object (local). */
        syncedInfo syncInfoLocal_;

#ifdef VISCOM_USE_SGCT
        /** Holds the synchronized object (synced). */
        sgct::SharedObject<syncedInfo> syncInfoSynced_;
#endif
    };
}