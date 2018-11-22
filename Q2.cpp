#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <fstream>
#include <chrono> //For timing the code.
#include <iomanip>
#include <string>

unsigned int fate_tester(unsigned int odds) {
    return static_cast<unsigned int>(ceil(static_cast<double>(rand()) / (static_cast<unsigned long>(RAND_MAX)) * odds));
}

/**
 *
 */
class Person {
public:
    void genesis(const bool infected, const bool imm=false){
        I = infected;
        A = true;
        R = imm;
    }

    void sickDay(){
        auto score = static_cast<__int8_t>(fate_tester(100));
        if(score <= 12){
            I = false;
            R = true;
        }
        else if(score >= 100){
            A = false;
        }
    };

    void meetInfected(){
        if(!R){
            auto infectionScore = static_cast<__int8_t>(fate_tester(2));
            if(infectionScore <= 1){
                I = true;
            }
        }
    };

    void personCondition(){
        std::cout << "\n\n\n" << std::endl;
        std::cout << "Alive: " << A << std::endl;
        std::cout << "Infected: " << I << std::endl;
        std::cout << "Immune: " << R << std::endl;
        std::cout << "\n\n\n" << std::endl;
    }

    bool checkInfected(){
        return I;
    }
    bool checkAlive(){
        return A;
    }
    bool checkImmune(){
        return R;
    }
private:
    bool I;
    bool A;
    bool R;
};


int simulate(const bool debug = false){
    srand(static_cast<unsigned int>(time(nullptr)));
    static const int maxPop = 200000;
    static const int runLength = 365;
    static const int meetingsPerDay = 50000;
    std::vector<Person> population;

    static auto startOne = std::chrono::high_resolution_clock::now();
    /**--------------------------------------------------------------------------------------------------------------*/
    /**
     * LOOP ONE
     */
    for(unsigned int i = 1; i <= maxPop; i++){
        {
            Person person{};
            person.genesis(i <= 100);
            population.push_back(person);
        }
    }
    /**--------------------------------------------------------------------------------------------------------------*/
    static auto finishOne = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsedOne = finishOne - startOne;
    std::cout << "\n\nTime taken to generate population:\t\t" <<  std::fixed << std::setprecision(3) << elapsedOne.count() << " s" << std::endl;

    std::ofstream brains;
    brains.open("Pandemic.csv");
    brains << "Day,Infected,Uninfected,Immune,Alive,Dead\r";
    /**--------------------------------------------------------------------------------------------------------------*/
    /**
     * LOOP TWO
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
        /**
         * LOOP THREE
         */
        auto startThree = std::chrono::high_resolution_clock::now();
        for(unsigned int j = 1; j <= meetingsPerDay; j++) {

            /**
             * maxPop will need to be changed to a new variable currentPop to take into account population reducing
             * when someone dies. Not necessarily, as we'd expect number of interactions to reduce as people die.
             */

            unsigned int personOneSeed = fate_tester(maxPop);
            unsigned int personTwoSeed = fate_tester(maxPop);
            if(personOneSeed == personTwoSeed){
                j--;
                continue;
            }

            Person& personOne = population[personOneSeed];
            Person& personTwo = population[personTwoSeed];
            if(!personOne.checkAlive() or !personTwo.checkAlive()){
                j--;
                continue;
            }
            if(personOne.checkImmune() or personTwo.checkImmune()){
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

        for (auto &it : population) {

            if(it.checkAlive()){
                aliveCount++;
                if(it.checkInfected()){
                    sickCount++;
                    it.sickDay();
                }
                else if(it.checkImmune()){
                    recoveredCount++;
                }
            }

            if(debug){
                if(it.checkInfected() and it.checkImmune()){
                    std::cout << "Infected and immune detected, error in code. Ending process..." << std::endl;
                    return 0;
                }
                if(!it.checkInfected() and !it.checkAlive()){
                    std::cout << "Dead and not infected detected, error in code. Ending process..." << std::endl;
                    return 0;
                }
                if(it.checkImmune() and !it.checkAlive()){
                    std::cout << "Dead and immune detected, error in code. Ending process..." << std::endl;
                    return 0;
                }
            }
        }


        deadCount = maxPop - aliveCount;
        neverInfected = maxPop - sickCount - deadCount - recoveredCount;
        if(debug){
            unsigned int totalOne = aliveCount+deadCount;
            unsigned int totalTwo = sickCount+neverInfected+recoveredCount+deadCount;
            if((totalOne != maxPop) or (totalTwo != maxPop)){
                std::cout << "ERROR: Sum of persons in each possible state does not equal total population." << std::endl;
                std::cout << "Some people are either being counted multiple times or not being counted at all." << std::endl;
                return 1;
            }
        }

        /**
         * This is currently writing from the 1st day as supposed to the 0th day, so the first sick count entry != 100.
         */
        brains << n << "," << sickCount << "," << neverInfected << "," << recoveredCount << "," << aliveCount << "," << deadCount << "," << "\r" << std::endl;

    }
    /**--------------------------------------------------------------------------------------------------------------*/
    static const double meanLoopThreeTime = loopThreeTotal/runLength;
    static const auto finishTwo = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsedTwo = finishTwo - startTwo;
    std::cout << "Entire year simulation took:\t\t\t" <<  std::fixed << std::setprecision(3) << elapsedTwo.count() << " s " << std::endl;
    std::cout << "All meetings took:\t\t\t\t" <<  std::fixed << std::setprecision(3) << loopThreeTotal << " s " << std::endl;
    std::cout << "Mean duration of day's meetings:\t\t" <<  std::fixed << std::setprecision(3) << meanLoopThreeTime << " s " << std::endl;
    std::cout << "All population scan took:\t\t\t" <<  std::fixed << std::setprecision(3) << elapsedTwo.count()-loopThreeTotal << " s " << std::endl;

    std::cout << "\n\nSick:\t\t\t" << std::fixed << std::setprecision(2) <<  sickCount/2000. << "%" << std::endl;
    std::cout << "Alive:\t\t\t" << std::fixed << std::setprecision(2) <<  aliveCount/2000. << "%" << std::endl;
    std::cout << "Dead:\t\t\t" << std::fixed << std::setprecision(2) <<  deadCount/2000. << "%" << std::endl;
    std::cout << "Immune:\t\t\t" << std::fixed << std::setprecision(2) <<  recoveredCount/2000. << "%" << std::endl;
    std::cout << "Never infected:\t\t" << std::fixed << std::setprecision(2) <<  neverInfected/2000. << "%" << std::endl;
    return 1;
}

int main(){
    simulate();
}
