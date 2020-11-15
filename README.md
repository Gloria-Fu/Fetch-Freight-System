# Fetch-and-Freight-Robots-Scheduling-Simulation-System
We develop a simulation platform with hundreds of fetch and freight robots to evaluate the sechduling algorithms. 

## File list
* CET: The back-end code for the simulation platform with C++ (With the effective approaches we proposed in our paper). 
  The parameter.txt in CET is the input parameter setting, the parameter can be changed according to different simulation scenarios. The folder tasks and goods contains the input   goods information and tasks information for 500 rounds of experiments.
* Var-T1, Var-T2, Var-T3, Var-E1, Var_E2, Var-E3, Var-C1, Var-C2 are the variants of CET, correspond to eight varients in our paper.
* Settings in Three Senarios: In our paper, we test the performance of CET and the eight variants in three differents senarios. 3X3 represents the column goods area number is 3 and the row goods area number is 3. The CET and eight variants here take the 6X6 senario, if you want to test the algorithm in other serarios, you can take the files under this folder to replace the files in CET and other variants.
* Goods Information: The given goods storage information for three simulation scenarios.
* Tasks Information: The given tasks storage information for three simulation scenarios.
* Interface: Display the simulation platform.
* Video: The video contains the simulation and demo system.
* Results in paper: The total results of the simulations we proposed in the paper.

## How to run the CET program
* Windows: Use Visual Studio 2019 to open the CET/FetchandFreight.sln, then press F5 to run the program.
* Linux: Under folder CET, run the command: bash solve.sh

## How to run the draw program
* Windows: Use IDEA 2020 to open the project, change the path of map.txt in function drawRobot(), the path of news.txt in function showInfo(), the path of news2.txt in function showInfo2() and the path of robots.txt in function drawRobots() as the path of the currently running back-end program, then start running the program.

## Input of the simulation platform
* Given task information of 500 batches simulations.
* Given goods information of 500 batches simulations.
* Define the related parameters in Parameters.txt, including the 1.Map Width 2.Map Height 3.Goods Area Column Number 4.Goods Area Row Number 5.Total Goods Area Number 6.Goods Type Number 7.Fetch Robot Number 8.Freight Robot Number 9.Conveyor number

## Output of the simulation platform
* The output results will be stored in the folder results under CET. The last line of numbers is the makespan.  

