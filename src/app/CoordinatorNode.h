/**
 * @file   CoordinatorNode.h
 * @author Sebastian Maisch <sebastian.maisch@uni-ulm.de>
 * @date   2016.11.25
 *
 * @brief  Declaration of the ApplicationNodeImplementation for the coordinator node.
 */

#pragma once

#include "app/ApplicationNodeImplementation.h"

namespace viscom {

    class CoordinatorNode final : public ApplicationNodeImplementation
    {
    public:
        explicit CoordinatorNode(ApplicationNodeInternal* appNode);
        virtual ~CoordinatorNode() override;

        void Draw2D(FrameBuffer& fbo) override;

        void UpdateFrame(double currenttime, double) override;
#ifdef VISCOM_USE_SGCT
        void PreSync() override;
        void EncodeData() override;
#endif // VISCOM_USE_SGCT

        bool bAcquireTrackingDataByWaitingForVREvents = false;

    private:

        bool useLeftHandController = true;
        bool initVr_ = false;
        std::vector<ovr::DeviceInfo> connectedDevices_ = std::vector<ovr::DeviceInfo>();
    };
}
