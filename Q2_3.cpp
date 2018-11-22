#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <chrono> //For timing the code.
#include <iomanip>

unsigned int fateTester(unsigned int odds) {
    return static_cast<unsigned int>(ceil(static_cast<double>(rand()) / (static_cast<unsigned long>(RAND_MAX)) * odds));
}


class Person {
public:
    /** Initialises the class instance manually so that the number of initially infected can be changed.
     * @param cond
     * Conditions:
     * 0 - Healthy, never been infected.
     * 1 - Infected.
     * 2 - Immune.
     * 3 - Dead.
     */
    void genesis(const bool cond){
        condition = cond;
    }

    /**
     * Simulates a day of illness for a sick person. Each time this method is called the Person instance has a 12%
     * chance of recovering and a 1% chance of dying.
     */
    void sickDay(){
        auto score = static_cast<__int8_t>(fateTester(100));
        if(score <= 12){
            condition = 2;
        }
        else if(score >= 100){
            condition = 3;
        }
    };

    /**
     * Returns the condition of the Person class instance.
     * @return class instances condition.
     */
    char getCond(){
        return condition;
    }

    /**
     * Simulates meeting an infected person. Each time this method is called the Person instance has a 50% chance
     * of catching the illness.
     */
    void meetInfected(){
        if(condition == 0){
            auto infectionScore = static_cast<__int8_t>(fateTester(2));
            if(infectionScore <= 1){
                condition = 1;
            }
        }
    };
private:
    char condition;
};


/**
 *
 * @param debug. If debug is true then checks are made that the counting of different Person conditions is being done
 * correctly.
 * @return
 */
int simulate(const bool debug = false){
    srand(static_cast<unsigned int>(time(nullptr)));


    static const int maxPop = 200000;
    static const int runLength = 365;
    static const int meetingsPerDay = 50000;
    std::vector<Person> population;



    static auto startOne = std::chrono::high_resolution_clock::now();
    /**--------------------------------------------------------------------------------------------------------------*/
    /**--------------------------------------------------------------------------------------------------------------*/
    /**
     * INITIAL LOOP. GENERATING THE POPULATION.
     */
    for(unsigned int i = 1; i <= maxPop; i++){
        {
            Person person{};
            person.genesis(i <= 100);
            population.push_back(person);
        }
    }
    /**
     * END OF INITIAL LOOP - POPULATION GENERATION.
     */
    /**--------------------------------------------------------------------------------------------------------------*/
    static auto finishOne = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsedOne = finishOne - startOne;
    std::cout << "\nTime taken to generate population:\t\t" <<  std::fixed << std::setprecision(3) << elapsedOne.count() << " s" << std::endl;


    std::ofstream brains;
    brains.open("Pandemic.csv");
    brains << "Day,Infected,Uninfected,Immune,Alive,Dead\r";
    /**--------------------------------------------------------------------------------------------------------------*/
    /**--------------------------------------------------------------------------------------------------------------*/
    /**
     * MAIN LOOP. BEGINNING OF YEAR SIMULATION.
     */
    double loopThreeTotal = 0;
    static auto startTwo = std::chrono::high_resolution_clock::now();
    unsigned int sickCount = 0;
    unsigned int neverInfected = 0;
    unsigned int recoveredCount = 0;
    unsigned int aliveCount = 0;
    unsigned int deadCount = 0;


    for(unsigned int n = 1; n <= runLength; n++){

        sickCount = 0;
        neverInfected = 0;
        recoveredCount = 0;
        aliveCount = 0;
        deadCount = 0;


        /**--------------------------------------------------------------------------------------------------------------*/
        /**--------------------------------------------------------------------------------------------------------------*/
        /**
         * LOOP ONE. BEGINNING OF DAILY MEETINGS.
         */
        auto startThree = std::chrono::high_resolution_clock::now();
        for(unsigned int j = 1; j <= meetingsPerDay; j++) {

            unsigned int personOneSeed = fateTester(maxPop);
            unsigned int personTwoSeed = fateTester(maxPop);
            if(personOneSeed == personTwoSeed){
                j--;
                continue;
            }

            Person& personOne = population[personOneSeed];
            Person& personTwo = population[personTwoSeed];
            char personOneCondition = personOne.getCond();
            char personTwoCondition = personTwo.getCond();
            char jointCondition = static_cast<char>(personOneCondition + 10 * personTwoCondition);
            switch (jointCondition){
                case 10: {
                    personOne.meetInfected();
                    goto exitLoop;
                }

                case 1: {
                    personTwo.meetInfected();
                    goto exitLoop;
                }

                case 3: {
                    j--;
                    goto exitLoop;
                }

                case 13: {
                    j--;
                    goto exitLoop;
                }

                case 23: {
                    j--;
                    goto exitLoop;
                }

                case 30: {
                    j--;
                    goto exitLoop;
                }

                case 31: {
                    j--;
                    goto exitLoop;
                }

                case 32: {
                    j--;
                    goto exitLoop;
                }

                case 33: {
                    j--;
                    goto exitLoop;
                }

                default: goto exitLoop;
            }
            exitLoop: ;
        }
        auto finishThree = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsedThree = finishThree - startThree;
        loopThreeTotal = loopThreeTotal + elapsedThree.count();
        /**
         * END OF LOOP ONE - DAILY MEETINGS.
         */
        /**--------------------------------------------------------------------------------------------------------------*/



        /**--------------------------------------------------------------------------------------------------------------*/
        /**--------------------------------------------------------------------------------------------------------------*/
        /**
         * LOOP TWO. BEGINNING OF POPULATION CHECK AND SICK DAYS.
         */
        for (auto &it : population) {
            switch(it.getCond()){
                case 0:{
                    aliveCount++;
                    goto exit_loop2;
                }
                case 3:{
                    goto exit_loop2;
                }
                case 1:{
                    aliveCount++;
                    sickCount++;
                    it.sickDay();
                    goto exit_loop2;
                }
                case 2:{
                    aliveCount++;
                    recoveredCount++;
                    goto exit_loop2;
                }
                default: goto exit_loop2;
            }
            exit_loop2: ;
        }
        /**
         * END OF LOOP TWO - POPULATION CHECK AND SICK DAYS.
         */
        /**--------------------------------------------------------------------------------------------------------------*/

        deadCount = maxPop - aliveCount;
        neverInfected = maxPop - sickCount - deadCount - recoveredCount;
        if(debug){
            unsigned int totalOne = aliveCount+deadCount;
            unsigned int totalTwo = sickCount+neverInfected+recoveredCount+deadCount;
            if((totalOne != maxPop) or (totalTwo != maxPop)){
                std::cout << "ERROR: Sum of persons in each possible state does not equal total population." << std::endl;
                std::cout << "Some people are either being counted multiple times or not being counted at all." << std::endl;
                return -1;
            }
        }
        brains << n << "," << sickCount << "," << neverInfected << "," << recoveredCount << "," << aliveCount << "," << deadCount << "," << "\r" << std::endl;
    }
    /**
     * END OF MAIN LOOP - YEAR SIMULATION.
     */
    /**--------------------------------------------------------------------------------------------------------------*/
    brains.close();

    static const double meanLoopThreeTime = loopThreeTotal/runLength;
    static const auto finishTwo = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsedTwo = finishTwo - startTwo;
    /**--------------------------------------------------------------------------------------------------------------*/

    std::cout << "Entire year simulation took:\t\t\t" <<  std::fixed << std::setprecision(3) << elapsedTwo.count() << " s " << std::endl;
    std::cout << "All meetings took:\t\t\t\t" <<  std::fixed << std::setprecision(3) << loopThreeTotal << " s " << std::endl;
    std::cout << "Mean duration of day's meetings:\t\t" <<  std::fixed << std::setprecision(3) << meanLoopThreeTime << " s " << std::endl;
    std::cout << "All population scan took:\t\t\t" <<  std::fixed << std::setprecision(3) << elapsedTwo.count()-loopThreeTotal << " s " << std::endl;

    std::cout << "\n\nPercentage of population in different conditions:" << std::endl;
    std::cout << "Sick:\t\t\t" << std::fixed << std::setprecision(2) <<  sickCount/2000. << "%" << std::endl;
    std::cout << "Alive:\t\t\t" << std::fixed << std::setprecision(2) <<  aliveCount/2000. << "%" << std::endl;
    std::cout << "Dead:\t\t\t" << std::fixed << std::setprecision(2) <<  deadCount/2000. << "%" << std::endl;
    std::cout << "Immune:\t\t\t" << std::fixed << std::setprecision(2) <<  recoveredCount/2000. << "%" << std::endl;
    std::cout << "Never infected:\t\t" << std::fixed << std::setprecision(2) <<  neverInfected/2000. << "%\n\n" << std::endl;
    return 1;
}

int main(){
    simulate();
}

