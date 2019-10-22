/**
 * @file   CoordinatorNode.cpp
 * @author Sebastian Maisch <sebastian.maisch@uni-ulm.de>
 * @date   2016.11.25
 *
 * @brief  Implementation of the coordinator application node.
 */

#include "CoordinatorNode.h"
#include "core/open_gl.h"
#include <imgui.h>
#include <glm\gtc\matrix_transform.hpp>
#include <glm/gtc/random.hpp>
#include <fstream>
#include <string>

namespace viscom {

    CoordinatorNode::CoordinatorNode(ApplicationNodeInternal* appNode) :
        ApplicationNodeImplementation{ appNode }
    {
        initVr_ = InitialiseVR();
        InitialiseDisplayVR();

        // Read Color Correction File
        std::ifstream correctionFile;
        correctionFile.open("colorCorrection.txt");

        std::string line;
        if (std::getline(correctionFile, line))
        {
            syncInfoLocal_.brightness_ = float(std::atof(line.c_str()));
        }

        for (int i = 0; i < 12; i++) {
            
            if (std::getline(correctionFile, line))
            {
                float r = float(std::atof(line.substr(0, line.find(' ')).c_str()));
                line = line.substr(line.find(' ') + 1, line.length());
                float g = float(std::atof(line.substr(0, line.find(' ')).c_str()));
                line = line.substr(line.find(' ') + 1, line.length());
                float b = float(std::atof(line.c_str()));

                syncInfoLocal_.color_[i] = glm::vec3(r, g, b);
            }
        }

        correctionFile.close();
    }

    CoordinatorNode::~CoordinatorNode() = default;

    void CoordinatorNode::UpdateFrame(double currenttime, double elapsedtime) {
        connectedDevices_ = GetConnectedDevices();
        std::uint32_t controllerindex = 0;
        for (ovr::DeviceInfo d : connectedDevices_) {
            if (useLeftHandController&& d.deviceRole_ == ovr::TrackedDeviceRole::CONTROLLER_LEFT_HAND) {
                controllerindex = d.deviceId_;
            }
            if (!useLeftHandController && d.deviceRole_ == ovr::TrackedDeviceRole::CONTROLLER_RIGHT_HAND) {
                controllerindex = d.deviceId_;
            }
        }

        ApplicationNodeImplementation::UpdateFrame(currenttime, elapsedtime);

    }


#ifdef VISCOM_USE_SGCT
    void CoordinatorNode::PreSync()
    {
        syncInfoSynced_.setVal(syncInfoLocal_);
    }

    void CoordinatorNode::EncodeData()
    {
        sgct::SharedData::instance()->writeObj(&syncInfoSynced_);
    }
#endif

    void CoordinatorNode::Draw2D(FrameBuffer& fbo)
    {
        fbo.DrawToFBO([this]() {
            ImGui::ShowTestWindow();
            ImGui::SetNextWindowPos(ImVec2(60, 60), ImGuiSetCond_FirstUseEver);
            ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiSetCond_FirstUseEver);
            ImGui::StyleColorsClassic();
            if (ImGui::Begin("Coordinator GUI", nullptr))
            {

                if (ImGui::Button("Load Correction From Image Colors"))
                {
                    std::ifstream readColorsFile;
                    readColorsFile.open("colors_from_image.txt");

                    for (int i = 0; i < 12; i++) {

                        std::string line;
                        if (std::getline(readColorsFile, line))
                        {
                            int r = std::stoi(line.substr(0, line.find(' ')));
                            line = line.substr(line.find(' ') + 1, line.length());
                            int g = std::stoi(line.substr(0, line.find(' ')));
                            line = line.substr(line.find(' ') + 1, line.length());
                            int b = std::stoi(line);

                            syncInfoLocal_.color_[i] = glm::vec3(255.0 / r, 255.0 / g, 255.0 / b);
                        }
                    }

                    readColorsFile.close();
                }

                if (ImGui::Button("Save Color Correction"))
                {
                    std::ofstream correctionFile;
                    correctionFile.open("colorCorrection.txt");

                    correctionFile << syncInfoLocal_.brightness_ << "\n";
                    
                    for (int i = 0; i < 12; i++) {

                        correctionFile << syncInfoLocal_.color_[i].x << " ";
                        correctionFile << syncInfoLocal_.color_[i].y << " ";
                        correctionFile << syncInfoLocal_.color_[i].z << "\n";
                    }

                    correctionFile.close();
                }

                ImGui::Checkbox("Calibrate", &syncInfoLocal_.calibrateColor_);
                ImGui::Checkbox("Normalized", &normalizedColors_);

                if (normalizedColors_)
                {
                    ImGui::SliderFloat("Brightness", &syncInfoLocal_.brightness_, 0.0f, 1.0f);
                }
                else
                {
                    int br = int(syncInfoLocal_.brightness_ * 255.0f);

                    ImGui::InputInt("Brightness", &br);

                    syncInfoLocal_.brightness_ = float(br) / 255.0f;
                }

                for (int i = 0; i < 12; i++) {

                    std::string labelr = "Red " + std::to_string(i);
                    std::string labelg = "Green " + std::to_string(i);
                    std::string labelb = "Blue " + std::to_string(i);

                    // Color Slider
                    ImGui::Text("Projector %i", i);
                    if (normalizedColors_)
                    {
                        ImGui::SliderFloat(labelr.c_str(), &syncInfoLocal_.color_[(i + 2) % 12].x, 0.0f, 1.0f);
                        ImGui::SliderFloat(labelg.c_str(), &syncInfoLocal_.color_[(i + 2) % 12].y, 0.0f, 1.0f);
                        ImGui::SliderFloat(labelb.c_str(), &syncInfoLocal_.color_[(i + 2) % 12].z, 0.0f, 1.0f);
                    }
                    else
                    {
                        int cr = int(syncInfoLocal_.color_[(i + 2) % 12].x * 255.0f);
                        int cg = int(syncInfoLocal_.color_[(i + 2) % 12].y * 255.0f);
                        int cb = int(syncInfoLocal_.color_[(i + 2) % 12].z * 255.0f);

                        ImGui::InputInt(labelr.c_str(), &cr);
                        ImGui::InputInt(labelg.c_str(), &cg);
                        ImGui::InputInt(labelb.c_str(), &cb);

                        syncInfoLocal_.color_[(i + 2) % 12].x = float(cr) / 255.0f;
                        syncInfoLocal_.color_[(i + 2) % 12].y = float(cg) / 255.0f;
                        syncInfoLocal_.color_[(i + 2) % 12].z = float(cb) / 255.0f;
                    }
                }
            }
            ImGui::End();
        });
        ApplicationNodeImplementation::Draw2D(fbo);
    }
}