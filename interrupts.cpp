/**
 *
 * @file interrupts.cpp
 * @author Chris Canton 101000259
 * @author Joshua Nicholls 101211810
 * @author Template By: Sasisekhar Govind
 *
 */

#include "interrupts.hpp"

int main(int argc, char** argv) {

    //vectors is a C++ std::vector of strings that contain the address of the ISR
    //delays  is a C++ std::vector of ints that contain the delays of each device
    //the index of these elemens is the device number, starting from 0
    auto [vectors, delays] = parse_args(argc, argv);
    std::ifstream input_file(argv[1]);

    std::string trace;      //!< string to store single line of trace file
    std::string execution;  //!< string to accumulate the execution output

    /******************ADD YOUR VARIABLES HERE*************************/
    
    int currentTime = 0;
    int isrDuration = 30;
    int contextSaveTime = 25;
    int ioDeltaT = 0;
    int syscallDeltaT = 0;
    std::string space = " ";
    std::string comma = ", ";
    std::string CPU = "CPU";
    int totalTime = 0;
    int tempTime = 0;
    
    

    
    
    /******************************************************************/

    //parse each line of the input trace file
    while(std::getline(input_file, trace)) {
        auto [activity, duration_intr] = parse_trace(trace);
    
    /******************ADD YOUR SIMULATION CODE HERE*************************/
    if(activity == CPU){
    
    execution +=  std::to_string(currentTime) + comma + std::to_string(duration_intr) + ", CPU Burst.\n";
    currentTime += duration_intr;
    
    
    
    }

    if(activity == "SYSCALL"){
    //Main interupt sequence. 
    
    execution += intr_boilerplate(currentTime,duration_intr,contextSaveTime,vectors).first;
    tempTime = currentTime;
    currentTime = intr_boilerplate(currentTime,duration_intr,contextSaveTime,vectors).second;
    
    

    //Executing an ISR. Note 40 used for duration (isrDuration) of ISR to match lab manual specifications but could just as easily use a lookup toble to set differnt durations for differnt syscalls.
    execution += std::to_string(currentTime) + comma + std::to_string(isrDuration) + comma + "SYSCALL: run ISR (Device Driver) run. \n";
    currentTime += isrDuration;
    
    execution += std::to_string(currentTime) + comma + std::to_string(isrDuration) + comma + "Transfer data from device to memory\n";
    currentTime += isrDuration;
    

    syscallDeltaT = delays[duration_intr] - 2*(isrDuration);

    execution += std::to_string(currentTime) + comma + std::to_string(syscallDeltaT) + comma + "Check for errors.\n";
    currentTime += syscallDeltaT;
    




    
    
    //Calling IRET.
   // execution += std::to_string(currentTime) + comma + std::to_string(1) + comma + "IRET \n";
    //currentTime++ ;

    
    
    
    
    }
    //Ends IO operation adding time from device table.
    if(activity == "END_IO"){
    execution += intr_boilerplate(currentTime,duration_intr,contextSaveTime,vectors).first;
    tempTime = currentTime;
    currentTime = intr_boilerplate(currentTime,duration_intr,contextSaveTime,vectors).second;
    
    
    execution += std::to_string((currentTime)) + comma + std::to_string(isrDuration) + " ENDIO: run the ISR (Device Driver). " + std::to_string(duration_intr) + "\n";
    currentTime += isrDuration;
    

    ioDeltaT = delays[duration_intr] - isrDuration;

    execution += std::to_string((currentTime)) + comma + std::to_string(ioDeltaT) + " Check Device " + std::to_string(duration_intr) + " status.\n";
    currentTime += ioDeltaT;
    



   
    }
    /************************************************************************/
    
    
    }
 //  execution += "\n Overhead Time:" + std::to_string(overHeadTime) + "/n" + "Cpu Time: "+std::to_string(cpuTime) + "\n" + "Total Time: " + std::to_string(overHeadTime + cpuTime);

    input_file.close();

    write_output(execution);
    
    return 0;
}
