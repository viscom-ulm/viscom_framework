/**
 * @file   CoordinatorNode.h
 * @author Sebastian Maisch <sebastian.maisch@uni-ulm.de>
 * @date   2016.11.25
 *
 * @brief  Declaration of the ApplicationNodeImplementation for the coordinator node.
 */

#pragma once

#include "app/ApplicationNodeImplementation.h"
#include <openvr.h>

namespace viscom {

    class CoordinatorNode final : public ApplicationNodeImplementation
    {
    public:
        explicit CoordinatorNode(ApplicationNodeInternal* appNode);
        virtual ~CoordinatorNode() override;

        void Draw2D(FrameBuffer& fbo) override;
        void InitOpenGL() override;
        void CleanUp() override;

        void UpdateFrame(double currenttime, double) override;

        bool bAcquireTrackingDataByWaitingForVREvents = false;

    private:

        float posdx = 0.0f;
        float posdy = 0.0f;
        bool useLeftHandController = true;
        bool initVr_ = false;
        std::vector<DeviceInfo> connectedDevices_ = std::vector<DeviceInfo>();
        glm::vec2 displayPos = glm::vec2();
    };
}
