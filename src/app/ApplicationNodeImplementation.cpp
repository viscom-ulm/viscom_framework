/**
 * @file   ApplicationNodeImplementation.cpp
 * @author Sebastian Maisch <sebastian.maisch@uni-ulm.de>
 * @date   2016.11.30
 *
 * @brief  Implementation of the application node class.
 */

#include "ApplicationNodeImplementation.h"
#include "Vertices.h"
#include <imgui.h>
#include "core/gfx/mesh/MeshRenderable.h"
#include "core/gfx/mesh/AnimMeshRenderable.h"
#include <iostream>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>

namespace viscom {

    ApplicationNodeImplementation::ApplicationNodeImplementation(ApplicationNodeInternal* appNode) :
        ApplicationNodeBase{ appNode }
    {
        LOG(INFO) << "InitOpenGL called in AppNImpl";


        InitialiseVR();
        if (!InitialiseDisplayVR()) {
            CalibrateVR(ovr::CalibrateMethod::CALIBRATE_BY_POINTING);
        }

        colorQuad_ = new FullscreenQuad("showColor.frag", this);

        colorUniformLoc_ = glGetUniformLocation(colorQuad_->GetGPUProgram()->getProgramId(), "inputColor");
    }

    ApplicationNodeImplementation::~ApplicationNodeImplementation()
    {
    }

    void ApplicationNodeImplementation::UpdateFrame(double currentTime, double)
    {
    }

    void ApplicationNodeImplementation::ClearBuffer(FrameBuffer& fbo)
    {
        fbo.DrawToFBO([]() {
            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        });
    }

    void ApplicationNodeImplementation::DrawFrame(FrameBuffer& fbo)
    {
        fbo.DrawToFBO([this]() {

            glUseProgram(colorQuad_->GetGPUProgram()->getProgramId());

            glUniform3f(colorUniformLoc_, 1.0f, 1.0f, 1.0f);

            colorQuad_->Draw();

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
            glUseProgram(0);
        });
    }

#ifdef VISCOM_USE_SGCT
    void ApplicationNodeImplementation::UpdateSyncedInfo()
    {
        syncInfoLocal_ = syncInfoSynced_.getVal();
    }

     void ApplicationNodeImplementation::DecodeData()
     {
        sgct::SharedData::instance()->readObj(&syncInfoSynced_);
     }
#endif

}
