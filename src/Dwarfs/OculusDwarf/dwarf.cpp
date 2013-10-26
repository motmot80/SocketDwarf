//
//  SocketDwarf++ - A multiplatform HTML 5 peripherals adapter
//  
//  Copyright (C) 2013 Thomas Stollenwerk
//  
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//  

//EARLY DRAFT

#include <string>
#include <OVR.h>
#include "../include/dwarf.hpp"

namespace {
    const char DEVICESTATE[]            = "devicestate";
    const char CONNECTED[]              = "connected";
    const char DATA[]                   = "data";
    const char UID[]                    = "uid";
    const char COMMAND[]                = "command";
    const char COMMAND_GETINFO[]        = "GetInfo";
    const char COMMAND_GETORIENTATION[] = "GetOrientation";

    OVR::Ptr<OVR::DeviceManager>    pManager;
    OVR::Ptr<OVR::HMDDevice>        pHMD;
    OVR::Ptr<OVR::SensorDevice>     pSensor;
    OVR::SensorFusion*              pFusionResult;

    void CheckInitSensor ()
    {
        if (!pSensor) {
            pSensor = pHMD ? *pHMD->GetSensor() : *pManager->EnumerateDevices<OVR::SensorDevice>().CreateDevice();
	        if (pSensor)
	        {
	           pFusionResult->AttachToSensor(pSensor);
	        }
        }
    }

    const std::string GetOrientation(int uid)
    {
        CheckInitSensor ();
        Json::Value responseRoot;
        responseRoot[UID] = uid;
        responseRoot[COMMAND] = COMMAND_GETORIENTATION;
        if (pHMD && pSensor) {
            OVR::Quatf quaternion = pFusionResult->GetOrientation();
		    float yaw, pitch, roll;
		    quaternion.GetEulerAngles<OVR::Axis_Y, OVR::Axis_X, OVR::Axis_Z>(&yaw, &pitch, &roll);
            responseRoot[DEVICESTATE][CONNECTED] = true;
            responseRoot[DATA]["yaw"] = yaw;
            responseRoot[DATA]["pitch"] = pitch;
            responseRoot[DATA]["roll"] = roll;
        }
        else {
            responseRoot[DEVICESTATE][CONNECTED] = false;
        }        
        std::ostringstream output;
        output << responseRoot;
        return output.str();
    }

    const std::string GetInfo (int uid)
    {
        Json::Value responseRoot;
        OVR::HMDInfo info;
        responseRoot[UID] = uid;
        responseRoot[COMMAND] = COMMAND_GETINFO;
        if (pHMD && pHMD->GetDeviceInfo(&info))
        {         
            responseRoot[DEVICESTATE][CONNECTED] = true;
            responseRoot[DATA]["DisplayDeviceName"]       = info.DisplayDeviceName;
            responseRoot[DATA]["ProductName"]             = info.ProductName;
            responseRoot[DATA]["Manufacturer"]            = info.Manufacturer;
            responseRoot[DATA]["Version"]                 = info.Version;
            responseRoot[DATA]["HResolution"]             = info.HResolution;
            responseRoot[DATA]["VResolution"]             = info.VResolution;
            responseRoot[DATA]["HScreenSize"]             = info.HScreenSize;
            responseRoot[DATA]["VScreenSize"]             = info.VScreenSize;
            responseRoot[DATA]["VScreenCenter"]           = info.VScreenCenter;
            responseRoot[DATA]["EyeToScreenDistance"]     = info.EyeToScreenDistance;
            responseRoot[DATA]["LensSeparationDistance"]  = info.LensSeparationDistance;
            responseRoot[DATA]["InterpupillaryDistance"]  = info.InterpupillaryDistance;
            responseRoot[DATA]["DistortionK_0"]           = info.DistortionK[0];
            responseRoot[DATA]["DistortionK_1"]           = info.DistortionK[1];
            responseRoot[DATA]["DistortionK_2"]           = info.DistortionK[2];
            responseRoot[DATA]["DistortionK_3"]           = info.DistortionK[3];            
        }
        else {
            responseRoot[DEVICESTATE][CONNECTED] = false;
        }        
        std::ostringstream output;
        output << responseRoot;
        return output.str();
    }

};

Export void Init ()
{
    OVR::System::Init();
    pFusionResult = new OVR::SensorFusion();
	pManager = *OVR::DeviceManager::Create();
    if (pManager) {
        pHMD = *pManager->EnumerateDevices<OVR::HMDDevice>().CreateDevice();
        CheckInitSensor();
    }
}

Export void Free ()
{
    if (pSensor)
    {
        pSensor.Clear();
    }
    if (pHMD)
    {
        pHMD.Clear();
    }
    if (pManager) 
    {
	    pManager.Clear();
    }
    if (pFusionResult)
    {
    	delete pFusionResult;
    }
    OVR::System::Destroy();
}

Export const std::string ProcessRequest (const std::string & data)
{
    Json::Value requestRoot;   
    Json::Reader requestReader;
    if (requestReader.parse(data, requestRoot)) {
        std::string command = requestRoot.get("command", std::string()).asString();
        int uid = requestRoot.get("uid", -1).asInt();
        if (uid != -1) 
        {
            if (command == COMMAND_GETORIENTATION)
            {
                return ::GetOrientation(uid);
            }
            else if (command == COMMAND_GETINFO)
            {
                return ::GetInfo(uid);
            }
        }
    }
    return std::string();
}

