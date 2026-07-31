/*  File    : pixhawk_sil_connector.cpp
 *  Abstract:
 *
 *  Simulink C++ S-function for software-in-the-loop (SIL) simulation with Pixhawk.
 *
 *  Copyright (c) 2023 Kiril Boychev
 */

#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>

#include "SILConnector.h"

#define S_FUNCTION_LEVEL 2
#define S_FUNCTION_NAME pixhawk_sil_connector

/*
 * Need to include simstruc.h for the definition of the SimStruct and
 * its associated macro definitions.
 */
#include "simstruc.h"

/*====================*
 * S-function methods *
 *====================*/

/* Function: mdlInitializeSizes ===============================================
 * Abstract:
 *    The sizes information is used by Simulink to determine the S-function
 *    block's characteristics (number of inputs, outputs, states, etc.).
 */
static void mdlInitializeSizes(SimStruct *S)
{
    ssSetNumSFcnParams(S, 0);

    if (ssGetNumSFcnParams(S) != ssGetSFcnParamsCount(S))
    {
        return;
    }

    ssSetSimStateCompliance(S, DISALLOW_SIM_STATE);
    ssSetNumContStates(S, 0);
    ssSetNumDiscStates(S, 1);

    if (!ssSetNumInputPorts(S, 10))
    {
        return;
    }

    ssSetInputPortWidth(S, 0, 1); // time (s)
    ssSetInputPortDirectFeedThrough(S, 0, 1);
    ssSetInputPortWidth(S, 1, 3); // A_measured (m/s^2)
    ssSetInputPortDirectFeedThrough(S, 1, 1);
    ssSetInputPortWidth(S, 2, 3); // omega_b_measured (rad/s)
    ssSetInputPortDirectFeedThrough(S, 2, 1);
    ssSetInputPortWidth(S, 3, 3); // B_measured (gauss)
    ssSetInputPortDirectFeedThrough(S, 3, 1);
    ssSetInputPortWidth(S, 4, 1); // P_measured (hPa)
    ssSetInputPortDirectFeedThrough(S, 4, 1);
    ssSetInputPortWidth(S, 5, 1); // T_measured (degC)
    ssSetInputPortDirectFeedThrough(S, 5, 1);
    ssSetInputPortWidth(S, 6, 1); // h_measured (m)
    ssSetInputPortDirectFeedThrough(S, 6, 1);
    ssSetInputPortWidth(S, 7, 1); // q_measured (hPa)
    ssSetInputPortDirectFeedThrough(S, 7, 1);
    ssSetInputPortWidth(S, 8, 12); // xyz_measured (-)
    ssSetInputPortDirectFeedThrough(S, 8, 1);
    // ssSetInputPortWidth(S, 9, 13); // rc_channels (-)
    // ssSetInputPortDirectFeedThrough(S, 9, 1);
    ssSetInputPortWidth(S, 9, 17); // ground_truth (-)
    ssSetInputPortDirectFeedThrough(S, 9, 1);

    if (!ssSetNumOutputPorts(S, 2)) 
    {
        return;
    }

    ssSetOutputPortWidth(S, 0, 16);
    ssSetOutputPortWidth(S, 1, 1);

    // ssSetNumSampleTimes(S, 1);
    // ssSetNumPWork(S, 1);
    // ssSetNumRWork(S, 0);
    // ssSetNumIWork(S, 0);
    // ssSetNumModes(S, 0);
    // ssSetNumNonsampledZCs(S, 0);
    // ssSetOptions(S, 0);
    ssSetNumSampleTimes(S, 1);
    ssSetNumPWork(S, 1);
    ssSetNumDWork(S, 0);
    ssSetNumIWork(S, 0);
    ssSetNumRWork(S, 1); // previous time
    ssSetNumModes(S, 0);
    ssSetNumNonsampledZCs(S, 0);
    ssSetOptions(S, 0);
}

/* Function: mdlInitializeSampleTimes =========================================
 * Abstract:
 *    This function is used to specify the sample time(s) for your
 *    S-function. You must register the same number of sample times as
 *    specified in ssSetNumSampleTimes.
 */
static void mdlInitializeSampleTimes(SimStruct *S)
{
    ssSetSampleTime(S, 0, 0.004);
    ssSetOffsetTime(S, 0, 0.0);
    ssSetModelReferenceSampleTimeDefaultInheritance(S);
}

#define MDL_START /* Change to #undef to remove function */
#if defined(MDL_START)
/* Function: mdlStart =======================================================
 * Abstract:
 *    This function is called once at start of model execution, for every
 *    instance of this block present in the compiled diagram -- including one
 *    sitting inside a disabled Enabled Subsystem (Enabled Subsystem semantics
 *    only gate mdlOutputs/per-step execution, not mdlStart/initialization).
 *    So the actual blocking TCP accept() is deliberately NOT done here
 *    anymore -- it's deferred to the first mdlOutputs call (see below), which
 *    DOES respect Enabled Subsystem gating. This lets the block be wrapped in
 *    an Enabled Subsystem (e.g. gated on a SITL/HITL runtime-mode switch)
 *    without it blocking the simulation waiting for a connection nobody
 *    wants in that run. mdlStart here just marks "not yet connected".
 */
static void mdlStart(SimStruct *S)
{
    ssSetPWorkValue(S, 0, nullptr);
}
#endif /*  MDL_START */

/* Function: mdlOutputs =======================================================
 * Abstract:
 *    In this function, you compute the outputs of your S-function
 *    block.
 */
static void mdlOutputs(SimStruct *S, int_T tid)
{

    if (ssIsSampleHit(S, 0, tid)){

        static std::string eStatus;
        // Consecutive-exception tolerance added 2026-07-30: every one of several live SITL runs
        // (see PLAN_INCORPORACION_AERONAVES_JSBSIM_SITL.md) halted cleanly ~90s into a healthy
        // flight -- PX4's own .ulg log shows nav_state still AUTO_MISSION, armed, failsafe=0, and
        // every failure_detector_status flag 0 right up to the last recorded sample, ruling out a
        // PX4-side fault. The only other thing that can stop this sim outright is this catch
        // block's ssSetErrorStatus call below, which previously fired on the very first exception
        // from send()/receive() -- including a single transient WSL2/loopback TCP hiccup that a
        // healthy connection would otherwise recover from next step. Tolerate a run of isolated
        // failures (logging each one) and only escalate to a hard stop once they're persistent,
        // i.e. the connection is actually gone rather than having blipped once.
        static int consecutiveFailures = 0;
        constexpr int kMaxConsecutiveFailures = 25;

        try
        {

            SILConnector *sil_connector = (SILConnector *)ssGetPWorkValue(S,0);

            if (sil_connector == nullptr)
            {
                // First real step for this instance -- connect now, lazily. Heap-allocated (not a
                // function-local static like the old mdlStart code) so each S-Function instance
                // owns its own connector, freed in mdlTerminate.
                sil_connector = new SILConnector("0.0.0.0", 4560);

                mexPrintf("Waiting for PX4 to connect on TCP port 4560...\n");

                sil_connector->open();

                mexPrintf("PX4 connected on TCP port 4560.\n");

                ssSetPWorkValue(S, 0, (void *)sil_connector);
            }

            InputRealPtrsType time = ssGetInputPortRealSignalPtrs(S, 0);
            InputRealPtrsType A_measured = ssGetInputPortRealSignalPtrs(S, 1);
            InputRealPtrsType omega_b_measured = ssGetInputPortRealSignalPtrs(S, 2);
            InputRealPtrsType B_measured = ssGetInputPortRealSignalPtrs(S, 3);
            InputRealPtrsType P_measured = ssGetInputPortRealSignalPtrs(S, 4);
            InputRealPtrsType T_measured = ssGetInputPortRealSignalPtrs(S, 5);
            InputRealPtrsType h_measured = ssGetInputPortRealSignalPtrs(S, 6);
            InputRealPtrsType q_measured = ssGetInputPortRealSignalPtrs(S, 7);
            InputRealPtrsType xyz_measured = ssGetInputPortRealSignalPtrs(S, 8);
            // InputRealPtrsType rc_channels = ssGetInputPortRealSignalPtrs(S, 9);
            InputRealPtrsType ground_truth = ssGetInputPortRealSignalPtrs(S, 9);

            uint64_t time_usec = (uint64_t)((*time[0]) * 1e6);

            SensorIMU imu;

            imu.xacc = (float)(*A_measured[0]);
            imu.yacc = (float)(*A_measured[1]);
            imu.zacc = (float)(*A_measured[2]);
            imu.xgyro = (float)(*omega_b_measured[0]);
            imu.ygyro = (float)(*omega_b_measured[1]);
            imu.zgyro = (float)(*omega_b_measured[2]);
            imu.xmag = (float)(*B_measured[0]);
            imu.ymag = (float)(*B_measured[1]);
            imu.zmag = (float)(*B_measured[2]);

            SensorAirData air_data;

            air_data.abs_pressure = (float)(*P_measured[0]);
            air_data.diff_pressure = (float)(*q_measured[0]);
            air_data.temperature = (float)(*T_measured[0]);

            SensorGPS gps;
            gps.fix_type = (uint8_t)(*xyz_measured[0]);
            gps.lat = (int32_t)(*xyz_measured[1]);
            gps.lon = (int32_t)(*xyz_measured[2]);
            gps.alt = (int32_t)(*xyz_measured[3]);
            // xyz_measured[4]/[5] are the gps.slx horizontal/vertical position accuracy in
            // meters (parameters.gps.horzPositionAccuracy_m/vertPositionAccuracy_m, e.g. 0.8/1.5).
            // HIL_GPS.eph/epv are uint16 in centimeters per PX4's own ingestion
            // (simulator_mavlink.cpp: "gps.eph = (float)hil_gps.eph * 1e-2f; // cm -> m") --
            // casting the raw meters value straight to uint16_t truncated 0.8 -> 0, telling
            // EKF2 the GPS position was exact (zero accuracy/error). Confirmed this fed into
            // gps_control.cpp's `pos_noise = max(gps_sample.hacc, EKF2_GPS_P_NOISE)` as hacc=0,
            // though the EKF2_GPS_P_NOISE floor (default 0.5m, not overridden by this airframe)
            // meant the practical effect was smaller than the raw eph=0 alone suggests. See
            // PLAN_INCORPORACION_AERONAVES_JSBSIM_SITL.md section 4.4 for the full reset
            // investigation this was found during.
            gps.eph = (uint16_t)(*xyz_measured[4] * 100.0f);
            gps.epv = (uint16_t)(*xyz_measured[5] * 100.0f);
            gps.vel = (uint16_t)std::floor(*xyz_measured[6]);
            gps.vn = (int16_t)std::floor(*xyz_measured[7]);
            gps.ve = (int16_t)std::floor(*xyz_measured[8]);
            gps.vd = (int16_t)std::floor(*xyz_measured[9]);
            gps.cog = (uint16_t)(*xyz_measured[10]);
            gps.satellites_visible = (uint8_t)(*xyz_measured[11]);

            SensorAltimeter altimeter;

            altimeter.current_distance = (uint16_t)(*h_measured[0]);

            // Inputs inputs;
            // for(unsigned int i=0;i<12;i++){
            //     inputs.channels[i]=(uint16_t)(*rc_channels[i]);
            // }
            // inputs.rssi = (uint8_t)(*rc_channels[12]);

            GroundTruth gt;

            gt.phi=(float)(*ground_truth[0]);
            gt.theta=(float)(*ground_truth[1]);
            gt.psi=(float)(*ground_truth[2]);
            gt.rollspeed=(float)(*ground_truth[3]);
            gt.pitchspeed=(float)(*ground_truth[4]);
            gt.yawspeed=(float)(*ground_truth[5]);
            gt.lat = (int32_t)(*ground_truth[6]);
            gt.lon = (int32_t)(*ground_truth[7]);
            gt.alt = (int32_t)(*ground_truth[8]);
            gt.vx = (int16_t)(*ground_truth[9]);
            gt.vy = (int16_t)(*ground_truth[10]);
            gt.vz = (int16_t)(*ground_truth[11]);
            gt.ind_airspeed = (uint16_t)(*ground_truth[12]);
            gt.true_airspeed = (uint16_t)(*ground_truth[13]);
            gt.xacc = (int16_t)(*ground_truth[14]);
            gt.yacc = (int16_t)(*ground_truth[15]);
            gt.zacc = (int16_t)(*ground_truth[16]);

            // sil_connector->send_sensors(
            //                     time_usec,
            //                     imu,
            //                     air_data,
            //                     gps,
            //                     altimeter,
            //                     inputs,
            //                     gt
            //                     );
            sil_connector->send_sensors(
                time_usec,
                imu,
                air_data,
                gps,
                altimeter,
                gt
            );


            sil_connector->read_tcp_socket();

            auto hil_actuator_controls = sil_connector->get_hil_actuator_controls();
            auto hil_actuator_mode = sil_connector->get_hil_actuator_mode();

            real_T *pwm = ssGetOutputPortRealSignal(S, 0);
            real_T *mode = ssGetOutputPortRealSignal(S, 1);


            for (auto i = 0; i < 16; i++)
            {
                pwm[i] = (real_T)hil_actuator_controls[i];
            }
            mode[0] = (real_T)hil_actuator_mode[0];

            consecutiveFailures = 0;

        }
        catch (const std::exception &e)
        {
            consecutiveFailures++;
            mexPrintf("pixhawk_sil_connector: step failed (%d/%d consecutive): %s\n",
                      consecutiveFailures, kMaxConsecutiveFailures, e.what());
            if (consecutiveFailures >= kMaxConsecutiveFailures)
            {
                eStatus = std::string(e.what());
                ssSetErrorStatus(S, eStatus.c_str());
            }
        }
    }
}

/* Function: mdlTerminate =====================================================
 * Abstract:
 *    In this function, you should perform any actions that are necessary
 *    at the termination of a simulation.  For example, if memory was
 *    allocated in mdlStart, this is the place to free it.
 */
static void mdlTerminate(SimStruct *S)
{
    SILConnector *sil_connector = (SILConnector *)ssGetPWorkValue(S,0);
    if(sil_connector){
        mexPrintf("Closing SILConnector...\n");
        sil_connector->close();
        delete sil_connector;
        ssSetPWorkValue(S, 0, nullptr);
    }
}
/*======================================================*
 * See sfuntmpl.doc for the optional S-function methods *
 *======================================================*/

/*=============================*
 * Required S-function trailer *
 *=============================*/

#ifdef MATLAB_MEX_FILE /* Is this file being compiled as a MEX-file? */
#include "simulink.c"  /* MEX-file interface mechanism */
#else
#include "cg_sfun.h" /* Code generation registration function */
#endif