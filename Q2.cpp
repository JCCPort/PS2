#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <fstream>
#include <chrono> //For timing the code.


int fate_tester(int odds) {
    return static_cast<int>(ceil(static_cast<double>(rand()) / (static_cast<long>(RAND_MAX)) * odds));
}

/**
 *
 */
class Person {
public:
    void generate(bool infected){
        I = infected;
        A = true;
        Immune = false;
    }

    void sickDay(){
        int recoveryScore = fate_tester(100);
        int deathScore = fate_tester(100);
        if(recoveryScore <= 12){
            I = false;
            Immune = true;
        }
        else if(deathScore <= 1){
            A = false;
        }
    };

    void meetInfected(){
        int infectionScore = fate_tester(2);
        if(infectionScore <= 1 and !Immune){
            I = true;
        }
    };

    bool checkInfected(){
        return I;
    }
    bool checkAlive(){
        return A;
    }
    bool checkImmune(){
        return Immune;
    }
private:
    bool I;
    bool A;
    bool Immune;
};


int main(){
    srand(static_cast<unsigned int>(time(nullptr)));

    const int maxPop = 200000;
    const int runLength = 365;
    const int meetingsPerDay = 50000;
    std::vector<Person> population;

    auto startOne = std::chrono::high_resolution_clock::now();
    /**--------------------------------------------------------------------------------------------------------------*/
    /**
     * LOOP ONE
     */
    for(int i = 1; i <= maxPop; i++){
        {
            Person person{};
            person.generate(i <= 100);
            population.push_back(person);
        }
    }
    /**--------------------------------------------------------------------------------------------------------------*/
    auto finishOne = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsedOne = finishOne - startOne;
    std::cout << "Generation loop: Code took " << elapsedOne.count() << "s " << std::endl;

    std::ofstream brains;
    brains.open("Pandemic.csv");
    brains << "Day,Infected,Uninfected,Immune,Alive,Dead\r";
    /**--------------------------------------------------------------------------------------------------------------*/
    /**
     * LOOP TWO
     */
    double loopThreeTotal = 0;
    auto startTwo = std::chrono::high_resolution_clock::now();
    for(int n = 1; n <= runLength; n++){


        /**--------------------------------------------------------------------------------------------------------------*/
        /**
         * LOOP THREE
         */
        auto startThree = std::chrono::high_resolution_clock::now();
        for(int j = 1; j <= meetingsPerDay; j++) {

            /**
             * maxPop will need to be changed to a new variable currentPop to take into account population reducing
             * when someone dies. Not necessarily, as we'd expect number of interactions to reduce as people die.
             */

            int personOneSeed = fate_tester(maxPop);
            int personTwoSeed = fate_tester(maxPop);
            if(personOneSeed == personTwoSeed){
                j--;
                continue;
            }

            Person& personOne = population[personOneSeed];
            Person& personTwo = population[personTwoSeed];
            if(!personOne.checkAlive() or !personTwo.checkAlive() or personOne.checkImmune() or personTwo.checkImmune()){
                continue;
            }
            else{
                bool personOneSick = personOne.checkInfected();
                bool personTwoSick = personTwo.checkInfected();
                if(personOneSick != personTwoSick){
                    if(personOneSick){
                        population[personTwoSeed].meetInfected();
                    }
                    else{
                        population[personOneSeed].meetInfected();
                    }
                }
            }
        }
        auto finishThree = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsedThree = finishThree - startThree;
        loopThreeTotal = loopThreeTotal + elapsedThree.count();
        /**--------------------------------------------------------------------------------------------------------------*/



        int sickCount = 0;
        int uninfectedCount = 0;
        int recoveredCount = 0;
        int aliveCount = 0;
        int deadCount = 0;
        int count = 0;

        for (auto &it : population) {

            if(it.checkAlive()){
                aliveCount++;
                if(it.checkInfected()){
                    sickCount++;
                    it.sickDay();
                }
                if(it.checkImmune()){
                    recoveredCount++;
                }
            }


//            if(!it.checkAlive()){
//                deadCount++;
//            }
//            if(it.checkInfected() and it.checkAlive()){
//                it.sickDay();
//            }
//            count++;
//            if(!it.checkImmune() and it.checkAlive()){
//                std::cout << "Person \t" << count << ":" << std::endl;
//                std::cout << "Infected: \t" << it.checkInfected() << std::endl;
//                std::cout << "Immune: \t" << it.checkImmune() << std::endl;
//                std::cout << "Alive: \t\t" << it.checkAlive() << "\n\n\n";
//            }

//            if(it.checkInfected() and it.checkImmune()){
//                std::cout << "Infected and immune detected, error in code. Ending process..." << std::endl;
//                return 0;
//            }
//            if(!it.checkInfected() and !it.checkAlive()){
//                std::cout << "Dead and not infected detected, error in code. Ending process..." << std::endl;
//                return 0;
//            }
//            if(it.checkImmune() and !it.checkAlive()){
//                std::cout << "Dead and immune detected, error in code. Ending process..." << std::endl;
//                return 0;
//            }
        }


        deadCount = maxPop - aliveCount;
        uninfectedCount = maxPop - sickCount - deadCount - recoveredCount;
        /**
         * This is currently writing from the 1st day as supposed to the 0th day, so the first sick count entry != 100.
         */
        brains << n << "," << sickCount << "," << uninfectedCount << "," << recoveredCount << "," << aliveCount << "," << deadCount << "," << "\r" << std::endl;

//        std::cout << "\n\nNumber sick: " << sickCount << std::endl;
//        std::cout << "Number uninfected: " << uninfectedCount << std::endl;
//        std::cout << "Number recovered: " << recoveredCount << std::endl;
//        std::cout << "Number alive: " << aliveCount << std::endl;
//        std::cout << "Number dead: " << deadCount << std::endl;
//        std::cout << "Total: " << aliveCount+deadCount << std::endl;
//        std::cout << "Total2: " << sickCount+uninfectedCount+recoveredCount+deadCount << std::endl;

    }
    /**--------------------------------------------------------------------------------------------------------------*/
    double meanLoopThreeTime = loopThreeTotal/runLength;
    auto finishTwo = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsedTwo = finishTwo - startTwo;
    std::cout << "All loop three took: " << loopThreeTotal << std::endl;
    std::cout << "Mean loop three took: " << meanLoopThreeTime << std::endl;
    std::cout << "Loop 2 - Loop 3 Code took " << elapsedTwo.count()-loopThreeTotal << "s " << std::endl;
}

