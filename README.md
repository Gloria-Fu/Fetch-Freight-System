# Fetch-and-Freight-Robots-Scheduling-Simulation-System
We develop a simulation platform with hundreds of fetch and freight robots to evaluate the sechduling algorithms. 

## File list
* CET: The back-end code for the simulation platform with C++ (With the effective approaches we proposed in our paper). The parameter.txt in CET is the input parameter setting, the parameter can be changed according to different simulation scenarios. The folder tasks and goods contains the input goods information and tasks information for 500 rounds of experiments.
* Goods Information: The given goods storage information for three simulation scenarios.
* Tasks Information: The given tasks storage information for three simulation scenarios.
* Interface: Display the simulation platform.
* Video: The video contains the simulation and demo system.
* Results in paper: The total results of the simulations we proposed in the paper.

## How to run the CET progorm
* Windows: Use Visual Studio 2019 to open the CET/FetchandFreight.sln, then press F5 to run the program.
* Linux: Under floder CET, run the command: bash solve.sh

## How to run the draw progorm
* Windows: Use IDEA 2020 to open the project, change the path of map.txt in function drawRobot(), the path of news.txt in function showInfo(), the path of news2.txt in function showInfo2() and the path of robots.txt in function drawRobots() as the path of the currently running back-end program, then start running the program.




