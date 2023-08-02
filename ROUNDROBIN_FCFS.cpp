#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>

using namespace std;

struct Process
{
    string name;
    int instructionTime;  //instructionTime เวลาในการประมวลผล
    int responseTime = 0; //Response ระยะเวลาในการรอ
    int terminatTime = 0; //terminat/timeout ระยะเวลาจบการประมวลผล
    int runningCounter = 1; // รอบในการรัน
};

//******************************************************************************************//

void FCFS(const vector<Process>& processes, ofstream& fcfs_schedule) {
    int totalInstructionTime = 0; //เวลาในการประมวลผลรวม
    fcfs_schedule << "FCFS Scheduling: \n" << endl;
    for (Process process : processes) {
        process.responseTime = totalInstructionTime;
        process.terminatTime = totalInstructionTime + process.instructionTime;
        totalInstructionTime = process.terminatTime;
        fcfs_schedule << "" << process.name << endl;
        fcfs_schedule << "RunningTime : " << process.responseTime << " - " << process.terminatTime - 1 << endl;
        fcfs_schedule << "ResponseTime : " << process.responseTime << endl;
        fcfs_schedule << endl;
    }
}

//******************************************************************************************//

void RRB(const vector<Process>& processes, int quantum, ofstream& rr_schedule) {
    int currentTime = 0; //เวลาในการรันปัจจุบัน

    queue<Process> readyQueue;
    rr_schedule << "Round Robin (Quantum time = " << quantum << ") :\n" << endl;

    for (const Process& process : processes) {
        readyQueue.push(process);
    }

    while (!readyQueue.empty()) {
        Process& currentProcess = readyQueue.front(); // ใช้การอ้างอิงต่อกระบวนการที่อยู่ในคิว

        rr_schedule << currentProcess.name << "  Remain : " << currentProcess.instructionTime << endl;
        rr_schedule << "RunningCounter : " << currentProcess.runningCounter << "\n";
        rr_schedule << "RunningTime : " << currentTime << " - "; //RunningTime - Terminate Time : 


        if (currentProcess.instructionTime <= quantum) {
            // ประมวลผล terminate

            if (currentProcess.runningCounter > 1)
            {
                currentProcess.responseTime = currentTime - ((currentProcess.runningCounter - 1) * quantum);
            }
            else
            {
                currentProcess.responseTime = currentTime;
            }
            
            currentTime += currentProcess.instructionTime;
            currentProcess.runningCounter++;
            rr_schedule << currentTime - 1 << "\n";
            rr_schedule << "ResponseTime : " << currentProcess.responseTime << "\n\n";

        }
        //ไม่เข้าเงื่อนไข
        else {
            if (currentProcess.runningCounter > 1)
            {
                currentProcess.responseTime = currentTime - ((currentProcess.runningCounter - 1) * quantum);
            }
            else
            {
                currentProcess.responseTime = currentTime;
            }

            currentTime += quantum;
            rr_schedule << currentTime - 1 << endl;
            //rr_schedule << currentTime - 1 << " (Timeout " << quantum << ")" << endl;
            currentProcess.instructionTime -= quantum;
            currentProcess.runningCounter++;
            readyQueue.push(currentProcess); // เพิ่มกระบวนการ (Process) กลับเข้าสู่คิวอีกครั้ง
            rr_schedule << "ResponseTime : " << currentProcess.responseTime << "\n";
            rr_schedule << "\n";

        }

        readyQueue.pop(); // ลบกระบวนการ (Process) ที่เสร็จสิ้นการทำงานออกจากคิว
    }
}


//******************************************************************************************//

int main() {
    vector<Process> processes;
    string filename = "processes.txt";
    int quantum = 10;

    ifstream inputFile(filename);
    if (inputFile.is_open()) {
        string line;
        while (getline(inputFile, line)) {
            string name;
            int instructionTime;
            stringstream ss(line);
            ss >> name >> instructionTime;
            processes.push_back({ name, instructionTime });
        }
    }

    //FCFS output File 
    ofstream fcfs_schedule("fcfs_schedule.txt");
    if (fcfs_schedule.is_open()) {
        FCFS(processes, fcfs_schedule); //เรียกใช้ Function FCFS
        fcfs_schedule.close();
        cout << "Output has been written to fcfs_schedule.txt" << endl;
    }
    else {
        cout << "Failed to open fcfs_schedule.txt" << endl;
    }

    //Round Robin output File
    ofstream rr_schedule("rr_schedule.txt");
    if (rr_schedule.is_open()) {
        RRB(processes, quantum, rr_schedule); //เรียกใช้ Function RRB
        rr_schedule.close();
        cout << "Output has been written to rr_schedule.txt" << endl;
    }
    else {
        cout << "Failed to open rr_schedule.txt" << endl;
    }
    return 0;
}

//6440204052
//6440201793
//6440205919
//6440203062
//6440202049




