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

        for (int i = 0; i < 24; i++) {
            syncInfoLocal_.colors_[i] = glm::vec3(1.0);
        }

        // Read Color Correction File
        std::ifstream correctionFile;
        correctionFile.open(GetConfig().projectorColorData_);

        std::string line;
        if (std::getline(correctionFile, line))
        {
            syncInfoLocal_.brightness_ = float(std::atof(line.c_str()));
        }

        while(std::getline(correctionFile, line)){
            
            int projectorNo = int(std::atoi(line.substr(0, line.find(' ')).c_str()));
            line = line.substr(line.find(' ') + 1, line.length());
            float r = float(std::atof(line.substr(0, line.find(' ')).c_str()));
            line = line.substr(line.find(' ') + 1, line.length());
            float g = float(std::atof(line.substr(0, line.find(' ')).c_str()));
            line = line.substr(line.find(' ') + 1, line.length());
            float b = float(std::atof(line.c_str()));

            syncInfoLocal_.colors_[projectorNo] = glm::vec3(r, g, b);
            LOG(DBUG) << syncInfoLocal_.colors_[projectorNo].x * syncInfoLocal_.brightness_ << " " << syncInfoLocal_.colors_[projectorNo].y * syncInfoLocal_.brightness_ << " " << syncInfoLocal_.colors_[projectorNo].z * syncInfoLocal_.brightness_ << "\n";
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

                /*if (ImGui::Button("Load Correction From Image Colors"))
                {
                    float rmax = 0.0;
                    float gmax = 0.0;
                    float bmax = 0.0;

                    std::ifstream readColorsFile;
                    readColorsFile.open("colors_from_image2.txt");

                    for (int i = 0; i < 12; i++) {

                        std::string line;
                        if (std::getline(readColorsFile, line))
                        {
                            //Reading white point...
                            int wr = std::stoi(line.substr(0, line.find(' ')));
                            line = line.substr(line.find(' ') + 1, line.length());
                            int wg = std::stoi(line.substr(0, line.find(' ')));
                            line = line.substr(line.find(' ') + 1, line.length());
                            int wb = std::stoi(line.substr(0, line.find(' ')));
                            //...and black point
                            line = line.substr(line.find(' ') + 1, line.length());
                            int br = std::stoi(line.substr(0, line.find(' ')));
                            line = line.substr(line.find(' ') + 1, line.length());
                            int bg = std::stoi(line.substr(0, line.find(' ')));
                            line = line.substr(line.find(' ') + 1, line.length());
                            int bb = std::stoi(line);

                            //Linearizing white and black point
                            glm::vec3 whiteRGBlinear = glm::pow(glm::vec3(1.0f * wr / 255.0f, 1.0f * wg / 255.0f, 1.0f * wb / 255.0f), glm::vec3(2.2f));
                            glm::vec3 blackRGBlinear = glm::pow(glm::vec3(1.0f * br / 255.0f, 1.0f * bg / 255.0f, 1.0f * bb / 255.0f), glm::vec3(2.2f));

                            //Storing white minus black
                            syncInfoLocal_.colors_[i] = glm::vec3(whiteRGBlinear.x - blackRGBlinear.x, whiteRGBlinear.y - blackRGBlinear.y, whiteRGBlinear.z - blackRGBlinear.z);

                            //Storing max values for red, green and blue
                            if (syncInfoLocal_.colors_[i].x > rmax) rmax = syncInfoLocal_.colors_[i].x;
                            if (syncInfoLocal_.colors_[i].y > gmax) gmax = syncInfoLocal_.colors_[i].y;
                            if (syncInfoLocal_.colors_[i].z > bmax) bmax = syncInfoLocal_.colors_[i].z;
                        }
                    }

                    readColorsFile.close();

                    for (int i = 0; i < 12; i++) {

                        //Scaling the stored values with the max rgb values and inverting to get the final mask values
                        syncInfoLocal_.colors_[i].x = rmax / syncInfoLocal_.colors_[i].x;
                        syncInfoLocal_.colors_[i].y = gmax / syncInfoLocal_.colors_[i].y;
                        syncInfoLocal_.colors_[i].z = bmax / syncInfoLocal_.colors_[i].z;
                    }
                }*/

                if (ImGui::Button("Save Color Correction"))
                {
                    std::ofstream correctionFile;
                    correctionFile.open(GetConfig().projectorColorData_);

                    correctionFile << syncInfoLocal_.brightness_ << "\n";
                    
                    for (int slaveId = 1; slaveId < sgct_core::ClusterManager::instance()->getNumberOfNodes(); slaveId++) { // Starting with the first worker node
                        for (int windowId = 0; windowId < sgct_core::ClusterManager::instance()->getNodePtr(slaveId)->getNumberOfWindows(); windowId++) {

                            auto projectorNo = GetApplication()->GetFramework().GetGlobalProjectorId(slaveId, windowId);

                            correctionFile << projectorNo << " ";
                            correctionFile << syncInfoLocal_.colors_[projectorNo].x << " ";
                            correctionFile << syncInfoLocal_.colors_[projectorNo].y << " ";
                            correctionFile << syncInfoLocal_.colors_[projectorNo].z << "\n";
                        }
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

                for (int slaveId = 1; slaveId < sgct_core::ClusterManager::instance()->getNumberOfNodes(); slaveId++) { // Starting with the first worker node
                    for (int windowId = 0; windowId < sgct_core::ClusterManager::instance()->getNodePtr(slaveId)->getNumberOfWindows(); windowId++) {

                        auto projectorNo = GetApplication()->GetFramework().GetGlobalProjectorId(slaveId, windowId);

                        std::string labelr = "Red " + std::to_string(projectorNo);
                        std::string labelg = "Green " + std::to_string(projectorNo);
                        std::string labelb = "Blue " + std::to_string(projectorNo);

                        ImGui::Text("Node %i, Projector %i, GlobalProjectorId: %i", slaveId, windowId, projectorNo);
                        if (normalizedColors_)
                        {
                            ImGui::SliderFloat(labelr.c_str(), &syncInfoLocal_.colors_[projectorNo].x, 0.0f, 2.0f);
                            ImGui::SliderFloat(labelg.c_str(), &syncInfoLocal_.colors_[projectorNo].y, 0.0f, 2.0f);
                            ImGui::SliderFloat(labelb.c_str(), &syncInfoLocal_.colors_[projectorNo].z, 0.0f, 2.0f);
                        }
                        else
                        {
                            int cr = int(syncInfoLocal_.colors_[projectorNo].x * 255.0f);
                            int cg = int(syncInfoLocal_.colors_[projectorNo].y * 255.0f);
                            int cb = int(syncInfoLocal_.colors_[projectorNo].z * 255.0f);

                            ImGui::InputInt(labelr.c_str(), &cr);
                            ImGui::InputInt(labelg.c_str(), &cg);
                            ImGui::InputInt(labelb.c_str(), &cb);

                            syncInfoLocal_.colors_[projectorNo].x = float(cr) / 255.0f;
                            syncInfoLocal_.colors_[projectorNo].y = float(cg) / 255.0f;
                            syncInfoLocal_.colors_[projectorNo].z = float(cb) / 255.0f;
                        }
                    }
                }
            }
            ImGui::End();
        });
        ApplicationNodeImplementation::Draw2D(fbo);
    }
}