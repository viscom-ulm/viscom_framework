/**
 * @file   CoordinatorNode.cpp
 * @author Sebastian Maisch <sebastian.maisch@uni-ulm.de>
 * @date   2016.11.25
 *
 * @brief  Implementation of the coordinator application node.
 */

#include "CoordinatorNode.h"
#include <imgui.h>

namespace viscom {

    CoordinatorNode::CoordinatorNode(ApplicationNodeInternal* appNode) :
        ApplicationNodeImplementation{ appNode }
    {
    }

    CoordinatorNode::~CoordinatorNode() = default;

    void CoordinatorNode::Draw2D(FrameBuffer& fbo)
    {
        fbo.DrawToFBO([]() {
            ImGui::ShowTestWindow();

            ImGui::SetNextWindowPos(ImVec2(60, 60), ImGuiSetCond_FirstUseEver);
            ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiSetCond_FirstUseEver);
            ImGui::StyleColorsDark();
            if (ImGui::Begin("Select input directory", nullptr, ImGuiWindowFlags_MenuBar))
            {
                ImGui::Text("Hello World on Master!");
            }
            ImGui::End();
        });

        ApplicationNodeImplementation::Draw2D(fbo);
    }

}
