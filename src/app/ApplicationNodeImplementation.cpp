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
        textureQuad_ = new FullscreenQuad("showTexture.frag", this);

        colorUniformLoc_ = glGetUniformLocation(colorQuad_->GetGPUProgram()->getProgramId(), "inputColor");
        textureUniformLoc_ = glGetUniformLocation(textureQuad_->GetGPUProgram()->getProgramId(), "inputTexture");

        tex_ = GetTextureManager().GetResource("/textures/cam colors inv.png");
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

            size_t nodeID = sgct_core::ClusterManager::instance()->getThisNodeId();
            size_t windowID = GetCurrentWindowID();
            size_t projectorID = 2 * (nodeID - 1) + windowID;

            glUseProgram(colorQuad_->GetGPUProgram()->getProgramId());

            glm::vec3 color = glm::vec3(syncInfoLocal_.colors_[projectorID].x, syncInfoLocal_.colors_[projectorID].y, syncInfoLocal_.colors_[projectorID].z);
            color = color * syncInfoLocal_.brightness_;
            color = glm::pow(color, glm::vec3(1.0f / 2.2f));

            if (!syncInfoLocal_.calibrateColor_) color = glm::vec3(1.0);

            glUniform3f(colorUniformLoc_, color.x, color.y, color.z);

            colorQuad_->Draw();


            /*glUseProgram(textureQuad_->GetGPUProgram()->getProgramId());

            glUniform1i(textureUniformLoc_, 0);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, tex_->getTextureId());

            textureQuad_->Draw();*/

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
