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

#include <string>
#include "../include/dwarf.hpp"
#include <sixense.h>


namespace {
    
    const char UID[]                              = "uid";
    const char COMMAND[]                          = "command";
    const char COMMAND_GETALLNEWESTDATA[]         = "GetAllNewestData";
    const char FIELD_DATA[]                       = "data";
    const char FIELD_BASEID[]                     = "baseid";   
    const char FIELD_BASES[]                      = "bases"; 
    const char FIELD_CONTROLLERS[]                = "controllers";  
    const char FIELD_POS[]                        = "pos";                
    const char FIELD_ROT_MAT[]                    = "rot_mat";
    const char FIELD_JOYSTICK_X[]                 = "joystick_x";
    const char FIELD_JOYSTICK_Y[]                 = "joystick_y";
    const char FIELD_TRIGGER[]                    = "trigger";
    const char FIELD_BUTTONS[]                    = "buttons";
    const char FIELD_SEQUENCE_NUMBER[]            = "sequence_number";
    const char FIELD_ROT_QUAT[]                   = "rot_quat";
    const char FIELD_FIRMWARE_REVISION[]          = "firmware_revision";
    const char FIELD_HARDWARE_REVISION[]          = "hardware_revision";
    const char FIELD_PACKET_TYPE[]                = "packet_type";
    const char FIELD_MAGNETIC_FREQUENCY[]         = "magnetic_frequency";
    const char FIELD_ENABLED[]                    = "enabled";
    const char FIELD_CONTROLLER_INDEX[]           = "controller_index";
    const char FIELD_IS_DOCKED[]                  = "is_docked";
    const char FIELD_WHICH_HAND[]                 = "which_hand";
    const char FIELD_HEMI_TRACKING_ENABLED[]      = "hemi_tracking_enabled";

    const int ControllerEnabledCount ()
    {
        int controllerCount = 0;
        for (int i = 0; i < sixenseGetMaxControllers(); i++)
            if (sixenseIsControllerEnabled(i))
                controllerCount++;
        return controllerCount;
    }

    const std::string GetAllNewestData(int uid)
    {
        Json::Value responseRoot;
        responseRoot[UID] = uid;
        responseRoot[COMMAND] = COMMAND_GETALLNEWESTDATA;
        for(int baseId = 0; baseId < sixenseGetMaxBases(); baseId++ ) {
            bool controllerFound = false;
            sixenseSetActiveBase(baseId);
            Json::Value baseRoot;
            baseRoot[FIELD_BASEID] = baseId;
            sixenseAllControllerData acd;
            sixenseGetAllNewestData(&acd);
            for (int controllerId = 0; controllerId < sixenseGetMaxControllers(); controllerId++)
            {
                if (sixenseIsControllerEnabled(controllerId)) {
                    Json::Value controllerRoot;
                    for (int posId = 0; posId < 3; posId++) {
                        Json::Value posValue;
                        posValue = acd.controllers[controllerId].pos[posId];
                        controllerRoot[FIELD_POS].append(posValue);
                    }
                    for (int rotMatXId = 0; rotMatXId < 3; rotMatXId++) {
                        Json::Value rotMatXValue;
                        for (int rotMatYId = 0; rotMatYId < 3; rotMatYId++) {
                            Json::Value rotMatYValue;
                            rotMatYValue = acd.controllers[controllerId].rot_mat[rotMatXId][rotMatYId];
                            rotMatXValue[FIELD_ROT_MAT].append(rotMatYValue);
                        }
                        controllerRoot[FIELD_ROT_MAT].append(rotMatXValue);
                    }
                    controllerRoot[FIELD_JOYSTICK_X] = acd.controllers[controllerId].joystick_x;
                    controllerRoot[FIELD_JOYSTICK_Y] = acd.controllers[controllerId].joystick_y;
                    controllerRoot[FIELD_TRIGGER] = acd.controllers[controllerId].trigger;
                    controllerRoot[FIELD_BUTTONS] = acd.controllers[controllerId].buttons;
                    controllerRoot[FIELD_SEQUENCE_NUMBER] = acd.controllers[controllerId].sequence_number;
                    for (int rotQuatId = 0; rotQuatId < 4; rotQuatId++) {
                        Json::Value rotQuatValue;
                        rotQuatValue = acd.controllers[controllerId].rot_quat[rotQuatId];
                        controllerRoot[FIELD_ROT_QUAT].append(rotQuatValue);
                    }
                    controllerRoot[FIELD_FIRMWARE_REVISION] = acd.controllers[controllerId].firmware_revision;
                    controllerRoot[FIELD_HARDWARE_REVISION] = acd.controllers[controllerId].hardware_revision;
                    controllerRoot[FIELD_PACKET_TYPE] = acd.controllers[controllerId].packet_type;
                    controllerRoot[FIELD_MAGNETIC_FREQUENCY] = acd.controllers[controllerId].magnetic_frequency;
                    controllerRoot[FIELD_ENABLED] = acd.controllers[controllerId].enabled;
                    controllerRoot[FIELD_CONTROLLER_INDEX] = acd.controllers[controllerId].controller_index;
                    controllerRoot[FIELD_IS_DOCKED] = acd.controllers[controllerId].is_docked;
                    controllerRoot[FIELD_WHICH_HAND] = acd.controllers[controllerId].which_hand;
                    controllerRoot[FIELD_HEMI_TRACKING_ENABLED] = acd.controllers[controllerId].hemi_tracking_enabled;
                    baseRoot [FIELD_CONTROLLERS].append(controllerRoot);                    
                    controllerFound = true;
                }
                if (controllerFound) {
                    responseRoot[FIELD_DATA][FIELD_BASES].append(baseRoot);
                }
            }
        }
        std::ostringstream output;
        output << responseRoot;
        return output.str();
    }
};

DWARF_INIT()
{
    sixenseInit();
}

DWARF_FREE()
{
    sixenseExit();
}

DWARF_PROCESSREQUEST(data)
{
    Json::Value requestRoot;   
    Json::Reader requestReader;
    if (requestReader.parse(data, requestRoot)) {
        std::string command = requestRoot.get(COMMAND, std::string()).asString();
        int uid = requestRoot.get(UID, -1).asInt();
        if (uid != -1) 
        {
            if (command == COMMAND_GETALLNEWESTDATA)
            {
                return ::GetAllNewestData(uid);
            }            
        }
    }
    return std::string();
}

DWARF_GETSTATE(currentState)
{
    int controllerCount = ControllerEnabledCount ();
    if (controllerCount > 0)
    {
        currentState.State = EDwarfState::DeviceReady; 
        currentState.StateDescription = "Hydra(s) connected [" + std::to_string(controllerCount) + "]"; 
    }
    else {
        currentState.State = EDwarfState::NoDevice; 
        currentState.StateDescription = "No hydra connected"; 
    }
}